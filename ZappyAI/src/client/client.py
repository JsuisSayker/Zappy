import socket
import selectors
import subprocess

from ZappyAI.src.ai.ai import AI
import select
import errno

from ZappyAI.src.ai.infos import Activity


class Client():
    def __init__(self, port: int, teamName: str, host: str,
                 clientId: int):
        self.port: int = int(port)
        self.teamName: str = teamName
        self.host = host
        self.socket = socket.socket()
        self.selector = selectors.DefaultSelector()
        self.logged: bool = False
        self.actualStep: int = 0
        self.clientId = clientId
        self.ai = AI(teamName)
        self.ai.clientId = clientId

    def connectWithServer(self) -> None:
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setblocking(False)
        try:
            self.socket.connect((self.host, self.port))
        except BlockingIOError:
            pass

        try:
            while True:
                _, write_ready, _ = select.select([], [self.socket], [], 5)
                if write_ready:
                    try:
                        self.socket.connect((self.host, self.port))
                    except BlockingIOError:
                        continue
                    except OSError as e:
                        if e.errno == errno.EISCONN:
                            break
                        else:
                            raise
        except KeyboardInterrupt:
            print("Ctrl+C pressed, closing connection...")
            self.socket.close()
            exit(0)
        except ConnectionRefusedError:
            self.socket.close()
            print("Connection refused")
            exit(0)

        possibleEvents = selectors.EVENT_READ | selectors.EVENT_WRITE
        self.selector.register(self.socket, possibleEvents)

    def saveStartingInformation(self, data: str, nb: int) -> None:
        if nb == 1:
            self.ai.availableSlots = int(data)
            self.ai.dataToSend = ""
            self.actualStep = 2
        else:
            data = data.split()
            width = int(data[0])
            height = int(data[1])
            self.ai.heightValue = height
            self.ai.widthValue = width
            self.ai.minWidthValue = self.ai.widthValue // 2
            self.ai.minHeightValue = self.ai.heightValue * 2
            self.ai.dataToSend = ""
            self.actualStep = 3
            self.logged = True
            self.ai.run = True

    def forkingProcess(self) -> None:
        newClientId = int(self.clientId) + 1
        subprocess.Popen(["python3",
                          "zappy_ai",
                          "-p",
                          str(self.port),
                          "-n",
                          self.teamName,
                          "-h",
                          self.host,
                          "-i",
                          str(newClientId)])

    def launch_client(self) -> None:
        try:
            while True:
                event = self.selector.select(None)
                for _, mask in event:
                    if mask & selectors.EVENT_READ:
                        try:
                            data = self.socket.recv(10000).decode("utf-8")
                        except ConnectionResetError:
                            print("Connection reset by peer")
                            self.closeConnection()
                            exit(0)
                        if not data:
                            continue
                        tmpReceivedData = data.split("\n")
                        for element in tmpReceivedData[:-1]:
                            if "WELCOME" in element and self.logged is False:
                                self.ai.dataToSend = self.teamName + "\n"
                            elif "message" in element:
                                if self.ai.clearRead is True:
                                    self.ai.clearRead = False
                                    continue
                                self.ai.parseReceivedMessageFromBroadcast(
                                    element)
                                continue
                            elif self.actualStep < 3:
                                self.saveStartingInformation(element,
                                                             self.actualStep)
                            elif "dead" in element:
                                exit(0)
                            elif self.ai.dataToSend == "Look\n" and "message"\
                                    not in element:
                                self.ai.look = element
                            elif "Take" in self.ai.dataToSend and "food"\
                                    not in self.ai.dataToSend and "ok"\
                                    in element and "message" not in element:
                                self.ai.updateSharedInventory()
                                self.ai.newRessource = True
                            elif "Inventory" in self.ai.dataToSend and\
                                    "messasge" not in element:
                                try:
                                    self.ai.parsingAiInventory(element)
                                except IndexError:
                                    pass
                                except ValueError:
                                    pass
                            elif "Elevation underway" in element:
                                self.ai.actualActivity = Activity.CLEARING_DATA
                            elif "Current level" in element:
                                self.ai.level = int(''.join(filter(
                                    str.isdigit, element)))
                                if self.ai.level == 8:
                                    exit(0)
                                self.ai.newRessource = False
                                self.ai.searchingRessource = ""
                                self.ai.incantation = False
                                self.ai.nbReadyPlayers = 1
                                self.ai.clearRead = False
                                self.ai.clearBroadcast = False
                                self.ai.actualActivity = \
                                    Activity.FINISHING_INCANTATION

                            elif self.ai.dataToSend == "Fork\n" and "message"\
                                    not in element:
                                self.forkingProcess()
                                self.ai.canFork = False
                            self.ai.run = True

                    if mask & selectors.EVENT_WRITE:
                        if self.logged and self.ai.run is True:
                            self.ai.algorithm()
                        if self.ai.dataToSend and self.ai.run is True:
                            if self.ai.dataToSend == (
                                    self.teamName + '\n'
                            ) and self.logged is False:
                                self.actualStep = 1
                            self.socket.send(self.ai.dataToSend.encode())
                            self.ai.run = False

        except KeyboardInterrupt:
            print("Closing connection...")
            self.socket.close()
        except ConnectionResetError:
            print("Connection reset by peer")
            self.closeConnection()
            exit(0)

    def closeConnection(self) -> None:
        fd = self.socket.fileno()
        if fd == -1:
            return
        self.selector.unregister(self.socket)
        self.socket.close()
