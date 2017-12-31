# re

# qmake /Users/rabenda/NDR_Client/ndr-client.pro -spec macx-clang CONFIG+=debug CONFIG+=x86_64 && /usr/bin/make qmake_all


# macosx && linux_deb && linux_rpm

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

if [ $# != 3 ]; then
    QMAKE="qmake"
else
    QMAKE=$3
fi

mkdir build
cd build

$QMAKE ../ndr-client.pro -spec $platform CONFIG+=$configBuildMode CONFIG+=$2

make
macdeployqt ./client/NDR\ Client.app/ -dmg
mv ./client/NDR\ Client.dmg ./../NDR_Client.dmg
cd ../
rm -rf ./build/
printf "deployqt done!"
