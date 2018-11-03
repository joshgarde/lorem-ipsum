#include "chapter.h"

Chapter::Chapter(QObject *parent) : Section(parent) {
  this->setObjectName("Chapter");
}

Chapter::Chapter(const Chapter& copy) : Section(copy.parent()),
    title(copy.title), contents(copy.contents) {
  this->setObjectName("Chapter");
}

QJsonObject Chapter::serialize() {

}
