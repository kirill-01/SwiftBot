#!/bin/bash
realm=${1}
method=${2}
params=${3}
msg='{"procedure": "'${method}'", "args": '${params}'}'
echo $msg
result=$(curl -s -H "Content-Type: application/json" -d "${msg}" https://${realm}.cc.finance/call)
echo ""
echo ${result}
echo ""
