from ZappyAI.src.ai.infos import LEVELS


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
        self.clientNumber: int = 0

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


# do the algorithm


# reste au sol les ressources
