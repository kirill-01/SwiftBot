#!/bin/bash
TARGET_HOST="root@vel.cc.finance"
rsync -av --progress ~/SWIFT/src/swift-bot/opt/swiftbot/js ${TARGET_HOST}:/opt/swiftbot/
rsync -av --progress ~/SWIFT/src/swift-bot/opt/swiftbot/html ${TARGET_HOST}:/opt/swiftbot/

