#ifndef COPYRIGHT_H
#define COPYRIGHT_H

#include "src/data/section.h"

class Copyright : public Section {
  QString type = "copyright";
public:
  QString contents;
  Copyright();
  QString getType();
  QJsonObject serialize();
};

#endif
