#ifndef TABLEOFCONTENTS_H
#define TABLEOFCONTENTS_H

#include "section.h"
#include "sectionmodel.h"

class TableOfContents : public Section {
  Q_OBJECT

  QList<Section*>* sections;
public:
  TableOfContents(SectionModel *parent = nullptr);
  ~TableOfContents();
  SectionType type();
  bool multiplePages();
  QJsonObject serialize();
};

#endif
