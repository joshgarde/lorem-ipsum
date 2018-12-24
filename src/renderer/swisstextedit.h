#ifndef SWISSTEXTEDIT_H
#define SWISSTEXTEDIT_H

#include <QTextEdit>
#include <QWheelEvent>

class SwissTextEdit : public QTextEdit {
  Q_OBJECT

public:
  SwissTextEdit(QWidget* parent);
  void ensureCursorVisible();
protected:
  void wheelEvent(QWheelEvent *e);
  void insertFromMimeData(const QMimeData *source);
  void timerEvent(QTimerEvent *e);
  void mousePressEvent(QMouseEvent *e);
};

#endif
