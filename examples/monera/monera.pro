TEMPLATE = app
QT += qml quick gui core


OTHER_FILES = \
                    monera.qml \
                    Wall.qml \
                    coral2.png \
                    hexa.png \
                    shell.png \
                    spikes.png \
                    coral.png \
                    monera.png \
                    README.md \
                    SpeciesInfo.qml \
                    Wall.qml \
                    fungus.png \
                    shell2.png \
                    SpeciesModel.qml \
                    doc/src/*.qdoc

EXAMPLE_FILES = \
                    monera.qml \
                    Wall.qml \
                    coral2.png \
                    hexa.png \
                    shell.png \
                    spikes.png \
                    coral.png \
                    monera.png \
                    README.md \
                    SpeciesInfo.qml \
                    Wall.qml \
                    fungus.png \
                    shell2.png \
                    SpeciesModel.qml

target.path = $$[QT_INSTALL_EXAMPLES]/quick/box2d/monera

INSTALLS = target

SOURCES += main.cpp

RESOURCES += \
    monera.qrc
