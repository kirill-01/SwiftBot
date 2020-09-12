#!/bin/bash
clear
str=$(cat VERSION)
QT_SELECT="qt514"
QTTOOLDIR="/home/kkuznetsov/Qt/5.14.2/gcc_64/bin"
QTLIBDIR="/home/kkuznetsov/Qt/5.14.2/gcc_64/lib"
qtdir="/home/kkuznetsov/Qt/5.14.2/gcc_64"
dockerbuilddir="${PWD}/DockerBuild"
dockerbuildbasedir="${PWD}/DockerBuildBase"
export PATH=$PATH:${qtdir}/bin:${qtdir}/lib
export QT5PREFIX=${qtdir}
export QT5BINDIR=$QT5PREFIX/bin

increment_version() {
 local v=$1
 if [ -z $2 ]; then
    local rgx='^((?:[0-9]+\.)*)([0-9]+)($)'
 else
    local rgx='^((?:[0-9]+\.){'$(($2-1))'})([0-9]+)(\.|$)'
    for (( p=`grep -o "\."<<<".$v"|wc -l`; p<$2; p++)); do
       v+=.0; done; fi
 val=`echo -e "$v" | perl -pe 's/^.*'$rgx'.*$/$2/'`
 echo "$v" | perl -pe s/$rgx.*$'/${1}'`printf %0${#val}s $(($val+1))`/
}

copy_binary() {
 local binpath=$1
 cp ${binpath} opt/swiftbot/bin/
 (($? != 0)) && { printf '%s\n' "Cant prepare and copy binary ${binpath}"; exit 1; }
}

newver=$(increment_version $str)
sed "s/${str}/${newver}/" -i swift-bot/main.cpp
sed "s/${str}/${newver}/" -i swift-arbitrage-api-node/main.cpp
sed "s/${str}/${newver}/" -i workers/settingshostworker.cpp
sed "s/${str}/${newver}/" -i workers/ordersworker.cpp
sed "s/${str}/${newver}/" -i VERSION
sed "s/${str}/${newver}/" -i DEBIAN/control
CURNAME=$(basename $PWD)
echo '' > err_log_${CURNAME}
echo '' > build_log_${CURNAME}
echo "Building binaries for version: ${newver}" 
[ -f ${CURNAME}.deb ] && rm -rf ${CURNAME}.deb
echo "Cleaning previous build..."
#make -j8 --quiet clean > build_log_${CURNAME} 2> err_log_${CURNAME}
#cd swift-lib && make -j8 --quiet clean && make -j8 --quiet distclean && cd ..
#cd swift-bot && make -j8 --quiet clean && make -j8 --quiet distclean && cd ..
#cd swift-arbitrage-api-node && make -j8 --quiet clean && make -j8 --quiet distclean && cd ..
#cd workers && make -j8 --quiet clean && make -j8 --quiet distclean && cd ..
#make -j8 --quiet distclean > build_log_${CURNAME} 2> err_log_${CURNAME}
qmake -qt=qt514  CONFIG+="release" -r .
echo "Building app..."
make --quiet -j8 all
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; cat err_log_${CURNAME}; exit 1; }
echo "Preparing binaries..."
[ -d opt/swiftbot/bin ] || mkdir -p opt/swiftbot/bin
copy_binary "swift-bot/swift-bot"
copy_binary "swift-arbitrage-api-node/swift-arbitrage-api-node"
copy_binary "swift-orderbooks-proxy/swift-orderbooks-proxy"
copy_binary "workers/workers"
rm -rf opt/swiftbot/lib/libQt* 
rm -rf opt/swiftbot/plugins/sqldrivers/*
cp -rf /usr/lib/x86_64-linux-gnu/libcrypto.so* opt/swiftbot/lib/
cp -rf /usr/lib/x86_64-linux-gnu/libssl.so* opt/swiftbot/lib/
cp -rf ${qtdir}/plugins/sqldrivers/*  opt/swiftbot/plugins/sqldrivers/

cp -rf ${qtdir}/lib/libQt5WebSockets.so.*  opt/swiftbot/lib/
cp -rf ${qtdir}/lib/libQt5Network.so.* opt/swiftbot/lib/
cp -rf ${qtdir}/lib/libQt5Sql.so.* opt/swiftbot/lib/
cp -rf ${qtdir}/lib/libQt5Core.so.* opt/swiftbot/lib/
rm -rf opt/swiftbot/lib/*.debug
rm -rf opt/swiftbot/lib/libswift*
cp -rf swift-lib/libswift-lib.so.2* opt/swiftbot/lib
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; exit 1; }
rm -rf opt/swiftbot/lib/libqmsgpack*
rm -rf opt/swiftbot/lib/qwamp*
cp -rf qmsgpack/bin/libqmsgpack.so* opt/swiftbot/lib/
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; exit 1; }

cp -rf VERSION opt/swiftbot/build_version
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; exit 1; }
#[ -d $PWD/.git ] && mv -f ${PWD}/.git ${PWD}/../._git
cd ${PWD}/.. && fakeroot dpkg-deb --build ${CURNAME} && cd core
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; exit 1; }
#scp ../core.deb root@example.cc.finance:/tmp/

scp ../core.deb user@pchel.cc.finance:/tmp/
