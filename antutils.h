#ifndef ANTUTIL_H
#define ANTUTIL_H

#include <QByteArray>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QPixmap>
#include <QPixmapCache>
#include <QStringList>
#include <QVector>

#include <QString>
#include <QStringList>

namespace Ant {

template <typename T>
struct tuple {
  T x;
  T y;
};

// all Qt supported image suffix
const static QStringList Image_Suffix = {
    "*.bmp", "*.gif", "*.jpg", "*.jpeg", "*.png",
    "*.pbm", "*.pgm", "*.ppm", "*.xbm",  "*.xpm",
};

QStringList listImageNames(const QString& root);

QString path_join(const QString& parent, const QString& sub);

class AntConfigure {
 public:
  /**
   * @brief loadConfigure
   * @param filename
   * @return -1 file not found; -2 not a json file; > 0 succeed
   */
  static int loadConfigure(const QString& filename);
  inline static AntConfigure& configure() { return mSelf; }

  inline QString workspace() const { return mJsonObj["workspace"].toString(); }
  inline void setWorkspace(const QString& path) {
    mJsonObj.insert("workspace", path);
  }

  inline void unpackCropsize(const QString& str, int* w, int* h) {
    sscanf(str.toLocal8Bit(), "%dx%d", w, h);
  }
  inline QString cropsize() const { return mJsonObj["cropsize"].toString(); }
  inline QString cropsize(int* w, int* h) {
    QString crop = cropsize();
    unpackCropsize(crop, w, h);
    return crop;
  }
  inline void setCropsize(int w, int h) {
    mJsonObj.insert("cropsize", QString::asprintf("%dx%d", w, h));
  }
  inline void setCropsize(const QString& cs) {
    mJsonObj.insert("cropsize", cs);
  }

  inline QString outspace() const { return mJsonObj["outspace"].toString(); }
  inline void setOutspace(const QString& path) {
    mJsonObj.insert("outspace", path);
  }

  inline double zoomf() const { return mJsonObj["zoomf"].toDouble(0.05); }

  inline QVariant operator[](const QString& key) const {
    return mJsonObj[key].toVariant();
  }

  inline QVariant get(const QString& key) const {
    return mJsonObj[key].toVariant();
  }

  inline void set(const QString& key, const QVariant& value) {
    mJsonObj.insert(key, value.toJsonValue());
  }

  /**
   * @brief setZoomf
   * @param zf > 0
   */
  inline void setZoomf(double zf) {
    if (zf <= 0) {
      return;
    }
    mJsonObj.insert("zoomf", zf);
  }

  int saveConfigure();

 private:
  AntConfigure();

 private:
  QJsonObject mJsonObj;
  QString mFileName;
  static AntConfigure mSelf;
};

static AntConfigure& ac = AntConfigure::configure();

/**
 * @brief The AntEditAgent class
 */
class AntEditProxy {
 public:
  class iterator;
  struct __items {
    bool isChecked;
    tuple<int> cropTopLeft;
  };

 public:
  AntEditProxy();
  virtual ~AntEditProxy();

  void loadFromAntConfig();
  void setWorkspace(const QString& wp);
  void setOutspace(const QString& os);
  inline void setCropsize(int w, int h) { mCropsize = {w, h}; }

  inline QString& workspace() { return mWorkspace; }
  inline QString& outspace() { return mOutspace; }
  inline tuple<int>& cropsize() { return mCropsize; }

  iterator begin();
  iterator end();

 private:
  QVector<__items>* mCheckList;
  tuple<int> mCropsize;
  QPixmapCache mWsCache;
  QPixmapCache mOsCache;
  QString mWorkspace;
  QString mOutspace;
};

class iterator {
 public:
  iterator(AntEditProxy* parient) {
    pthis =
        (QVector<AntEditProxy::__items>*)(parient + offsetofclass(AntEditProxy,
                                                                  mCheckList));
  }

  iterator prev();
  iterator next();

  QPixmap& pixmap();
  inline bool isChecked() { return pthis->at(mIndex).isChecked; }

 private:
  int mIndex;
  QVector<AntEditProxy::__items>* pthis;
};

};  // namespace Ant

#endif  // ANTUTIL_H
