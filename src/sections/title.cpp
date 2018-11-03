#include "title.h"

Title::Title(QString title) : Section() {
  this->setObjectName("Title");
  this->title = title;
}

QJsonObject Title::serialize() {

}
