#!/bin/bash
#: Title       : build-release.sh
#: Date        : 2020-12-25
#: Author      : Kjetil Kristoffer Solberg <post@ikjetil.no>
#: Version     : 1.0
#: Description : Builds debug TestApplication and TestDaemon.
echo "Building TestApplication..."
echo "> using release build <"
g++ TestApplication.cpp -std=c++23 -luuid -D_GLIBCXX_GTHREAD_USE_WEAK=0 -static -pthread -o TestApplication
if [[ $? -eq 0 ]]
then
    echo "> TestApplication build ok <"
else
    echo "> TestApplication build error <"
fi
echo "> build process complete <"

echo ""

echo "Building TestDaemon ..."
echo "> using release build <"
g++ TestDaemon.cpp --std=c++23 -o TestDaemon
if [[ $? -eq 0 ]]
then
    echo "> TestDaemon build ok <"
else
    echo "> TestDaemon build error <"
fi
echo "> build process complete <"

echo ""

echo "Building TestClient..."
echo "> using release build <"
g++ TestClient.cpp -o TestClient -std=c++23
if [[ $? -eq 0 ]]
then
    echo "> TestClient build ok <"
else
    echo "> TestClient build error <"
fi
echo "> build process complete <"

echo ""

echo "Building TestServer..."
echo "> using release build <"
g++ TestServer.cpp -o TestServer -std=c++23
if [[ $? -eq 0 ]]
then
    echo "> TestServer build ok <"
else
    echo "> TestServer build error <"
fi
echo "> build process complete <"
