TEMPLATE = app
QT += qml quick gui core


OTHER_FILES = motorjoint.qml \
                    boat.png \
                    wave.png \
doc/src/*.qdoc


EXAMPLE_FILES = motorjoint.qml \
                    boat.png \
                    wave.png

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/motorjoint

INSTALLS = target

SOURCES += main.cpp

RESOURCES += \
    motorjoint.qrc
