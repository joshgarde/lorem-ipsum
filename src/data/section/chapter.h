#ifndef CHAPTER_H
#define CHAPTER_H

#include "src/data/section.h"

class Chapter : public Section {
  QString type;
public:
  QString title;
  QString contents;
  Chapter();
  QString getType();
  QJsonObject serialize();
};

#endif
