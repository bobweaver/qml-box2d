TEMPLATE = app
QT += qml quick gui core

OTHER_FILES = fixtures.qml \
                    Wall.qml \
                    images/wall.jpg \
                    doc/src/fixtures.qdoc


EXAMPLE_FILES = fixtures.qml \
                    Wall.qml \
                    images/wall.jpg

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/fixtures

INSTALLS = target

SOURCES += main.cpp

RESOURCES += \
    fixtures.qrc
