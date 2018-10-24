#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <QJsonObject>

class Serializable {
  virtual QJsonObject serialize() = 0;
};

#endif
