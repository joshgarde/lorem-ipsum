#include "bookoptions.h"
#include <QDebug>

BookOptions::BookOptions(SectionModel* model, QWidget* parent) :
    QDialog(parent),
    model(model),
    layout(this),
    buttonBox(QDialogButtonBox::Save | QDialogButtonBox::Apply | QDialogButtonBox::Cancel) {
  setWindowTitle("Book Options");
  
  layout.addLayout(&formLayout);
  layout.addWidget(&buttonBox);
  
  QStringList sizeList = generateSizeList();
  
  titleFontLayout.addWidget(&titleFontSelector);
  titleFontLayout.addWidget(&titleSizeSelector);
  titleSizeSelector.addItems(sizeList);
  titleSizeSelector.setEditable(true);
  setFontSelection(titleFontSelector, titleSizeSelector, model->fontMap["title"]);
  formLayout.addRow("Title Font", &titleFontLayout);
  
  authorFontLayout.addWidget(&authorFontSelector);
  authorFontLayout.addWidget(&authorSizeSelector);
  authorSizeSelector.addItems(sizeList);
  authorSizeSelector.setEditable(true);
  setFontSelection(authorFontSelector, authorSizeSelector, model->fontMap["author"]);
  formLayout.addRow("Author Font", &authorFontLayout);
  
  copyrightFontLayout.addWidget(&copyrightFontSelector);
  copyrightFontLayout.addWidget(&copyrightSizeSelector);
  copyrightSizeSelector.addItems(sizeList);
  copyrightSizeSelector.setEditable(true);
  setFontSelection(copyrightFontSelector, copyrightSizeSelector, model->fontMap["copyright"]);
  formLayout.addRow("Copyright Font", &copyrightFontLayout);
  
  tocHeaderFontLayout.addWidget(&tocHeaderFontSelector);
  tocHeaderFontLayout.addWidget(&tocHeaderSizeSelector);
  tocHeaderSizeSelector.addItems(sizeList);
  tocHeaderSizeSelector.setEditable(true);
  setFontSelection(tocHeaderFontSelector, tocHeaderSizeSelector, model->fontMap["tableOfContentsTitle"]);
  formLayout.addRow("Table of Contents Header Font", &tocHeaderFontLayout);
  
  tocFontLayout.addWidget(&tocFontSelector);
  tocFontLayout.addWidget(&tocSizeSelector);
  tocSizeSelector.addItems(sizeList);
  tocSizeSelector.setEditable(true);
  setFontSelection(tocFontSelector, tocSizeSelector, model->fontMap["tableOfContents"]);
  formLayout.addRow("Table of Contents Font", &tocFontLayout);
  
  chapterNumberFontLayout.addWidget(&chapterNumberFontSelector);
  chapterNumberFontLayout.addWidget(&chapterNumberSizeSelector);
  chapterNumberSizeSelector.addItems(sizeList);
  chapterNumberSizeSelector.setEditable(true);
  setFontSelection(chapterNumberFontSelector, chapterNumberSizeSelector, model->fontMap["chapterNumber"]);
  formLayout.addRow("Chapter Number Font", &chapterNumberFontLayout);
  
  chapterNameFontLayout.addWidget(&chapterNameFontSelector);
  chapterNameFontLayout.addWidget(&chapterNameSizeSelector);
  chapterNameSizeSelector.addItems(sizeList);
  chapterNameSizeSelector.setEditable(true);
  setFontSelection(chapterNameFontSelector, chapterNameSizeSelector, model->fontMap["chapterName"]);
  formLayout.addRow("Chapter Name Font", &chapterNameFontLayout);
  
  chapterFontLayout.addWidget(&chapterFontSelector);
  chapterFontLayout.addWidget(&chapterSizeSelector);
  chapterSizeSelector.addItems(sizeList);
  chapterSizeSelector.setEditable(true);
  setFontSelection(chapterFontSelector, chapterSizeSelector, model->fontMap["chapterContents"]);
  formLayout.addRow("Chapter Font", &chapterFontLayout);
  
  pageNumberFontLayout.addWidget(&pageNumberFontSelector);
  pageNumberFontLayout.addWidget(&pageNumberSizeSelector);
  pageNumberSizeSelector.addItems(sizeList);
  pageNumberSizeSelector.setEditable(true);
  setFontSelection(pageNumberFontSelector, pageNumberSizeSelector, model->fontMap["pageNumber"]);
  formLayout.addRow("Page Number Font", &pageNumberFontLayout);
  
  chapterLineSpacingField.setValue(model->optionMap["chapterLineSpacing"].value<float>());
  formLayout.addRow("Chapter Line Spacing", &chapterLineSpacingField);
  
  connect(buttonBox.button(QDialogButtonBox::Cancel), SIGNAL(clicked(bool)), this, SLOT(close()));
  connect(buttonBox.button(QDialogButtonBox::Apply), SIGNAL(clicked(bool)), this, SLOT(apply()));
  connect(buttonBox.button(QDialogButtonBox::Save), SIGNAL(clicked(bool)), this, SLOT(saveAndClose()));
}

QStringList BookOptions::generateSizeList() {
  QStringList list;
  for (int i = 1; i <= 120; i++) {
    list.append(QString::number(i));
  }
  return list;
}

void BookOptions::setFontSelection(QFontComboBox& fontBox, QComboBox& sizeBox, QFont& font) {
  int index = fontBox.findText(font.family());
  fontBox.setCurrentIndex(index);
  sizeBox.setCurrentIndex(font.pixelSize() - 1);
}

QFont BookOptions::convertSelection(QFontComboBox& fontBox, QComboBox& sizeBox) {
  QFont font = fontBox.currentFont();
  font.setPixelSize(sizeBox.currentIndex() + 1);
  return font;
}

void BookOptions::saveAndClose() {
  model->fontMap["title"] = convertSelection(titleFontSelector, titleSizeSelector);
  model->fontMap["author"] = convertSelection(authorFontSelector, authorSizeSelector);
  model->fontMap["copyright"] = convertSelection(copyrightFontSelector, copyrightSizeSelector);
  model->fontMap["tableOfContentsTitle"] = convertSelection(tocHeaderFontSelector, tocHeaderSizeSelector);
  model->fontMap["tableOfContents"] = convertSelection(tocFontSelector, tocSizeSelector);
  model->fontMap["chapterNumber"] = convertSelection(chapterNumberFontSelector, chapterNumberSizeSelector);
  model->fontMap["chapterName"] = convertSelection(chapterNameFontSelector, chapterNameSizeSelector);
  model->fontMap["chapterContents"] = convertSelection(chapterFontSelector, chapterSizeSelector);
  model->fontMap["pageNumber"] = convertSelection(pageNumberFontSelector, pageNumberSizeSelector);
  model->optionMap["chapterLineSpacing"] = QVariant(chapterLineSpacingField.value());
  close();
}

void BookOptions::apply() {
  model->fontMap["title"] = convertSelection(titleFontSelector, titleSizeSelector);
  model->fontMap["author"] = convertSelection(authorFontSelector, authorSizeSelector);
  model->fontMap["copyright"] = convertSelection(copyrightFontSelector, copyrightSizeSelector);
  model->fontMap["tableOfContentsTitle"] = convertSelection(tocHeaderFontSelector, tocHeaderSizeSelector);
  model->fontMap["tableOfContents"] = convertSelection(tocFontSelector, tocSizeSelector);
  model->fontMap["chapterNumber"] = convertSelection(chapterNumberFontSelector, chapterNumberSizeSelector);
  model->fontMap["chapterName"] = convertSelection(chapterNameFontSelector, chapterNameSizeSelector);
  model->fontMap["chapterContents"] = convertSelection(chapterFontSelector, chapterSizeSelector);
  model->fontMap["pageNumber"] = convertSelection(pageNumberFontSelector, pageNumberSizeSelector);
  model->optionMap["chapterLineSpacing"] = QVariant(chapterLineSpacingField.value());
  for (int i = 0; i < model->rowCount(); i++) {
    Section* section = model->data(model->index(i, 0), Qt::UserRole).value<Section*>();
    switch (section->type()) {
      case SectionType::HALFTITLE: {
        section->fontMap["title"] = model->fontMap["title"];
        break;
      }
      
      case SectionType::TITLE: {
        section->fontMap["title"] = model->fontMap["title"];
        section->fontMap["author"] = model->fontMap["author"];
        break;
      }
      
      case SectionType::COPYRIGHT: {
        section->fontMap["copyright"] = model->fontMap["copyright"];
        break;
      }
      
      case SectionType::TABLE_OF_CONTENTS: {
        section->fontMap["tableOfContentsTitle"] = model->fontMap["tableOfContentsTitle"];
        section->fontMap["tableOfContents"] = model->fontMap["tableOfContents"];
        break;
      }
      
      case SectionType::CHAPTER: {
        section->fontMap["chapterNumber"] = model->fontMap["chapterNumber"];
        section->fontMap["chapterName"] = model->fontMap["chapterName"];
        section->fontMap["chapterContents"] = model->fontMap["chapterContents"];
        section->fontMap["pageNumber"] = model->fontMap["pageNumber"];
        break;
      }
      
      default: {
        qDebug() << "[Error] Unknown SectionType @ BookOptions::apply()";
        throw new std::runtime_error("Unknown SectionType @ BookOptions::apply()");
      }
    }
  }
}
