import json
import math
import random
import re
from collections import Counter

from ZappyAI.src.ai.encryptString import xorStrings
from ZappyAI.src.ai.lookingSizeOfMap import lookingSizeOfMap
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

    def parsingAiInventory(self, data: str) -> None:
        readyToIncantate = True
        tmpData: list = []
        for char in "[]":
            data = data.replace(char, "")
        tmpData = data.split(",")
        for i in range(len(tmpData)):
            if tmpData[i][0] == " ":
                tmpData[i] = tmpData[i][1:]
        for elem in tmpData:
            if elem:
                self.inventory[elem.split()[0]] = int(elem.split()[1])
        for resources in LEVELS[self.level]:
            if self.inventory[resources] < LEVELS[self.level][resources]:
                readyToIncantate = False
        if readyToIncantate is True:
            self.actualActivity = Activity.PREPA_FOR_INCANTATION

    def parseReceivedMessageFromBroadcast(self, messageReceived: str) -> None:
        signalDirection = int(messageReceived[8])
        parsedReceivedMessage = xorStrings(self.teamName,
                                           bytes.fromhex(messageReceived[11:]
                                                         ).decode("utf-8"))
        if "inventory" in parsedReceivedMessage:
            self.parsingOfSharedInventory(parsedReceivedMessage[9:])
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
            return
        if "ready" in parsedReceivedMessage:
            self.nbReadyPlayers += 1

    def checkIncanationActivity(self) -> None:
        if self.newRessource is True:
            if not self.isIncantationPossible():
                messageToSend = bytes(
                    xorStrings(self.teamName,
                               ("inventory" + str(
                                   self.clientId) + "|" + str(
                                   self.level) + "|" + str(
                                   json.dumps(
                                       self.inventory)))),
                    "utf-8").hex()
                self.dataToSend = "Broadcast " + messageToSend + "\n"
            else:
                messageToSend = bytes(xorStrings(self.teamName, (str(
                    self.clientId) + "|incantation|" + str(
                        self.level))), "utf-8").hex()
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
        """
        The function `prepareForIncantationActivity` prepares data for an
        incantation activity based on certain conditions. It can send a
        broadcast message to other players to inform them that the player is
        ready to perform the incantation. It can also send a command to the
        server.

        @return The function `prepareForIncantationActivity` returns None.
        """
        if self.incantation is False:
            messageToSend = bytes(xorStrings(self.teamName, str(
                self.clientId) + " on my way"), "utf-8").hex()
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
        """
        This function performs an incantation activity by executing a series of
        steps and preparing data to send. It start the incantaion if possible
        and drop the required ressources to perform the ritual.

        @return None
        """
        self.clearBroadcast = False
        self.startingIncantation()
        self.dropRessourcesWhileIncantating()
        if self.commandList:
            self.dataToSend = self.commandList.pop(0)
        else:
            self.dataToSend = "Inventory\n"
        return

    def executeCommand(self) -> None:
        """
        The `executeCommand` function pops a command from a list or sets a
        default command if the list is empty.

        @return None
        """
        if self.commandList:
            self.dataToSend = self.commandList.pop(0)
        else:
            self.dataToSend = "Inventory\n"
            self.actualActivity = Activity.CHECK_INCANTATION
        return

    def parsingOfSharedInventory(self, data: str) -> None:
        receivedClientId, receivedMessage, receivedInventory = data.split("|")
        self.sharedInventory[self.clientId] = self.inventory
        self.sharedInventory[receivedClientId] = json.loads(receivedInventory)
        updateCounter: Counter = Counter()
        for key in self.sharedInventory:
            if key != "commonResources":
                updateCounter.update(self.sharedInventory[key])
        self.sharedInventory["commonResources"] = dict(updateCounter)

    def updateSharedInventory(self) -> None:
        self.sharedInventory[self.clientId] = self.inventory
        updateCounter: Counter = Counter()
        for key in self.sharedInventory:
            if key != "commonResources":
                updateCounter.update(self.sharedInventory[key])
        self.sharedInventory["commonResources"] = dict(updateCounter)

    def isIncantationPossible(self) -> bool:
        requiredRessources = LEVELS[self.level]
        tmpInventory: dict = {}
        if self.level < 3:
            tmpInventory = self.inventory
        elif "commonResources" in self.sharedInventory:
            tmpInventory = self.sharedInventory["commonResources"]
        if self.searchingRessource in tmpInventory:
            tmpInventory[self.searchingRessource] += 1
        for ressource in requiredRessources:
            if tmpInventory[ressource] < requiredRessources[ressource]:
                return False
        return True

    def goToBroadcastSignal(self, signalDirection: int) -> None:
        if self.playerIsReady is True or self.commandList:
            return
        match signalDirection:
            case 0:
                messageToSend = bytes(xorStrings(
                    self.teamName, ("ready")), "utf-8").hex()
                self.dataToSend = "Broadcast " + messageToSend + "\n"
                self.playerIsReady = True
                self.commandList = []
            case 1 | 2 | 8:
                self.commandList.insert(0, "Forward\n")
            case 5 | 6 | 7:
                self.commandList.insert(0, "Right\n")
            case _:
                self.commandList.insert(0, "Left\n")

    def startingIncantation(self) -> None:
        data = self.look.split(",")[0]
        while True:
            if len(data) == 0 or data[0].isalpha():
                break
            data = data[1:]
        tmpData = data.split(" ")
        tmpInventory = {}
        if self.level < 3:
            tmpInventory = self.inventory
        elif "commonResources" in self.sharedInventory:
            tmpInventory = self.sharedInventory["commonResources"]
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
        for ressource in requiredRessources:
            for elem in ressourcesData:
                if ressource == elem:
                    requiredRessources[ressource] -= 1
        for ressource in requiredRessources:
            if requiredRessources[ressource] < 1:
                continue
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
            self.dataToSend = "Fork\n"
            self.canFork = False
        else:
            self.dataToSend = "Look\n"
        self.actualActivity = Activity.EXECUTE_COMMAND

    def generateEmptyMap(self) -> list:
        return [[[] for i in range(self.minWidthValue)] for j in range(
            self.minHeightValue)]

    def fillMapWithObjects(self, fakeMap: list, dataList: list) -> list:
        nb = 1
        minValue = (self.minWidthValue - 1)
        maxHeight = 0
        indexOfElemInLisr = 0

        line = int(math.sqrt(len(dataList)))
        for _ in range(line):
            diffMinMax = minValue - maxHeight
            for _ in range(nb):
                fakeMap[diffMinMax][maxHeight].append(dataList[
                    indexOfElemInLisr])
                diffMinMax += 1
                indexOfElemInLisr += 1
            nb = (nb + 2)
            maxHeight += 1
        return fakeMap

    def findObjectOnMap(self, fakeMap: list, object: str) -> list[int] | None:
        minValue = (self.minWidthValue - 1)
        maxHeight = 0
        while maxHeight < lookingSizeOfMap(fakeMap[minValue]):
            if fakeMap[minValue][maxHeight] != [] and object in fakeMap[
                    minValue][maxHeight][0]:
                return [minValue, maxHeight]
            for offset in range(1, maxHeight + 1):
                if fakeMap[minValue - offset][maxHeight] != [] and object in\
                        fakeMap[minValue - offset][maxHeight][0]:
                    return [minValue - offset, maxHeight]
                if fakeMap[minValue + offset][maxHeight] != [] and object in\
                        fakeMap[minValue + offset][maxHeight][0]:
                    return [minValue + offset, maxHeight]
            maxHeight += 1
        return None

    def fillingInventory(self, data: str, objectToSearch: str) -> list | list[
            str]:
        """
        The `fillingInventory` function takes in a string of data and an object
        to search for, generates a map, finds the object on the map, and
        returns a list of actions to take based on the object's position.

        @param data The `fillingInventory` function seems to be a method that
        processes some data to generate a list of actions based on a search
        for a specific object within a map. It's part of a
        larger program or system that involves navigating and interacting with
        objects in a virtual environment.

        @param objectToSearch The `objectToSearch` parameter in the
        `fillingInventory` method is the object that you are searching for in
        the inventory. This method takes a string `data` which contains a list
        of objects and their positions, and `objectToSearch` which is the
        object you want to find in

        @return The `fillingInventory` method returns a list of instructions
        for navigating a map and interacting with objects based on the input
        data and the object to search for. The returned list may contain a
        sequence of "Forward", "Right", and "Left" movements, as well as
        instructions to "Take" the specified object and display the
        "Inventory".
        """
        tmpList = []
        finalResult = []
        tmpData = data.split(",")
        for i in range(len(tmpData)):
            tmpList.append(' '.join(re.split(r'\W+', tmpData[i])[1:]))
        generatedMap = self.generateEmptyMap()
        self.fillMapWithObjects(generatedMap, tmpList)
        objectPosition = self.findObjectOnMap(generatedMap,
                                              objectToSearch)
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
        """
        The `fillingActivity` function checks the inventory for food and fills
        it if below 50, otherwise it searches for a resource and fills the
        inventory with it.

        @param data The `data` parameter in the `fillingActivity` method seems
        to be a string that is being passed as an argument to the method.
        This string likely contains information or instructions related to the
        activity being performed. The method uses this data to determine how
        to fill the inventory based on certain conditions.
        """
        if "food" in self.inventory and self.inventory["food"] < 50:
            self.commandList = self.fillingInventory(data, "food")
        else:
            self.searchingRessource = self.searchingActivity()
            self.commandList = self.fillingInventory(data,
                                                     self.searchingRessource)
        self.actualActivity = Activity.EXECUTE_COMMAND

    def lookingActivity(self) -> None:
        """
        The function `lookingActivity` sets attributes related to broadcasting
        and data transmission for a specific activity.
        """
        self.clearBroadcast = False
        self.dataToSend = "Look\n"
        self.actualActivity = Activity.FILLING

    def searchingActivity(self) -> str:
        """
        This function searches for a resource needed for a specific level
        activity and returns a random resource if not all required resources
        are available in the inventory.

        @return The `searchingActivity` method returns either "food" if all
        required resources are available in the inventory, or a randomly
        chosen resource from the list of resources that are needed for the
        activity.
        """
        tmpList = []
        requiredRessources = LEVELS[self.level]
        for ressource in requiredRessources:
            if requiredRessources[ressource] > self.inventory[ressource] or (
                    ressource not in self.inventory):
                tmpList.append(ressource)
        if len(tmpList) == 0:
            return "food"
        return random.choice(tmpList)

    def finishingIncantation(self) -> None:
        """
        The function `finishingIncantation` sets the `dataToSend` attribute to
        "Inventory\n" and the `actualActivity` attribute to
        `Activity.WRITING_LAST_MESSAGE`.
        """
        self.dataToSend = "Inventory\n"
        self.actualActivity = Activity.WRITING_LAST_MESSAGE

    def writeLastMessage(self) -> None:
        """
        The function `writeLastMessage` prepares a message to be broadcasted
        with encoded data based on the team's information.
        """
        messageToSend = bytes(xorStrings(self.teamName, (
            "inventory" + str(self.clientId) + "|" + str(
                self.level) + "|" + str(
                    json.dumps(self.inventory)))
        ), "utf-8").hex()
        self.dataToSend = "Broadcast " + messageToSend + "\n"
        self.actualActivity = Activity.EXECUTE_COMMAND

    def algorithm(self) -> None:
        """
        This Python function defines a series of activities based on the
        current state of the program.
        """
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
