#ifndef COPYRIGHT_H
#define COPYRIGHT_H

#include "section.h"

class Copyright : public Section {
public:
  Copyright() {
    this->type = "copyright";
  }
};

#endif
