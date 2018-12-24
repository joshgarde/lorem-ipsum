#include "mainwindow.h"
#include <QDebug>
#include <QIcon>
#include <QFileDialog>
#include <QMenu>
#include <QListWidgetItem>
#include <QJsonArray>
#include <QJsonDocument>
#include <QPdfWriter>
#include <QPainter>
#include <QMessageBox>
#include <QMenuBar>
#include "addsectiondialog.h"
#include "src/sections/section.h"
#include "src/sections/chapter.h"
#include "src/sections/copyright.h"
#include "src/sections/tableofcontents.h"
#include "src/sections/title.h"
#include "src/sections/halftitle.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  this->setWindowTitle("Lorem Ipsum");
  
  QMenu* fileMenu = menuBar()->addMenu("File");
  fileMenu->addAction("Open");
  fileMenu->addAction("Save");
  fileMenu->addAction("Render PDF");
  QMenu* editMenu = menuBar()->addMenu("Edit");
  QMenu* optionsMenu = menuBar()->addMenu("Options");
  QMenu* aboutMenu = menuBar()->addMenu("Help");
  aboutMenu->addAction("Update");
  aboutMenu->addAction("About");

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
  toolbar.addSeparator();
  renderBookAction = toolbar.addAction(QIcon(":/assets/render.png"), "Render Book");
  statusbar.setMaximumHeight(statusbar.minimumHeight());

  saveBookAction->setEnabled(false);
  renderBookAction->setEnabled(false);

  tableOfContents.setContextMenuPolicy(Qt::CustomContextMenu);

  splitter.addWidget(&tableOfContents);
  splitter.addWidget(&viewer);
  splitter.setSizes(QList<int>({splitter.width() / 10 * 2, splitter.width() / 10 * 8}));
  splitter.setOrientation(Qt::Orientation::Horizontal);

  this->setCentralWidget(&base);

  connect(newBookAction, SIGNAL(triggered()), this, SLOT(newBook()));
  connect(openBookAction, SIGNAL(triggered()), this, SLOT(openBook()));
  connect(saveBookAction, SIGNAL(triggered()), this, SLOT(saveBook()));
  connect(renderBookAction, SIGNAL(triggered()), this, SLOT(renderBook()));
  connect(&tableOfContents, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showSectionMenu(QPoint)));
  connect(&tableOfContents, SIGNAL(activated(const QModelIndex&)), this, SLOT(loadSection(const QModelIndex&)));
  connect(&viewer, SIGNAL(updateToc()), this, SLOT(reloadTocItem()));
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
  currentBook = new SectionModel();
  tableOfContents.setModel(currentBook);
  viewer.setSectionModel(currentBook);
  saveBookAction->setEnabled(true);
  renderBookAction->setEnabled(true);
  this->setWindowTitle("Ipsum Lorem - " + filename.toUtf8());
}

void MainWindow::openBook() {
  QString filename = QFileDialog::getOpenFileName(this, "Open a Book", "", "JSON Book Format (*.json)");
  if (filename == "") return;
  if (!filename.endsWith(".json")) filename.append(".json");

  qDebug() << "[DEBUG] Opening book: " << filename.toUtf8();
  currentFile = new QFile(filename);
  currentFile->open(QIODevice::ReadWrite);
  currentBook = new SectionModel();

  QJsonDocument document = QJsonDocument::fromJson(currentFile->readAll());
  QJsonObject jsonBook = document.object();

  QJsonObject jsonSize = jsonBook["size"].toObject();
  currentBook->size = QSize(jsonSize["width"].toInt(), jsonSize["height"].toInt());

  QJsonObject jsonFontMap = jsonBook["fontMap"].toObject();
  for (auto it = jsonFontMap.begin(); it != jsonFontMap.end(); it++) {
    QFont font;
    font.fromString(it->toString());
    currentBook->fontMap[it.key()] = font;
  }

  QJsonArray jsonSections = jsonBook["sections"].toArray();
  for (auto it = jsonSections.begin(); it != jsonSections.end(); it++) {
    QJsonObject jsonSection = it->toObject();
    int idx = currentBook->rowCount();
    QModelIndex modelIdx = currentBook->index(idx, 0);
    Section* section;
    QString type = jsonSection["type"].toString();
    currentBook->insertRow(idx);
    if (type == "Half Title") {
      section = new HalfTitle(currentBook);
      ((HalfTitle*)section)->title = jsonSection["title"].toString();
    } else if (type == "Title") {
      section = new Title(currentBook);
      ((Title*)section)->title = jsonSection["title"].toString();
    } else if (type == "Copyright") {
      section = new Copyright(currentBook);
      ((Copyright*)section)->contents = jsonSection["contents"].toString();
    } else if (type == "Table of Contents") {
      section = new TableOfContents(currentBook);
    } else if (type == "Chapter") {
      section = new Chapter(currentBook);
      ((Chapter*)section)->name = jsonSection["name"].toString();
      ((Chapter*)section)->contents = jsonSection["contents"].toString();
    } else {
      throw std::runtime_error("Unimplemented section selection");
    }
    currentBook->setData(modelIdx, QVariant::fromValue(section), 0);
    QJsonObject jsonSectionFontMap = jsonSection["fontMap"].toObject();
    for (auto jt = jsonSectionFontMap.begin(); jt != jsonSectionFontMap.end(); jt++) {
      QFont font;
      font.fromString(jt->toString());
      section->fontMap[jt.key()] = font;
    }
  }

  tableOfContents.setModel(currentBook);
  viewer.setSectionModel(currentBook);
  saveBookAction->setEnabled(true);
  renderBookAction->setEnabled(true);
  this->setWindowTitle("Ipsum Lorem - " + filename.toUtf8());
}

void MainWindow::saveBook() {
  statusbar.showMessage("Saving book...");

  QJsonDocument document = currentBook->serialize();
  
  QTextStream fileStream(currentFile);
  currentFile->resize(0);
  fileStream << document.toJson();

  statusbar.showMessage("Ready");
}

void MainWindow::renderBook() {
  QString filename = QFileDialog::getSaveFileName(this, "Render Book", "", "PDF File (*.pdf)");
  if (filename == "") return;
  if (!filename.endsWith(".pdf")) filename.append(".pdf");

  qDebug() << "[DEBUG] Rendering book: " << filename;
  QPdfWriter pdfWriter(filename);
  pdfWriter.setPageSize(QPageSize(currentBook->size, QPageSize::Inch));
  pdfWriter.setPageMargins(QMarginsF(0, 0, 0, 0));
  pdfWriter.setResolution(96);
  QPainter painter(&pdfWriter);

  int page = 1;
  for (int i = 0; i < currentBook->rowCount(); i++) {
    QModelIndex modelIdx = currentBook->index(i, 0);
    page = viewer.renderSection(&pdfWriter, &painter, modelIdx, page);
    if (i != currentBook->rowCount() - 1) pdfWriter.newPage();
  }

  QMessageBox statusMessage(this);
  statusMessage.setText("Successfully exported to " + filename);
  statusMessage.exec();
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
  if (section == "Half Title") {
    currentBook->setData(modelIdx, QVariant::fromValue(new HalfTitle(currentBook)), 0);
  } else if (section == "Title") {
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
  QMessageBox deleteWarningMessage;
  deleteWarningMessage.setText("WARNING: SECTION DELETION");
  deleteWarningMessage.setInformativeText("Are you sure you want to delete this section?");
  deleteWarningMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
  deleteWarningMessage.setDefaultButton(QMessageBox::Cancel);
  int ret = deleteWarningMessage.exec();

  if (ret == QMessageBox::Yes) {
    QModelIndex index = tableOfContents.selectionModel()->currentIndex();
    currentBook->removeRows(index.row(), 1);
  }
}

void MainWindow::loadSection(const QModelIndex &index) {
  currentSectionIdx = index;
  QMetaObject::invokeMethod(&viewer, "loadSection", Q_ARG(QModelIndex, index));
}

void MainWindow::reloadTocItem() {
  QMetaObject::invokeMethod(currentBook, "markDirty", Q_ARG(QModelIndex, currentSectionIdx));
}
