import socket



HOST="192.168.1.117"
PORT = 8888



try: 
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
    print ("Socket successfully created")
except socket.error as err: 
    print ("socket creation failed with error %s" %(err) )
  
s.connect((HOST, PORT))

with open("ding", "rb") as file:
    s.send(file.read())