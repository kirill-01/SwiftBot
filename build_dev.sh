#!/bin/bash
clear
str=$(cat VERSION)
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
sed "s/${str}/${newver}/" -i opt/swiftbot/html/index.html
sed "s/${str}/${newver}/" -i VERSION
sed "s/${str}/${newver}/" -i DEBIAN/control
CURNAME=$(basename $PWD)
echo '' > err_log_${CURNAME}
echo '' > build_log_${CURNAME}
echo "Building binaries for version: ${newver}" 
[ -f ${CURNAME}.deb ] && rm -rf ${CURNAME}.deb
echo "Cleaning previous build..."
make -j8 --quiet clean > build_log_${CURNAME} 2> err_log_${CURNAME}
make -j8 --quiet distclean > build_log_${CURNAME} 2> err_log_${CURNAME}
qmake -qt=qt513 -r . > build_log_${CURNAME} 2> err_log_${CURNAME}
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; cat err_log_${CURNAME}; exit 1; }
make -j8 --quiet > build_log_${CURNAME} 2> err_log_${CURNAME}
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; cat err_log_${CURNAME}; exit 1; }

echo "Preparing binaries"
copy_binary "swift-bot/swift-bot"
copy_binary "swift-console/swift-console"
copy_binary "swift-arbitrage-api-node/swift-arbitrage-api-node"
copy_binary "swift-config/swift-config"
copy_binary "swift-setconfig/swift-setconfig"
copy_binary "swift-showconfig/swift-showconfig"
copy_binary "swift-orderbooks-proxy/swift-orderbooks-proxy"
copy_binary "workers/workers"

cp -rf swift-lib/libswift-lib* opt/swiftbot/lib/
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; exit 1; }
cp -rf qmsgpack/bin/libqmsgpack.so* opt/swiftbot/lib/
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; exit 1; }


cp -rf VERSION opt/swiftbot/build_version
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; exit 1; }
[ -d $PWD/.git ] && mv -f ${PWD}/.git ${PWD}/../._git

rm -rf opt/swiftbot/js/node_modules
cd ${PWD}/.. && fakeroot dpkg-deb --build ${CURNAME}
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; exit 1; }

cd ${CURNAME}
[ -d $PWD/../._git ] && mv -f $PWD/../._git ${PWD}/.git
[ -f $PWD/../${CURNAME}.deb ] && mv $PWD/../${CURNAME}.deb /tmp/${CURNAME}-${newver}.deb

echo "Copy package to main host"

rsync -av --progress /tmp/${CURNAME}-${newver}.deb root@cc.finance:/opt/swiftbot/html/package-dev.deb  > build_log_${CURNAME} 2> err_log_${CURNAME}
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; exit 1; }
rm -rf /tmp/${CURNAME}-${newver}.deb
