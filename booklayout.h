#ifndef BOOKLAYOUT_H
#define BOOKLAYOUT_H
#include <QPlainTextDocumentLayout>
#include <QTextDocument>

class BookLayout : public QPlainTextDocumentLayout {
  Q_OBJECT
public:
  BookLayout(QTextDocument* doc);
  void draw(QPainter *, const PaintContext &);
};

#endif // BOOKLAYOUT_H
