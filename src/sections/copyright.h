#ifndef COPYRIGHT_H
#define COPYRIGHT_H

#include "section.h"
#include "sectionmodel.h"

class Copyright : public Section {
  Q_OBJECT

public:
  QString contents;
  Copyright(SectionModel *parent = nullptr);
  SectionType type();
  int pages();
  QJsonObject serialize();
};

#endif
