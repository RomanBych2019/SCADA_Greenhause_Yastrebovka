QT += core gui
QT += network
QT += serialbus serialport
QT += sql
QT += charts
QT += multimedia
QT += printsupport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    alarmwindow.cpp \
    charts_ref.cpp \
    charts_sclad.cpp \
    charts_tepl.cpp \
    formrain.cpp \
    formteplica.cpp \
    formteplica_short.cpp \
    formyandex.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    refrigerator.cpp \
    settingNetwork.cpp \
    soundplayer.cpp \
    water.cpp \
    watertemper.cpp \
    weather_dounload.cpp

OTHER_FILES += weatherinfo.qml \
    icons/*


HEADERS += \
    alarmwindow.h \
    charts_ref.h \
    charts_sclad.h \
    charts_tepl.h \
    formrain.h \
    formteplica.h \
    formteplica_short.h \
    formyandex.h \
    mainwindow.h \
    qcustomplot.h \
    refrigerator.h \
    settingNetwork.h \
    soundplayer.h \
    ui_water.h \
    water.h \
    watertemper.h \
    weather_dounload.h
    target.path = $$[QT_INSTALL_EXAMPLES]/positioning/weatherinfo

FORMS += \
    alarmwindow.ui \
    charts_ref.ui \
    charts_sclad.ui \
    charts_tepl.ui \
    formrain.ui \
    formteplica.ui \
    formteplica_short.ui \
    formyandex.ui \
    mainwindow.ui \
    refrigerator.ui \
    settingNetwork.ui \
    temper.ui \
    water.ui \
    water_short.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc


