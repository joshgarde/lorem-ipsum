#ifndef BOOKRENDERER_H
#define BOOKRENDERER_H

#include <QScrollArea>
#include <QVBoxLayout>
#include "pagerenderer.h"
#include "src/sections/section.h"

class PageRenderer;

class BookRenderer : public QScrollArea {
  enum PageDirection {
    LEFT = 0,
    RIGHT = 1
  };

  Q_OBJECT

  QWidget scrollPane;
  QVBoxLayout layout;
  QList<PageRenderer*> renderers;
  SectionModel* model;
  Section* currentSection;
  double scale = 1;
  int basePageNumber;

  void reset();
  QMap<QString, int> generateTableOfContents();
public:
  BookRenderer(QWidget *parent = nullptr);
  void setSectionModel(SectionModel* model);
  void renderSection(QPagedPaintDevice* paintDevice, QPainter* painter, Section* section);

signals:
  void updateToc();

public slots:
  void loadSection(QModelIndex index);
  void updateSection();
  void reloadSection(int currentPage);
};

#endif // BOOKRENDERER_H
