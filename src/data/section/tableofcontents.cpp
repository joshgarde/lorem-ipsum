#include "tableofcontents.h"

TableOfContents::TableOfContents(QList<Section*>* sections) : Section() {
  this->type = "tableofcontents";
  this->sections = sections;
}

QString TableOfContents::getType() {
  return this->type;
}

QJsonObject TableOfContents::serialize() {
  
}
