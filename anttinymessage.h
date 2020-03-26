#ifndef TINYMESSAGE_H
#define TINYMESSAGE_H

#include <QDialog>
#include <QString>

namespace Ui {
class AntTinyMessage;
}

class AntTinyMessage : public QDialog {
  Q_OBJECT

 public:
  explicit AntTinyMessage(const QString &msg, QWidget *parent = nullptr);
  ~AntTinyMessage();

  /**
   * @brief moveAbove
   * @param mount
   */
  void moveAbove(QWidget *mount);

 private:
  Ui::AntTinyMessage *ui;
};

#endif // TINYMESSAGE_H
