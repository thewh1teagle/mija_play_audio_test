import socket
from time import sleep


HOST="192.168.1.117"
PORT = 8888

PACKET_SIZE = 1024

try: 
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
    print ("Socket successfully created")
except socket.error as err: 
    print ("socket creation failed with error %s" %(err) )
  
s.connect((HOST, PORT))

with open("booting", "rb") as file:
    data = file.read(1024)
    while data:
        s.send(data)
        data = file.read(1024)
        
s.close()