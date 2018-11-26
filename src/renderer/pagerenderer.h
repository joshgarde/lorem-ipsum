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
  Q_OBJECT

  SectionModel* model;
  Section* section;
  BookRenderer* renderer;
  QVBoxLayout layout;
  QMap<QString, QTextEdit*> fields;
  int page;
public:
  PageRenderer(Section* section, int page, BookRenderer* renderer);

signals:
  void textBoxSelected();
  void rerenderRequired();

public slots:
  void fieldResize();
  void rerenderCheck();
};

#endif
