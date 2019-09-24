#!/bin/sh
# macosx && linux_deb && arch pkg.tar.xz

version="0.80"

buildRootDir=`pwd`

clean(){
    rm -rf ./build/
    rm -f ./NDR_Client.dmg ~/Desktop/ndr_${version}_macosx.dmg
    rm -f ./debian/opt/ndr/bin/ndr-client
    rm -rf ./debian/opt/ndr/lib
    rm -rf ./debian/opt/ndr/lib/platforms
    rm -rf ./debian/opt/ndr/bin/plugins
    rm -rf ./package/arch/debian
    printf "Clean Done!\n"
}

if [ $# = 1 -a $1 = "clean" ]; then
    clean
    exit 0
fi

if [ $# != 3 ]; then
    printf "parameter invaild\n"
# Please Add Usage():
    exit 1
fi

if [ $1 = "macosx" ]; then
    platform="macx-clang"
elif [ $1 = "debian" -o $1 = "arch" ]; then
    platform="linux-g++"
else
    printf "platdorm invaild\n"
    exit 1
fi

if [ $2 = "debug" -o $2 = "release" ]; then
    configBuildMode=$2
else
    printf "buildInfo invaild\n"
    exit 1
fi

QT=$3
QMAKE=$QT/bin/qmake

clean # clean
# sudo apt install -y build-essential network-manager-dev libgl1-mesa-dev

rm -rf ./build/
cd client/ts-file/
chmod +x ./update.sh
sh ./update.sh
chmod +x ./release.sh
sh ./release.sh
cd ../../

mkdir build
cd build

$QMAKE ../ndr-client.pro -spec $platform CONFIG+=$configBuildMode PREFIX=/opt/ndr/
make -j1

cd ..
if [ $1 = "macosx" ]; then
    $QT/bin/macdeployqt ./build/client/NDR\ Client.app/ -qmldir=.
    printf "deployqt done!\n"
    appdmg ./client/icons/package.json ~/Desktop/ndr_${version}_macosx.dmg
    printf "new dmg in desktop\n"
    exit 0
elif [ $1 = "debian" -o $1 = "arch" ]; then
    chmod +x linuxdeployqt-continuous-x86_64.AppImage
    ./linuxdeployqt-continuous-x86_64.AppImage ./build/client/ndr-client # -no-plugins
    mkdir -p ./debian/opt/ndr/bin/
    cp -f	./build/client/ndr-client	./debian/opt/ndr/bin/ndr-client
    cp -f	./build/client/qt.conf	./debian/opt/ndr/bin/qt.conf
    cp -rf	./build/client/lib		./debian/opt/ndr/bin/lib
    cp -rf	./build/client/plugins	./debian/opt/ndr/bin/plugins
    cp -rf  ./build/client/doc ./debian/opt/ndr/bin/doc
    printf "deployqt done!\n"
    if [ $1 = "debian" ]; then
        dpkg -b debian ndr_${version}_amd64.deb
        mv ndr_${version}_amd64.deb ~/Desktop/ndr_${version}_amd64.deb
        printf "create deb in desktop\n"
        exit 0
    elif [ $1 = "arch" ]; then
        cp -rf debian $buildRootDir/package/arch/debian
        cd $buildRootDir/package/arch
        makepkg -s
        printf "create arch pkg.tar.xz in desktop\n"
        exit 0
    fi
fi
