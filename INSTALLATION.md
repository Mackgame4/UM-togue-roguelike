## Installing dependencies
>cc
```bash
sudo apt-get install build-essential
```
or
>gcc
```bash
sudo apt-get install gcc
```

or install and use [MinGW](http://www.mingw.org/) in Windows.

>ncurses
```bash
sudo apt-get install libncurses-dev
```

or in Windows, using [MinGW](http://www.mingw.org/):
1. Open MinGW Installation Manager.
2. Look for the following packages and mark the checkbox:
- mingw32-libncurses (dll)
- mingw32-libcurses (dev)
- mingw32-libpdcurses (dll)
- mingw32-libpdcurses (dev)

## Compile/Build
Build the project using the Makefile:
```bash
make
```

## Clean
Clean the build folder and output files:
```bash
make clean
```

## Rebuild
Clean the build folder and output files and build the project again, so changes in the source code are applied:
(Note: Only works if you have already built the project once)
```bash
make rebuild
```

## Run
```bash
./build/jogo
```