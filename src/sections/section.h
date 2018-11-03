#ifndef SECTION_H
#define SECTION_H

#include <QObject>
#include <QJsonObject>

class Section : public QObject {
  Q_OBJECT

public:
  Section(QObject *parent = nullptr) : QObject(parent) { }
  virtual QJsonObject serialize() = 0;
};

#endif
