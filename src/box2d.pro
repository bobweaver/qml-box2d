TEMPLATE = lib
TARGET = Box2D
CONFIG += qt plugin
QT += quick qml core network
TARGET = $$qtLibraryTarget($$TARGET)
uri = Box2D
MOC_DIR = .moc
OBJECTS_DIR = .obj

contains(QT_CONFIG, reduce_exports): CONFIG += hide_symbols

INCLUDEPATH += "../Box2D/"
INCLUDEPATH += ../
include(../Box2D/box2d.pri)

SOURCES += box2dplugin.cpp \
    box2dworld.cpp \
    box2dbody.cpp \
    box2dfixture.cpp \
    box2ddebugdraw.cpp \
    box2djoint.cpp \
    box2ddistancejoint.cpp \
    box2dprismaticjoint.cpp \
    box2drevolutejoint.cpp \
    box2ddestructionlistener.cpp \
    box2dmotorjoint.cpp \
    box2dweldjoint.cpp \
    box2dpulleyjoint.cpp \
    box2dfrictionjoint.cpp \
    box2dwheeljoint.cpp \
    box2dmousejoint.cpp \
    box2dgearjoint.cpp \
    box2dropejoint.cpp

HEADERS += \
    box2dplugin.h \
    box2dworld.h \
    box2dbody.h \
    box2dfixture.h \
    box2ddebugdraw.h \
    box2djoint.h \
    box2ddistancejoint.h \
    box2dprismaticjoint.h \
    box2drevolutejoint.h \
    box2ddestructionlistener.h \
    box2dmotorjoint.h \
    box2dweldjoint.h \
    box2dpulleyjoint.h \
    box2dfrictionjoint.h \
    box2dwheeljoint.h \
    box2dmousejoint.h \
    box2dgearjoint.h \
    box2dropejoint.h

OTHER_FILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

