#ifndef TITLE_H
#define TITLE_H

#include "section.h"

class Title : public Section {
public:
  Title() {
    this->type = "title";
  }
};

#endif
