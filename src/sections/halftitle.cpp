#include "halftitle.h"

HalfTitle::HalfTitle(SectionModel *parent) : Section(parent) {
  setObjectName("Half Title");

  fontMap["title"] = parent->fontMap["title"];

  title = "Ultricies Mi Eget";
}

SectionType HalfTitle::type() {
  return SectionType::HALFTITLE;
}

bool HalfTitle::multiplePages() {
  return false;
}

QJsonObject HalfTitle::serialize() {
  QJsonObject json;
  json["title"] = title;
  return json;
}

void HalfTitle::deserialize(QJsonObject json) {
  
}
