TEMPLATE = app
QT += qml quick gui core

OTHER_FILES = friction.qml \
                    Wall.qml \
                    images/wall.jpg \
doc/src/*.qdoc


EXAMPLE_FILES = friction.qml \
                    Wall.qml \
                    images/wall.jpg

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/friction

INSTALLS = target

SOURCES += main.cpp


RESOURCES += \
    friction.qrc
