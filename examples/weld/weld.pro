TEMPLATE = app
QT += qml quick gui core


OTHER_FILES = weld.qml \
                    Wall.qml \
                    images/wall.jpg

EXAMPLE_FILES = weld.qml \
                    Wall.qml \
                    images/wall.jpg

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/weld

INSTALLS = target

SOURCES += main.cpp

RESOURCES += \
    weld.qrc
