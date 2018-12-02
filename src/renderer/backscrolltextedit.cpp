#include "backscrolltextedit.h"
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
