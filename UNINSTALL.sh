#!/usr/bin/env bash

echo create-cpp-project uninstall script
echo "--------------------------------------"
echo

cd build

if sudo make uninstall; then
    echo Uninstalled
else
    echo
    echo "--------------------------------------"
    echo Uninstall failed :\(
fi

echo
echo "--------------------------------------"
echo Done! create-cpp-project was uninstalled.