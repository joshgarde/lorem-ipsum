#ifndef CHAPTER_H
#define CHAPTER_H

#include "section.h"

class Chapter : public Section {
  QString type;
public:
  QString title;
  QString contents;
  Chapter(QObject *parent = nullptr);
  Chapter(const Chapter& copy);
  QJsonObject serialize();
};

Q_DECLARE_METATYPE(Chapter)

#endif
