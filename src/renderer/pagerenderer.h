#ifndef PAGERENDERER_H
#define PAGERENDERER_H

#include <QWidget>
#include <QSize>
#include <QVBoxLayout>
#include <QTextEdit>
#include "bookrenderer.h"
#include "src/sections/section.h"

#define PPI 96.0f

class BookRenderer;

class PageRenderer : public QWidget {
  SectionModel* model;
  Section* section;
  QVBoxLayout layout;
  QList<QTextEdit*> fields;
  int page;
public:  
  PageRenderer(Section* section, int page, BookRenderer* renderer);
  QSize sizeHint() const;
};

#endif
