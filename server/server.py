import sys
import threading
import psycopg2
import socket
import datetime

TCP_IP = ''
TCP_PORT = 1883 
BUFFER_SIZE = 64 


# Faz a classificacao de pacotes
def classificatePackage(conn, addr):
    while 1:
        msg = ''
        dados = bytearray()
        fim_msg = False
        try:
            while not fim_msg:
                recvd = conn.recv(BUFFER_SIZE)
                if not recvd:
                    raise ConnectionError()
                    break
                dados += recvd
                if b'\n' in recvd:
                    msg = dados.rstrip(b'\n').decode('utf-8')
                    fim_msg = True
            print("\n[SERVER ", addr, "] Received ", len(dados), " bytes from ", addr)

            comando = msg.split(' ')
            operacao = comando[0]
            if operacao == '[LOG]':
                print(comando);
                
            else:
                resposta = 'ERROR'

        except BaseException as erro:
            break
    print("\n[SERVER ", addr, "] Closing connection ", addr)
    conn.close()


print("\n[SERVER] Starting")

print("\n[SERVER] Opening port " + str(TCP_PORT) + " and listening")
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

while 1:
    conn, addr = s.accept()
    thread = threading.Thread(target=classificatePackage,
                              args=[conn, addr],
                              daemon=True)
    thread.start()

print("\n[SERVER] Closing port " + str(TCP_PORT))
s.close()
