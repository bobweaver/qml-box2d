defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}

!minQtVersion(5, 2, 0) {
    message("Cannot build qbs with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.2.0.  There is a older version of box2d located \n Here \n git@gitorious.org:qml-box2d/qml-box2d.git ")
}

TEMPLATE = subdirs
SUBDIRS += \
                        src/box2d.pro \
                        examples/examples.pro

OTHER_FILES =  doc/*.qdocconf \
                                doc/src/*.qdoc \
                                doc/doc.pri \
                                README \
                                COPYING

include(doc/doc.pri)
include(box2d_version.pri)
=======
TEMPLATE = lib
CONFIG += plugin
QT += quick
TARGET = $$qtLibraryTarget(Box2D)
TARGETPATH = Box2D
MOC_DIR = .moc
OBJECTS_DIR = .obj

contains(QT_CONFIG, reduce_exports): CONFIG += hide_symbols

INCLUDEPATH += .
include(Box2D/box2d.pri)

importPath = $$[QT_INSTALL_QML]
isEmpty(importPath): importPath = $$[QT_INSTALL_IMPORTS]
target.path = $${importPath}/$$TARGETPATH

qmldir.path +=  $${importPath}/$$TARGETPATH
qmldir.files += $$PWD/qmldir


INSTALLS += target qmldir

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
