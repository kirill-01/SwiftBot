#!/bin/bash
str=$(cat VERSION)

commentgui() {
 sed "s/swift-gui/#swift-gui/" -i swift-bot-2.pro
}

[[ -z $(cat swift-bot-2.pro | grep '#swift-gui') ]] && commentgui

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

newver=$(increment_version $str)
sed "s/${str}/${newver}/" -i swift-bot/main.cpp
sed "s/${str}/${newver}/" -i swift-bot/userstatuskeeper.cpp
sed "s/${str}/${newver}/" -i opt/swiftbot/html/index.html
sed "s/${str}/${newver}/" -i VERSION
sed "s/${str}/${newver}/" -i DEBIAN/control

git add .
git commit -am "Deploy version ${newver}"
git push origin deploy

