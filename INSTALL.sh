#!/usr/bin/env bash

echo create-cpp-project Installation script
echo "--------------------------------------"
echo

echo Create build directory:

mkdir build

echo Building...

cd build

cmake -DCMAKE_BUILD_TYPE=Release ..

make

echo Installing...

if sudo make install; then
    echo Installed :\)
else
    echo
    echo "--------------------------------------"
    echo Installation failed :\(
fi

echo
echo "--------------------------------------"
echo Done! You can run the utility using the \'create-cpp-project\' command.

echo If you want to uninstall, run UNINSTALL.sh