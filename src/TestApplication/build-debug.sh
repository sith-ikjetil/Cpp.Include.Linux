#!/bin/bash
#: Title       : build-debug.sh
#: Date        : 2022-07-23
#: Author      : Kjetil Kristoffer Solberg <post@ikjetil.no>
#: Version     : 1.0
#: Description : Builds TestApplication and TestDaemon.
echo "Building TestApplication..."
echo "> using debug build <"

g++ -ggdb TestApplication.cpp -std=c++17 -luuid -D_GLIBCXX_GTHREAD_USE_WEAK=0 -static -pthread -o TestApplication
if [[ $? -eq 0 ]]
then
    echo "> TestApplication build ok <"
else
    echo "> TestApplication build error <"
fi
echo "> build process complete <"

echo ""

echo "Building TestDaemon ..."
echo "> using debug build <"
g++ -g TestDaemon.cpp --std=c++17 -o TestDaemon
if [[ $? -eq 0 ]]
then
    echo "> TestDaemon build ok <"
else
    echo "> TestDaemon build error <"
fi
echo "> build process complete <"