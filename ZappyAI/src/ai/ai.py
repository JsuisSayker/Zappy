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
        self.level: int = 1
        self.incantation: bool = False
        # self.nbNeededPlayers: int = 1
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
        # self.minWidthValue: int = 0
        # self.minHeightValue: int = 0
        self.newRessource: bool = False
        self.actualActivity: Activity = Activity.STARTING

    def lookingSize(self, array: list) -> int:
        return next((i for i, elem in enumerate(array) if elem == []),
                    len(array))

    def parse_inventory(self, data):
        for char in "[]":
            data = data.replace(char, "")
        data = data.split(",")
        for i in range(len(data)):
            if data[i][0] == " ":
                data[i] = data[i][1:]
        # data[len(data) - 1] = data[len(data) - 1][:-1]
        print(f"data in parse inventory: {data}")
        for elem in data:
            # Uncomment the following lines to avoid the killing of the AI while incantating
            # if "ok" or "ko" in elem:
            #     return
            if elem:
                print(f"While parsing inventory: |{elem}|")
                self.inventory[elem.split()[0]] = int(elem.split()[1])

    def sxor(self, s1: str, s2: str):
        """Xor two strings"""
        return ''.join(chr(ord(c)^ord(k)) for c,k in zip(s2, cycle(s1)))

    def parse_broadcast(self, messageReceived: str):
        signalDirection = int(messageReceived[8])
        print(f"signalDirection: {signalDirection}")
        print(f"messageReceived before: {messageReceived}")
        print(f"the second part of the message received: [{messageReceived[11:]}]")
        parsedReceivedMessage = self.sxor(self.teamName,
                                    bytes.fromhex(messageReceived[11:]
                                                  ).decode("utf-8"))
        print(f"parsedReceivedMessage value: {parsedReceivedMessage}")
        if "inventory" in parsedReceivedMessage:
            print("parsing inventory")
            print(f"yay of inventory: {parsedReceivedMessage[9:]}")
            self.parse_inventory(parsedReceivedMessage[9:])
        if "incantation" in parsedReceivedMessage:
            if self.incantation is True:
                self.goToBroadcastSignal(signalDirection)
        if "on my way" in parsedReceivedMessage:
            return
        if "ready" in parsedReceivedMessage:
            print("ready")
            self.nbReadyPlayers += 1

    def checkIncanationActivity(self):
        if self.newRessource is True:
            if self.newRessource is True:
                if not self.isIncantationPossible():
                    messageToSend = bytes(
                        self.sxor(self.teamName,
                                  ("inventory" + str(
                                      self.clientId) + ";" + str(
                                          self.level) + ";" + str(
                                              json.dumps(
                                                  self.inventory)))),
                        "utf-8").hex()
                    print("SENDING INVENTORY AND OTHER THINGS")
                    print(f"messageToSend: {messageToSend}")
                    self.dataToSend = "Broadcast " + messageToSend + "\n"
                    self.incantation = False
                else:
                    messageToSend = bytes(self.sxor(self.teamName, (str(
                        self.clientId) + ";incantation;" + str(
                            self.level))), "utf-8").hex()
                    print("SENDING INCANTATION AND OTHER THINGS")
                    print(f"messageToSend: {messageToSend}")
                    # messageToSend = messageToSend[2:-1]
                    self.dataToSend = "Broadcast " + messageToSend + "\n"
                    self.newRessource = False
                    self.incantation = True
                    self.actualActivity = Activity.PREPA_FOR_INCANTATION
                    return
        else:
            self.actualActivity = Activity.LOOKING
        return

    def prepareForIncantationActivity(self):
        if self.incantation is False:
            messageToSend = bytes(self.sxor(self.teamName, str(
                self.clientId) + " on my way"), "utf-8").hex()
            print("SENDING ON MY WAY")
            self.dataToSend = "Broadcast " + messageToSend + "\n"
        elif self.commandList and not self.playerIsReady:
            self.dataToSend = self.commandList.pop(0)
        elif (self.nbReadyPlayers >= self.level and self.incantation is True)\
                or (self.playerIsReady is True and
                    "Broadcast" in self.dataToSend):
            self.actualActivity = Activity.INCANTATING
        else:
            self.dataToSend = ""
        return

    def incantatingActivity(self):
        self.startingIncantation()
        self.dropRessourcesWhileIncantating()
        if self.commandList:
            self.dataToSend = self.commandList.pop(0)
        else:
            self.dataToSend = "Inventory\n"
        return

    def executeCommand(self):
        if self.commandList:
            self.dataToSend = self.commandList.pop(0)
        else:
            self.dataToSend = "Inventory\n"
            self.actualActivity = Activity.CHECK_INCANTATION
        return

    def updateSharedInventory(self):
        self.sharedInventory[self.clientId] = self.inventory
        updateCounter = Counter()
        for key in self.sharedInventory:
            updateCounter.update(self.sharedInventory[key])

    def isIncantationPossible(self) -> bool:
        requiredRessources = LEVELS[self.level]
        if self.searchingRessource in self.inventory:
            self.inventory[self.searchingRessource] += 1
        for ressource in requiredRessources:
            if self.inventory[ressource] < requiredRessources[ressource]:
                return False
        return True

    def goToBroadcastSignal(self, signalDirection: int):
        if self.playerIsReady is True or self.commandList != []:
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
                self.commandList.append("Forward\n")
            case 5 | 6 | 7:
                self.commandList.append("Right\n")
            case _:
                self.commandList.append("Left\n")

    def startingIncantation(self) -> None:
        data = self.look.split(",")[0]
        # self.look.split(",")
        while True:
            if len(data) == 0 or data[0].isalpha():
                break
            data = data[1:]
        requiredRessources = LEVELS[self.level]
        for ressource in requiredRessources:
            for elem in data:
                if ressource == elem:
                    requiredRessources[ressource] -= 1
        for ressource in requiredRessources:
            if requiredRessources[ressource] > 0:
                return
        self.commandList.append("Incantation\n")

    def dropRessourcesWhileIncantating(self):
        ressourcesData = self.look.split(",")[0]
        while True:
            if len(ressourcesData) == 0 or ressourcesData[0].isalpha():
                break
            ressourcesData = ressourcesData[1:]
        ressourcesData = ressourcesData.split(" ")
        requiredRessources = LEVELS[self.level]
        for ressource in requiredRessources:
            for elem in ressourcesData:
                if ressource == elem:
                    requiredRessources[ressource] -= 1
        for ressource in requiredRessources:
            if requiredRessources[ressource] < 1:
                continue
            print(f"requiredRessources[{ressource}]: {requiredRessources[
                ressource]}")
            if requiredRessources[ressource] != 0 and\
                    ressource in self.inventory:
                self.commandList.append("Set " + ressource + "\n")
                self.commandList.append("Look\n")
                self.inventory[ressource] -= 1

    def startingActivity(self):
        self.dataToSend = "Connect_nbr\n"
        if self.availableSlots > 0:
            self.canFork = True
        self.actualActivity = Activity.POPULATING

    def populatingActivity(self):
        if self.availableSlots > 0 and int(self.clientId) < 6:
            print("Fork")
            self.dataToSend = "Fork\n"
            self.canFork = False
        else:
            self.dataToSend = "Look\n"
        self.actualActivity = Activity.EXECUTE_COMMAND

    def generateEmptyMap(self) -> list:
        # return [[[] for _ in range(self.widthValue)] for _ in range(
        #     self.heightValue)]
        return [[[] for i in range(9)] for j in range(17)]
        # return [["" for _ in range(self.widthValue)
        #          ] for _ in range(self.heightValue)]

    def fillMapWithObjects(self, map: list, dataList: list):
        # nb = 1
        # v = 8
        # h = 0
        # i = 0

        # line = int(math.sqrt(len(dataList)))
        # for j in range(line):
        #     tv = v - h
        #     for a in range(nb):
        #         map[tv][h].append(dataList[i])
        #         tv += 1
        #         i += 1
        #     nb = (nb + 2)
        #     h += 1
        # return map
        nb = 1
        # v = self.minHeightValue - 1
        v = 8
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

    def findObjectOnMap(self, map: list, object: str):
        # v = 8
        # h = 0
        # while h < self.lookingSize(map[v]):
        #     if map[v][h] != [] and object in map[v][h][0]:
        #         return [v, h]
        #     for offset in range(1, h + 1):
        #         if map[v - offset][h] != [] and object in map[
        #                 v - offset][h][0]:
        #             return [v - offset, h]
        #         if map[v + offset][h] != [] and object in map[
        #                 v + offset][h][0]:
        #             return [v + offset, h]
        #     h += 1
        # return None
        # v = self.heightValue
        v = 8
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

    def fillingInventory(self, data: str, objectToSearch: str):
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
        if objectPosition is None:
            for i in range(3):
                finalResult.append(random.choice(["Forward\n", "Right\n",
                                                  "Left\n"]))
            return finalResult
        elif (objectPosition[0] == 8 and objectPosition[1] == 0):
            return ["Take " + objectToSearch + "\n"]
        else:
            for i in range(int(objectPosition[0]) - 8):
                finalResult.append("Forward\n")
            if (objectPosition[0] == 8):
                for i in range(int(objectPosition[1])):
                    finalResult.append("Forward\n")
                finalResult.append("Take " + objectToSearch + "\n")
                finalResult.append("Inventory\n")
            if (objectPosition[1] == 0):
                finalResult.append("Take " + objectToSearch + "\n")
            if (int(objectPosition[0]) < 8):
                finalResult.append("Left\n")
                for i in range(8 - int(objectPosition[0])):
                    finalResult.append("Forward\n")
                finalResult.append("Take " + objectToSearch + "\n")
                finalResult.append("Inventory\n")
            if (int(objectPosition[0]) > 8):
                finalResult.append("Right\n")
                for i in range(int(objectPosition[0]) - 8):
                    finalResult.append("Forward\n")
                finalResult.append("Take " + objectToSearch + "\n")
                finalResult.append("Inventory\n")
        return finalResult
        # tmpList = []
        # finalResult = []
        # print(f"data: {data}")
        # tmpData = data.split(",")
        # for i in range(len(tmpData)):
        #     tmpList.append(' '.join(re.split(r'\W+', tmpData[i])[1:]))
        # print(f"tmpList: {tmpList}")
        # generatedMap = self.generateEmptyMap()
        # self.fillMapWithObjects(generatedMap, tmpList)
        # objectPosition = self.findObjectOnMap(generatedMap, objectToSearch)
        # print(f"coordinates: {objectPosition}")
        # if objectPosition is None:
        #     for _ in range(3):
        #         finalResult.append(random.choice(["Forward\n", "Right\n",
        #                                           "Left\n"]))
        #     return finalResult
        # elif (objectPosition[0] == self.heightValue and
        #       objectPosition[1] == 0):
        #     return ["Take " + objectToSearch + "\n"]
        # else:
        #     for _ in range(int(objectPosition[0]) - self.heightValue):
        #         finalResult.append("Forward\n")
        #     if (objectPosition[0] == self.heightValue):
        #         for _ in range(int(objectPosition[1])):
        #             finalResult.append("Forward\n")
        #         finalResult.append("Take " + objectToSearch + "\n")
        #         finalResult.append("Inventory\n")
        #     if (objectPosition[1] == 0):
        #         finalResult.append("Take " + objectToSearch + "\n")
        #     if (int(objectPosition[0]) < self.heightValue):
        #         finalResult.append("Left\n")
        #         for _ in range(self.heightValue - int(objectPosition[0])):
        #             finalResult.append("Forward\n")
        #         finalResult.append("Take " + objectToSearch + "\n")
        #         finalResult.append("Inventory\n")
        #     if (int(objectPosition[0]) > self.heightValue):
        #         finalResult.append("Right\n")
        #         for _ in range(int(objectPosition[0]) - self.heightValue):
        #             finalResult.append("Forward\n")
        #         finalResult.append("Take " + objectToSearch + "\n")
        #         finalResult.append("Inventory\n")
        # return finalResult

    def fillingActivity(self, data: str):
        if "food" in self.inventory and self.inventory["food"] < 50:
            self.commandList = self.fillingInventory(data, "food")
        else:
            self.searchingRessource = self.searchingActivity()
            self.commandList = self.fillingInventory(data,
                                                     self.searchingRessource)
        self.actualActivity = Activity.EXECUTE_COMMAND

    def lookingActivity(self):
        self.dataToSend = "Look\n"
        self.actualActivity = Activity.FILLING

    def searchingActivity(self) -> str:
        tmpList = []
        requiredRessources = LEVELS[self.level]
        for ressource in requiredRessources:
            if requiredRessources[ressource] > self.inventory[ressource] or (
                    ressource not in self.inventory):
                tmpList.append(ressource)
        if len(tmpList) == 0:
            return "food"
        return random.choice(tmpList)

    def finishingIncantation(self):
        self.dataToSend = "Inventory\n"
        print("I'm finishing the incantation")
        self.actualActivity = Activity.WRITING_LAST_MESSAGE

    def writeLastMessage(self):
        print("I'm writing the last message")
        messageToSend = bytes(self.sxor(self.teamName, (
            "inventory" + str(self.clientId) + ";" + str(
                self.level) + ";" + str(
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
