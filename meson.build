project(
  'mailnaggertray',
  'c', 'cpp',
  version: '0.4',
  license: 'GPL',
  default_options: ['c_std=c11', 'cpp_std=c++17']
)

# Enabling QT
qt = import('qt6')
qt_deps = dependency('qt6', modules: ['Core', 'Gui', 'Widgets', 'DBus'])

# Set include directories
includes = include_directories('src')

# Main non processed source files
sources = files(
  'src/main.cpp',
  'src/tray.cpp',
  'src/emailswindow.cpp',
  'src/listwidget.cpp',
  'src/mailnaggerdbus.cpp',
  'src/util.cpp',
)

# QT processed files
qtprocessed = qt.preprocess(
  moc_headers: ['src/tray.h', 'src/emailswindow.h', 'src/listwidget.h'],
  include_directories: includes
)

sources += qtprocessed

exe = executable('mailnaggertray',
  sources,
  include_directories: includes,
  dependencies: [qt_deps],
  install: true,
  install_dir: 'bin'
)

install_data('data/mailnaggertray.desktop',
  install_dir: 'share/applications'
)

