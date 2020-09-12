#!/bin/bash
# depends
apt install git python3.6 python3-pip php7.2-fpm nginx php7.2-mbstring php7.2-xml composer nodejs npm libmysqlclient-dev mc build-essential libssl1.0-dev  software-properties-common libsnappy-dev htop curl php-apcu php7.2-json php7.2-mysql php7.2-intl node-gyp apache2-utils libsnappy-dev certbot -y
# percona
wget https://repo.percona.com/apt/percona-release_latest.$(lsb_release -sc)_all.deb
dpkg -i percona-release_latest.$(lsb_release -sc)_all.deb
percona-release setup ps80
apt-get install percona-server-server -y
# crossbar
#mkdir -p /crossbar/
#cd /crossbar
#pip3 install crossbar
# swiftbot
cd /tmp
wget http://cc.finance:82/package.deb
dpkg -i /tmp/package.deb
rm -rf /tmp/package.deb

mkdir -p /opt/swiftbot/ssl
ln -s /etc/letsencrypt/archive/wamp.kaa.cc.finance/fullchain1.pem /opt/swiftbot/ssl/wcert.pem
ln -s /etc/letsencrypt/archive/wamp.kaa.cc.finance/privkey1ri.pem /opt/swiftbot/ssl/wkey.pem
ln -s /etc/letsencrypt/archive/monitor.kaa.cc.finance/fullchain1.pem /opt/swiftbot/ssl/cert.pem
ln -s /etc/letsencrypt/archive/monitor.kaa.cc.finance/privkey1.pem /opt/swiftbot/ssl/key.pem
chown -R www-data:www-data /opt/swiftbot/ssl

