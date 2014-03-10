TEMPLATE = app
QT += qml quick gui core

OTHER_FILES = \
                    MovableBox.qml \
                    movingBox.qml \
                    Wall.qml \
                    WoodenBox.qml

EXAMPLE_FILES = \
                    MovableBox.qml \
                    movingBox.qml \
                    Wall.qml \
                    WoodenBox.qml

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/movingBox

INSTALLS = target

SOURCES += main.cpp

RESOURCES += \
    movingBox.qrc
