import socket
import selectors
from ZappyAI.src.ai.ai import AI
import select
import errno


class Client():
    def __init__(self, port: int, teamName: str, host: str):
        self.port: int = int(port)
        self.teamName: str = teamName
        self.host = host
        self.socket = None
        self.selector = selectors.DefaultSelector()
        self.ai = AI(teamName)
        self.logged = False
        self.widthValue = 0
        self.heightValue = 0
        self.actualStep = 0

    def connectWithServer(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setblocking(False)
        try:
            self.socket.connect((self.host, self.port))
        except BlockingIOError:
            pass

        # Use select to wait for connect to complete
        while True:
            _, write_ready, _ = select.select([], [self.socket], [], 5)
            if write_ready:
                try:
                    # This should raise an error indicating that the connection
                    # is in progress
                    self.socket.connect((self.host, self.port))
                except BlockingIOError:
                    continue
                except OSError as e:
                    if e.errno == errno.EISCONN:
                        # Socket is connected, break the loop
                        break
                    else:
                        # An actual error occurred
                        raise
        possibleEvents = selectors.EVENT_READ | selectors.EVENT_WRITE
        self.selector.register(self.socket, possibleEvents)

    def saveStartingInformation(self, data, nb):
        if nb == 1:
            print(f"int value of data : {int(data)}")
            self.ai.dataToSend = ""
            self.actualStep = 2
        else:
            tmpList = []
            for i in range(len(data)):
                data[i] = data[i].split(" ")
                for j in range(len(data[i])):
                    tmpList.append(data[i][j])
            self.ai.clientNumber = int(tmpList[0])
            self.widthValue = int(tmpList[1])
            self.heightValue = int(tmpList[2])
            self.ai.dataToSend = ""
            self.actualStep = 3
            self.logged = True
            self.ai.run = True

    def launch_client(self):
        # message = ""
        while True:
            event = self.selector.select(timeout=None)
            for _, mask in event:
                if mask & selectors.EVENT_READ:
                    data = self.socket.recv(1024).decode("utf-8")
                    print(f"data: {data}")
                    if data:
                        print("adding the teamName to the dataToSend")
                    else:
                        print("closing the connection")
                        self.closeConnection()
                    tmpReceivedData = data.split("\n")
                    print(f"tmpReceivedData: {tmpReceivedData}")
                    print(f"tmpReceivedData: {tmpReceivedData[:-1]}")
                    for element in tmpReceivedData[:-1]:
                        print(f"element: {element}")
                        if "WELCOME" in element and self.logged is False:
                            print("Welcomed and not logged")
                            self.ai.dataToSend = self.teamName + "\n"
                        elif self.actualStep < 3:
                            self.saveStartingInformation(tmpReceivedData[:-1],
                                                         self.actualStep)
                        elif "dead" in element:
                            print("I'm dead")
                            exit(0)

                if mask & selectors.EVENT_WRITE:
                    if self.logged and self.ai.run is True:
                        # self.ia.algorithm()
                        print()
                    if self.ai.dataToSend and self.ai.run is not False:
                        if self.ai.dataToSend == (
                                self.teamName + '\n') and self.logged is False:
                            # self.just_log = 1
                            print("sending the teamName")
                        self.socket.send(self.ai.dataToSend.encode())
                        self.ai.run = False

    def closeConnection(self):
        self.selector.unregister(self.socket)
        self.socket.close()
