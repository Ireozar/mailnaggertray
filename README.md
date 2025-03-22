# Mailnagger Tray

A tray icon applet for mail monitoring using mailnagger and written on QT to
support any desktop environment that supports a standard system tray.

* OS: Linux
* License: GPLv2

## Differences from original

* removed GOA integration and notification support
* use mailnagger-config instead of mailnag-config (more up-to-date)

## Requirements

* [Mailnagger](https://github.com/tikank/mailnagger) - mail notifications
* [QT6](https://www.qt.io/) - graphical interface and dbus ipc
* [Libnotify](https://gitlab.gnome.org/GNOME/libnotify) - message notifications
* [Meson](https://mesonbuild.com/) - build the application

## Compilation

To build the application execute the following commands:

```sh
meson setup build --prefix /usr
meson compile -C build
```

## Installation

After a successfull build you can install the utility globally by executing:

```sh
sudo meson install -C build
```
