#ifndef HALFTITLE_H
#define HALFTITLE_H

#include "section.h"
#include "sectionmodel.h"

class HalfTitle : public Section {
  Q_OBJECT

public:
  HalfTitle(SectionModel *parent = nullptr);
  QString title;
  SectionType type();
  bool multiplePages();
  QJsonObject serialize();
  void deserialize(QJsonObject json);
};

#endif
