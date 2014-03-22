TEMPLATE = app

QT += qml quick gui core

OTHER_FILES = filtering.qml \
                    Wall.qml \
doc/src/*.qdoc

EXAMPLE_FILES = filtering.qml \
                    Wall.qml

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/filtering

INSTALLS = target
SOURCES += main.cpp

RESOURCES += \
    filtering.qrc
