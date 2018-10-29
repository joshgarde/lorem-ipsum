#include "addsectiondialog.h"
#include <QMetaObject>

AddSectionDialog::AddSectionDialog(QWidget *parent) : QDialog(parent), button("Add") {
  QDialog::setWindowTitle("Add Section");
  
  list.addItem("Title");
  list.addItem("Copyright");
  list.addItem("Table of Contents");
  list.addItem("Chapter");  
  
  layout.addWidget(&list);
  layout.addWidget(&button);

  setLayout(&layout);
  
  connect(&button, SIGNAL(clicked(bool)), this, SLOT(addBtnPushed()));
}

void AddSectionDialog::addBtnPushed() {
  QString selection = list.currentItem()->text();
  QMetaObject::invokeMethod(parent(), "addSection", Qt::QueuedConnection, Q_ARG(QString, selection));
  this->done(0);
}
