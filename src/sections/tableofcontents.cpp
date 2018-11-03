#include "tableofcontents.h"

TableOfContents::TableOfContents(QList<Section*>* sections) : Section() {
  this->setObjectName("TableOfContents");
  this->sections = sections;
}

TableOfContents::TableOfContents(const TableOfContents& copy) :
    Section(copy.parent()), sections(copy.sections) {

}

QJsonObject TableOfContents::serialize() {

}
