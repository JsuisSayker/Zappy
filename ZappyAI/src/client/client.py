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
        self.socket = None
        self.selector = selectors.DefaultSelector()
        self.logged: bool = False
        self.actualStep: int = 0
        self.clientId = clientId
        self.ai = AI(teamName)
        self.ai.clientId = clientId

    def connectWithServer(self):
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
            return 0
        except ConnectionRefusedError:
            self.socket.close()
            print("Connection refused")
            exit(0)

        possibleEvents = selectors.EVENT_READ | selectors.EVENT_WRITE
        self.selector.register(self.socket, possibleEvents)

    def saveStartingInformation(self, data, nb):
        # tmpList = []
        # for i in range(len(data)):
        #     data[i] = data[i].split(" ")
        #     for j in range(len(data[i])):
        #         tmpList.append(data[i][j])
        # print(f"tmpList while saving: {tmpList}")
        # if tmpList[0] == "ko":
        #     exit(0)
        # else:
        #     self.ai.availableSlots = int(tmpList[0])
        # print(f"availableSlots: {self.ai.availableSlots}")
        # self.ai.widthValue = int(tmpList[1])
        # self.ai.heightValue = int(tmpList[2])
        # self.ai.dataToSend = ""
        # self.actualStep = 3
        # self.logged = True
        # self.ai.run = True
        if nb == 1:
            self.ai.availableSlots = int(data)
            self.ai.dataToSend = ""
            self.actualStep = 2
        else:
            data = data.split()
            # width = data[0:-3]
            width = int(data[0])
            height = int(data[1])
            # height = data[3:]
            # self.data_width = width
            # self.data_height = height
            self.ai.heightValue = height
            self.ai.widthValue = width
            # self.ai.minWidthValue = self.ai.widthValue // 2
            # self.ai.minHeightValue = self.ai.heightValue // 2
            self.ai.dataToSend = ""
            self.actualStep = 3
            self.logged = True
            self.ai.run = True

    def storePreviousData(self, data, filename="previous_data.txt"):
        with open(filename, 'w') as file:
            file.write(data)

    def loadPreviousData(self, filename="previous_data.txt") -> str:
        try:
            with open(filename, 'r') as file:
                return file.read()
        except FileNotFoundError:
            return ""

    def forkingProcess(self):
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

    def launch_client(self):
        previousData = ""
        try:
            while True:
                event = self.selector.select(None)
                for _, mask in event:
                    if mask & selectors.EVENT_READ:
                        try:
                            data = self.socket.recv(1024).decode("utf-8")
                        except ConnectionResetError:
                            print("Connection reset by peer")
                            self.closeConnection()
                            exit(0)
                        print(f"DATA: {data}")
                        self.storePreviousData(self.ai.dataToSend)

                        if previousData == "":
                            previousData = ""
                        print(f"MINE PREVIOUS DATA: {previousData}")
                        print(f"self.ai.dataToSend: {self.ai.dataToSend}")

                        if not data:
                            print("closing the connection")
                            self.closeConnection()
                        tmpReceivedData = data.split("\n")
                        print(f"tmpReceivedData: {tmpReceivedData[:-1]}")
                        for element in tmpReceivedData[:-1]:
                            print(f"element: {element}")
                            if "WELCOME" in element and self.logged is False:
                                self.ai.dataToSend = self.teamName + "\n"
                            elif self.actualStep < 3:
                                self.saveStartingInformation(element,
                                                             self.actualStep)
                            elif "dead" in element:
                                print("I'm dead")
                                exit(0)
                            elif self.ai.dataToSend == "Look\n":
                                self.ai.look = element
                                print(f"look: {self.ai.look}")
                            elif "Take" in self.ai.dataToSend and "food"\
                                    not in self.ai.dataToSend and "ok"\
                                    in element:
                                self.ai.updateSharedInventory()
                                self.ai.newRessource = True
                            elif "Inventory" in previousData and "Inventory" in self.ai.dataToSend:
                                try:
                                    self.ai.parse_inventory(element)
                                except IndexError:
                                    print("ValueError", element)
                                    pass
                            elif "Elevation underway" in element:
                                self.ai.actualActivity = Activity.CLEARING_DATA
                            elif "Current level" in element:
                                self.ai.level = int(''.join(filter(
                                    str.isdigit, element)))
                                if self.ai.level == 8:
                                    print("VICTORY")
                                    exit(0)
                                print(
                                    f"Current element in current elem: {element}")
                                self.ai.newRessource = False
                                self.ai.searchingRessource = ""
                                self.ai.incantation = False
                                self.ai.nbReadyPlayers = 1
                                self.ai.actualActivity = \
                                    Activity.FINISHING_INCANTATION

                            elif "message" in element:
                                self.ai.parse_broadcast(element)
                                # continue
                                # return
                            elif self.ai.dataToSend == "Fork\n":
                                self.forkingProcess()
                                print("forking")
                                self.ai.canFork = False
                            self.ai.run = True

                    if mask & selectors.EVENT_WRITE:
                        previousData = self.loadPreviousData()
                        if self.logged and self.ai.run is True:
                            # self.storePreviousData(self.ai.dataToSend)
                            # print(f"DO BICH {self.ai.dataToSend}")
                            self.ai.algorithm()
                            # print(f"DO BICH AFTER {self.ai.dataToSend}")
                            # exit(0)
                        if self.ai.dataToSend and self.ai.run is True:
                            if self.ai.dataToSend == (
                                    self.teamName + '\n'
                            ) and self.logged is False:
                                self.actualStep = 1
                            self.socket.send(self.ai.dataToSend.encode())
                            print(f"DATA SENT: {self.ai.dataToSend}")
                            self.ai.run = False

        except KeyboardInterrupt:
            print("Closing connection...")
            self.socket.close()
        except ConnectionResetError:
            print("Connection reset by peer")
            self.closeConnection()
            exit(0)

    def closeConnection(self):
        fd = self.socket.fileno()
        if fd == -1:
            return
        self.selector.unregister(self.socket)
        self.socket.close()
