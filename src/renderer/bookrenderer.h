#ifndef BOOKVIEWER_H
#define BOOKVIEWER_H

#include <QScrollArea>

class BookViewer : public QScrollArea {
  Q_OBJECT
  const int MARGIN_TOP = 30;
  const int MARGIN_BOTTOM = 30;
  const int PAGE_WIDTH = 816;
  const int PAGE_HEIGHT = 1224;

  double scale = 1;
public:
  explicit BookViewer(QWidget *parent = nullptr);
  void paintEvent(QPaintEvent* event);

signals:

public slots:
};

#endif // BOOKVIEWER_H
