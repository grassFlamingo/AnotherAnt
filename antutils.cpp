#include "antutils.h"

using namespace Ant;
using Ant::AntConfigure;

QStringList Ant::listImageNames(const QString& root) {
  QDir path(root);
  path.setNameFilters(Ant::Image_Suffix);
  return path.entryList(QDir::Files);
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

AntEditProxy::AntEditProxy() : mCheckList(NULL) {}

AntEditProxy::~AntEditProxy() {
  if (mCheckList != NULL) {
    delete mCheckList;
  }
}

void AntEditProxy::setWorkspace(const QString& wp) {
  mWorkspace = wp;
  QStringList imgs = listImageNames(wp);
  mCheckList = new QVector<bool>(imgs.size(), false);
}

void AntEditProxy::setOutspace(const QString& os) {
  mOutspace = os;
  if (!QFile::exists(os)) {
    QDir().mkdir(os);
  }
}
