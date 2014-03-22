TEMPLATE = app
QT += qml quick gui core


OTHER_FILES = prismatic.qml \
                    Wall.qml \
                    Ball.qml \
                    Square.qml \
                    doc/src/*.qdoc


EXAMPLE_FILES = prismatic.qml \
                    Wall.qml \
                    Ball.qml \
                    Square.qml

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/prismatic

INSTALLS = target

SOURCES += main.cpp


RESOURCES += \
    prismatic.qrc
