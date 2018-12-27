#include "pagerenderer.h"
#include <QDebug>
#include <QEvent>
#include <QApplication>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>
#include "src/sections/halftitle.h"
#include "src/sections/title.h"
#include "src/sections/copyright.h"
#include "src/sections/tableofcontents.h"
#include "src/sections/chapter.h"

const QString PageRenderer::fieldHtmlTemplate =
  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
  "<html><head><meta name=\"qrichtext\" content=\"1\" /><style>p {line-height: %1; margin: 0;}</style></head>"
  "<body><p align=\"%2\">%3</p></div></html>";

PageRenderer::PageRenderer(BookRenderer* renderer, SectionModel* model, QModelIndex index, int page, int contentIdx, PageDirection direction) :
    QWidget(renderer), renderer(renderer), model(model), index(index), page(page), dir(direction) {

  section = model->data(index, Qt::UserRole).value<Section*>();

  QPalette palette;
  palette.setColor(QPalette::Background, Qt::white);
  setPalette(palette);
  setAutoFillBackground(true);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  setLayout(&layout);
  setFixedSize(QSize(model->size.width() * PPI, model->size.height() * PPI));
  setStyleSheet("SwissTextEdit:focus { background-color: rgba(230, 230, 230, 100%); }");

  int left, top, right, bottom;
  if (direction == RIGHT) {
    left = (model->size.width() * PPI * .25f * .6f);
    right = model->size.width() * PPI * .25f * .4f;
  } else {
    left = model->size.width() * PPI * .25f * .4f;
    right = model->size.width() * PPI * .25f * .6f;
  }
  top = (model->size.height() * PPI * .15f * .55f);
  bottom = (model->size.height() * PPI * .15f * .45f);

  switch (section->type()) {
    case SectionType::HALFTITLE: {
      layout.addStretch(2);

      SwissTextEdit* titleField = new SwissTextEdit(this);
      titleField->setFont(section->fontMap["title"]);
      connect(titleField, SIGNAL(textChanged()), this, SLOT(fieldResize()));
      titleField->show();
      titleField->setHtml(generateFormattedHtml(((HalfTitle*)section)->title, Qt::AlignCenter, 1));
      connect(titleField, SIGNAL(textChanged()), renderer, SLOT(updateSection()));
      fields.insert("title", titleField);

      QFrame* line = new QFrame();
      line->setFrameShape(QFrame::HLine);
      line->setFrameShadow(QFrame::Plain);

      layout.addWidget(titleField);
      layout.addWidget(line);

      layout.addStretch(3);
      break;
    }

    case SectionType::TITLE: {
      layout.addStretch(2);

      SwissTextEdit* titleField = new SwissTextEdit(this);
      titleField->setFont(section->fontMap["title"]);
      connect(titleField, SIGNAL(textChanged()), this, SLOT(fieldResize()));
      titleField->show();
      titleField->setHtml(generateFormattedHtml(((Title*)section)->title, Qt::AlignCenter, 1));
      connect(titleField, SIGNAL(textChanged()), renderer, SLOT(updateSection()));
      fields.insert("title", titleField);

      QFrame* line = new QFrame();
      line->setFrameShape(QFrame::HLine);
      line->setFrameShadow(QFrame::Plain);

      SwissTextEdit* authorField = new SwissTextEdit(this);
      authorField->setFont(section->fontMap["author"]);
      connect(authorField, SIGNAL(textChanged()), this, SLOT(fieldResize()));
      authorField->setHtml(generateFormattedHtml(((Title*)section)->author, Qt::AlignCenter, 1));
      connect(authorField, SIGNAL(textChanged()), renderer, SLOT(updateSection()));
      fields.insert("author", authorField);

      layout.addWidget(titleField);
      layout.addWidget(line);
      layout.addWidget(authorField);

      layout.addStretch(3);
      break;
    }

    case SectionType::COPYRIGHT: {
      layout.addStretch(2);

      SwissTextEdit* copyrightField = new SwissTextEdit(this);
      copyrightField->setFont(section->fontMap["copyright"]);
      copyrightField->setPlainText(((Copyright*)section)->contents);
      connect(copyrightField, SIGNAL(textChanged()), renderer, SLOT(updateSection()));
      fields.insert("copyright", copyrightField);
      layout.addWidget(copyrightField);

      layout.addStretch(1);
      break;
    }

    case SectionType::TABLE_OF_CONTENTS: {
      tableOfContents = renderer->tableOfContents();
      QString htmlTable = generateTocHtml(tableOfContents, contentIdx);
      startIndex = contentIdx;

      if (contentIdx == 0) {
        layout.addStretch(1);

        SwissTextEdit* title = new SwissTextEdit(this);
        title->setFont(section->fontMap["tableOfContentsTitle"]);
        connect(title, SIGNAL(textChanged()), this, SLOT(fieldResize()));
        title->setHtml(generateFormattedHtml(((TableOfContents*)section)->title, Qt::AlignCenter));
        connect(title, SIGNAL(textChanged()), renderer, SLOT(updateSection()));
        fields.insert("title", title);

        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Plain);

        layout.addWidget(title);
        layout.addWidget(line);

        layout.addStretch(1);
      }

      SwissTextEdit* tableOfContentsField = new SwissTextEdit(this);
      tableOfContentsField->setFont(section->fontMap["tableOfContents"]);
      tableOfContentsField->setHtml(htmlTable);
      tableOfContentsField->setTextInteractionFlags(Qt::NoTextInteraction);
      fields.insert("tableOfContents", tableOfContentsField);
      layout.addWidget(tableOfContentsField, 12);

      break;
    }

    case SectionType::CHAPTER: {
      if (contentIdx == 0) {
        layout.addStretch(1);

        SwissTextEdit* chapterNumberField = new SwissTextEdit(this);
        chapterNumberField->setFont(section->fontMap["chapterNumber"]);
        chapterNumberField->setTextInteractionFlags(Qt::NoTextInteraction);
        connect(chapterNumberField, SIGNAL(textChanged()), this, SLOT(fieldResize()));
        int chapterNumber = renderer->calculateChapterNumber(index);
        chapterNumberField->setPlainText(QString("Chapter ") + QString::number(chapterNumber));
        chapterNumberField->setAlignment(Qt::AlignCenter);

        SwissTextEdit* chapterNameField = new SwissTextEdit(this);
        chapterNameField->setFont(section->fontMap["chapterName"]);
        chapterNameField->show();
        connect(chapterNameField, SIGNAL(textChanged()), this, SLOT(fieldResize()));
        chapterNameField->setPlainText(((Chapter*)section)->name);
        chapterNameField->setAlignment(Qt::AlignCenter);
        connect(chapterNameField, SIGNAL(textChanged()), renderer, SLOT(updateSection()));

        fields.insert("chapterNumber", chapterNumberField);
        fields.insert("chapterName", chapterNameField);

        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Plain);

        layout.addWidget(chapterNumberField);
        layout.addWidget(chapterNameField);
        layout.addWidget(line);

        layout.addStretch(1);
      }

      SwissTextEdit* chapterContentsField = new SwissTextEdit(this);
      chapterContentsField->setFont(section->fontMap["chapterContents"]);
      chapterContentsField->setTabStopDistance(40);
      QString chapterContents = ((Chapter*)section)->contents.mid(
        contentIdx,
        ((Chapter*)section)->contents.size() - contentIdx + 1
      );
      chapterContentsField->setHtml(generateFormattedHtml(chapterContents, Qt::AlignJustify, ((Chapter*)section)->lineSpacing));
      chapterContentsField->setAlignment(Qt::AlignJustify);
      fields.insert("chapterContents", chapterContentsField);
      layout.addWidget(chapterContentsField, 3);
      //connect(chapterContentsField, SIGNAL(textChanged()), renderer, SLOT(updateSection()));
      connect(chapterContentsField, SIGNAL(textChanged()), this, SLOT(sendReload()));

      SwissTextEdit* pageNumberField = new SwissTextEdit(this);
      pageNumberField->setFont(section->fontMap["pageNumber"]);
      pageNumberField->setTextInteractionFlags(Qt::NoTextInteraction);
      connect(pageNumberField, SIGNAL(textChanged()), this, SLOT(fieldResize()));
      pageNumberField->setPlainText(QString::number(page));
      pageNumberField->show();

      if (direction == LEFT) {
        pageNumberField->setAlignment(Qt::AlignLeft);
      } else {
        pageNumberField->setAlignment(Qt::AlignRight);
      }

      fields.insert("pageNumber", pageNumberField);
      layout.addWidget(pageNumberField);

      bottom -= fields["pageNumber"]->height();

      break;
    }
  }

  setContentsMargins(left, top, right, bottom);
}

QString PageRenderer::generateTocHtml(QList<QPair<QString, int>> tableOfContents, int start, int end) {
  QString formattedTable = "<table width=\"100%\">";
  QString rowTemplate = "<tr><td>%1</td><td align=\"right\">%2</td></tr>";
  if (end == -1) end = tableOfContents.size();
  for (int i = start; i < end; i++) {
    formattedTable += rowTemplate.arg(tableOfContents[i].first).arg(tableOfContents[i].second);
  }
  formattedTable += "</table>";
  return formattedTable;
}

QString PageRenderer::generateFormattedHtml(QString raw, Qt::Alignment alignment, float lineSpacing) {
  QString align = "left";
  switch (alignment) {
    case Qt::AlignRight: {
      align = "right";
      break;
    }

    case Qt::AlignCenter: {
      align = "center";
      break;
    }

    case Qt::AlignJustify: {
      align = "justify";
      break;
    }
  }


  QString formatted = fieldHtmlTemplate
    .arg(lineSpacing)
    .arg(align)
    .arg(raw.replace("\n", QString("</p><p align=\"%1\">").arg(align)));

  return formatted;
}

int PageRenderer::truncate() {
  SwissTextEdit* field;
  QString contents;
  int lineSpacing = 1;

  switch (section->type()) {
    case SectionType::TABLE_OF_CONTENTS: {
      field = fields["tableOfContents"];
      QTextCursor cursor = field->cursorForPosition(
        QPoint(
          field->width() - 1,
          field->height() - 1
        )
      );

      int lastLine = (cursor.blockNumber() - 1) / 2;

      field->setHtml(generateTocHtml(tableOfContents, startIndex, lastLine));
      return lastLine + 1;
      break;
    }

    case SectionType::CHAPTER: {
      field = fields["chapterContents"];
      contents = field->toPlainText();
      lineSpacing = ((Chapter*)section)->lineSpacing;
      break;
    }

    default:
      return 1;
  }

  //disconnect(field, SIGNAL(textChanged()), renderer, SLOT(updateSection()));
  disconnect(field, SIGNAL(textChanged()), this, SLOT(sendReload()));

  field->verticalScrollBar()->setValue(0);

  QFontMetrics fontMetrics(field->currentFont());
  QTextCursor cursor = field->textCursor();
  int endIndex;

  if (field->viewport()->size().height() < field->document()->documentLayout()->documentSize().height()) {
    cursor.movePosition(QTextCursor::Start);
    field->setTextCursor(cursor);
    QKeyEvent keyEvent(QEvent::KeyPress, Qt::Key_PageDown, Qt::NoModifier);
    QCoreApplication::sendEvent(field, &keyEvent);
    cursor = field->textCursor();
    field->verticalScrollBar()->setValue(0);
    if (field->cursorRect().bottom() > field->viewport()->size().height()) cursor.movePosition(QTextCursor::Up);
  } else {
    cursor.movePosition(QTextCursor::End);
  }

  endIndex = cursor.position();
  if (endIndex < contents.size() && contents.at(endIndex) == '\n') {
    endIndex++;
  }

  contents = contents.left(endIndex);
  field->setHtml(generateFormattedHtml(contents, Qt::AlignJustify, ((Chapter*)section)->lineSpacing));

  QByteArray textData = field->toPlainText().toUtf8();
  checksum = qChecksum(textData.data(), textData.size());

  //connect(field, SIGNAL(textChanged()), renderer, SLOT(updateSection()));
  connect(field, SIGNAL(textChanged()), this, SLOT(sendReload()));

  this->endIndex = endIndex;
  return endIndex;
}

bool PageRenderer::requiresRerender() {
  SwissTextEdit* field = fields["chapterContents"];
  QByteArray textData = field->toPlainText().toUtf8();
  quint16 newChecksum = qChecksum(textData.data(), textData.size());

  return checksum != newChecksum;
}

int PageRenderer::contentSize() {
  return endIndex;
}

int PageRenderer::pageNumber() {
  return page;
}

PageDirection PageRenderer::direction() {
  return dir;
}

int PageRenderer::restoreCursor(int position) {
  SwissTextEdit* field;
  bool canOverflow;

  switch (section->type()) {
    case SectionType::HALFTITLE: {
      field = fields["title"];
      canOverflow = false;
      break;
    }
    case SectionType::TITLE: {
      field = fields["title"];
      canOverflow = false;
      break;
    }
    case SectionType::COPYRIGHT: {
      field = fields["copyright"];
      canOverflow = false;
      break;
    }
    case SectionType::TABLE_OF_CONTENTS: {
      field = fields["tableOfContents"];
      canOverflow = true;
      break;
    }
    case SectionType::CHAPTER: {
      field = fields["chapterContents"];
      canOverflow = true;
      break;
    }
    default: {
      qDebug() << "Invalid SectionType @ PageRenderer::restoreCursor:" << section->type();
      throw new std::runtime_error("Invalid SectionType @ PageRenderer::restoreCursor");
    }
  }

  int overflow = 0;
  int textSize = field->toPlainText().size();
  QTextCursor cursor = field->textCursor();

  if (canOverflow && (textSize < position)) {
    overflow = position - textSize;
    cursor.movePosition(QTextCursor::End);
    field->setTextCursor(cursor);
  } else {
    cursor.setPosition(position);
    field->setTextCursor(cursor);
  }

  field->verticalScrollBar()->setValue(0);

  return overflow;
}

void PageRenderer::fieldResize() {
  SwissTextEdit* field = (SwissTextEdit*)sender();
  QFontMetrics metrics(field->currentFont());
  int width = model->size.width() * PPI * .75f - 20;
  int height = model->size.height() * PPI;
  QRect rect = metrics.boundingRect(QRect(0, 0, width, height),
      Qt::AlignVCenter | Qt::TextWordWrap, field->toPlainText());
  field->setFixedHeight(rect.height() + 5);
}

void PageRenderer::sendReload() {
  QMetaObject::invokeMethod(renderer, "reloadSection", Qt::QueuedConnection, Q_ARG(int, page));
}
