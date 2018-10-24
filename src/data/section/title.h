#ifndef TITLE_H
#define TITLE_H

#include "src/data/section.h"

class Title : public Section {
  QString type = "title";
  QString* title;
public:
  Title(QString* title);
  QString getType();
  QJsonObject serialize();
};

#endif
