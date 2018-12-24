#include "tableofcontents.h"

TableOfContents::TableOfContents(SectionModel *parent) : Section(parent) {
  setObjectName("Table of Contents");

  fontMap["tableOfContentsTitle"] = parent->fontMap["tableOfContentsTitle"];
  fontMap["tableOfContents"] = parent->fontMap["tableOfContents"];

  title = "Contents";
}

SectionType TableOfContents::type() {
  return SectionType::TABLE_OF_CONTENTS;
}

bool TableOfContents::multiplePages() {
  return true;
}

QJsonObject TableOfContents::serialize() {
  QJsonObject json;
  return json;
}

void TableOfContents::deserialize(QJsonObject json) {
  
}
