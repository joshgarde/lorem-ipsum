#include "copyright.h"

Copyright::Copyright(QObject *parent) : Section(parent) {
  this->setObjectName("Copyright");
}

Copyright::Copyright(const Copyright& copy) : Section(copy.parent()),
    contents(copy.contents) {
  this->setObjectName("Copyright");
}

QJsonObject Copyright::serialize() {

}
