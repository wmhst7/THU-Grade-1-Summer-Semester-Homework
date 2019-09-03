#-------------------------------------------------
#
# Project created by QtCreator 2019-08-26T19:34:30
#
#-------------------------------------------------

QT       += core gui multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chess
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

CONFIG += c++11

SOURCES += \
        dialog.cpp \
        dialog1.cpp \
        dialoghe.cpp \
        kernel.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        dialog.h \
        dialog1.h \
        dialoghe.h \
        kernel.h \
        mainwindow.h

FORMS += \
        dialog.ui \
        dialog1.ui \
        dialoghe.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    Material/Normal/black_bishop.png \
    Material/Normal/black_king.png \
    Material/Normal/black_knight.png \
    Material/Normal/black_pawn.png \
    Material/Normal/black_queen.png \
    Material/Normal/black_rook.png \
    Material/Normal/white_bishop.png \
    Material/Normal/white_king.png \
    Material/Normal/white_knight.png \
    Material/Normal/white_pawn.png \
    Material/Normal/white_queen.png \
    Material/Normal/white_rook.png \
    Material/Transparent/black_bishop.png \
    Material/Transparent/black_king.png \
    Material/Transparent/black_knight.png \
    Material/Transparent/black_pawn.png \
    Material/Transparent/black_queen.png \
    Material/Transparent/black_rook.png \
    Material/Transparent/white_bishop.png \
    Material/Transparent/white_king.png \
    Material/Transparent/white_knight.png \
    Material/Transparent/white_pawn.png \
    Material/Transparent/white_queen.png \
    Material/Transparent/white_rook.png \
    Material/sample_input.txt
