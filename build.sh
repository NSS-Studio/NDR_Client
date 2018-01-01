#!/bin/sh
# macosx && linux_deb

version="0.74"

if [ $# = 1 -a $1 = "clean" ]; then
    rm -rf ./build/
    rm -f ./NDR_Client.dmg
    printf "clean done!\n"
    exit 0
fi

if [ $# != 2 -a $# != 3 ]; then
    printf "parameter invaild\n"
    exit 1
fi

if [ $1 = "macosx" ]; then
    platform="macx-clang"
elif [ $1 = "debian" ]; then
    platform="linux-g++"
else
    printf "platdorm invaild\n"
    exit 1
fi

if [ $2 = "debug" -o $2 = "release" ]; then
    configBuildMode=$2
    if [ $2 = "debug" ]; then
        ndrTesting="true"
    elif [ $2 = "release" ]; then
        ndrTesting="false"
    fi
else
    printf "buildInfo invaild\n"
    exit 1
fi

if [ $# != 3 ]; then
    QMAKE="qmake"
else
    QMAKE=$3
fi

if [ $1 = "debian" ]; then
    sudo apt update
    sudo apt install build-essential
    sudo apt install network-manager-dev
    sudo apt install libgl1-mesa-dev
fi


if [ $1 = "macosx" ]; then
    rm -f ./NDR_Client.dmg
fi

rm -rf ./build/
cd client/ts-file/
chmod +x ./update.sh
sh ./update.sh
chmod +x ./release.sh
sh ./release.sh
cd ../../

mkdir build
cd build

$QMAKE ../ndr-client.pro -spec $platform CONFIG+=$configBuildMode CONFIG+=$2 DEFINES+=NDR_TESTING=$ndrTesting
make
if [ $1 = "macosx" ]; then
    macdeployqt ./client/NDR\ Client.app/
    cd ..
    appdmg ./client/icons/package.json ~/Desktop/ndr_${version}_macosx.dmg
    printf "deployqt done!\n"
    printf "new dmg in desktop\n"
    exit 0

else if [ $1 = "debian" ]; then

    printf "deployqt done!\n"
    printf "create deb in desktop\n"
    exit 0
fi



