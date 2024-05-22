from ZappyAI.src.ai.infos import LEVELS, Activity


class AI():
    def __init__(self, teamName: str):
        self.teamName: str = teamName
        self.inventory: dict[str, int] = {"food": 0, "linemate": 0,
                                          "deraumere": 0, "sibur": 0,
                                          "mendiane": 0, "phiras": 0,
                                          "thystame": 0}
        self.level: int = 1
        self.incantation: bool = False
        self.searchingRessource: str = ""
        self.look: str = ""
        self.commandList: list[str] = []
        self.run = True
        self.dataToSend: str = ""
        self.availableSlots: int = 0
        self.canFork: bool = False
        self.clientId: str = ""
        self.clientIdList: list[str] = []
        self.actualActivity: Activity = Activity.STARTING

    def isIncantationPossible(self) -> bool:
        requiredRessources = LEVELS[self.level]
        if self.searchingRessource in self.inventory:
            requiredRessources[self.searchingRessource] += 1
        for ressource in requiredRessources:
            if self.inventory[ressource] < requiredRessources[ressource]:
                return False
        return True

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
        if self.availableSlots > 0 and len(self.clientIdList) < 6:
            print("Fork")
            self.dataToSend = "Fork\n"
            self.canFork = False
        else:
            self.dataToSend = "Look\n"
        self.actualActivity = Activity.LOOKING

    def lookingActivity(self):
        self.dataToSend = "Look\n"
        # self.actualActivity = Activity.SEARCHING

    def switchCase(self, activity: Activity):
        return {
            Activity.STARTING: self.startingActivity(),
            Activity.POPULATING: self.populatingActivity(),
            Activity.LOOKING: self.lookingActivity(),
        }[activity]

    def algorithm(self) -> None:
        self.switchCase(self.actualActivity)
        return


# reste au sol les ressources
