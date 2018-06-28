#-------------------------------------------------
#
# Project created by QtCreator 2018-06-09T02:10:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#requires(qtConfig(filedialog))


TARGET = TipManager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_FILE = icon.rc

SOURCES += \
        main.cpp \
        tipmanager.cpp \
    window.cpp

HEADERS += \
        tipmanager.h \
    window.h

FORMS += \
        tipmanager.ui

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/dialogs/findfiles
INSTALLS += target
QMAKE_CXXFLAGS += -std=c++11
