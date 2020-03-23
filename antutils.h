#ifndef ANTUTIL_H
#define ANTUTIL_H

#include <QByteArray>
#include <QString>
#include <QStringList>

namespace Ant {

// all Qt supported image suffix
const static QStringList Image_Suffix = {
    "*.bmp", "*.gif", "*.jpg", "*.jpeg", "*.png",
    "*.pbm", "*.pgm", "*.ppm", "*.xbm",  "*.xpm",
};

QString path_join(const QString& parent, const QString& sub);

};  // namespace Ant

#endif // ANTUTIL_H
