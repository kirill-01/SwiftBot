#!/bin/bash
clear
str=$(cat VERSION)
qtdir="/usr/local/Qt-5.14.1"
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

newver=$str
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
qmake -qt=qt514 -r .
echo "Building app..."
make --quiet -j8 all
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; cat err_log_${CURNAME}; exit 1; }
echo "Preparing binaries..."
[ -d opt/swiftbot/bin ] || mkdir -p opt/swiftbot/bin
copy_binary "swift-bot/swift-bot"
copy_binary "swift-arbitrage-api-node/swift-arbitrage-api-node"
copy_binary "swift-config/swift-config"
copy_binary "swift-setconfig/swift-setconfig"
copy_binary "swift-showconfig/swift-showconfig"
copy_binary "swift-orderbooks-proxy/swift-orderbooks-proxy"
copy_binary "workers/workers"
cp -rf opt/swiftbot/swift-up.sh /usr/local/bin/swift-up.sh
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
rm -rf opt/swiftbot/js/node_modules
rm -rf .git
cd ${PWD}/.. && fakeroot dpkg-deb --build ${CURNAME} && cd core
(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; exit 1; }
 
rm -rf /var/www/app/web/core-latest.deb && rm -rf ${dockerbuilddir}/core.deb
cp -rf ../${CURNAME}.deb /var/www/app/web/core-latest.deb
cp -rf ../${CURNAME}.deb ${dockerbuilddir}/core.deb

cd ${dockerbuildbasedir}
docker build --force-rm	--no-cache --rm -t swiftbot-core-base:latest .


cd ${dockerbuilddir}
docker build --force-rm	--no-cache -t swiftbot-core:latest .

echo ${newver} > /var/www/app/web/core-version.txt

FILS=""
for filename in /swift/docker/*.yml; do
    FILS="$FILS -f $filename"
done
cd /swift/docker
echo "Recreating containers"
msg='{"procedure": "firebase.sendmessage", "args": ["SwiftBot", "Производится обновление системы до версии '${newver}'."]}'
docker-compose $FILS up --force-recreate --remove-orphans --renew-anon-volumes -d
#curl -H "Content-Type: application/json"     -d "${msg}" ws.cc.finance:8080/callkaa
#curl -H "Content-Type: application/json"     -d "${msg}" ws.cc.finance:8080/callpchel
dpkg -i /var/www/app/web/core-latest.deb && systemctl restart swift-orderbooks-proxy.service
#curl -H "Content-Type: application/json"     -d '{"procedure": "swiftbot.system.update", "args": []}' ws.cc.finance:8080/callkaa
#curl -H "Content-Type: application/json"     -d '{"procedure": "swiftbot.system.update", "args": []}' ws.cc.finance:8080/callpchel


#echo "Git publishing"
#git commit -am "Version ${newver} deploy"  > build_log_${CURNAME} 2> err_log_${CURNAME}
#git push origin master  > build_log_${CURNAME} 2> err_log_${CURNAME}
#(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; exit 1; }
#echo "Copy package to main host"

#rsync -av --progress -e "ssh -p 20002" /tmp/${CURNAME}-${newver}.deb root@cc.finance:/opt/swiftbot/html/package.deb  > build_log_${CURNAME} 2> err_log_$#{CURNAME}
#rsync -av --progress -e "ssh -p 20002" VERSION root@cc.finance:/opt/swiftbot/html/Version  > build_log_${CURNAME} 2> err_log_${CURNAME}
#(($? != 0)) && { printf '%s\n' "Command exited with non-zero"; exit 1; }
#[ -d ../docker_build ] && rm -rf ../docker_build

#mkdir -p ../docker_build
#cp -rf /tmp/${CURNAME}-${newver}.deb ../docker_build/package.deb
#[ -d /swift ] && cp -rf /tmp/${CURNAME}-${newver}.deb /swift/core-latest.deb
#rm -rf /tmp/${CURNAME}-${newver}.deb
#cd ../docker_build
#echo "Docker building and publishing"
#docker login -u Wounderer -p e0828f44bdaf504069556d89b2ede480874899f8 docker.pkg.github.com
#docker build -t cc.finance:5000/swiftbot:latest --no-cache .
#docker push cc.finance:5000/swiftbot:latest
