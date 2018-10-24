#ifndef CHAPTER_H
#define CHAPTER_H

#include "section.h"

class Chapter : public Section {
public:
  Chapter() {
    this->type = "chapter";
  }
};

#endif
