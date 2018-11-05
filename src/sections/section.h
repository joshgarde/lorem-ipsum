#ifndef SECTION_H
#define SECTION_H

#include <QObject>
#include <QJsonObject>
#include "sectionmodel.h"

class SectionModel;

enum SectionType {
  CHAPTER, COPYRIGHT, TABLE_OF_CONTENTS, TITLE
};

class Section : public QObject {
  Q_OBJECT

public:
  Section(SectionModel *parent = nullptr) : QObject((QObject*) parent) { }
  inline SectionModel* parent() {
    return (SectionModel*)QObject::parent();
  }
  virtual SectionType type() = 0;
  virtual int pages() = 0;
  virtual QJsonObject serialize() = 0;
};

#endif
