QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../build-QtManQEmu-Desktop_Qt_5_12_3_MinGW_32_bit-Debug/debug/moc_predefs.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    QtManQEmu_de_DE.ts

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    docs/license.txt \
    stylsheets/Adaptic/Adaptic.png \
    stylsheets/Adaptic/Adaptic.qss \
    stylsheets/Adaptic/License.txt \
    stylsheets/Combinear/Combinear.png \
    stylsheets/Combinear/Combinear.qss \
    stylsheets/Combinear/License.txt \
    stylsheets/DeepBox/DeepBox.png \
    stylsheets/DeepBox/DeepBox.qss \
    stylsheets/DeepBox/License.txt \
    stylsheets/Irrorater/Irrorater.png \
    stylsheets/Irrorater/Irrorater.qss \
    stylsheets/Irrorater/License.txt \
    stylsheets/application.qss \
    stylsheets/nostyle.qss
