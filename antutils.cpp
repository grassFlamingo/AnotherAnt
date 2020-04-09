#include "antutils.h"

using namespace Ant;
using Ant::AntConfigure;

QStringList Ant::listImageNames(const QString& root, const QDir& sub) {
  QDir path(root);
  if (!sub.isEmpty()) {
    path.cd(sub.absolutePath());
  }
  path.setNameFilters(Ant::Image_Suffix);
  return path.entryList(QDir::Files, QDir::Name);
}

QStringList Ant::listSubfolders(const QString& root, const QDir& sub) {
  QDir path(root);
  if (!sub.isEmpty()) {
    path.cd(sub.absolutePath());
  }
  return path.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
}

QString Ant::path_join(const QString& parent, const QString& sub) {
  QString out(parent);
  if (!parent.endsWith("/")) {
    out.append('/');
  }
  out.append(sub);
  return out;
}

/************ Ant Configure ***************/

AntConfigure AntConfigure::mSelf = AntConfigure();

AntConfigure::AntConfigure() {}

/**
 * @brief loadConfigure
 * @param filename
 * @return -1 file not found; -2 not a json file;  -3 josn not correct; > 0
 * succeed
 */
int AntConfigure::loadConfigure(const QString& filename) {
  QFile conf(filename);
  if (!conf.exists()) {
    assert(QFile::copy(":/json/ant-default.json", filename));
    QFile::setPermissions(filename,
                          QFileDevice::ReadOwner | QFileDevice::WriteOwner);
  }

  if (!conf.open(QIODevice::ReadOnly)) {
    qDebug() << "cannot open" << filename;
    return -1;
  }
  QByteArray array = conf.readAll();
  conf.close();

  QJsonParseError e;
  auto doc = QJsonDocument::fromJson(array, &e);
  if (e.error != QJsonParseError::NoError || doc.isNull()) {
    return -2;
  }
  mSelf.mJsonObj = doc.object();
  mSelf.mFileName = QString(filename);

  // out space is not exisis
  QString os = mSelf.outspace();
  if (os.isEmpty() || !QFile::exists(os)) {
    QDir().mkpath(os);
  }
  return true;
}

int AntConfigure::saveConfigure() {
  QFile f(mFileName);
  if (!f.open(QIODevice::WriteOnly)) {
    qDebug() << "cannot open file" << mFileName;
    return -1;
  }
  QJsonDocument doc(mJsonObj);
  QByteArray ba = doc.toJson(QJsonDocument::Indented);
  f.write(ba);
  f.close();
  return ba.length();
}

/************************** AntEditProxy **************************/

AntEditProxy::AntEditProxy() : mLogFile(NULL), mCounter(0) {}

AntEditProxy::~AntEditProxy() {
  if (mLogFile != NULL) {
    std::fclose(mLogFile);
    mLogFile = NULL;
  }
}

void AntEditProxy::loadFromAntConfig() {
  setOutspace(ac.outspace());
  setWorkspace(ac.workspace());
  tuple<int> cs;
  ac.cropsize(&cs.x, &cs.y);
  setCropsize(cs.x, cs.y);
  if (mLogFile != NULL) {
    fclose(mLogFile);
  }
  mLogFile =
      std::fopen(ac.getstr("logfile", "./ant-log.txt").toLocal8Bit(), "wa");
}

void AntEditProxy::setWorkspace(const QString& wp) {
  mWorkspace = wp;
  mCounter = 0;
  QStringList imgs = listImageNames(wp);
  QStringList::iterator pimg = imgs.begin();
  auto clist = QVector<__items>(imgs.size());
  for (int i = 0; i < clist.size(); i++) {
    __items& p = clist[i];
    p.cropTopLeft = {0, 0};
    p.name = *pimg;
    pimg++;
    if (mOutspace.isEmpty()) {
      p.isChecked = false;
    } else {
      p.isChecked = QFile::exists(path_join(mOutspace, p.name));
      if (p.isChecked) {
        mCounter++;
      }
    }
  }
  mCheckList = clist;
}

void AntEditProxy::setOutspace(const QString& os) {
  mOutspace = os;
  if (!QFile::exists(os)) {
    QDir().mkdir(os);
  }
}

bool AntEditProxy::cacheOutPixmap(const QString& name, const QPixmap& pix) {
  if (name.endsWith(":")) {
    return false;
  }
  QString path = path_join(mOutspace, name);
  pix.save(path);
  return QPixmapCache::insert(path, pix);
}

QPixmap* AntEditProxy::findPix(const QString& path, QString& name) {
  if (name.endsWith(":")) {
    return nullptr;
  }
  QString fullpath = path_join(path, name);
  QPixmap* out = QPixmapCache::find(fullpath);
  if (out == nullptr) {
    bool _suc = QPixmapCache::insert(fullpath, QPixmap(fullpath));
    if (!_suc) {
      name.append(":");
      return nullptr;
    }
  }
  return QPixmapCache::find(fullpath);
}

QPixmap* AntEditProxy::findPix(const QString& path, __items* item) {
  return findPix(path, item->name);
}

void AntEditProxy::writelog(__items* item) {
  QByteArray date =
      QDateTime::currentDateTime().toString("MMM dd, yyyy hh:mm:ss").toUtf8();

  std::fwrite(date, sizeof(char), date.size(), mLogFile);

  std::fprintf(mLogFile, "\t %d, (%d, %d, %d, %d) %s\n", item->isChecked,
               item->cropTopLeft.x, item->cropTopLeft.y, item->cropWH.x,
               item->cropWH.y, (const char*)item->name.toUtf8());
}

bool AntEditProxy::__iterBasic::savePixmap(const QPixmap& img, tuple<int>& tl,
                                           tuple<int>& br) {
  if (mMe->name.endsWith(":")) {
    return false;
  }
  mMe->cropTopLeft = tl;
  mMe->cropWH = br - tl;
  tuple<int>& cs = pthis->cropsize();
  QPixmap out = img;
  if (img.width() != cs.x || img.height() != cs.y) {
    out = img.scaled(cs.x, cs.y, Qt::IgnoreAspectRatio);
  }
  pthis->writelog(mMe);
  pthis->cacheOutPixmap(mMe->name, out);
  bool suc = out.save(path_join(outspace(), mMe->name));
  if (!suc) {
    return false;
  }
  if (mMe->isChecked == false) {
    pthis->stepCounter();
  }
  mMe->isChecked = true;
  return true;
}

void AntEditProxy::__iterBasic::removePixmap() {
  if (mMe->name.endsWith(":")) {
    return;
  }
  mMe->cropTopLeft.setup(0, 0);
  QFile::remove(path_join(outspace(), mMe->name));
  if (mMe->isChecked) {
    pthis->stepCounter(-1);
  }
  mMe->isChecked = false;
}
