#include "chapter.h"

Chapter::Chapter(SectionModel *parent) : Section(parent) {
  setObjectName("Chapter");

  fontMap["chapterNumber"] = parent->fontMap["chapterNumber"];
  fontMap["chapterName"] = parent->fontMap["chapterName"];
  fontMap["chapterContents"] = parent->fontMap["chapterContents"];

  name = "Lorem Ipsum Dolor Sit Amet";

  contents = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
    "eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad "
    "minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip "
    "ex ea commodo consequat. Duis aute irure dolor in reprehenderit in "
    "voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur "
    "sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt "
    "mollit anim id est laborum.";
}

SectionType Chapter::type() {
  return SectionType::CHAPTER;
}

int Chapter::pages() {
  return 0;
}

QJsonObject Chapter::serialize() {

}
