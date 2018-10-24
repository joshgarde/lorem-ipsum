#ifndef TABLEOFCONTENTS_H
#define TABLEOFCONTENTS_H

#include "src/data/section.h"

class TableOfContents : public Section {
  QString type = "tableofcontents";
  QList<Section*>* sections;
public:
  TableOfContents(QList<Section*>* sections);
  QString getType();
  QJsonObject serialize();
};

#endif
