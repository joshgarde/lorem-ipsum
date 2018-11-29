#ifndef BACKSCROLLTEXTEDIT_H
#define BACKSCROLLTEXTEDIT_H

#include <QTextEdit>
#include <QWheelEvent>

class BackscrollTextEdit : public QTextEdit {
protected:
  void wheelEvent(QWheelEvent *e);
public:
  BackscrollTextEdit(QWidget* parent);
};

#endif
