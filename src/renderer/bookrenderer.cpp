#include "bookrenderer.h"
#include <QDebug>
#include <QPainter>
#include <QLabel>
#include <QPagedPaintDevice>
#include <QApplication>
#include <QScrollBar>
#include "pagerenderer.h"
#include "backscrolltextedit.h"
#include "src/sections/title.h"
#include "src/sections/copyright.h"
#include "src/sections/tableofcontents.h"
#include "src/sections/chapter.h"

BookRenderer::BookRenderer(QWidget *parent) : QScrollArea(parent) {
  scrollPane.setLayout(&layout);
  QPalette palette;
  palette.setColor(QPalette::Background, Qt::gray);
  scrollPane.setPalette(palette);
  scrollPane.setAutoFillBackground(true);
  layout.setContentsMargins(0, 0, 0, 0);
  layout.setAlignment(Qt::AlignHCenter);
  setWidget(&scrollPane);
  setWidgetResizable(true);
}

void BookRenderer::setSectionModel(SectionModel* model) {
  this->model = model;
}

void BookRenderer::reset() {
  QMutableListIterator<PageRenderer*> i(renderers);
  while (i.hasNext()) {
    PageRenderer* renderer = i.next();
    layout.removeWidget(renderer);
    delete renderer;
    i.remove();
  }
}

int BookRenderer::calculatePages(QModelIndex index) {
  Section* section = model->data(index, Qt::UserRole).value<Section*>();

  if (section->multiplePages()) {
    int pages = 0;
    int currentIndex = 0;
    int targetIndex = calculateTargetIndex(section);

    while (currentIndex < targetIndex) {
      PageRenderer renderer(this, model, index, 0, currentIndex);
      renderer.show();
      currentIndex += renderer.truncate();
      pages++;
    }

    return pages;
  } else {
    return 1;
  }
}

// Locate closest table of contents or just default to first section
int BookRenderer::locateStart(int max) {
  if (max == -1) max = model->rowCount();

  for (int i = 0; i < max; i++) {
    if (model->data(model->index(i, 0), Qt::UserRole).value<Section*>()->type() == SectionType::TABLE_OF_CONTENTS) {
      return i;
    }
  }
  return 0;
}

void BookRenderer::generateTableOfContents() {
  int start = locateStart(model->rowCount()) + 1;
  int page = 1;
  QList<QPair<QString, int>> tableOfContents;

  for (int i = start; i < model->rowCount(); i++) {
    QModelIndex index = model->index(i, 0);
    Section* section = model->data(model->index(i, 0), Qt::UserRole).value<Section*>();
    if (section->type() == SectionType::CHAPTER) {
      tableOfContents.append(QPair<QString, int>(((Chapter*)section)->name, page));
      page += calculatePages(index);
    } else {
      tableOfContents.append(QPair<QString, int>(section->objectName(), page));
      page += calculatePages(index);
    }
  }

  toc = tableOfContents;
}

int BookRenderer::calculateTargetIndex(Section* section) {
  switch (section->type()) {
    case SectionType::TABLE_OF_CONTENTS: {
      generateTableOfContents();
      return toc.size();
    }

    case SectionType::CHAPTER: {
      return ((Chapter*)section)->contents.size();
    }

    default: {
      return 1;
    }
  }
}

int BookRenderer::calculateChapterNumber(QModelIndex index) {
  int start = locateStart();
  if (start >= index.row()) return 0;

  return index.row() - start;
}

QList<QPair<QString, int>> BookRenderer::tableOfContents() {
  return toc;
}

int BookRenderer::renderSection(QPagedPaintDevice* paintDevice, QPainter* painter, QModelIndex index, int page) {
  Section* section = model->data(index, Qt::UserRole).value<Section*>();
  qDebug() << "[DEBUG] Rendering section: " << section->objectName();

  int start = locateStart();
  int contentIndex = 0;
  int targetIndex = calculateTargetIndex(section);
  do {
    PageRenderer renderer(this, model, index, page, contentIndex);
    renderer.show();
    contentIndex += renderer.truncate();
    renderer.render(painter);
    page++;
    if (contentIndex < targetIndex) paintDevice->newPage();
  } while (contentIndex < targetIndex);

  if (start == index.row()) {
    return 1;
  } else {
    return page;
  }
}

void BookRenderer::loadSection(QModelIndex index) {
  this->index = index;
  currentSection = model->data(index, Qt::UserRole).value<Section*>();
  qDebug() << "[DEBUG] Loading section:" << currentSection->objectName();

  int start = locateStart(index.row()) + 1;

  // Calculate base page
  basePageNumber = 1;
  for (int i = start; i < index.row(); i++) {
    basePageNumber += calculatePages(model->index(i, 0));
  }

  reset();
  generateTableOfContents();

  PageRenderer* renderer = new PageRenderer(this, model, index, basePageNumber);
  renderers.append(renderer);
  layout.addWidget(renderer);

  if (currentSection->multiplePages()) {
    int currentIndex = 0;
    int targetIndex = calculateTargetIndex(currentSection);

    int currentPage = basePageNumber + 1;

    while (true) {
      renderer->repaint();
      renderer->show();
      currentIndex += renderer->truncate();
      if (currentIndex >= targetIndex) break;

      renderer = new PageRenderer(this, model, index, currentPage, currentIndex);
      renderers.append(renderer);
      layout.addWidget(renderer);

      currentPage++;
    }
  }

  QApplication::processEvents();
  verticalScrollBar()->setValue(0);
}

void BookRenderer::updateSection() {
  QMutableListIterator<PageRenderer*> it(renderers);
  QMap<QString, QString> fields;
  while (it.hasNext()) {
    PageRenderer* renderer = it.next();
    for (auto jt = renderer->fields.begin(); jt != renderer->fields.end(); jt++) {
      fields[jt.key()] += (*jt)->toPlainText().toHtmlEscaped();
    }
  }

  switch (currentSection->type()) {
    case SectionType::TITLE: {
      ((Title*)currentSection)->title = fields["title"];
      break;
    }

    case SectionType::COPYRIGHT: {
      ((Copyright*)currentSection)->contents = fields["copyright"];
      break;
    }

    case SectionType::TABLE_OF_CONTENTS: {
      break;
    }

    case SectionType::CHAPTER: {
      ((Chapter*)currentSection)->name = fields["chapterName"];
      ((Chapter*)currentSection)->contents = fields["chapterContents"];
      break;
    }
  }

  emit updateToc();
}

void BookRenderer::reloadSection(int currentPage) {
  BackscrollTextEdit* field = qobject_cast<BackscrollTextEdit*>(focusWidget());
  if (field == nullptr) return;
  int cursorPosition = field->textCursor().position();
  int scrollBarposition = verticalScrollBar()->value();

  int contentIdx = 0;
  QMutableListIterator<PageRenderer*> it(renderers);
  while (it.hasNext()) {
    PageRenderer* renderer = it.next();
    if (!renderer->requiresRerender()) {
      contentIdx += renderer->contentSize();
      continue;
    }

    int startPage = renderer->pageNumber();

    setUpdatesEnabled(false);

    layout.removeWidget(renderer);
    delete renderer;
    it.remove();

    while (it.hasNext()) {
      renderer = it.next();
      layout.removeWidget(renderer);
      delete renderer;
      it.remove();
    }

    int targetIndex = calculateTargetIndex(currentSection);

    while (targetIndex > contentIdx || startPage == basePageNumber) {
      renderer = new PageRenderer(this, model, index, startPage, contentIdx);
      renderer->show();
      renderers.append(renderer);
      layout.addWidget(renderer);
      contentIdx += renderer->truncate();
      startPage++;
    }
    break;
  }

  // Restore cursor
  int overflow = cursorPosition;
  int rendererIdx;
  if ((currentPage - basePageNumber) > renderers.size()){
    rendererIdx = renderers.size() - 1;
  } else {
    rendererIdx = currentPage - basePageNumber;
  }

  do {
    overflow = renderers[rendererIdx]->restoreCursor(overflow);
    if (rendererIdx < (renderers.size() - 1)) {
      rendererIdx++;
    } else {
      break;
    }
  } while (overflow > 0);

  QApplication::processEvents();
  QApplication::processEvents();
  QApplication::processEvents();
  QApplication::processEvents();

  verticalScrollBar()->setValue(scrollBarposition);
  setUpdatesEnabled(true);
}
