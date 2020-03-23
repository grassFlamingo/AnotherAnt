#ifndef QLABLECLICK_H
#define QLABLECLICK_H

#include <QPainter>
#include <QPixmap>
#include <QWidget>

class WidgetClick : public QWidget {
  Q_OBJECT
 public:
  explicit WidgetClick(QWidget *parent = nullptr);

  void setPixmap(const QPixmap &map);

  void paintEvent(QPaintEvent *event) override;

 signals:
  void clicked();

 protected:
  void mouseReleaseEvent(QMouseEvent *event) override;

 private:
  QPixmap mBackground;
};

#endif // QLABLECLICK_H
