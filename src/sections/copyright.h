#ifndef COPYRIGHT_H
#define COPYRIGHT_H

#include "section.h"

class Copyright : public Section {
  QString type = "copyright";
public:
  QString contents;
  Copyright(QObject *parent = nullptr);
  Copyright(const Copyright& copy);
  QJsonObject serialize();
};

Q_DECLARE_METATYPE(Copyright)

#endif
