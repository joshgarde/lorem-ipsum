#include "copyright.h"

Copyright::Copyright() : Section() {
  this->type = "copyright";
}

QString Copyright::getType() {
  return this->type;
}

QJsonObject Copyright::serialize() {
  
}
