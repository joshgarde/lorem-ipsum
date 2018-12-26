#include "title.h"

Title::Title(SectionModel *parent) : Section(parent) {
  setObjectName("Title");

  fontMap["title"] = parent->fontMap["title"];
  fontMap["author"] = parent->fontMap["author"];

  title = "Lorem Ipsum";
  author = "The Open Source Software Foundation";
}

SectionType Title::type() {
  return SectionType::TITLE;
}

bool Title::multiplePages() {
  return false;
}

QJsonObject Title::serialize() {
  QJsonObject json;
  json["title"] = title;
  json["author"] = author;
  return json;
}

void Title::deserialize(QJsonObject json) {
  title = json["title"].toString();
  author = json["author"].toString();
}
