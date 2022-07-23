#!/bin/bash
#: Title       : build-release.sh
#: Date        : 2020-12-25
#: Author      : Kjetil Kristoffer Solberg <post@ikjetil.no>
#: Version     : 1.0
#: Description : Builds debug TestApplication and TestDaemon.
echo "Building TestApplication..."
echo "> using release build <"

g++ TestApplication.cpp --std=c++17 -luuid -lpthread -o TestApplication
if [[ $? -eq 0 ]]
then
    echo "> TestApplication build ok <"
else
    echo "> TestApplication build error <"
fi
echo "> build process complete <"

echo "Building TestDaemon ..."
echo "> using release build <"
g++ TestDaemon.cpp --std=c++17 -o TestDaemon
if [[ $? -eq 0 ]]
then
    echo "> TestDaemon build ok <"
else
    echo "> TestDaemon build error <"
fi
echo "> build process complete <"