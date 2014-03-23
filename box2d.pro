defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}


!minQtVersion(5, 2, 0) {
    message("Cannot build qbs with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.2.0.  There is a older version of box2d located \n Here \n git@gitorious.org:qml-box2d/qml-box2d.git ")
}

minQtVersion(5,2,0){
message("building Box2D 1.1 qt bindings for $${QT_VERSION}.")
}

## this is just for me at the moment and will be removed when Pushed to main branch
CONFIG = warn_off

TEMPLATE = subdirs
SUBDIRS += \
                        src/box2d.pro \
                        examples/examples.pro

OTHER_FILES =  doc/*.qdocconf \
                                doc/src/*.qdoc \
                                doc/doc.pri \
                                README \
                                COPYING

include(doc/doc.pri)
include(box2d_version.pri)
