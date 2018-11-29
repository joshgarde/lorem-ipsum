#include "backscrolltextedit.h"
#include <QApplication>
#include <QDebug>

BackscrollTextEdit::BackscrollTextEdit(QWidget* parent) : QTextEdit(parent) {

}

void BackscrollTextEdit::wheelEvent(QWheelEvent *e) {
  QWheelEvent* event = new QWheelEvent(*e);
  QApplication::postEvent(parent(), event);
}
