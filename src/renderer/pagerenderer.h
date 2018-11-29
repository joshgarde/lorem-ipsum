#ifndef PAGERENDERER_H
#define PAGERENDERER_H

#include <QWidget>
#include <QSize>
#include <QVBoxLayout>
#include "backscrolltextedit.h"
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
  int page;
  bool initialized = false;

  static const QString chapterContentsHtml;
public:
  QMap<QString, BackscrollTextEdit*> fields;
  PageRenderer(Section* section, BookRenderer* renderer, int page, int contentIdx = 0, int cursorPosition = 0);
  int truncate();

signals:
  void textBoxSelected();
  void rerenderRequired();

public slots:
  void fieldResize();
  void sendReload();
};

#endif
