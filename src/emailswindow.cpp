#include "emailswindow.h"

#include <QAction>
#include <QDate>
#include <QLabel>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QMouseEvent>
#include <QStatusBar>
#include <QToolBar>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "listwidget.h"
#include "mailnaggerdbus.h"
#include "util.h"

EmailsWindow::EmailsWindow() : QMainWindow() {
  setWindowTitle(tr("New E-mails"));
  setWindowIcon(QIcon::fromTheme("mailnag"));

  // Toolbar
  _toolBar = addToolBar(tr("Main Toolbar"));
  _toolBar->setIconSize(QSize(32, 32));

  _checkEmailsAction =
      _toolBar->addAction(QIcon::fromTheme("mail-inbox"), tr("Update E-mails"));
  _checkEmailsAction->setStatusTip(tr("Check for New E-mail Messages"));
  connect(_checkEmailsAction, &QAction::triggered,
          &MailnagDBus::checkForNewMail);

  _toolBar->addSeparator();

  _mailNagConfigAction = _toolBar->addAction(QIcon::fromTheme("mailnag"),
                                             tr("Mailnagger Configuration"));
  _mailNagConfigAction->setStatusTip(tr("Launch Mailnagger Configuration"));
  connect(_mailNagConfigAction, &QAction::triggered,
          &Util::openMailnaggerConfig);

  _toolBar->addSeparator();

  _aboutAction =
      _toolBar->addAction(QIcon::fromTheme("gtk-about"), tr("About"));
  _aboutAction->setStatusTip(tr("About Mailnagger Tray"));
  connect(_aboutAction, &QAction::triggered, this, &EmailsWindow::onAbout);

  // Central Widget
  _layout = new QVBoxLayout();

  QLabel *emailsLabel = new QLabel(tr("E-mail Accounts:"));
  _layout->addWidget(emailsLabel);

  _emailsList = new ListWidget();
  _emailsList->setIconSize(QSize(48, 48));
  _layout->addWidget(_emailsList);
  connect(_emailsList, &ListWidget::leftClick, this,
          &EmailsWindow::onEmailsLClick);
  connect(_emailsList, &ListWidget::rightClick, this,
          &EmailsWindow::onEmailsRClick);

  QLabel *messagesLabel = new QLabel(tr("Messages:"), this);
  _layout->addWidget(messagesLabel);

  _messagesList = new QTreeWidget();
  _messagesList->setRootIsDecorated(false);
  _messagesList->setIndentation(0);
  _messagesList->setColumnCount(4);
  _messagesList->headerItem()->setText(0, tr("Date"));
  _messagesList->setColumnWidth(0, 100);
  _messagesList->headerItem()->setText(1, tr("From"));
  _messagesList->setColumnWidth(1, 100);
  _messagesList->headerItem()->setText(2, tr("E-mail"));
  _messagesList->setColumnWidth(2, 200);
  _messagesList->headerItem()->setText(3, tr("Subject"));
  _layout->addWidget(_messagesList);

  _centralWidget = new QWidget();
  _centralWidget->setLayout(_layout);

  setCentralWidget(_centralWidget);

  // Initialize status bar
  statusBar();
}

void EmailsWindow::addMessage(MailnaggerMessage message) {
  if (!_accountsMap[message.account_name]) {
    QListWidgetItem *item =
        new QListWidgetItem(QIcon::fromTheme("email"), message.account_name);
    item->setWhatsThis(message.account_name);
    _accountsMap[message.account_name] = item;
    _emailsList->addItem(item);
  }
  _messagesMap[message.account_name].append(message);

  QString emailText = message.account_name;
  emailText +=
      " (" + QString::number(_messagesMap[message.account_name].size()) + ")";
  _accountsMap[message.account_name]->setText(emailText);
}

void EmailsWindow::clearMessages() { _messagesMap.clear(); }

void EmailsWindow::updateMessages() {
  QListWidgetItem *item = _emailsList->currentItem();

  if (item) {
    accountActivated(item);
  }
}

void EmailsWindow::removeMessage(QString account, QString id) {
  size_t pos = 0;
  for (const MailnaggerMessage &message : _messagesMap[account]) {
    if (message.id == id) {
      _messagesMap[account].removeAt(pos);
      break;
    }
    pos++;
  }
}

void EmailsWindow::accountActivated(QListWidgetItem *item) {
  if (!item)
    return;

  QString account = item->whatsThis();
  _messagesList->clear();
  for (const MailnaggerMessage &message : _messagesMap[account]) {
    QDateTime time = QDateTime::fromSecsSinceEpoch(message.datetime);
    time.toString("%a %_d %b, %I:%M %p");
    QStringList columns = {time.toString("ddd d MMMM yyyy, h:m ap"),
                           message.sender_name, message.sender_addr,
                           message.subject};
    QTreeWidgetItem *item = new QTreeWidgetItem(columns);
    item->setIcon(0, QIcon::fromTheme("mail-unread"));
    _messagesList->addTopLevelItem(item);
  }
}

QList<EmailStats> EmailsWindow::getStats() {
  QList<EmailStats> stats;
  for (const QListWidgetItem *item : _accountsMap) {
    stats.append({.account = item->whatsThis(),
                  .messages = _messagesMap[item->whatsThis()].count()});
  }
  return stats;
}

void EmailsWindow::onEmailsRClick() {
  accountActivated(_emailsList->currentItem());
}

void EmailsWindow::onEmailsLClick() {
  accountActivated(_emailsList->currentItem());
}

void EmailsWindow::onAbout() {
  QMessageBox::about(this, tr("About Mailnagger Tray"),
                     tr("A tray icon applet for mail monitoring using "
                        "Mailnagger and written on QT."
                        "\n\n"
                        "License: GPL\n"
                        "Website: https://github.com/ireozar/mailnaggertray"));
}
