#!/bin/bash
echo "Building TestApplication..."
echo "> Using debug build <"

#cd $HOME/Code/C++/Cpp.Include.Linux/src/TestApplication

g++ TestApplication.cpp --std=c++17 -luuid -lpthread -o TestApplication
if [[ $? -eq 0 ]]
then
    echo "> TestApplication build Ok <"
else
    echo "> TestApplication build Error <"
fi

g++ TestDaemon.cpp --std=c++17 -o TestDaemon
if [[ $? -eq 0 ]]
then
    echo "> TestDaemon build Ok <"
else
    echo "> TestDaemon build Error <"
fi

echo "> Build process Complete <"