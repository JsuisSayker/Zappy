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

    def launch_client(self):
        # message = ""
        while True:
            event = self.selector.select(timeout=None)
            for _, mask in event:
                if mask & selectors.EVENT_READ:
                    data = self.socket.recv(1024).decode("utf-8")
                    print(f"data: {data}")

                if mask & selectors.EVENT_WRITE:
                    if self.logged and self.ai.run is True:
                        # self.ia.algorithm()
                        print("Algorithm of the AI is needed here")
                    if self.ai.dataToSend and self.ai.run is not False:
                        if self.ai.dataToSend == (
                                self.teamName + '\n') and self.logged is False:
                            # self.just_log = 1
                            print("something needed there")
                        self.socket.send(self.ai.dataToSend.encode())
                        self.ai.run = False

    def closeConnection(self):
        self.selector.unregister(self.socket)
        self.socket.close()
