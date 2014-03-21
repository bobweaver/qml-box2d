## QML Box2D plugin
----------------

This plugin is meant to be installed to your Qt/imports directory, or shipped
in a directory of which the parent is added as import path.

The goal is to expose the functionality of Box2D as QML components, in order
to make it easy to write physics based games in QML.


## INSTALLING

```
 qmake
 make
 make install  (goes to Qt's import path, run with sudo if necessary)
```

## RUNNING THE EXAMPLE (With Out QtCreator)

```
 qmlscene examples/boxes/boxes.qml
```

## USING QT CREATOR

Open QtCreator 
Open the box2d.pro file from the root dorectory
Pick the Example that you want to run
compile and run
