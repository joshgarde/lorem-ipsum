#ifndef BACKSCROLLTEXTEDIT_H
#define BACKSCROLLTEXTEDIT_H

#include <QTextEdit>
#include <QWheelEvent>

class BackscrollTextEdit : public QTextEdit {
  Q_OBJECT

protected:
  void wheelEvent(QWheelEvent *e);
  void insertFromMimeData(const QMimeData *source);
public:
  BackscrollTextEdit(QWidget* parent);
};

#endif
