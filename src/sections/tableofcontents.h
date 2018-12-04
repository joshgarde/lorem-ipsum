#ifndef TABLEOFCONTENTS_H
#define TABLEOFCONTENTS_H

#include "section.h"
#include "sectionmodel.h"

class TableOfContents : public Section {
  Q_OBJECT
public:
  QString title;

  TableOfContents(SectionModel *parent = nullptr);
  SectionType type();
  bool multiplePages();
  QJsonObject serialize();
};

#endif
