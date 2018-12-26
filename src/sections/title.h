#ifndef TITLE_H
#define TITLE_H

#include "section.h"
#include "sectionmodel.h"

class Title : public Section {
  Q_OBJECT

public:
  Title(SectionModel *parent = nullptr);
  QString title;
  QString author;
  SectionType type();
  bool multiplePages();
  QJsonObject serialize();
  void deserialize(QJsonObject json);
};

#endif
