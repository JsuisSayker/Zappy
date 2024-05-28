from enum import Enum


class Activity(Enum):
    STARTING = 0
    SEARCHING = 1
    GATHERING = 2
    CREATINGEGG = 3
    LOOKING = 4
    POPULATING = 5
    INVENTORY = 6
    FILLING = 7
    EXECUTE_COMMAND = 8
    CHECK_INCANTATION = 9


LEVELS: dict[int, dict[str, int]] = {
    1: {"linemate": 1},
    2: {"linemate": 1, "deraumere": 1, "sibur": 1},
    3: {"linemate": 2, "sibur": 1, "phiras": 2},
    4: {"linemate": 1, "deraumere": 1, "sibur": 2, "phiras": 1},
    5: {"linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 3},
    6: {"linemate": 1, "deraumere": 2, "sibur": 3, "phiras": 1},
    7: {"linemate": 2, "deraumere": 2, "sibur": 2, "mendiane": 2, "phiras": 2,
        "thystame": 1}
}
