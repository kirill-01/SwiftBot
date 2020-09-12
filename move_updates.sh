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

CURNAME=$(basename $PWD)
echo '' > err_log_${CURNAME}
echo '' > build_log_${CURNAME}

echo "Cleaning previous build..."
#make -j8 --quiet clean > build_log_${CURNAME} 2> err_log_${CURNAME}
#cd swift-lib && make -j8 --quiet clean && make -j8 --quiet distclean && cd ..
#cd swift-bot && make -j8 --quiet clean && make -j8 --quiet distclean && cd ..
#cd swift-arbitrage-api-node && make -j8 --quiet clean && make -j8 --quiet distclean && cd ..
#cd workers && make -j8 --quiet clean && make -j8 --quiet distclean && cd ..
#make -j8 --quiet distclean > build_log_${CURNAME} 2> err_log_${CURNAME}
qmake -qt=qt513  -r .
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; cat err_log_${CURNAME}; exit 1; }
make all
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; cat err_log_${CURNAME}; exit 1; }

echo "Preparing binaries"
copy_binary "swift-bot/swift-bot"
copy_binary "swift-arbitrage-api-node/swift-arbitrage-api-node"
copy_binary "swift-config/swift-config"
copy_binary "swift-setconfig/swift-setconfig"
copy_binary "swift-showconfig/swift-showconfig"
copy_binary "swift-orderbooks-proxy/swift-orderbooks-proxy"
copy_binary "workers/workers"
rm -rf opt/swiftbot/lib/libQt* 
cp -rf /home/optimusprime/Qt/5.13.2/gcc_64/lib/libQt5WebSockets.so.*  opt/swiftbot/lib/
cp -rf /home/optimusprime/Qt/5.13.2/gcc_64/lib/libQt5Network.so.* opt/swiftbot/lib/
cp -rf /home/optimusprime/Qt/5.13.2/gcc_64/lib/libQt5Sql.so.* opt/swiftbot/lib/
cp -rf /home/optimusprime/Qt/5.13.2/gcc_64/lib/libQt5Core.so.* opt/swiftbot/lib/
rm -rf opt/swiftbot/lib/*.debug
rm -rf opt/swiftbot/lib/libswift-lib*
cp -rf swift-lib/libswift-lib.so.2* opt/swiftbot/lib
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; exit 1; }
cp -rf qmsgpack/bin/libqmsgpack.so* opt/swiftbot/lib/
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; exit 1; }

echo "Copy package to main host"

rsync -av --progress opt/swiftbot/bin root@vel.cc.finance:/opt/swiftbot/
rsync -av --progress opt/swiftbot/lib/libswift-lib* root@vel.cc.finance:/opt/swiftbot/lib/

