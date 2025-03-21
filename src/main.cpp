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
    int runs = 0;
    bool tray_found = true;
    do {
      QThread::sleep(1);
      runs++;
      if (runs == 5) {
        tray_found = false;
        break;
      }
    } while (!QSystemTrayIcon::isSystemTrayAvailable());
    if (!tray_found) {
      QString message = QObject::tr("No system tray detected on this system.");
      QMessageBox::critical(nullptr, title, message);
      qDebug() << message;
      return 1;
    }
  }

  if (!MailnagDBus::running()) {
    int runs = 0;
    bool running = true;
    do {
      QThread::sleep(1);
      runs++;
      if (runs == 10) {
        running = false;
        break;
      }
    } while (!MailnagDBus::running());
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
