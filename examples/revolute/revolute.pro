TEMPLATE = app
QT += qml quick gui core

OTHER_FILES = revolute.qml \
                    Wall.qml \

EXAMPLE_FILES = revolute.qml \
                    Wall.qml \

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/revolute

INSTALLS = target

SOURCES += main.cpp

RESOURCES += \
    revolute.qrc
