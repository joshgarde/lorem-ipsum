#include "title.h"

Title::Title(SectionModel *parent) : Section(parent) {
  setObjectName("Title");
  QFont font("Times New Roman");
  font.setPixelSize(36);
  fontMap.insert("title", font);
}

SectionType Title::type() {
  return SectionType::TITLE;
}

int Title::pages() {
  return 1;
}

QJsonObject Title::serialize() {

}
