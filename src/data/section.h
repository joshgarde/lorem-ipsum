#ifndef SECTION_H
#define SECTION_H

#include "serializable.h"

class Section : public Serializable {
public:
  QString type;
  QJsonObject serialize();
};

#endif
