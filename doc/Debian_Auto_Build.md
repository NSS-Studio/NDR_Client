## 依赖
```
Qt 5.11.2 or later
linuxdeployqt-continuous-x86_64.AppImage
```

## 安装

qt

https://www.qt.io/download/

linuxdeployqt

https://github.com/probonopd/linuxdeployqt

```
wget -c https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
chmod +x linuxdeployqt-continuous-x86_64.AppImage
export PATH=`pwd`:$PATH
```

## 命令
```sh
请先指定PATH路径
QT=<Install Path>

清理构建内容垃圾
sh ./build.sh clean

构建debian 的 Release版本 指定Qt bin
sh ./build.sh macosx release $QT

构建debian 的 debug版本 指定Qt bin
sh ./build.sh macosx debug $QT
```