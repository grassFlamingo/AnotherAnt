#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QFileDialog>
#include <QKeyEvent>
#include <QLinkedList>
#include <QMainWindow>
#include <QMessageBox>
#include <QDir>

#include "antstatusboard.h"
#include "antutils.h"
#include "linkedring.h"
#include "widgetclick.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void initLoad();

 protected:
  void keyPressEvent(QKeyEvent *event) override;

 private:
  bool load_file_list();
  void load_images();

 private slots:
  void on_loadButton_clicked();
  void on_cropLabel_editingFinished();
  void on_showBoxButton_clicked();
  void on_antButton_clicked();

  void onDrawPrevClicked();
  void onDrawNextClicked();

 private:
  inline int next_piximage_index(int i = 1) {
    mPixIndex = (mPixIndex + i + mFileList.size()) % mFileList.size();
    return mPixIndex;
  }

  inline QString curr_piximage_path() {
    return Ant::path_join(mPath, mFileList[mPixIndex]);
  }

 private:
  Ui::MainWindow *ui;
  QString mPath;
  QStringList mFileList;

  LinkedRing<QPixmap> mPixRing;
  int mPixIndex;
  Ant::tuple<int> mCropsize;
  AntStatusBoard mAntBoard;
};
#endif // MAINWINDOW_H
