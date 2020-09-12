#!/bin/bash
remote_version=$(rm -rf core-version.txt && wget -q https://main.cc.finance/core-version.txt && cat core-version.txt)
local_version=$(cat /opt/swiftbot/build_version)
[ ${remote_version} != ${local_version} ] && {
  echo "Has update to ${remote_version} from ${local_version} "
  rm -rf core-latest.deb
  wget -q https://main.cc.finance/core-latest.deb && dpkg -i core-latest.deb 
  systemctl daemon-reload
}
