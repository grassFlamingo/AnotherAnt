#ifndef MAINWINDOW_H
#define MAINWINDOW_H

<<<<<<< HEAD
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

=======
#include <QDebug>
#include <QFileDialog>
#include <QKeyEvent>
#include <QLinkedList>
#include <QMainWindow>
#include <QMessageBox>

#include <QDir>

#include "antutils.h"
#include "linkedring.h"
#include "widgetclick.h"

>>>>>>> 111ec3a... display image
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

<<<<<<< HEAD
=======
 protected:
  void keyPressEvent(QKeyEvent *event) override;

 private:
  void load_file_list();
  void load_images();

>>>>>>> 111ec3a... display image
 private slots:
  void on_loadButton_clicked();

 private:
  Ui::MainWindow *ui;
  QString mPath;
  QStringList mFileList;

<<<<<<< HEAD
=======
  LinkedRing<QPixmap> mPixRing;
  int mPixIndex;

  inline QString next_piximage_path(int i = 1) {
    mPixIndex = (mPixIndex + i + mFileList.size()) % mFileList.size();
    return Ant::path_join(mPath, mFileList[mPixIndex]);
  }
>>>>>>> 111ec3a... display image
};
#endif // MAINWINDOW_H
