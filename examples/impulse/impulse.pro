TEMPLATE = app
QT += qml quick gui core

OTHER_FILES = impulse.qml \
                    Wall.qml \
                    Ball.qml

EXAMPLE_FILES = impulse.qml \
                    Wall.qml \
                    Ball.qml

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/impulse

INSTALLS = target

SOURCES += main.cpp

RESOURCES += \
    impulse.qrc
