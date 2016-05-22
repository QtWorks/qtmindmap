QT       += core gui svg xml widgets

CONFIG   += warn_on

TARGET    = qtmindmap

TEMPLATE  = app


SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/graphwidget.cpp \
           src/graphlogic.cpp \
           src/node.cpp \
           src/edge.cpp \
           src/commands.cpp


HEADERS  += include/mainwindow.h \
            include/graphwidget.h \
            include/graphlogic.h \
            include/node.h \
            include/edge.h \
            include/commands.h


FORMS        += ui/mainwindow.ui

RESOURCES    += images/qtmindmap.qrc


INSTALLS += target translations desktop icon

target.path = /usr/bin

desktop.files += other/qtmindmap.desktop
desktop.path += /usr/share/applications

icon.files += images/qtmindmap.svg
icon.path += /usr/share/pixmaps
