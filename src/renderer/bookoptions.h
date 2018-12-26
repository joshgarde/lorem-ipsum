#ifndef BOOKOPTIONS_H
#define BOOKOPTIONS_H
#include <QDialog>
#include <QObject>
#include <QFormLayout>
#include <QFontComboBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QStringList>
#include <QDialogButtonBox>
#include <QPushButton>
#include "src/sections/sectionmodel.h"

class BookOptions : public QDialog {
  Q_OBJECT

  SectionModel* model;
  QVBoxLayout layout;
  QFormLayout formLayout;
  QDialogButtonBox buttonBox;
  
  QHBoxLayout titleFontLayout;
  QFontComboBox titleFontSelector;
  QComboBox titleSizeSelector;
  
  QHBoxLayout authorFontLayout;
  QFontComboBox authorFontSelector;
  QComboBox authorSizeSelector;
  
  QHBoxLayout copyrightFontLayout;
  QFontComboBox copyrightFontSelector;
  QComboBox copyrightSizeSelector;
  
  QHBoxLayout tocHeaderFontLayout;
  QFontComboBox tocHeaderFontSelector;
  QComboBox tocHeaderSizeSelector;
  
  QHBoxLayout tocFontLayout;
  QFontComboBox tocFontSelector;
  QComboBox tocSizeSelector;
  
  QHBoxLayout chapterNumberFontLayout;
  QFontComboBox chapterNumberFontSelector;
  QComboBox chapterNumberSizeSelector;
  
  QHBoxLayout chapterNameFontLayout;
  QFontComboBox chapterNameFontSelector;
  QComboBox chapterNameSizeSelector;
  
  QHBoxLayout chapterFontLayout;
  QFontComboBox chapterFontSelector;
  QComboBox chapterSizeSelector;
  
  QHBoxLayout pageNumberFontLayout;
  QFontComboBox pageNumberFontSelector;
  QComboBox pageNumberSizeSelector;
  
  QDoubleSpinBox chapterLineSpacingField;
  
  QStringList generateSizeList();
  void setFontSelection(QFontComboBox& fontBox, QComboBox& sizeBox, QFont& font);
  QFont convertSelection(QFontComboBox& fontBox, QComboBox& sizeBox);
public:
  BookOptions(SectionModel* model, QWidget* parent = nullptr);
public slots:
  void saveAndClose();
  void apply();
};

#endif
