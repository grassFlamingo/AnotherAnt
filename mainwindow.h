#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  inline void initLoad() {
    if (load_file_list()) {
      load_images();
    }
  }

 protected:
  void keyPressEvent(QKeyEvent *event) override;

 private:
  bool load_file_list();
  void load_images();

 private slots:
  void on_loadButton_clicked();
  void ondrawPrev_clecked();
  void ondrawNext_clecked();

  void on_cropButton_clicked();

 private:
  Ui::MainWindow *ui;
  QString mPath;
  QStringList mFileList;

  LinkedRing<QPixmap> mPixRing;
  int mPixIndex;

  inline int next_piximage_index(int i = 1) {
    mPixIndex = (mPixIndex + i + mFileList.size()) % mFileList.size();
    return mPixIndex;
  }

  inline QString curr_piximage_path() {
    return Ant::path_join(mPath, mFileList[mPixIndex]);
  }
};
#endif // MAINWINDOW_H
