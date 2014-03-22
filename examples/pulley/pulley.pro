TEMPLATE = app
QT += qml quick gui core

OTHER_FILES = pulley.qml \
                    Wall.qml \
                    images/wall.jpg \
                    doc/src/*.qdoc


EXAMPLE_FILES = pulley.qml \
                    Wall.qml \
                    images/wall.jpg

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/pulley

INSTALLS = target

SOURCES += main.cpp


RESOURCES += \
    pulley.qrc
