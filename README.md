# SpriterPlusPlusQt

A WIP to implement Spriter files in Qt5 Scene Graph to be used in QML

## Requirements

You need a Qt5 dev environment, a c+11 compiler and a recent CMake.

## HowTo Build

```
mkdir build && cd build
cmake ../
make
```

## HowTo Run

Run the example C++ app with the library linked:

```
cd bin
./SpriterQtExample
```

Or run the qml app using qml executable and Spriter qml plugin:

```
cd bin
qml main.qml
```

## HowTo Use

Use the qml plugin:

* Copy bin/Spriter to your qml app.

Link the lib:

* See example/ folder.
