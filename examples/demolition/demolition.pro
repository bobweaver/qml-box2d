TEMPLATE = app
QT += qml quick gui core

OTHER_FILES = demolition.qml \
                    Building.qml \
                    Wall.qml \
                    WoodenBox.qml \
                    images/background.png \
                    images/brickwall.png \
                    images/plate.png \
                    images/skyline.png \
                    images/wheel.png \
                    images/woodenbox.png \
                    doc/src/demolition.qdoc

EXAMPLE_FILES =  demolition.qml \
                    Wall.qml \
                    WoodenBox.qml \
                    Building.qml \
                    images/background.png \
                    images/brickwall.png \
                    images/plate.png \
                    images/skyline.png \
                    images/wheel.png \
                    images/woodenbox.png

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/demolition

INSTALLS = target
SOURCES += main.cpp

RESOURCES += \
    demolition.qrc
