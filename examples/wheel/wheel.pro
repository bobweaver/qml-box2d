TEMPLATE = app
QT += qml quick gui core


OTHER_FILES = wheel.qml \
                    Wall.qml \
                    images/wall.jpg \
                    images/wheel.png \
                    images/arrow.png

EXAMPLE_FILES = wheel.qml \
                    Wall.qml \
                    images/wall.jpg \
                    images/wheel.png \
                    images/arrow.png

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/wheel

INSTALLS = target

SOURCES += main.cpp

RESOURCES += \
    wheel.qrc
