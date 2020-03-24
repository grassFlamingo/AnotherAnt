#ifndef ANTUTIL_H
#define ANTUTIL_H

#include <QByteArray>
#include <QDebug>
#include <QFile>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

#include <QString>
#include <QStringList>

namespace Ant {

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

  inline QString workspace() const {
    return mSelf.mJsonObj["workspace"].toString();
  }

  inline void setWorkspace(const QString& path) {
    mSelf.mJsonObj.insert("workspace", path);
  }

  int saveConfigure();

 private:
  AntConfigure();

 private:
  QJsonObject mJsonObj;
  QString mFileName;
  static AntConfigure mSelf;
};

inline AntConfigure& conf() { return AntConfigure::configure(); }

};  // namespace Ant

#endif  // ANTUTIL_H
