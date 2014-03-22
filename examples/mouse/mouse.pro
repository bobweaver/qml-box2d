TEMPLATE = app
QT += qml quick gui core

OTHER_FILES = mouse.qml \
                    Wall.qml \
                    images/wall.jpg \
doc/src/*.qdoc


EXAMPLE_FILES = mouse.qml \
                    Wall.qml \
                    images/wall.jpg

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/mouse

INSTALLS = target

SOURCES += main.cpp

RESOURCES += \
    mouse.qrc
