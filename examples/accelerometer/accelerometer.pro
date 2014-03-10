TEMPLATE = app
QT += qml quick gui core

OTHER_FILES = accelerometer.qml \
                              Wall.qml \
                               WoodenBox.qml

EXAMPLE_FILES =  accelerometer.qml \
                    background.png \
                    woodenbox.png \
                    Wall.qml \
                    WoodenBox.qml

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/accelerometer

INSTALLS = target qml

SOURCES += main.cpp

RESOURCES += \
    accelerometer.qrc

