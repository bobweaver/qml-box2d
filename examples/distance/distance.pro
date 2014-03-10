TEMPLATE = app
QT += qml quick gui core


OTHER_FILES = distance.qml \
                    Ball.qml \
                    Wall.qml \
                    WoodenBox.qml \
                    Square.qml

EXAMPLE_FILES = distance.qml \
                    Ball.qml \
                    Wall.qml \
                    WoodenBox.qml \
                    Square.qml

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/distance

INSTALLS = target
SOURCES += main.cpp

RESOURCES += \
    distance.qrc
