#ifndef BOOKRENDERER_H
#define BOOKRENDERER_H

#include <QScrollArea>
#include <QVBoxLayout>
#include "pagerenderer.h"
#include "src/sections/section.h"

class PageRenderer;

class BookRenderer : public QScrollArea {
  Q_OBJECT

  QWidget scrollPane;
  QVBoxLayout layout;
  QList<PageRenderer*> renderers;
  double scale = 1;
  Section* currentSection;
  
  void reset();
public:
  BookRenderer(QWidget *parent = nullptr);

signals:

public slots:
  void loadSection(Section* section);
};

#endif // BOOKRENDERER_H
