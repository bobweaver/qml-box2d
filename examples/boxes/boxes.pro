TEMPLATE = app
QT += qml quick gui core

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/boxes

INSTALLS = target
SOURCES += main.cpp
RESOURCES += boxes.qrc
OTHER_FILES = boxes.qml \
                              Wall.qml \
                              WoodenBox.qml \
                              doc/src/boxes.qdoc

EXAMPLE_FILES +=  boxes.qml \
                    background.png \
                    woodenbox.png \
                    Wall.qml \
                    WoodenBox.qml \

