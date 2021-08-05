# pico-shell
Pico-sized shell for (mostly) embedded projects

## Introduction
pico-shell (pSH) is:
- a microscopic shell
- written in pure C
- without any external dependencies (even `libc`!)
- without dynamic memory allocation

## Compilation
Examples are placed in the `platform` director. You can use the provided python-based
build system or integrate sources in your own! Remember to just add the `src` directory to the include list.

## Architecture
The library loosely follow ideas behind unix philosophy 'everything is a file', yet pSH lacks a beefy 
filesystem, just simple flat representation. You provide `read()`, `write()` and `execute()` functions,
which operate on file structures. This way a user may access your hardware via the command line.

It is assumend that `stdin` is the terminal input (serial in, keyboard, etc.) and `stdout` is the terminal
output (serial out, screen, etc.). These 'files' are used by the shell itself, but also are passed to 
commands to be executed.

## Licence
Written by Grzegorz Grzęda under MIT Licence in 2021, Poland.