# musicpp

Clean music production framework for C++. *Stonk* up your creativity!

MPP tries to ease the process of creating new sounds by providing an extensible framework.

## Build

```cpp
make
```

Creates the `out/main.wav` files out of the `src/main.cpp` source file.

*Note: the library depends on some features of C++ 20.*

*Also note: only `g++-10` has been tested and is currently expected to work. Feel free to open a merge request for any issues encountered using other compilers.*

### Examples

You'll find many example usages of the library in the `examples` directory.

To build e.g. `examples/circle`, just run:

```cpp
make -C examples/circle
```

This will build the `main.wav` file under `examples/circle/out`.

### Watch `src` recursively for changes (linux)

*Note: you need the `play` executable in your path for this to work*

```cpp
make watch
```

Rebuilds and plays `main.wav` whenever updating anything under `src`.

### Build and play (linux)

*Note: you need the `play` executable in your path for this to work*

```cpp
make play
```

Builds and plays `main.wav`.
