import random
import re
from collections import Counter
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
        self.newRessource: bool = False
        self.actualActivity: Activity = Activity.STARTING

    def findObjectOnMap(self, map: list, object: str) -> dict | None:
        for i, row in enumerate(map):
            if object in row:
                return {"x": row.index(object), "y": i}
        return None

    def parse_inventory(self, data):
        for char in "[]":
            data = data.replace(char, "")
        data = data.split(",")
        for i in range(len(data)):
            data[i] = data[i][1:]
        data[len(data) - 1] = data[len(data) - 1][:-1]
        for elem in data:
            if elem:
                self.inventory[elem.split()[0]] = int(elem.split()[1])

    def checkIncanationActivity(self):
        if self.newRessource is True:
            if self.newRessource is True:
                if not self.isIncantationPossible():
                    messageToSend = "toto"
                    self.dataToSend = "Broadcast " + messageToSend + "\n"
                else:
                    messageToSend = "incantation"
                    self.dataToSend = "Broadcast " + messageToSend + "\n"
                    self.newRessource = False
                    self.actualActivity = Activity.PREPA_FOR_INCANTATION
                    return
        else:
            self.actualActivity = Activity.LOOKING
        return

    def prepareForIncantationActivity(self):
        return

    def executeCommand(self):
        if self.commandList:
            self.dataToSend = self.commandList.pop(0)
        else:
            self.dataToSend = "Inventory\n"
            self.actualActivity = Activity.CHECK_INCANTATION

    def updateSharedInventory(self):
        self.sharedInventory[self.clientId] = self.inventory
        updateCounter = Counter()
        for key in self.sharedInventory:
            updateCounter.update(self.sharedInventory[key])

    def isIncantationPossible(self) -> bool:
        requiredRessources = LEVELS[self.level]
        if self.searchingRessource in self.inventory:
            requiredRessources[self.searchingRessource] += 1
        for ressource in requiredRessources:
            if self.inventory[ressource] < requiredRessources[ressource]:
                return False
        return True

    # def goToBroadcastSignal(self):

    def startingIncantation(self) -> None:
        data = self.look.split(",")[0]
        # self.look.split(",")
        if len(data) == 0:
            return
        # main loop
        requiredRessources = LEVELS[self.level]
        for ressource in requiredRessources:
            for elem in data:
                if ressource == elem:
                    requiredRessources[ressource] -= 1
        for ressource in requiredRessources:
            if requiredRessources[ressource] > 0:
                return
        self.commandList.append("Incantation\n")

    def startingActivity(self):
        self.dataToSend = "Connect_nbr\n"
        if self.availableSlots > 0:
            self.canFork = True
        self.actualActivity = Activity.POPULATING

    def populatingActivity(self):
        # if self.availableSlots > 0 and int(self.clientId) < 6:
        #     print("Fork")
        #     self.dataToSend = "Fork\n"
        #     self.canFork = False
        # else:
        #     self.dataToSend = "Look\n"
        self.actualActivity = Activity.EXECUTE_COMMAND

    def generateEmptyMap(self) -> list:
        return [["" for _ in range(self.widthValue)
                 ] for _ in range(self.heightValue)]

    def fillMapWithObjects(self, map: list, dataList: list):
        data_iter = iter(dataList)
        for i, row in enumerate(map):
            for j, _ in enumerate(row):
                try:
                    map[i][j] = next(data_iter)
                except StopIteration:
                    break
        return map

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
        else:
            if objectPosition["x"] > self.widthValue / 2:
                finalResult.append("Right\n")
            elif objectPosition["x"] < self.widthValue / 2:
                finalResult.append("Left\n")
            if objectPosition["y"] > self.heightValue / 2:
                finalResult.append("Forward\n")
            elif objectPosition["y"] < self.heightValue / 2:
                finalResult.append("Forward\n")
            finalResult.append("Take " + objectToSearch + "\n")
            finalResult.append("Inventory\n")
            return finalResult

    def fillingActivity(self, data: str):
        if "food" in self.inventory and self.inventory["food"] < 50:
            self.commandList = self.fillingInventory(data, "food")
            self.actualActivity = Activity.EXECUTE_COMMAND
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

    # def inventoryActivity(self):
    #     self.dataToSend = "Inventory\n"

    # def switchCase(self, activity: Activity):
    #     return {
    #         Activity.STARTING: self.startingActivity(),
    #         Activity.POPULATING: self.populatingActivity(),
    #         Activity.LOOKING: self.lookingActivity(),
    #         Activity.FILLING: self.fillingActivity(self.look),
    #         Activity.EXECUTE_COMMAND: self.executeCommand(),
    #         # Activity.INVENTORY: self.inventoryActivity(),
    #     }[activity]

    def algorithm(self) -> None:
        # self.switchCase(self.actualActivity)
        if self.actualActivity == Activity.STARTING:
            self.startingActivity()
        elif self.actualActivity == Activity.POPULATING:
            self.populatingActivity()
        elif self.actualActivity == Activity.LOOKING:
            self.lookingActivity()
        elif self.actualActivity == Activity.FILLING:
            self.fillingActivity(self.look)
        elif self.actualActivity == Activity.EXECUTE_COMMAND:
            self.executeCommand()
        elif self.actualActivity == Activity.CHECK_INCANTATION:
            self.checkIncanationActivity()
        elif self.actualActivity == Activity.PREPA_FOR_INCANTATION:
            self.prepareForIncantationActivity()
        return


# reste au sol les ressources
