TEMPLATE = app
QT += qml quick gui core

OTHER_FILES = cannon.qml \
                    Wall.qml\
                    images/cannon_base.png \
                    images/cannon.png \
                    images/wall.jpg \
                    sounds/gear.wav \
                    sounds/cannon.wav \
                    doc/src/cannon.qdoc

EXAMPLE_FILES =  cannon.qml \
                    Wall.qml \
                    images/cannon_base.png \
                    images/cannon.png \
                    images/wall.jpg \
                    sounds/gear.wav \
                    sounds/cannon.wav

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/cannon

INSTALLS = target
SOURCES += main.cpp

RESOURCES += \
    cannon.qrc
