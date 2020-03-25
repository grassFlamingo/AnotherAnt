#ifndef ANTSTATUSBOARD_H
#define ANTSTATUSBOARD_H

#include <QCompleter>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QWidget>

#include "antutils.h"

namespace Ui {
class AntStatusBoard;
}

class AntStatusBoard : public QWidget {
  Q_OBJECT

 public:
  explicit AntStatusBoard(QWidget *parent = nullptr);
  ~AntStatusBoard();

 private:
  /**
   * @brief path_compitition
   * if path is end with '/' list is sub-directories
   * else show its bast match
   * @param path
   */
  QStringList path_compitition(const QString &path);

 private slots:
  void on_saveButton_clicked();
  void on_workspaceEdit_textEdited(const QString &path);
  void on_outspaceEdit_textEdited(const QString &path);

  void on_workspaceEdit_editingFinished();

 private:
  Ui::AntStatusBoard *ui;
  QPalette mPassPalete;
  QPalette mWarnPalete;
};

#endif // ANTSTATUSBOARD_H
