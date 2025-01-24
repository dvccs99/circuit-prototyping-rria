import socket


mysock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


host = socket.gethostbyname("www.google.com")


#mysock.connect((host,80))

mysock.connect(("192.168.158.113",80))

#message = b"GET / HTTP/1.1\r\n\r\n"
message = b"MOTOR / \r\n\r\n"

mysock.sendall(message)

data = mysock.recv(1000)

print(data)
