#include "mainwindow.h"
#include <QDebug>
#include <QIcon>
#include <QFileDialog>
#include <QMenu>
#include "addsectiondialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  this->setWindowTitle("Ipsum Lorem");

  base.setLayout(&layout);
  base.setStyleSheet("");
  
  layout.setContentsMargins(0,0,0,0);
  layout.setSpacing(0);
  layout.addWidget(&toolbar);
  layout.addWidget(&splitter);
  layout.addWidget(&statusbar);
  
  toolbar.setMaximumHeight(32);
  newBookAction = toolbar.addAction(QIcon("assets/new.png"), "New Book");
  openBookAction = toolbar.addAction(QIcon("assets/open.png"), "Open Book");
  saveBookAction = toolbar.addAction(QIcon("assets/save.png"), "Save Book");
  toolbar.addSeparator();
  previousPageAction = toolbar.addAction(QIcon("assets/previous.png"), "Previous Page");
  nextPageAction = toolbar.addAction(QIcon("assets/next.png"), "Next Page");
  statusbar.setMaximumHeight(statusbar.minimumHeight());
  
  saveBookAction->setEnabled(false);
  previousPageAction->setEnabled(false);
  nextPageAction->setEnabled(false);
  
  tableOfContents.setContextMenuPolicy(Qt::CustomContextMenu);
  
  splitter.addWidget(&tableOfContents);
  splitter.addWidget(&viewer);
  splitter.setSizes(QList<int>({splitter.width() / 10 * 2, splitter.width() / 10 * 8}));
  splitter.setOrientation(Qt::Orientation::Horizontal);

  this->setCentralWidget(&base);
  
  connect(newBookAction, SIGNAL(triggered()), this, SLOT(newBook()));
  connect(openBookAction, SIGNAL(triggered()), this, SLOT(openBook()));
  connect(&tableOfContents, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showSectionMenu(QPoint)));
}

QSize MainWindow::sizeHint() const {
  return QSize(1200, 600);
}

void MainWindow::newBook() {
  QString filename = QFileDialog::getSaveFileName(this, "Create a New Book", "", "JSON Book Format (*.json)");
  if (!filename.endsWith(".json")) filename.append(".json");
  qDebug() << "Creating new book @" << filename.toUtf8();
  currentFile = new QFile(filename);
  currentFile->open(QIODevice::ReadWrite);
  currentBook = new Book();
  saveBookAction->setEnabled(true);
  previousPageAction->setEnabled(true);
  nextPageAction->setEnabled(true);
  this->setWindowTitle("Ipsum Lorem - " + filename.toUtf8());
}

void MainWindow::openBook() {
  printf("Test\n");
}

void MainWindow::showSectionMenu(QPoint pos) {
  if (currentFile == nullptr) return;
  QMenu menu(this);
  menu.addAction(QIcon("assets/add.png"), "Add Section", this, SLOT(showAddSection()));
  menu.addAction(QIcon("assets/delete.png"), "Delete Section", this, SLOT(deleteSection()));
  menu.exec(mapToGlobal(pos));
}

void MainWindow::showAddSection() {
  AddSectionDialog dialog(this);
  dialog.exec();
}

void MainWindow::addSection(QString section) {
  qDebug() << "[DEBUG] Adding section: " << section;
}

void MainWindow::deleteSection() {
  
}
