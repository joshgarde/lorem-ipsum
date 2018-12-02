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

void BookRenderer::renderSection(QPagedPaintDevice* paintDevice, QPainter* painter, Section* section) {
  qDebug() << "[DEBUG] Rendering section: " << section->objectName();
  int contentIdx = 0;
  PageRenderer renderer(this, model, section, 0, contentIdx);
  renderer.render(painter);
}

void BookRenderer::loadSection(QModelIndex index) {
  currentSection = model->data(index, Qt::UserRole).value<Section*>();
  qDebug() << "[DEBUG] Loading section:" << currentSection->objectName();

  // Locate closest table of contents or just default to page 1
  int start = 0;
  basePageNumber = 1;
  for (int i = 1; i < index.row(); i++) {
    if (model->data(model->index(i, 0), Qt::UserRole).value<Section*>()->type() == SectionType::TABLE_OF_CONTENTS) {
      start = i;
      break;
    }
  }
  start++;

  // Calculate base page
  for (int i = start; i < index.row(); i++) {
    Section* section = model->data(model->index(i, 0), Qt::UserRole).value<Section*>();
    if (section->multiplePages()) {
      int currentIndex = 0;
      int targetIndex;

      switch (section->type()) {
        case SectionType::TABLE_OF_CONTENTS: {
          break; // Not implemented yet
        }

        case SectionType::CHAPTER: {
          targetIndex = ((Chapter*)section)->contents.size();
          break;
        }

        default: {
          qDebug() << "Invalid SectionType @ BookRenderer::loadSection:" << section->type();
          throw new std::runtime_error("Invalid SectionType @ BookRenderer::loadSection");
        }
      }

      while (currentIndex < targetIndex) {
        PageRenderer renderer(this, model, section);
        renderer.show();
        currentIndex += renderer.truncate();
        basePageNumber++;
      }
    } else {
      basePageNumber++;
    }
  }

  reset();

  PageRenderer* renderer = new PageRenderer(this, model, currentSection, basePageNumber);
  renderers.append(renderer);
  layout.addWidget(renderer);

  if (currentSection->multiplePages()) {
    int currentIndex = 0;
    int targetIndex;

    switch (currentSection->type()) {
      case SectionType::TABLE_OF_CONTENTS: {
        break; // Not implemented yet
      }

      case SectionType::CHAPTER: {
        targetIndex = ((Chapter*)currentSection)->contents.size();
        break;
      }

      default: {
        qDebug() << "Invalid section type @ BookRenderer::populate" << currentSection->type();
        throw new std::runtime_error("Invalid section type @ BookRenderer::populate");
      }
    }

    int currentPage = basePageNumber + 1;

    while (true) {
      renderer->show();
      currentIndex += renderer->truncate();
      if (currentIndex >= targetIndex) break;

      renderer = new PageRenderer(this, model, currentSection, currentPage, currentIndex);
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

    Chapter* section = qobject_cast<Chapter*>(currentSection);
    int targetIndex = section->contents.size();

    while (targetIndex > contentIdx) {
      renderer = new PageRenderer(this, model, currentSection, startPage, contentIdx);
      renderers.append(renderer);
      layout.addWidget(renderer);
      renderer->show();
      contentIdx += renderer->truncate();
      startPage++;
    }
    setUpdatesEnabled(true);
    break;
  }

  // Restore cursor
  int overflow = cursorPosition;
  if ((currentPage - basePageNumber) > renderers.size()) currentPage = renderers.size() + basePageNumber;

  do {
    overflow = renderers[currentPage - basePageNumber - 1]->restoreCursor(overflow);
    currentPage++;
  } while (overflow > 0);
}
