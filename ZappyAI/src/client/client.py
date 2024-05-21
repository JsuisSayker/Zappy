import socket
import selectors
from ZappyAI.src.ai.ai import AI


class Client():
    def __init__(self, port: int, teamName: str, host: str):
        self.port: int = int(port)
        self.teamName: str = teamName
        self.host = host
        self.socket = None
        self.selector = selectors.DefaultSelector()
        self.ai = AI(teamName)
        self.logged = False

    def connectWithServer(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setblocking(False)
        self.socket.connect((self.host, self.port))
        possibleEvents = selectors.EVENT_READ | selectors.EVENT_WRITE
        self.selector.register(self.socket, possibleEvents)

    def launch_client(self):
        # message = ""
        while True:
            event = self.selectors.select(timeout=None)
            for _, mask in event:
                if mask & selectors.EVENT_READ:
                    data = self.socket.recv(1024).decode("utf-8")
                    print(data)

                if mask & selectors.EVENT_WRITE:
                    if self.logged and self.ia.run is True:
                        # self.ia.algorithm()
                        print("Algorithm of the AI is needed here")
                    if self.ia.dataToSend and self.ia.run is not False:
                        if self.ia.dataToSend == (
                                self.teamName + '\n') and self.logged is False:
                            # self.just_log = 1
                            print("something needed there")
                        self.socket.send(self.ia.dataToSend.encode())
                        self.ia.run = False

    def closeConnection(self):
        self.selector.unregister(self.socket)
        self.socket.close()
