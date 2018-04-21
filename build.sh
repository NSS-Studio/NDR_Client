#!/bin/sh
# macosx && linux_deb

version="0.75"

if [ $# = 1 -a $1 = "clean" ]; then
    rm -rf ./build/
    rm -f ./NDR_Client.dmg
    rm -f ./debian/opt/ndr/bin/ndr-client
    rm -rf ./debian/opt/ndr/lib
    rm -rf ./debian/opt/ndr/lib/platforms
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

if [ $# != 3 -a $1 != "debian" ]; then
    QMAKE="qmake"
else
    QT=$3
    QMAKE=$QT/bin/qmake
fi



if [ $1 = "macosx" ]; then
    rm -f ./NDR_Client.dmg
elif [ $1 = "debian" ]; then
    rm -f ./debian/opt/ndr/bin/ndr-client
    rm -rf ./debian/opt/ndr/bin/lib
    rm -rf ./debian/opt/ndr/bin/plugins
    # sudo apt install build-essential network-manager-dev libgl1-mesa-dev
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

cd ..
if [ $1 = "macosx" ]; then
    macdeployqt ./build/client/NDR\ Client.app/
    printf "deployqt done!\n"
    appdmg ./client/icons/package.json ~/Desktop/ndr_${version}_macosx.dmg
    printf "new dmg in desktop\n"
    exit 0
elif [ $1 = "debian" ]; then
    linuxdeployqt-continuous-x86_64.AppImage ./build/client/ndr-client # -no-plugins
    mkdir -p ./debian/opt/ndr/bin/
    cp -f	./build/client/ndr-client	./debian/opt/ndr/bin/ndr-client
    cp -f	./build/client/qt.conf	./debian/opt/ndr/bin/qt.conf
    cp -rf	./build/client/lib		./debian/opt/ndr/bin/lib
    cp -rf	./build/client/plugins	./debian/opt/ndr/bin/plugins
    cp -rf  ./build/client/doc ./debian/opt/ndr/bin/doc
    printf "deployqt done!\n"
    dpkg -b debian ndr_${version}_amd64.deb
    mv ndr_${version}_amd64.deb ~/Desktop/ndr_${version}_amd64.deb
    printf "create deb in desktop\n"
    exit 0
fi
