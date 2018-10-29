#ifndef BOOK_H
#define BOOK_H

#include <QList>
#include "section.h"

class Book : public Serializable {
public:
  QString title;
  QString author;
  QList<Section*> sections;
  QJsonObject serialize();
};

#endif
