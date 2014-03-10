TEMPLATE = app
QT += qml quick gui core


OTHER_FILES = polygons.qml \
                    Wall.qml \
                    Trapezoid.qml

EXAMPLE_FILES = polygons.qml \
                    Wall.qml \
                    Trapezoid.qml

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/polygons

INSTALLS = target

SOURCES += main.cpp


RESOURCES += \
    polygons.qrc
