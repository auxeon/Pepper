#!/bin/bash
gcc -lglfw -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon main.c ../core/ps_chronon.c ../lib/glad/src/glad.c -I ../lib/glad/include -I ../lib/glad/src
