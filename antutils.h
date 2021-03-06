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
#include <QTime>
#include <QVector>

#include <stdio.h>
#include <stdlib.h>

#include <QString>
#include <QStringList>

namespace Ant {

template <typename T>
class tuple {
 public:
  constexpr tuple(T x, T y) : x(x), y(y) {}
  constexpr tuple() : x(0), y(0) {}

  inline void setup(T x, T y) {
    this->x = x;
    this->y = y;
  }
  inline void setup(const QPoint& p) {
    this->x = p.x();
    this->y = p.y();
  }

  inline operator QPoint() const { return QPoint(x, y); }
  inline tuple<T> operator+(const tuple<T>& other) {
    return tuple<T>(x + other.x, y + other.y);
  }
  inline tuple<T> operator-(const tuple<T>& other) {
    return tuple<T>(x - other.x, y - other.y);
  }

 public:
  T x;
  T y;
};

// all Qt supported image suffix
const static QStringList Image_Suffix = {
    "*.bmp", "*.gif", "*.jpg", "*.jpeg", "*.png",
    "*.pbm", "*.pgm", "*.ppm", "*.xbm",  "*.xpm",
};

QStringList listImageNames(const QString& root, const QDir& sub = QDir());
QStringList listSubfolders(const QString& root, const QDir& sub = QDir());

QString path_join(const QString& parent, const QString& sub);

// annotation tag
class AnnTag {
 public:
  AnnTag() : label(""), id(-1) {}

  AnnTag(const QString& label, const int& id) {
    this->label = label;
    this->id = id;
  }

  operator QJsonObject() {
    QJsonObject obj;
    obj.insert("label", label);
    obj.insert("id", id);
    return obj;
  }
  inline operator QJsonValue() { return QJsonValue((QJsonObject) * this); }

 public:
  QString label;
  int id;
};

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

  inline QString logfile() const { return mJsonObj["logpath"].toString(); }
  inline void setLogfile(const QString& path) {
    mJsonObj.insert("logpath", path);
  }

  QVector<AnnTag> annotations() const {
    QJsonArray ann = mJsonObj["annotations"].toArray();
    QVector<AnnTag> out(ann.size());
    for (auto a : ann) {
      QJsonObject oa = a.toObject();
      out.append(AnnTag(oa["label"].toString(), oa["id"].toInt()));
    }
    return out;
  }

  void setAnnotations(const QVector<AnnTag>& list) {
    QJsonArray ann;
    for (auto l : list) {
      ann.append(l);
    }
    mJsonObj.insert("annotations", ann);
  }

  inline QVariant operator[](const QString& key) const {
    return mJsonObj[key].toVariant();
  }

  inline QVariant get(const QString& key) const {
    return mJsonObj[key].toVariant();
  }

  inline void set(const QString& key, const QVariant& value) {
    mJsonObj.insert(key, value.toJsonValue());
  }
  inline QString getstr(const QString& key, const QString& defstr = "") const {
    return mJsonObj[key].toString(defstr);
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
  class iteratorLoop;

 private:
  struct __items;

 public:
  AntEditProxy();
  virtual ~AntEditProxy();

  void loadFromAntConfig();
  void setWorkspace(const QString& wp);
  void setOutspace(const QString& os);
  inline void setCropsize(int w, int h) { mCropsize = {w, h}; }
  inline int size() { return mCheckList.size(); }
  inline int count() { return mCounter; }

  bool cacheOutPixmap(const QString& name, const QPixmap& pix);

  inline QPixmap* workspacePix(QString& name) {
    return findPix(mWorkspace, name);
  }
  inline QPixmap* outpsacePix(QString& name) {
    return findPix(mOutspace, name);
  }
  inline QPixmap* workspacePix(__items* item) {
    return findPix(mWorkspace, item);
  }
  inline QPixmap* outpsacePix(__items* item) {
    return findPix(mOutspace, item);
  }

  inline QString& workspace() { return mWorkspace; }
  inline QString& outspace() { return mOutspace; }
  inline tuple<int>& cropsize() { return mCropsize; }

  inline iterator begin() { return iterator(mCheckList.begin(), this); }
  inline iterator end() { return iterator(mCheckList.end(), this); }
  inline iterator operator[](int i) {
    int s = mCheckList.size();
    i = (i + s) % s;
    return iterator(&mCheckList[i], this);
  }

  inline iteratorLoop iterloop() {
    return iteratorLoop(mCheckList.begin(), mCheckList.end(), this);
  }

  void writelog(__items* item);

 private:
  QPixmap* findPix(const QString& path, QString& name);
  QPixmap* findPix(const QString& path, __items* item);
  inline void stepCounter(int i = 1) { mCounter += i; }
  inline __items* clistBegin() { return mCheckList.begin(); }

 private:
  QVector<__items> mCheckList;
  tuple<int> mCropsize;
  QString mWorkspace;
  QString mOutspace;
  FILE* mLogFile;
  int mCounter;  // count isChecked

 private:
  struct __items {
    int annID;
    bool isChecked;
    tuple<int> cropTopLeft;
    tuple<int> cropWH;  // crop width height
    QString name;
  };

  class __iterBasic {
   public:
    constexpr __iterBasic(__items* item, AntEditProxy* parient)
        : mMe(item), pthis(parient) {}

    /**
     * @brief savePixmap
     * @param img
     * @param tl top left
     * @param br bottom right
     * @return
     */
    bool savePixmap(const QPixmap& img, tuple<int>& tl, tuple<int>& br);

    void removePixmap();

    inline void setAnnotation(const int& id) { mMe->annID = id; }

    inline int locate() { return mMe - pthis->clistBegin(); }

    inline QPixmap* pixmap() { return pixmap(mMe->isChecked); }
    // this won't change `isChecked()`
    inline QPixmap* pixmap(bool check) {
      return check ? pthis->outpsacePix(mMe) : pthis->workspacePix(mMe);
    }

    inline bool& isChecked() { return mMe->isChecked; }
    inline tuple<int>& cropTopLeft() { return mMe->cropTopLeft; }
    inline QString& name() { return mMe->name; }
    inline QString& workspace() { return pthis->workspace(); }
    inline QString& outspace() { return pthis->outspace(); }
    inline tuple<int>& cropsize() { return pthis->cropsize(); }

    inline bool operator==(const __iterBasic& other) {
      return this->mMe == other.mMe;
    }

   protected:
    __items* mMe;
    AntEditProxy* pthis;
  };

 public:
  class iterator : public __iterBasic {
   public:
    constexpr iterator() : __iterBasic(nullptr, nullptr) {}
    constexpr iterator(__items* item, AntEditProxy* parient)
        : __iterBasic(item, parient) {}

    inline iterator prev() { return iterator(mMe - 1, pthis); }
    inline iterator next() { return iterator(mMe + 1, pthis); }
    inline iterator& operator++() {
      mMe++;
      return *this;
    }
    inline iterator& operator--() {
      mMe--;
      return *this;
    }
    inline iterator& operator++(int) {
      mMe++;
      return *this;
    }
    inline iterator& operator--(int) {
      mMe--;
      return *this;
    }
  };

  class iteratorLoop : public __iterBasic {
   public:
    constexpr iteratorLoop()
        : __iterBasic(nullptr, nullptr), mBegin(nullptr), mEnd(nullptr) {}
    constexpr iteratorLoop(__items* begin, __items* end, AntEditProxy* parient)
        : __iterBasic(begin, parient), mBegin(begin), mEnd(end) {}

    inline iteratorLoop& operator++() {
      mMe = mMe + 1 >= mEnd ? mBegin : mMe + 1;
      return *this;
    }
    inline iteratorLoop& operator++(int) {
      mMe = mMe + 1 >= mEnd ? mBegin : mMe + 1;
      return *this;
    }
    inline iteratorLoop& operator--() {
      mMe = mMe - 1 < mBegin ? mEnd - 1 : mMe - 1;
      return *this;
    }
    inline iteratorLoop& operator--(int) {
      mMe = mMe - 1 < mBegin ? mEnd - 1 : mMe - 1;
      return *this;
    }

    inline iteratorLoop prev() { return iteratorLoop(*this)--; }
    inline iteratorLoop next() { return iteratorLoop(*this)++; }

   private:
    __items* mBegin;
    __items* mEnd;
  };
};

};  // namespace Ant

#endif  // ANTUTIL_H
