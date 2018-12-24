#ifndef PAGERENDERER_H
#define PAGERENDERER_H

#include <QWidget>
#include <QSize>
#include <QVBoxLayout>
#include "swisstextedit.h"
#include "bookrenderer.h"
#include "src/sections/section.h"

#define PPI 96.0f

class BookRenderer;

class PageRenderer : public QWidget {
  Q_OBJECT

public:
  enum PageDirection {
    RIGHT = 0,
    LEFT = 1
  };

  BookRenderer* renderer;
  SectionModel* model;
  QModelIndex index;
  Section* section;
  QVBoxLayout layout;
  quint16 checksum;
  QList<QPair<QString, int>> tableOfContents;
  PageDirection direction;
  int page;
  int startIndex = 0;
  int endIndex = 0;
  bool initialized = false;

  static const QString fieldHtmlTemplate;

  QString generateTocHtml(QList<QPair<QString, int>> tableOfContents, int start, int end = -1);
  QString generateFormattedHtml(QString raw, Qt::Alignment alignment, int lineSpacing = 1);
public:
  QMap<QString, SwissTextEdit*> fields;

  PageRenderer(BookRenderer* renderer, SectionModel* model, QModelIndex index, int page = 0, int contentIdx = 0, PageDirection direction = RIGHT);
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
