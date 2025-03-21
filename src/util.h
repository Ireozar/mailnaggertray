#ifndef MAILNAGTRAY_UTIL_H
#define MAILNAGTRAY_UTIL_H

#include <QString>

class Util {
public:
  static void openMailnaggerConfig();
  static void showNotification(QString title, QString message,
                               QString icon = "mailnagger", int timeout = 0);
};

#endif // MAILNAGTRAY_TRAY_H
