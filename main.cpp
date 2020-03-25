#include "mainwindow.h"

#include <QApplication>
#include "antutils.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QString rpath(argv[0]);
  int si = rpath.lastIndexOf('/');
  rpath.chop(rpath.length() - si);
  rpath.append("/ant-conf.json");
  Ant::ac.loadConfigure(rpath);

  MainWindow w;
  w.show();
  w.initLoad();

  return a.exec();
}
