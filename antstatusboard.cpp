#include "antstatusboard.h"
#include "ui_antstatusboard.h"

AntStatusBoard::AntStatusBoard(QWidget *parent)
    : QWidget(parent), ui(new Ui::AntStatusBoard) {
  ui->setupUi(this);
  setWindowTitle("Ant Status Board");
  ui->workspaceEdit->setText(Ant::ac.workspace());
  ui->outspaceEdit->setText(Ant::ac.outspace());
  ui->cropsizeEdit->setText(Ant::ac.cropsize());

  mPassPalete = ui->workspaceEdit->palette();
  mWarnPalete.setColor(QPalette::Base, QColor(255, 70, 70, 100));
}

AntStatusBoard::~AntStatusBoard(){
  delete ui;
}

/**
 * @brief path_compitition
 * if path is end with '/' list is sub-directories
 * else show its bast match
 * @param path
 */
QStringList AntStatusBoard::path_compitition(const QString &path) {
  if (path.endsWith(QDir::separator())) {
    // list all subpaths
    QDir dp(path);
    QStringList out =
        dp.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    for (QString &s : out) {
      s = Ant::path_join(path, s);
    }
    return out;
  }
  QString mpath = path.section(QDir::separator(), 0, -2);
  QString mname = path.section(QDir::separator(), -1);
  mname.append('*');
  qDebug() << "mpath" << mpath << "mname" << mname;
  QDir dp(mpath);
  QStringList out = dp.entryList(QStringList(mname),
                                 QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
  for (QString &s : out) {
    s = Ant::path_join(mpath, s);
  }
  return out;
}

void AntStatusBoard::on_saveButton_clicked() {
  Ant::ac.saveConfigure();
  close();
}

void AntStatusBoard::on_workspaceEdit_textEdited(const QString &path) {
  if (QFile::exists(path)) {
    ui->workspaceEdit->setPalette(mPassPalete);
  } else {
    ui->workspaceEdit->setPalette(mWarnPalete);
    return;
  }
  QStringList paths = path_compitition(path);
  qDebug() << paths;
  QCompleter *cmp = new QCompleter(paths, this);
  //  cmp->setCompletionMode(QCompleter::PopupCompletion);
  ui->workspaceEdit->setCompleter(cmp);
}

void AntStatusBoard::on_outspaceEdit_textEdited(const QString &path) {
  QString tpath = QFileDialog::getExistingDirectory(
      this, "open a directory", path, QFileDialog::ShowDirsOnly);
  Ant::ac.setOutspace(tpath);
}

void AntStatusBoard::on_workspaceEdit_editingFinished() {
  QString tpath = ui->workspaceEdit->text();
  if (QFile::exists(tpath)) {
    Ant::ac.setWorkspace(tpath);
    ui->workspaceEdit->setPalette(mPassPalete);
  } else {
    ui->workspaceEdit->setPalette(mWarnPalete);
  }
}
