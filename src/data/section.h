#ifndef SECTION_H
#define SECTION_H

#include "serializable.h"

class Section : public Serializable {
public:
  virtual QString getType() = 0;
};

#endif
