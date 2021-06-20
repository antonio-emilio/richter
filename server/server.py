import sys
import threading
import psycopg2
import socket
import datetime

TCP_IP = ''
TCP_PORT = 1883  # Porta que sera aberta para a conexao websocket
BUFFER_SIZE = 64  # Tamanho do buffer que podera ser recebido


# Faz a classificacao de pacotes
def classificatePackage(conn, addr):
    while 1:
        msg = ''
        dados = bytearray()
        fim_msg = False
        try:
            # Contabiliza todos os caracteres que estao sendo recebidos
            while not fim_msg:
                recvd = conn.recv(BUFFER_SIZE)
                if not recvd:
                    raise ConnectionError()
                    break
                dados += recvd
                if b'\n' in recvd:
                    msg = dados.rstrip(b'\n').decode('utf-8')
                    fim_msg = True
            print("\n[SERVIDOR ", addr, "] Recebidos no total ", len(dados), " bytes de ", addr)

            # Inicia a interpretacao dos dados separando o buffer recebido em um array
            comando = msg.split(' ')
            operacao = comando[0]
            if operacao == '[LOG]':
                print(comando);
                
            else:
                resposta = 'ERRO'

        except BaseException as erro:
            break
    print("\n[SERVIDOR ", addr, "] Fechando a conexao ", addr)
    conn.close()


print("\n[SERVIDOR] Iniciando")

print("\n[SERVIDOR] Abrindo a porta " + str(TCP_PORT) + " e ouvindo")
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

while 1:
    conn, addr = s.accept()
    thread = threading.Thread(target=classificatePackage,
                              args=[conn, addr],
                              daemon=True)
    thread.start()

print("\n[SERVIDOR] Fechando a porta " + str(TCP_PORT))
s.close()