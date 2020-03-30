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

 private slots:
  void on_saveButton_clicked();
  void on_workspaceEdit_textEdited(const QString &path);
  void on_outspaceEdit_textEdited(const QString &path);
  void on_zoomfBox_valueChanged(double zf);

  void on_logfileEdit_textChanged(const QString &arg1);

 private:
  Ui::AntStatusBoard *ui;
  QPalette mPassPalete;
  QPalette mWarnPalete;
  QCompleter mDirComp;
  QCompleter mFileComp;
  QFileSystemModel mFsmDir;
  QFileSystemModel mFsmFile;
};

#endif // ANTSTATUSBOARD_H
