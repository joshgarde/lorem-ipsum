#ifndef BOOKRENDERER_H
#define BOOKRENDERER_H

#include <QScrollArea>

class BookRenderer : public QScrollArea {
  Q_OBJECT
  const int MARGIN_TOP = 30;
  const int MARGIN_BOTTOM = 30;
  const int PAGE_WIDTH = 816;
  const int PAGE_HEIGHT = 1224;

  double scale = 1;
  QWidget* scrollPane;
public:
  BookRenderer(QWidget *parent = nullptr);
  void paintEvent(QPaintEvent* event);

signals:

public slots:
  void addSection(QString section);
};

#endif // BOOKRENDERER_H
