// we include libnotify first otherwise causes compilation issues
#include <libnotify/notify.h>

#include "util.h"

#include <QFileInfo>
#include <QMessageBox>
#include <QObject>
#include <QProcess>
#include <QStringList>

void Util::openMailnaggerConfig() {
  if (QFileInfo::exists("/usr/bin/mailnagger-config"))
    QProcess::execute("mailnagger-config", QStringList());
  else
    QMessageBox::critical(
        nullptr, QObject::tr("mailnagger-config not found"),
        QObject::tr(
            "The Mailnagger Configuration tool was not found.\n\n"
            "Install Mailnagger for this application to work properly."));
}

void Util::showNotification(QString title, QString message, QString icon,
                            int timeout) {
  notify_init("mailnaggertray");
  NotifyNotification *notification =
      notify_notification_new(title.toUtf8(), message.toUtf8(), icon.toUtf8());
  notify_notification_set_timeout(notification, timeout * 1000);
  notify_notification_show(notification, NULL);
  g_object_unref(G_OBJECT(notification));
  notify_uninit();
}
