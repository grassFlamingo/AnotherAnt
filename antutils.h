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

  inline QString outspace() const { return mJsonObj["outspace"].toString(); }
  inline void setOutspace(const QString& path) {
    mJsonObj.insert("outspace", path);
  }

  inline double zoomf() const { return mJsonObj["zoomf"].toDouble(0.05); }
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

};  // namespace Ant

#endif  // ANTUTIL_H
