#include "swisstextedit.h"
#include <QScrollBar>
#include <QMimeData>
#include <QApplication>
#include <QDebug>

SwissTextEdit::SwissTextEdit(QWidget* parent) : QTextEdit(parent) {
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setFrameStyle(0);
}

void SwissTextEdit::wheelEvent(QWheelEvent *e) {
  QWheelEvent* event = new QWheelEvent(*e);
  QApplication::postEvent(parent(), event);
}

void SwissTextEdit::insertFromMimeData(const QMimeData* source) {
  if (source->hasHtml()) {
    QTextDocument doc;
    doc.setHtml(source->html());
    
    QMimeData* newSource = new QMimeData();
    newSource->setText(doc.toPlainText()
      .replace(QRegExp("\n"), "\n\t")
    );
    
    Qt::Alignment previousAlignment = alignment();
    QTextEdit::insertFromMimeData(newSource);
    setAlignment(previousAlignment);
    emit textChanged();
  } else {
    QTextEdit::insertFromMimeData(source);
  }
}

void SwissTextEdit::ensureCursorVisible() {
  qDebug() << "ensureCursorVisible";
}

void SwissTextEdit::timerEvent(QTimerEvent *e) {
  Q_UNUSED(e)
  return; // Bypass QTextEdit's autoscroll system
}

void SwissTextEdit::mousePressEvent(QMouseEvent *e) {
  QCoreApplication::sendEvent(parent(), e); // Forward mouse events to parent
  QTextEdit::mousePressEvent(e);
}
