#ifndef PIXRING_H
#define PIXRING_H

#include <QPixmap>
#include <QLinkedList>

template <typename T>
struct LinkedRingItem {
  T item;
  int id;
};

template <typename T>
class LinkedRing {
 public:
  LinkedRing(int capacity) {
    if (capacity <= 2) {
      capacity = 2;
    }
    mCapacity = capacity;
    mItems = new LinkedRingItem<T>[capacity];
    mIndex = 0;
  }

  virtual ~LinkedRing() { delete[] mItems; }

  /**
   * @brief step() move to next item
   */
  inline void step() { mIndex = (mIndex + 1) % mCapacity; }
  /**
   * @brief back() move back to previous item
   */
  inline void back() { mIndex = (mIndex - 1 + mCapacity) % mCapacity; }

  /**
   * @brief locate at i
   * i < 0 <-> previous i node\\
   * i = 0 <-> current node\\
   * i > 0 <-> next i node\\
   * @return
   */
  LinkedRingItem<T>* locate(int i = 0) {
    int idx = mIndex;
    if (i != 0) {
      // > 0
      idx = (mIndex + i) % mCapacity;
      if (idx < 0) {
        idx = (idx + mCapacity) % mCapacity;
      }
    }
    return mItems + idx;
  }

  inline T& locate_item(int i = 0) { return locate(i)->item; }
  inline int& locate_id(int i = 0) { return locate(i)->id; }

  inline LinkedRingItem<T>& operator[](int i) {
    return mItems[(i + mCapacity) % mCapacity];
  }

  inline LinkedRingItem<T>* begin() { return mItems; }
  inline LinkedRingItem<T>* end() { return mItems + mCapacity; }

 private:
  LinkedRingItem<T>* mItems;
  int mIndex;
  int mCapacity;
};

#endif // PIXRING_H
