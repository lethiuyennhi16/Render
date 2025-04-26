#!/bin/bash
g++ -g main.cpp Model.cpp texture.cpp shader.cpp glad.c -o model_viewer -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

gdb ./model_viewer

