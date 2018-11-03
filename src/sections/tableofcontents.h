#ifndef TABLEOFCONTENTS_H
#define TABLEOFCONTENTS_H

#include "section.h"

class TableOfContents : public Section {
  QString type = "tableofcontents";
  QList<Section*>* sections;
public:
  TableOfContents(QList<Section*>* sections);
  TableOfContents(const TableOfContents& copy);
  QJsonObject serialize();
};

Q_DECLARE_METATYPE(TableOfContents)

#endif
