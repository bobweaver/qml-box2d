TEMPLATE = app
QT += qml quick gui core

OTHER_FILES = gear.qml \
                    Wall.qml \
                    images/wall.jpg

EXAMPLE_FILES = gear.qml \
                    Wall.qml \
                    images/wall.jpg

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/gear

INSTALLS = target

SOURCES += main.cpp

RESOURCES += \
    gear.qrc
