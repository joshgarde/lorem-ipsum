#ifndef BOOKRENDERER_H
#define BOOKRENDERER_H

#include <QScrollArea>
#include <QVBoxLayout>
#include "pagerenderer.h"
#include "src/sections/section.h"

class PageRenderer;

class BookRenderer : public QScrollArea {
  enum PageDirection {LEFT, RIGHT};

  Q_OBJECT

  QWidget scrollPane;
  QVBoxLayout layout;
  QList<PageRenderer*> renderers;
  Section* currentSection;
  double scale = 1;
  int basePageNumber;

  void reset();
public:
  BookRenderer(QWidget *parent = nullptr);
  void renderSection(QPagedPaintDevice* paintDevice, QPainter* painter, Section* section);

signals:
  void updateToc();

public slots:
  void loadSection(Section* section, int currentPage = -1);
  void updateSection();
};

#endif // BOOKRENDERER_H
