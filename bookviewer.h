#ifndef BOOKVIEWER_H
#define BOOKVIEWER_H

#include <QPlainTextEdit>
#include "booklayout.h"

class BookViewer : public QTextEdit
{
  Q_OBJECT
  const int MARGIN_TOP = 30;
  const int MARGIN_BOTTOM = 30;
  const int PAGE_WIDTH = 816;
  const int PAGE_HEIGHT = 1224;

  double scale = 1;
  BookLayout* layout;
public:
  explicit BookViewer(QWidget *parent = nullptr);
  void paintEvent(QPaintEvent* event);

signals:

public slots:
};

#endif // BOOKVIEWER_H
