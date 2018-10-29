#ifndef ADDSECTIONDIALOG_H
#define ADDSECTIONDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QListWidget>
#include <QString>
#include <QPushButton>

class AddSectionDialog : public QDialog {
  Q_OBJECT
  QVBoxLayout layout;
  QListWidget list;
  QPushButton button;
public:
  AddSectionDialog(QWidget *parent = nullptr);

signals:

public slots:
  void addBtnPushed();
};

#endif // ADDSECTIONDIALOG_H
