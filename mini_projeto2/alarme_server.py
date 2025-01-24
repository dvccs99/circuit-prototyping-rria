import socket
import sys
import smtplib
from datetime import datetime
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

mysock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
hostname = socket.gethostname()
local_ip = socket.gethostbyname(hostname)
print(local_ip)

DT = str(datetime.now())
RECIPIENT = "faketestesp123@outlook.com"
SENDER = "faketestesp123@outlook.com"
SENDER_PWD = 'Secret123@'
SUBJECT = "ALERTA: INTRUSO DETECTADO"
EMAIL_TEXT = "DATA E HORÁRIO DA INVASÃO: " + DT
pwd_found = False
alarm_activated = False
starting_connection = False

try:
    mysock.bind(('', 80))
except socket.error:
    print("Failed to bind")
    sys.exit()
mysock.listen()
while True:
    TB = []
    conn, addr = mysock.accept()
    data = conn.recv(1000)

    if not data:
        break
    print("Got a request!!!")
    data = str(data, 'UTF-8')
    print("Request: ", data)

    try:
        file = open("banco_de_dados.txt")
        for line in file:
            words = line.split()
            TB.append(words)
    except IOError as e:
        print("Erro ao ler arquivo", e)

    if data == "ALARM":
        try:
            print("Tentando enviar email")
            smtpObj = smtplib.SMTP('smtp-mail.outlook.com', 587)
            smtpObj.ehlo()
            smtpObj.starttls()
            # smtpObj.ehlo()
            smtpObj.login(SENDER, SENDER_PWD)

            msg = MIMEMultipart("alternative")
            msg["Subject"] = SUBJECT
            msg["From"] = SENDER
            msg["To"] = RECIPIENT

            part1 = MIMEText(EMAIL_TEXT, "plain")
            msg.attach(part1)
            smtpObj.sendmail(SENDER, RECIPIENT, msg.as_string())
            smtpObj.quit()
            conn.sendall(bytes("SECURITY ALERTED", "utf-8"))
            alarm_activated = True
            print("Email enviado com sucesso!")
        except Exception:
            print("Erro ao enviar e-mail")

    if data == "Connected to server":
        print("ESP connected")
        starting_connection = True

    for senha in TB:
        if data == senha[1]:
            conn.sendall(bytes("BEM-VINDO(A) "+senha[0]), "utf-8")
            pwd_found = True

    if not pwd_found and not alarm_activated and not starting_connection:
        print("Senha incorreta")
        conn.sendall(bytes("Incorrect Password", "utf-8"))


print("Ending Connection")
conn.close()
mysock.close()
