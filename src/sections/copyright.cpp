#include "copyright.h"

Copyright::Copyright(SectionModel *parent) : Section(parent) {
  setObjectName("Copyright");

  fontMap["copyright"] = parent->fontMap["copyright"];

  contents = "Copyright © 2010 [NAME]\n"
    "All rights reserved. This book or any portion thereof "
    "may not be reproduced or used in any manner whatsoever "
    "without the express written permission of the publisher "
    "except for the use of brief quotations in a book review."
    "\n"
    "\nTypeset with Lorem Ipsum - an open source typesetter.";
}

SectionType Copyright::type() {
  return SectionType::COPYRIGHT;
}

bool Copyright::multiplePages() {
  return false;
}

QJsonObject Copyright::serialize() {
  QJsonObject json;
  json["contents"] = contents;
  return json;
}

void Copyright::deserialize(QJsonObject json) {
  contents = json["contents"].toString();
}
