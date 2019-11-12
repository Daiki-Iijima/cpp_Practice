#!/bin/sh
g++ -framework GLUT -framework OpenGL $1.cpp
./a.out
rm a.out