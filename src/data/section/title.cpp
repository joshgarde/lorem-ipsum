#include "title.h"

Title::Title(QString* title) : Section() {
  this->type = "title";
  this->title = title;
}

QString Title::getType() {
  return this->type;
}

QJsonObject Title::serialize() {
  
}
