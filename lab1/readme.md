# Lab 1

## App
The application code.

## HAL


## Utils

## Cloning
The repo uses a submodule. You will need access to the submodule repo and to run a few additional steps.

```bash
git submodule init
git submodule update
```

## Building

The root directory has a makefile that can be used:

```bash
make clean all program
```

It will call into the application, bsp, and required library directories to build them.
