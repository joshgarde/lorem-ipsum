#include "bookrenderer.h"
#include <QDebug>
#include <QPainter>
#include <QLabel>
#include <QPagedPaintDevice>
#include <QApplication>
#include <QScrollBar>
#include "pagerenderer.h"
#include "src/sections/title.h"
#include "src/sections/copyright.h"
#include "src/sections/tableofcontents.h"
#include "src/sections/chapter.h"

BookRenderer::BookRenderer(SectionModel* model, QWidget *parent) : QScrollArea(parent) {
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
  PageRenderer renderer(section, this, 0, contentIdx);
  renderer.render(painter);
}

void BookRenderer::loadSection(QModelIndex index) {
  currentSection = model.data(index, Qt::UserRole).value<Section*>();
  
  // Locate closest table of contents or just default to page 1
  int start = 0;
  basePageNumber = 1;
  for (int i = 1; i < index.row(); i++) {
    if (model.data(index, Qt::UserRole).value<Section*>().type() == SectionType::TABLE_OF_CONTENTS) {
      start = i;
      return;
    }
  }
  start++;
  // Calculate base page
  for (int i = start; i < index.row(); i++) {
    if (model.data(index, Qt::UserRole).value<Section*>().multiplePages()) {
      
    } else {
      basePageNumber++;
    }
  }

  reset();
  qDebug() << "[DEBUG] Loading section:" << section->objectName();

  populate();

  verticalScrollBar()->setValue(0);
}

BookRenderer::populate() {
  PageRenderer* renderer = new PageRenderer(section, this, basePageNumber);
  renderers.append(renderer);
  layout.addWidget(renderer);

  if (currentSection->multiplePages()) {
    int currentIndex = 0;
    basePageNumber++;

    while (true) {
      QApplication::processEvents();
      currentIndex += renderer->truncate();

      switch (section->type()) {
        case SectionType::TABLE_OF_CONTENTS: {
          break; // Not implemented yet
        }

        case SectionType::CHAPTER: {
          if (((Chapter*)section)->contents.size() - 1 <= currentIndex) return;
        }

        default:
          throw new std::runtime_error("Invalid section type @ BookRenderer::loadSection");
      }

      renderer = new PageRenderer(section, this, basePageNumber, currentIndex);
      renderers.append(renderer);
      layout.addWidget(renderer);
    }
  }
  QApplication::processEvents();
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
  qobject_cast<QTextEdit*>(focusWidget())->textCursor();
  int position = verticalScrollBar()->value();

  setUpdatesEnabled(false);
  reset();
  populate();
  setUpdatesEnabled(true);

  verticalScrollBar()->setValue(position);
}
