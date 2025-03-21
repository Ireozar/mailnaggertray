#ifndef MAILNAGTRAY_TRAY_H
#define MAILNAGTRAY_TRAY_H

#include <QDBusMessage>
#include <QSystemTrayIcon>

#include "mailnaggerdbus.h"

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
QT_END_NAMESPACE

class EmailsWindow;

class Tray : public QSystemTrayIcon {
  Q_OBJECT

public:
  Tray();

private:
  void loadIcons();
  void setupSettings();
  void setupMenu();
  void setupTimer();
  void registerMessages(QList<MailnaggerMessage> messages);

private slots:
  void iconActivated(QSystemTrayIcon::ActivationReason reason);
  void onSettingsClicked();
  void onMailTimer();
  void onMailsAdded(QDBusMessage message);
  void onMailsRemoved(QDBusMessage message);

private:
  EmailsWindow *_emailsWindow;

  QIcon _mailUnreadIcon;
  QIcon _mailReadIcon;

  QMenu *_iconMenu;
  QAction *_messagesAction;
  QAction *_mailNagConfigAction;
  QAction *_onlineAccountsAction;
  QAction *_quitAction;

  QTimer *_mailTimer;
};

#endif // MAILNAGTRAY_TRAY_H
