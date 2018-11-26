#include "pagerenderer.h"
#include <QDebug>
#include <QEvent>
#include <QCoreApplication>
#include "src/sections/title.h"
#include "src/sections/copyright.h"
#include "src/sections/tableofcontents.h"
#include "src/sections/chapter.h"

PageRenderer::PageRenderer(Section* section, int page, BookRenderer* renderer) : QWidget(renderer) {
  this->model = section->parent();
  this->section = section;
  this->renderer = renderer;
  this->page = page;

  QPalette palette;
  palette.setColor(QPalette::Background, Qt::white);
  setPalette(palette);
  setAutoFillBackground(true);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  setLayout(&layout);
  setContentsMargins(0, 0, 0, 0);

  switch (section->type()) {
    case SectionType::TITLE: {
      layout.addStretch(2);

      QTextEdit* titleField = new QTextEdit(this);
      titleField->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      titleField->setFont(section->fontMap["title"]);
      connect(titleField, SIGNAL(textChanged()), this, SLOT(fieldResize()));
      titleField->setPlainText(((Title*)section)->title);
      titleField->setAlignment(Qt::AlignCenter);
      fields.insert("title", titleField);
      layout.addWidget(titleField);

      layout.addStretch(3);
      break;
    }

    case SectionType::COPYRIGHT: {
      layout.addStretch(1);

      QTextEdit* copyrightField = new QTextEdit(this);
      copyrightField->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      copyrightField->setFont(section->fontMap["copyright"]);
      copyrightField->setPlainText(((Copyright*)section)->contents);
      fields.insert("copyright", copyrightField);
      layout.addWidget(copyrightField);

      break;
    }

    case SectionType::TABLE_OF_CONTENTS: {
      break;
    }

    case SectionType::CHAPTER: {
      if (page == 0) {
        layout.addStretch(1);

        QTextEdit* chapterNumberField = new QTextEdit(this);
        chapterNumberField->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        chapterNumberField->setFont(section->fontMap["chapterNumber"]);
        chapterNumberField->setTextInteractionFlags(Qt::NoTextInteraction);
        connect(chapterNumberField, SIGNAL(textChanged()), this, SLOT(fieldResize()));
        chapterNumberField->setPlainText("Chapter Null");
        chapterNumberField->setAlignment(Qt::AlignCenter);

        QTextEdit* chapterNameField = new QTextEdit(this);
        chapterNameField->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        chapterNameField->setFont(section->fontMap["chapterName"]);
        connect(chapterNameField, SIGNAL(textChanged()), this, SLOT(fieldResize()));
        chapterNameField->setPlainText(((Chapter*)section)->name);
        chapterNameField->setAlignment(Qt::AlignCenter);

        fields.insert("chapterNumber", chapterNumberField);
        fields.insert("chapterName", chapterNameField);
        layout.addWidget(chapterNumberField);
        layout.addWidget(chapterNameField);

        layout.addStretch(1);
      }

      QTextEdit* chapterContentsField = new QTextEdit(this);
      chapterContentsField->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      chapterContentsField->setFont(section->fontMap["chapterContents"]);
      chapterContentsField->setPlainText(((Chapter*)section)->contents);
      fields.insert("chapterContents", chapterContentsField);
      layout.addWidget(chapterContentsField, 2);

      break;
    }
  }
}

void PageRenderer::fieldResize() {
  QTextEdit* field = (QTextEdit*)sender();
  QFontMetrics metrics(field->currentFont());
  int width = model->size.width() * PPI - 20;
  int height = model->size.height() * PPI - 20;
  QRect rect = metrics.boundingRect(QRect(0, 0, width, height),
      Qt::AlignVCenter | Qt::TextWordWrap, field->toPlainText());
  field->setFixedHeight(rect.height() + 12);
}

void PageRenderer::rerenderCheck() {

}

QSize PageRenderer::sizeHint() const {
  return QSize(model->size.width() * PPI, model->size.height() * PPI);
}
