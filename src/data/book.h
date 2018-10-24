#ifndef BOOK_H
#define BOOK_H

#include "section.h"

class Book : public Serializable {
public:
  QString title;
  QString author;
  QList<Section*> sections;
  QJsonObject serializable();
};

#endif
