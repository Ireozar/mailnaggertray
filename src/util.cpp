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
