#ifndef SECTION_H
#define SECTION_H

#include <QObject>
#include <QJsonObject>
#include <QMap>
#include <QString>
#include <QFont>
#include "sectionmodel.h"
class SectionModel;

enum SectionType {
  CHAPTER, COPYRIGHT, TABLE_OF_CONTENTS, TITLE, HALFTITLE
};

class Section : public QObject {
  Q_OBJECT

public:
  Section(SectionModel *parent = nullptr) : QObject((QObject*) parent) { }

  inline SectionModel* parent() {
    return (SectionModel*)QObject::parent();
  }

  QMap<QString, QFont> fontMap;
  virtual SectionType type() = 0;
  virtual bool multiplePages() = 0;
  virtual QJsonObject serialize() = 0;
  virtual void deserialize(QJsonObject json) = 0;
};

#endif
