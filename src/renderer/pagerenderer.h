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

  BookRenderer* renderer;
  SectionModel* model;
  Section* section;
  QVBoxLayout layout;
  quint16 checksum;
  int page;
  int endIndex = 0;
  bool initialized = false;

  static const QString chapterContentsHtml;
public:
  QMap<QString, BackscrollTextEdit*> fields;
  PageRenderer(BookRenderer* renderer, SectionModel* model, Section* section, int page = 0, int contentIdx = 0);
  bool requiresRerender();
  int contentSize();
  int pageNumber();
  int truncate();
  int restoreCursor(int position);

signals:
  void textBoxSelected();
  void rerenderRequired();

public slots:
  void fieldResize();
  void sendReload();
};

#endif
