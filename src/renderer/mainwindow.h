#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QSplitter>
#include <QListView>
#include <QToolBar>
#include <QComboBox>
#include <QFontComboBox>
#include <QStatusBar>
#include <QAction>
#include <QFile>
#include "bookrenderer.h"
#include "src/sections/sectionmodel.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

  QFile* currentFile;
  SectionModel* currentBook;
  QModelIndex currentSectionIdx;

  QWidget base;
  QVBoxLayout layout;
  QToolBar toolbar;
  QFontComboBox fontSelector;
  QComboBox sizeSelector;
  QStatusBar statusbar;
  QSplitter splitter;
  QListView tableOfContents;
  BookRenderer viewer;

  QAction* newBookAction;
  QAction* openBookAction;
  QAction* saveBookAction;
  QAction* renderBookAction;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QSize sizeHint() const;

signals:

public slots:
  void newBook();
  void openBook();
  void saveBook();
  void renderBook();
  void showSectionMenu(QPoint pos);
  void showAddSection();
  void addSection(QString section);
  void deleteSection();
  void loadSection(const QModelIndex &index);
  void reloadTocItem();
};

#endif // MAINWINDOW_H
