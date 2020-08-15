#!/bin/bash
file=Bridge
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=${file}-out.txt _install/bin/${file}
