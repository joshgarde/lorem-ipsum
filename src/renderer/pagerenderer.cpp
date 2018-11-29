#include "pagerenderer.h"
#include <QDebug>
#include <QEvent>
#include <QApplication>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>
#include "src/sections/title.h"
#include "src/sections/copyright.h"
#include "src/sections/tableofcontents.h"
#include "src/sections/chapter.h"

const QString PageRenderer::chapterContentsHtml =
  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
  "<html><head><meta name=\"qrichtext\" content=\"1\" /><style>p {line-height: %1; margin: 0;}</style></head>"
  "<body><p align=\"justify\">%2</p></div></html>";

PageRenderer::PageRenderer(Section* section, BookRenderer* renderer, int page, int contentIdx, int cursorPosition) :
    QWidget(renderer), section(section), renderer(renderer), page(page) {

  QPalette palette;
  palette.setColor(QPalette::Background, Qt::white);
  setPalette(palette);
  setAutoFillBackground(true);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  setLayout(&layout);
  setContentsMargins(0, 0, 0, 0);
  setFixedSize(QSize(model->size.width() * PPI, model->size.height() * PPI));

  switch (section->type()) {
    case SectionType::TITLE: {
      layout.addStretch(2);

      BackscrollTextEdit* titleField = new BackscrollTextEdit(this);
      titleField->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      titleField->setFont(section->fontMap["title"]);
      titleField->setFrameStyle(0);
      connect(titleField, SIGNAL(textChanged()), this, SLOT(fieldResize()));
      titleField->setPlainText(((Title*)section)->title);
      titleField->setAlignment(Qt::AlignCenter);
      connect(titleField, SIGNAL(textChanged()), renderer, SLOT(updateSection()));
      fields.insert("title", titleField);
      layout.addWidget(titleField);

      layout.addStretch(3);
      break;
    }

    case SectionType::COPYRIGHT: {
      layout.addStretch(1);

      BackscrollTextEdit* copyrightField = new BackscrollTextEdit(this);
      copyrightField->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      copyrightField->setFont(section->fontMap["copyright"]);
      copyrightField->setFrameStyle(0);
      copyrightField->setPlainText(((Copyright*)section)->contents);
      connect(copyrightField, SIGNAL(textChanged()), renderer, SLOT(updateSection()));
      fields.insert("copyright", copyrightField);
      layout.addWidget(copyrightField);

      break;
    }

    case SectionType::TABLE_OF_CONTENTS: {
      break;
    }

    case SectionType::CHAPTER: {
      if (contentIdx == 0) {
        layout.addStretch(1);

        BackscrollTextEdit* chapterNumberField = new BackscrollTextEdit(this);
        chapterNumberField->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        chapterNumberField->setFont(section->fontMap["chapterNumber"]);
        chapterNumberField->setFrameStyle(0);
        chapterNumberField->setTextInteractionFlags(Qt::NoTextInteraction);
        connect(chapterNumberField, SIGNAL(textChanged()), this, SLOT(fieldResize()));
        chapterNumberField->setPlainText("Chapter Null");
        chapterNumberField->setAlignment(Qt::AlignCenter);

        BackscrollTextEdit* chapterNameField = new BackscrollTextEdit(this);
        chapterNameField->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        chapterNameField->setFont(section->fontMap["chapterName"]);
        chapterNameField->setFrameStyle(0);
        connect(chapterNameField, SIGNAL(textChanged()), this, SLOT(fieldResize()));
        chapterNameField->setPlainText(((Chapter*)section)->name);
        chapterNameField->setAlignment(Qt::AlignCenter);
        connect(chapterNameField, SIGNAL(textChanged()), renderer, SLOT(updateSection()));

        fields.insert("chapterNumber", chapterNumberField);
        fields.insert("chapterName", chapterNameField);
        layout.addWidget(chapterNumberField);
        layout.addWidget(chapterNameField);

        layout.addStretch(1);
      }

      BackscrollTextEdit* chapterContentsField = new BackscrollTextEdit(this);
      chapterContentsField->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      chapterContentsField->setFont(section->fontMap["chapterContents"]);
      chapterContentsField->setFrameStyle(0);
      QString chapterContents = ((Chapter*)section)->contents.mid(
        contentIdx,
        ((Chapter*)section)->contents.size()
      );
      chapterContentsField->setHtml(chapterContentsHtml
        .arg(((Chapter*)section)->lineSpacing)
        .arg(chapterContents.replace("\n", "</p><p align=\"justify\">"))
      );
      chapterContentsField->setAlignment(Qt::AlignJustify);
      fields.insert("chapterContents", chapterContentsField);
      layout.addWidget(chapterContentsField, 3);
      connect(chapterContentsField, SIGNAL(textChanged()), renderer, SLOT(updateSection()));
      connect(chapterContentsField->document()->documentLayout(), SIGNAL(documentSizeChanged(const QSizeF&)), this, SLOT(sendReload()));
      break;
    }
  }
}

int PageRenderer::pageNumber() {
  return page;
}

int PageRenderer::truncate() {
  BackscrollTextEdit* field;
  QString contents;
  int lineSpacing = 1;

  switch (section->type()) {
    case SectionType::TABLE_OF_CONTENTS: {
      return 0;
      break;
    }

    case SectionType::CHAPTER: {
      field = fields["chapterContents"];
      contents = field->toPlainText();
      lineSpacing = ((Chapter*)section)->lineSpacing;
      break;
    }

    default:
      throw new std::runtime_error("Invalid section type @ PageRenderer::truncate");
  }

  disconnect(field, SIGNAL(textChanged()), renderer, SLOT(updateSection()));
  disconnect(field->document()->documentLayout(), SIGNAL(documentSizeChanged(const QSizeF&)), this, SLOT(sendReload()));

  QFontMetrics fontMetrics(field->currentFont());

  int endIndex = field->cursorForPosition(
    QPoint(
      field->width() - 1,
      field->height() - (fontMetrics.height() * lineSpacing)
    )
  ).position();
  contents = contents.mid(0, endIndex);
  field->setHtml(chapterContentsHtml
    .arg(((Chapter*)section)->lineSpacing)
    .arg(contents.replace("\n", "</p><p align=\"justify\">"))
  );
  connect(field, SIGNAL(textChanged()), renderer, SLOT(updateSection()));
  connect(field->document()->documentLayout(), SIGNAL(documentSizeChanged(const QSizeF&)), this, SLOT(sendReload()));
  return endIndex;
}

int PageRenderer::restoreCursor(int position) {
  int overflow;
  switch (section->type()) {
    case SectionType::TITLE:
    case SectionType::COPYRIGHT: {
      break;
    }
    
    case SectionType::TABLE_OF_CONTENTS:
    case SectionType::CHAPTER:{
      break;
    }
  }
}

void PageRenderer::fieldResize() {
  BackscrollTextEdit* field = (BackscrollTextEdit*)sender();
  QFontMetrics metrics(field->currentFont());
  int width = model->size.width() * PPI - 20;
  int height = model->size.height() * PPI - 20;
  QRect rect = metrics.boundingRect(QRect(0, 0, width, height),
      Qt::AlignVCenter | Qt::TextWordWrap, field->toPlainText());
  field->setFixedHeight(rect.height() + 12);
}

void PageRenderer::sendReload() {
  if (!initialized) {
    initialized = true;
    return;
  }

  QMetaObject::invokeMethod(renderer, "loadSection", Qt::QueuedConnection, Q_ARG(Section*, section), Q_ARG(int, page));
}
