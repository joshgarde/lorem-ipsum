#include "tableofcontents.h"

TableOfContents::TableOfContents(SectionModel *parent) : Section(parent) {
  setObjectName("Table of Contents");
}

TableOfContents::~TableOfContents() {
  delete sections;
}

SectionType TableOfContents::type() {
  return SectionType::TABLE_OF_CONTENTS;
}

int TableOfContents::pages() {
  return 0;
}

QJsonObject TableOfContents::serialize() {

}
