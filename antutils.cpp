#include "antutils.h"

using namespace Ant;

QString Ant::path_join(const QString& parent, const QString& sub) {
  QString out(parent);
  if (!parent.endsWith("/")) {
    out.append('/');
  }
  out.append(sub);
  return out;
}
