#ifndef CHAPTER_H
#define CHAPTER_H

#include "section.h"
#include "sectionmodel.h"

class Chapter : public Section {
  Q_OBJECT

public:
  QString title;
  QString contents;
  Chapter(SectionModel *parent = nullptr);
  SectionType type();
  int pages();
  QJsonObject serialize();
};

#endif
