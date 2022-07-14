#!/bin/bash
cd $HOME/Code/C++/Cpp.Include.Linux/src/TestApplication
g++ -g TestApplication.cpp --std=c++17 -luuid -lpthread -o TestApplication
g++ -g TestDaemon.cpp --std=c++17 -o TestDaemon
