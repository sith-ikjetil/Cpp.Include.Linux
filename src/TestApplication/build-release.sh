#!/bin/bash
cd $HOME/Code/C++/Cpp.Include.Linux/src/TestApplication
g++ TestApplication.cpp --std=c++17 -luuid -lpthread
