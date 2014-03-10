TEMPLATE = app
QT += qml quick gui core


OTHER_FILES = rope.qml \
                    Wall.qml \
                    images/wall.jpg

EXAMPLE_FILES = rope.qml \
                    Wall.qml \
                    images/wall.jpg

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/rope

INSTALLS = target

SOURCES += main.cpp

RESOURCES += \
    rope.qrc
