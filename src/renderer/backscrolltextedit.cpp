#include "backscrolltextedit.h"
#include <QScrollBar>
#include <QMimeData>
#include <QApplication>
#include <QDebug>

BackscrollTextEdit::BackscrollTextEdit(QWidget* parent) : QTextEdit(parent) {
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setFrameStyle(0);
}

void BackscrollTextEdit::wheelEvent(QWheelEvent *e) {
  QWheelEvent* event = new QWheelEvent(*e);
  QApplication::postEvent(parent(), event);
}

void BackscrollTextEdit::insertFromMimeData(const QMimeData* source) {
  if (source->hasHtml()) {
    QMimeData* newSource = new QMimeData();
    newSource->setText(source->html().replace(QRegExp("</p>"), "\n\t").remove(QRegExp("<[^>]*>")));
    delete source;
    Qt::Alignment previousAlignment = alignment();
    QTextEdit::insertFromMimeData(newSource);
    setAlignment(previousAlignment);
    emit textChanged();
  } else {
    QTextEdit::insertFromMimeData(source);
  }
}
