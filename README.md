## QML Box2D plugin
----------------

The goal is to expose the functionality of 
[Box2D](http://box2d.org/) as QML components, in order
to make it easy to write physics based games in QML.


## INSTALLING

```
 qmake
 make
 make install  (goes to Qt's import path, run with sudo if necessary)
```

## Docs (Alpha)
tis will change and is under HEAVEY DEVELOPMENT
This assumes that you have already installed qml-box2d bindings.  
If you have not read the Installing part of this readme 

```
make docs 
make install_docs
```

### install docs for qtcreator
```
make qch_docs
make install_inst_qch_docs 
``` 
If that does not work you can just go in the doc build folder and install the compressed help files (*qch)
from qtcreator (tools->options->help->documentation) click add then pick the .qch file that is under the docs builddir.  

## RUNNING THE EXAMPLE (With Out QtCreator)

one can use the creator or one could just run with qmlscene
```
 qmlscene examples/boxes/boxes.qml
```

## USING QT CREATOR

⋅⋅* Open QtCreator 
⋅⋅* Open the box2d.pro file from the root dorectory
⋅⋅* Pick the Example that you want to run
⋅⋅* compile and run
