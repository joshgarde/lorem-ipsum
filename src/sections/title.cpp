#include "title.h"

Title::Title(SectionModel *parent) : Section(parent) {
  setObjectName("Title");
  fontMap.insert("title", DefaultFont());
}

SectionType Title::type() {
  return SectionType::TITLE;
}

int Title::pages() {
  return 1;
}

QJsonObject Title::serialize() {

}
