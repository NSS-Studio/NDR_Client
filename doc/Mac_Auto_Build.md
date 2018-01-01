## 依赖
```
Qt 5.9.3 or later
Nodejs 8
appdmg
```

## 安装

nodejs
https://nodejs.org/en/download/
qt
https://www.qt.io/download/
appdmg
https://github.com/LinusU/node-appdmg

```
npm install -g appdmg
```

## 命令
```sh
请先指定PATH路径
QT=<qt bin>
PATH="$QT:$PATH"

清理构建内容垃圾
sh ./build.sh clean

构建macosx 的 Release版本
sh ./build.sh macosx release

构建macosx 的 Debug版本
sh ./build.sh macosx debug

构建macosx 的 Release版本 指定Qt bin
sh ./build.sh macosx release $QT

构建macosx 的 debug版本 指定Qt bin
sh ./build.sh macosx debug $QT
```