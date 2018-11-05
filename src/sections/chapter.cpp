#include "chapter.h"

Chapter::Chapter(SectionModel *parent) : Section(parent) {
  setObjectName("Chapter");
}

SectionType Chapter::type() {
  return SectionType::CHAPTER;
}

int Chapter::pages() {
  return 0;
}

QJsonObject Chapter::serialize() {

}
