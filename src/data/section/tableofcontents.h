#ifndef TABLEOFCONTENTS_H
#define TABLEOFCONTENTS_H

#include "section.h"

class TableOfContents : public Section {
public:
  TableOfContents() {
    this->type = "tableofcontents";
  }
};

#endif
