from twisted.internet.defer import inlineCallbacks
from twisted.logger import Logger

from autobahn.twisted.util import sleep
from autobahn.twisted.wamp import ApplicationSession
from autobahn.wamp.exception import ApplicationError

import subprocess

class AppSession(ApplicationSession):

    log = Logger()

    @inlineCallbacks
    def onJoin(self, details):

        def upgradeAll():
            subprocess.run(["/swift/restart_compose.sh"])
            return 1

        def restartRealm( realm ):
            subprocess.run(["docker-compose -f /swift/docker/"+realm+".yml up --force-recreate --remove-orphans"])
            return 1

        def stopRealm( realm ):
            subprocess.run(["docker-compose -f /swift/docker/"+realm+".yml stop"])
            return 1

        def startRealm( realm ):
            subprocess.run(["docker-compose -f /swift/docker/"+realm+".yml up --force-recreate --remove-orphans"])
            return 1

        reg = yield self.register(startRealm, u'swiftbot.realm.start')
        reg1 = yield self.register(stopRealm, u'swiftbot.realm.stop')
        reg2 = yield self.register(restartRealm, u'swiftbot.realm.restart')
        reg3 = yield self.register(upgradeAll, u'swiftbot.realm.upgradeAll')