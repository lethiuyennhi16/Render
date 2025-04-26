#!/bin/bash

# Compile
g++ main.cpp Model.cpp texture.cpp shader.cpp glad.c -o model_viewer -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

# Kiểm tra compile có thành công không
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Kiểm tra có tham số được truyền vào không
if [ $# -eq 0 ]; then
    echo "Usage: $0 <model1.obj> <x1> <y1> <z1> [model2.obj x2 y2 z2] ..."
    echo "Example: $0 body.obj 0 0 0 head.obj 3 1 -2"
    exit 1
fi

# Chạy chương trình với các tham số được truyền vào
./model_viewer "$@"
