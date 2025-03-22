#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QThread>

#ifndef QT_NO_SYSTEMTRAYICON

#include "mailnaggerdbus.h"
#include "tray.h"

void print_help() {
  printf(
      "Mailnagger Tray - cross DE systray notifications implemented in QT.\n");
  printf("\n");
  printf("Usage: mailnaggertray [OPTION]\n");
  printf("\n");
  printf("  -h, --help       Show command-line help.\n");
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      QString argument(argv[i]);
      if (argument == "-h" || argument == "--help") {
        print_help();
        return 0;
      } else {
        print_help();
        return 0;
      }
    }
  }

  QApplication app(argc, argv);

  QString title = QObject::tr("Mailnagger Tray");

  if (!QSystemTrayIcon::isSystemTrayAvailable()) {
    bool tray_found = false;
    for (int i = 0; i < 5; i++) {
      QThread::sleep(1);
      if (QSystemTrayIcon::isSystemTrayAvailable()) {
        tray_found = true;
        break;
      }
    }
    if (!tray_found) {
      QString message = QObject::tr("No system tray detected on this system.");
      QMessageBox::critical(nullptr, title, message);
      qDebug() << message;
      return 1;
    }
  }

  if (!MailnagDBus::running()) {
    bool running = false;
    for (int i = 0; i < 10; i++) {
      QThread::sleep(1);
      if (MailnagDBus::running()) {
        running = true;
        break;
      }
    }
    if (!running) {
      QString message = QObject::tr("Mailnagger daemon not running.");
      QMessageBox::critical(nullptr, title, message);
      qDebug() << message;
      return 1;
    }
  }

  QApplication::setQuitOnLastWindowClosed(false);

  Tray tray;

  return app.exec();
}

#else // not QT_NO_SYSTEMTRAYICON

#include <QLabel>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QString text =
      QObject::tr("QSystemTrayIcon is not supported on this platform");

  QMessageBox::critical(nullptr, QObject::tr("Mailnagger Tray"), text);

  qDebug() << text;

  return 1;
}

#endif // QT_NO_SYSTEMTRAYICON
