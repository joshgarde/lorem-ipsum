#include "copyright.h"

Copyright::Copyright(SectionModel *parent) : Section(parent) {
  setObjectName("Copyright");
}

SectionType Copyright::type() {
  return SectionType::COPYRIGHT;
}

int Copyright::pages() {
  return 1;
}

QJsonObject Copyright::serialize() {

}
