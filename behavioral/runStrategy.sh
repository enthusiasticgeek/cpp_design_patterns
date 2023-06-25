#!/bin/bash
file=Strategy
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=${file}-out.txt _install/bin/${file}
