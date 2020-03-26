#ifndef ANTSTATUSBOARD_H
#define ANTSTATUSBOARD_H

#include <QCompleter>
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QMouseEvent>
#include <QStringList>
#include <QStringListModel>

#include "anttinymessage.h"
#include "antutils.h"

namespace Ui {
class AntStatusBoard;
}

class AntStatusBoard : public QDialog {
  Q_OBJECT

 public:
  explicit AntStatusBoard(QWidget *parent = nullptr);
  ~AntStatusBoard();

 protected:
  void mousePressEvent(QMouseEvent *event) override;

 private slots:
  void on_saveButton_clicked();
  void on_workspaceEdit_textEdited(const QString &path);
  void on_outspaceEdit_textEdited(const QString &path);
  void on_zoomfBox_valueChanged(double zf);

 private:
  Ui::AntStatusBoard *ui;
  QPalette mPassPalete;
  QPalette mWarnPalete;
  QCompleter mCompleter;
  QFileSystemModel mFsm;
};

#endif // ANTSTATUSBOARD_H
