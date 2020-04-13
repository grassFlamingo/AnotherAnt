#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QLinkedList>
#include <QMainWindow>
#include <QMessageBox>

#include "antstatusboard.h"
#include "antutils.h"
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

 private slots:
  void on_showBoxButton_clicked();
  void on_antButton_clicked();

  void onDrawPrevClicked();
  void onDrawNextClicked();

  void on_editBox_stateChanged(int state);

  void on_cropButton_clicked();

 private:
  bool load_file_list();
  void load_images();
  void load_annotations();
  void remove_edited_image();

 private:
  Ui::MainWindow *ui;
  QString mPathws;
  QString mPathos;
  Ant::AntEditProxy mEproxy;
  Ant::AntEditProxy::iteratorLoop mEPiter;

  Ant::tuple<int> mCropsize;
  AntStatusBoard mAntBoard;
  bool isEditMode;
};
#endif // MAINWINDOW_H
