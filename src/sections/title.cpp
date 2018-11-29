#include "title.h"

Title::Title(SectionModel *parent) : Section(parent) {
  setObjectName("Title");

  fontMap["title"] = parent->fontMap["title"];

  title = "Ultricies Mi Eget";
}

SectionType Title::type() {
  return SectionType::TITLE;
}

bool Title::multiplePages() {
  return false;
}

QJsonObject Title::serialize() {

}
