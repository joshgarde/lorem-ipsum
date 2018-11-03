#ifndef TITLE_H
#define TITLE_H

#include "section.h"

class Title : public Section {
  QString title;
public:
  Title(QObject *parent = nullptr, QString* title);
  QJsonObject serialize();
};

Q_DECLARE_METATYPE(Title)

#endif
