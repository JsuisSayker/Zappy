import socket
import selectors
import subprocess
from ZappyAI.src.ai.ai import AI
import select
import errno


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
            width = data[0:-3]
            height = data[3:]
            self.data_width = width
            self.data_height = height
            self.ai.dataToSend = ""
            self.actualStep = 3
            self.logged = True
            self.ai.run = True

    def launch_client(self):
        try:
            while True:
                event = self.selector.select(None)
                for _, mask in event:
                    if mask & selectors.EVENT_READ:
                        data = self.socket.recv(1024).decode("utf-8")
                        print(f"data: {data}")
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
                            elif "Inventory" in self.ai.dataToSend:
                                try:
                                    self.ai.parse_inventory(element)
                                except IndexError:
                                    print("ValueError", element)
                                    pass
                            elif "Elevation underway" in element:
                                self.ai.dataToSend = ""
                            elif "message" in element:
                                self.ai.parse_broadcast(element)
                                # return
                            elif self.ai.dataToSend == "Fork\n":
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
                                print("forking")
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
                            #     print("sending the teamName")
                            self.socket.send(self.ai.dataToSend.encode())
                            self.ai.run = False
        except KeyboardInterrupt:
            print("Closing connection...")
            self.socket.close()

    def closeConnection(self):
        self.selector.unregister(self.socket)
        self.socket.close()
