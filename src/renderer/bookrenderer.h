#ifndef BOOKRENDERER_H
#define BOOKRENDERER_H

#include <QScrollArea>
#include <QVBoxLayout>
#include "pagerenderer.h"
#include "src/sections/section.h"

class PageRenderer;
enum PageDirection : uint8_t;

class BookRenderer : public QScrollArea {
  Q_OBJECT

  QWidget scrollPane;
  QVBoxLayout layout;
  QList<PageRenderer*> renderers;
  SectionModel* model;
  Section* currentSection;
  QModelIndex index;
  QList<QPair<QString, int>> toc;
  double scale = 1;
  int basePageNumber;

  int calculatePages(QModelIndex index);
  int locateStart(int max = -1);
  void generateTableOfContents();
  int calculateTargetIndex(Section* section);

public:
  BookRenderer(QWidget *parent = nullptr);
  void setSectionModel(SectionModel* model);
  int renderSection(QPagedPaintDevice* paintDevice, QPainter* painter, QModelIndex index, int page, PageDirection& direction);
  QList<QPair<QString, int>> tableOfContents();
  int calculateChapterNumber(QModelIndex index);
  void reset();

protected:
  void mousePressEvent(QMouseEvent *event);

signals:
  void updateToc();
  void notifyFont(const QFont& font);

public slots:
  void loadSection(QModelIndex index);
  void updateSection();
  void reloadSection(int currentPage);
  void changeFont(QFont font);
};

#endif // BOOKRENDERER_H
