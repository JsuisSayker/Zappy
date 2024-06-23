import json
import math
import random
import re
from collections import Counter
from itertools import cycle
from ZappyAI.src.ai.infos import LEVELS, Activity


class AI():
    def __init__(self, teamName: str):
        self.teamName: str = teamName
        self.inventory: dict[str, int] = {"food": 0, "linemate": 0,
                                          "deraumere": 0, "sibur": 0,
                                          "mendiane": 0, "phiras": 0,
                                          "thystame": 0}
        self.sharedInventory: dict = {}
        self.clearRead = False
        self.clearBroadcast = False
        self.level: int = 1
        self.incantation: bool = False
        self.nbReadyPlayers: int = 1
        self.playerIsReady: bool = False
        self.searchingRessource: str = ""
        self.look: str = ""
        self.commandList: list[str] = []
        self.run = True
        self.dataToSend: str = ""
        self.availableSlots: int = 0
        self.canFork: bool = False
        self.clientId: int = 1
        self.widthValue: int = 0
        self.heightValue: int = 0
        self.minWidthValue: int = 0
        self.minHeightValue: int = 0
        self.newRessource: bool = False
        self.actualActivity: Activity = Activity.STARTING

    def lookingSize(self, array: list) -> int:
        return next((i for i, elem in enumerate(array) if elem == []),
                    len(array))

    def parsingAiInventory(self, data: str) -> None:
        readyToIncantate = True
        tmpData: list = []
        for char in "[]":
            data = data.replace(char, "")
        tmpData = data.split(",")
        for i in range(len(tmpData)):
            if tmpData[i][0] == " ":
                tmpData[i] = tmpData[i][1:]
        print(f"DATA IN PARSE INVENTORY: {tmpData}")
        for elem in tmpData:
            if elem:
                print(f"While parsing inventory: |{elem}|")
                print(f"Inventory: |{self.inventory}|")
                print(f"elem.split()[0]: |{elem.split()[0]}|")
                print(f"inventory object to change: |{
                      self.inventory[elem.split()[0]]}|")
                self.inventory[elem.split()[0]] = int(elem.split()[1])
        for resources in LEVELS[self.level]:
            print(f"GOUGOU: {resources}")
            if self.inventory[resources] < LEVELS[self.level][resources]:
                readyToIncantate = False
        if readyToIncantate is True:
            self.actualActivity = Activity.PREPA_FOR_INCANTATION

    def sxor(self, s1: str, s2: str):
        return ''.join(chr(ord(c) ^ ord(k)) for c, k in zip(s2, cycle(s1)))

    def parseReceivedMessageFromBroadcast(self, messageReceived: str) -> None:
        signalDirection = int(messageReceived[8])
        print(f"signalDirection: {signalDirection}")
        print(f"messageReceived before: {messageReceived}")
        parsedReceivedMessage = self.sxor(self.teamName,
                                          bytes.fromhex(messageReceived[11:]
                                                        ).decode("utf-8"))
        print(f"parsedReceivedMessage value: {parsedReceivedMessage}")
        if "inventory" in parsedReceivedMessage:
            print("parsing inventory")
            print(f"yay of inventory: {parsedReceivedMessage[9:]}")
            self.parse_shared_inventory(parsedReceivedMessage[9:])
        if "incantation" in parsedReceivedMessage:
            if self.clearBroadcast is True:
                self.clearBroadcast = False
                return
            if self.nbReadyPlayers >= 1 and int(
                    parsedReceivedMessage.split("|")[0]) > 0:
                self.incantation = False
                self.actualActivity = Activity.EXECUTE_COMMAND
                self.nbReadyPlayers = 1
            elif self.incantation is True:
                self.goToBroadcastSignal(signalDirection)
        if "on my way" in parsedReceivedMessage:
            self.goToBroadcastSignal(signalDirection)
            print("ON MY WAY")
            return
        if "ready" in parsedReceivedMessage:
            print("ready")
            self.nbReadyPlayers += 1

    def checkIncanationActivity(self) -> None:
        if self.newRessource is True:
            if not self.isIncantationPossible():
                messageToSend = bytes(
                    self.sxor(self.teamName,
                              ("inventory" + str(
                                  self.clientId) + "|" + str(
                                  self.level) + "|" + str(
                                  json.dumps(
                                                self.inventory)))),
                    "utf-8").hex()
                print("SENDING INVENTORY AND OTHER THINGS")
                print(f"messageToSend: {messageToSend}")
                self.dataToSend = "Broadcast " + messageToSend + "\n"
                # self.newRessource = False
            else:
                messageToSend = bytes(self.sxor(self.teamName, (str(
                    self.clientId) + "|incantation|" + str(
                        self.level))), "utf-8").hex()
                print("SENDING INCANTATION AND OTHER THINGS")
                print(f"messageToSend: {messageToSend}")
                self.dataToSend = "Broadcast " + messageToSend + "\n"
                self.newRessource = False
                self.incantation = True
                self.nbReadyPlayers = 1
                self.actualActivity = Activity.PREPA_FOR_INCANTATION
                return
            self.newRessource = False
        else:
            self.actualActivity = Activity.LOOKING
        return

    def prepareForIncantationActivity(self) -> None:
        if self.incantation is False:
            messageToSend = bytes(self.sxor(self.teamName, str(
                self.clientId) + " on my way"), "utf-8").hex()
            print("SENDING ON MY WAY")
            self.dataToSend = "Broadcast " + messageToSend + "\n"
            self.incantation = True
        elif self.commandList and not self.playerIsReady:
            self.dataToSend = self.commandList.pop(0)
            self.clearRead = True
        elif (self.nbReadyPlayers >= self.level and self.incantation is True)\
                or (self.playerIsReady is True and
                    "Broadcast" in self.dataToSend):
            self.actualActivity = Activity.INCANTATING
        return

    def incantatingActivity(self) -> None:
        self.clearBroadcast = False
        self.startingIncantation()
        self.dropRessourcesWhileIncantating()
        if self.commandList:
            self.dataToSend = self.commandList.pop(0)
        else:
            self.dataToSend = "Inventory\n"
        return

    def executeCommand(self) -> None:
        if self.commandList:
            self.dataToSend = self.commandList.pop(0)
        else:
            self.dataToSend = "Inventory\n"
            self.actualActivity = Activity.CHECK_INCANTATION
        return

    def parse_shared_inventory(self, data: str) -> None:
        receivedClientId, receivedMessage, receivedInventory = data.split("|")
        self.sharedInventory[self.clientId] = self.inventory
        self.sharedInventory[receivedClientId] = json.loads(receivedInventory)
        updateCounter: Counter = Counter()
        for key in self.sharedInventory:
            if key != "total":
                updateCounter.update(self.sharedInventory[key])
        self.sharedInventory["total"] = dict(updateCounter)

    def updateSharedInventory(self) -> None:
        self.sharedInventory[self.clientId] = self.inventory
        updateCounter: Counter = Counter()
        for key in self.sharedInventory:
            if key != "total":
                updateCounter.update(self.sharedInventory[key])
        self.sharedInventory["total"] = dict(updateCounter)

    def isIncantationPossible(self) -> bool:
        requiredRessources = LEVELS[self.level]
        print(f"REQUIRED RESSOURCES TO LEVEL UP : {requiredRessources}")
        tmpInventory: dict = {}
        if "total" in self.sharedInventory:
            tmpInventory = self.sharedInventory["total"]
        print(f"SHARED INVENTORY: {tmpInventory}")
        print(f"SEARCHED RESOURCE: {self.searchingRessource}")
        if self.searchingRessource in tmpInventory:
            tmpInventory[self.searchingRessource] += 1
        for ressource in requiredRessources:
            print(f"RESSOURCE: {ressource}")
            print(f"REQUIRED RESSOURCES: {requiredRessources[ressource]}")
            print(f"BIG INVENTORY: {tmpInventory[ressource]}")
            if tmpInventory[ressource] < requiredRessources[ressource]:
                return False
        return True

    def goToBroadcastSignal(self, signalDirection: int) -> None:
        if self.playerIsReady is True or self.commandList:
            return
        match signalDirection:
            case 0:
                messageToSend = bytes(self.sxor(
                    self.teamName, ("ready")), "utf-8").hex()
                print("SENDING READY")
                self.dataToSend = "Broadcast " + messageToSend + "\n"
                self.playerIsReady = True
                self.commandList = []
            case 1 | 2 | 8:
                print("I'M GOING FORWARD IN THE GO TO BROADCAST SIGNAL")
                self.commandList.insert(0, "Forward\n")
            case 5 | 6 | 7:
                print("I'M GOING RIGHT IN THE GO TO BROADCAST SIGNAL")
                self.commandList.insert(0, "Right\n")
            case _:
                print("I'M GOING LEFT IN THE GO TO BROADCAST SIGNAL")
                self.commandList.insert(0, "Left\n")

    def startingIncantation(self) -> None:
        data = self.look.split(",")[0]
        print(f"DATA IN STARTING INCANTATION: {data}")
        while True:
            if len(data) == 0 or data[0].isalpha():
                break
            data = data[1:]
        tmpData = data.split(" ")
        tmpInventory = {}
        if "total" in self.sharedInventory:
            tmpInventory = self.sharedInventory["total"]
        print(
            f"SHARED INVENTORY IN THE START OF THE INCANTATION: {
                tmpInventory}")
        print(f"TMPDATA IN THE START OF THE INCANTATION: {tmpData}")
        print(f"ACTUAL INVENTORY: {self.inventory}")
        requiredRessources = (LEVELS[self.level]).copy()
        for ressource in requiredRessources:
            for elem in tmpData:
                if ressource == elem:
                    requiredRessources[ressource] -= 1
        for ressource in requiredRessources:
            if requiredRessources[ressource] > 0:
                return
        self.dataToSend = "Incantation\n"
        self.commandList.insert(0, "Incantation\n")

    def dropRessourcesWhileIncantating(self) -> None:
        ressourcesData = self.look.split(",")[0]
        while True:
            if len(ressourcesData) == 0 or ressourcesData[0].isalpha():
                break
            ressourcesData = ressourcesData[1:]
        ressourcesData = ressourcesData.split(" ")
        requiredRessources = LEVELS[self.level].copy()
        print(
            f"REQUIRED RESSOURCES IN THE DROP FUNCTION: {requiredRessources}")
        for ressource in requiredRessources:
            for elem in ressourcesData:
                if ressource == elem:
                    requiredRessources[ressource] -= 1
        for ressource in requiredRessources:
            if requiredRessources[ressource] < 1:
                continue
            print(f"requiredRessources[{ressource}]: {requiredRessources[
                ressource]}")
            if self.inventory[ressource] != 0 and\
                    ressource in self.inventory:
                self.commandList.append("Set " + ressource + "\n")
                self.commandList.append("Look\n")
                self.inventory[ressource] -= 1
                return

    def startingActivity(self) -> None:
        self.dataToSend = "Connect_nbr\n"
        if self.availableSlots > 0:
            self.canFork = True
        self.actualActivity = Activity.POPULATING

    def populatingActivity(self) -> None:
        if self.availableSlots > 0 and int(self.clientId) < 6:
            print("Fork")
            self.dataToSend = "Fork\n"
            self.canFork = False
        else:
            self.dataToSend = "Look\n"
        self.actualActivity = Activity.EXECUTE_COMMAND

    def generateEmptyMap(self) -> list:
        return [[[] for i in range(self.minWidthValue)] for j in range(
            self.minHeightValue)]

    def fillMapWithObjects(self, map: list, dataList: list) -> list:
        nb = 1
        v = (self.minWidthValue - 1)
        h = 0
        i = 0

        line = int(math.sqrt(len(dataList)))
        for _ in range(line):
            tv = v - h
            for _ in range(nb):
                map[tv][h].append(dataList[i])
                tv += 1
                i += 1
            nb = (nb + 2)
            h += 1
        return map

    def findObjectOnMap(self, map: list, object: str) -> list[int] | None:
        v = self.minWidthValue - 1
        h = 0
        while h < self.lookingSize(map[v]):
            if map[v][h] != [] and object in map[v][h][0]:
                return [v, h]
            for offset in range(1, h + 1):
                if map[v - offset][h] != [] and object in map[
                        v - offset][h][0]:
                    return [v - offset, h]
                if map[v + offset][h] != [] and object in map[
                        v + offset][h][0]:
                    return [v + offset, h]
            h += 1
        return None

    def fillingInventory(self, data: str, objectToSearch: str) -> list | list[
            str]:
        tmpList = []
        finalResult = []
        print(f"data: {data}")
        tmpData = data.split(",")
        for i in range(len(tmpData)):
            tmpList.append(' '.join(re.split(r'\W+', tmpData[i])[1:]))
        print(f"tmpList: {tmpList}")
        generatedMap = self.generateEmptyMap()
        self.fillMapWithObjects(generatedMap, tmpList)
        objectPosition = self.findObjectOnMap(generatedMap,
                                              objectToSearch)
        print(f"coordinates: {objectPosition}")
        print(f"objectToSearch: {objectToSearch}")
        if objectPosition is None:
            for i in range(3):
                finalResult.append(random.choice(["Forward\n", "Right\n",
                                                  "Left\n"]))
            return finalResult
        elif (objectPosition[0] == (self.minWidthValue - 1)
              and objectPosition[1] == 0):
            return ["Take " + objectToSearch + "\n"]
        else:
            for i in range(int(objectPosition[0]) - (self.minWidthValue - 1)):
                finalResult.append("Forward\n")
            if (objectPosition[0] == (self.minWidthValue - 1)):
                for i in range(int(objectPosition[1])):
                    finalResult.append("Forward\n")
                finalResult.append("Take " + objectToSearch + "\n")
                finalResult.append("Inventory\n")
            elif (int(objectPosition[0]) < (self.minWidthValue - 1)):
                finalResult.append("Left\n")
                for i in range((self.minWidthValue - 1) -
                               int(objectPosition[0])):
                    finalResult.append("Forward\n")
                finalResult.append("Take " + objectToSearch + "\n")
                finalResult.append("Inventory\n")
            elif (int(objectPosition[0]) > (self.minWidthValue - 1)):
                finalResult.append("Right\n")
                for i in range(
                        int(objectPosition[0]) - (self.minWidthValue - 1)):
                    finalResult.append("Forward\n")
                finalResult.append("Take " + objectToSearch + "\n")
                finalResult.append("Inventory\n")
            elif (objectPosition[1] == 0):
                finalResult.append("Take " + objectToSearch + "\n")
        return finalResult

    def fillingActivity(self, data: str) -> None:
        if "food" in self.inventory and self.inventory["food"] < 50:
            self.commandList = self.fillingInventory(data, "food")
        else:
            self.searchingRessource = self.searchingActivity()
            self.commandList = self.fillingInventory(data,
                                                     self.searchingRessource)
        self.actualActivity = Activity.EXECUTE_COMMAND

    def lookingActivity(self) -> None:
        self.clearBroadcast = False
        self.dataToSend = "Look\n"
        self.actualActivity = Activity.FILLING

    def searchingActivity(self) -> str:
        tmpList = []
        requiredRessources = LEVELS[self.level]
        for ressource in requiredRessources:
            print(f"RESSOURCE SEARCHING METHOD: {ressource}")
            print(
                f"REQUIREDRESOURCE VALUE SEARCHING METHOD: {
                    requiredRessources[ressource]}")
            print(f"SELF INVENTORY IN SEARCHING METHOD: {self.inventory}")
            if requiredRessources[ressource] > self.inventory[ressource] or (
                    ressource not in self.inventory):
                tmpList.append(ressource)
        if len(tmpList) == 0:
            return "food"
        return random.choice(tmpList)

    def finishingIncantation(self) -> None:
        self.dataToSend = "Inventory\n"
        print("I'm finishing the incantation")
        self.actualActivity = Activity.WRITING_LAST_MESSAGE

    def writeLastMessage(self) -> None:
        print("I'm writing the last message")
        messageToSend = bytes(self.sxor(self.teamName, (
            "inventory" + str(self.clientId) + "|" + str(
                self.level) + "|" + str(
                    json.dumps(self.inventory)))
        ), "utf-8").hex()
        self.dataToSend = "Broadcast " + messageToSend + "\n"
        self.actualActivity = Activity.EXECUTE_COMMAND

    def algorithm(self) -> None:
        self.nbPlayers = int(self.clientId)
        match self.actualActivity:
            case Activity.STARTING:
                self.startingActivity()
            case Activity.POPULATING:
                self.populatingActivity()
            case Activity.LOOKING:
                self.lookingActivity()
            case Activity.FILLING:
                self.fillingActivity(self.look)
            case Activity.EXECUTE_COMMAND:
                self.executeCommand()
            case Activity.CHECK_INCANTATION:
                self.checkIncanationActivity()
            case Activity.PREPA_FOR_INCANTATION:
                self.prepareForIncantationActivity()
            case Activity.INCANTATING:
                self.incantatingActivity()
            case Activity.FINISHING_INCANTATION:
                self.finishingIncantation()
            case Activity.WRITING_LAST_MESSAGE:
                self.writeLastMessage()
            case Activity.CLEARING_DATA:
                self.dataToSend = ""
