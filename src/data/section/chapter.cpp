#include "chapter.h"

Chapter::Chapter() : Section() {
  this->type = "chapter";
}

QString Chapter::getType() {
  return this->type;
}

QJsonObject Chapter::serialize() {
  
}
