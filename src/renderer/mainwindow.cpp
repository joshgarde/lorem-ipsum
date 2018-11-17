#include "mainwindow.h"
#include <QDebug>
#include <QIcon>
#include <QFileDialog>
#include <QMenu>
#include <QListWidgetItem>
#include "addsectiondialog.h"
#include "src/sections/section.h"
#include "src/sections/chapter.h"
#include "src/sections/copyright.h"
#include "src/sections/tableofcontents.h"
#include "src/sections/title.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  this->setWindowTitle("Ipsum Lorem");

  base.setLayout(&layout);
  base.setStyleSheet(
    "QToolButton::hover:!pressed {background: grey;}"
    "QToolButton::pressed {background: darkgrey;}"
  );

  layout.setContentsMargins(0,0,0,0);
  layout.setSpacing(0);
  layout.addWidget(&toolbar);
  layout.addWidget(&splitter);
  layout.addWidget(&statusbar);

  fontSelector.setCurrentFont(QFont("Times New Roman"));

  for (int i = 1; i <= 120; i++) {
    sizeSelector.addItem(QString::number(i));
  }
  sizeSelector.setCurrentIndex(11); // Sets default font size to 12
  sizeSelector.setEditable(true);

  toolbar.setMaximumHeight(32);
  newBookAction = toolbar.addAction(QIcon(":/assets/new.png"), "New Book");
  openBookAction = toolbar.addAction(QIcon(":/assets/open.png"), "Open Book");
  saveBookAction = toolbar.addAction(QIcon(":/assets/save.png"), "Save Book");
  toolbar.addSeparator();
  toolbar.addWidget(&fontSelector);
  toolbar.addWidget(&sizeSelector);
  statusbar.setMaximumHeight(statusbar.minimumHeight());

  saveBookAction->setEnabled(false);

  tableOfContents.setContextMenuPolicy(Qt::CustomContextMenu);

  splitter.addWidget(&tableOfContents);
  splitter.addWidget(&viewer);
  splitter.setSizes(QList<int>({splitter.width() / 10 * 2, splitter.width() / 10 * 8}));
  splitter.setOrientation(Qt::Orientation::Horizontal);

  this->setCentralWidget(&base);

  connect(newBookAction, SIGNAL(triggered()), this, SLOT(newBook()));
  connect(openBookAction, SIGNAL(triggered()), this, SLOT(openBook()));
  connect(&tableOfContents, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showSectionMenu(QPoint)));
  connect(&tableOfContents, SIGNAL(activated(const QModelIndex&)), this, SLOT(loadSection(const QModelIndex&)));
}

QSize MainWindow::sizeHint() const {
  return QSize(1200, 600);
}

void MainWindow::newBook() {
  QString filename = QFileDialog::getSaveFileName(this, "Create a New Book", "", "JSON Book Format (*.json)");
  if (filename == "") return;
  if (!filename.endsWith(".json")) filename.append(".json");
  qDebug() << "[DEBUG] Creating new book:" << filename.toUtf8();
  currentFile = new QFile(filename);
  currentFile->open(QIODevice::ReadWrite);
  currentBook = new SectionModel(QSize(6, 9));
  tableOfContents.setModel(currentBook);
  saveBookAction->setEnabled(true);
  this->setWindowTitle("Ipsum Lorem - " + filename.toUtf8());
}

void MainWindow::openBook() {
  printf("Test\n");
}

void MainWindow::showSectionMenu(QPoint pos) {
  if (currentFile == nullptr) return;
  QMenu menu(this);
  menu.addAction(QIcon(":/assets/add.png"), "Add Section", this, SLOT(showAddSection()));
  menu.addAction(QIcon(":/assets/delete.png"), "Delete Section", this, SLOT(deleteSection()));
  menu.exec(mapToGlobal(pos));
}

void MainWindow::showAddSection() {
  AddSectionDialog dialog(this);
  dialog.exec();
}

void MainWindow::addSection(QString section) {
  qDebug() << "[DEBUG] Adding section:" << section;
  int idx = currentBook->rowCount();
  QModelIndex modelIdx = currentBook->index(idx, 0);
  currentBook->insertRow(idx);
  if (section == "Title") {
    currentBook->setData(modelIdx, QVariant::fromValue(new Title(currentBook)), 0);
  } else if (section == "Copyright") {
    currentBook->setData(modelIdx, QVariant::fromValue(new Copyright(currentBook)), 0);
  } else if (section == "Table of Contents") {
    currentBook->setData(modelIdx, QVariant::fromValue(new TableOfContents(currentBook)), 0);
  } else if (section == "Chapter") {
    currentBook->setData(modelIdx, QVariant::fromValue(new Chapter(currentBook)), 0);
  } else {
    throw std::runtime_error("Unimplemented section selection");
  }
}

void MainWindow::deleteSection() {

}

void MainWindow::loadSection(const QModelIndex &index) {
  Section* section = currentBook->data(index, Qt::UserRole).value<Section*>();
  QMetaObject::invokeMethod(&viewer, "loadSection", Q_ARG(Section*, section));
}
