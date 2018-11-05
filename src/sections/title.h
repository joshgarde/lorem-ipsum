#ifndef TITLE_H
#define TITLE_H

#include "section.h"
#include "sectionmodel.h"

class Title : public Section {
  Q_OBJECT
  
  QString title;
public:
  Title(SectionModel *parent = nullptr);
  SectionType type();
  int pages();
  QJsonObject serialize();
};

#endif
