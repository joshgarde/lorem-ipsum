#ifndef CHAPTER_H
#define CHAPTER_H

#include "section.h"
#include "sectionmodel.h"

class Chapter : public Section {
  Q_OBJECT

public:
  QString contents;
  QString name;
  float lineSpacing;

  Chapter(SectionModel *parent = nullptr);
  SectionType type();
  bool multiplePages();
  QJsonObject serialize();
};

#endif
