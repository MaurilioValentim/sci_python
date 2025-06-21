import serial
import struct
import time
import numpy as np
import matplotlib.pyplot as plt

# --- CONFIGURACOES ---
# Altere esta para a porta COM correta do seu microcontrolador
SERIAL_PORT = 'COM5'
BAUD_RATE = 115200

# --- DEFINICOES DO PROTOCOLO (devem ser identicas as do C) ---
# Comandos (do enum SCI_Command_e)
CMD_RECEIVE_INT = 1 # Comando para o PC enviar um int para o 28379D
CMD_SEND_INT    = 2 # Comando para o PC pedir um int para o 28379D
CMD_RECEIVE_WAVEFORM = 3
CMD_SEND_WAVEFORM = 4

NUM_PONTOS_WAVEFORM = 500

def main():
    """Funcao principal que gerencia a conexao e o menu do usuario."""
    print("--- Terminal de Teste SCI para 28379D ---")
    
    try:
        # Abre a porta serial usando um bloco 'with' para garantir que ela seja fechada
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=2) as ser:
            print(f"Porta serial {SERIAL_PORT} aberta com sucesso a {BAUD_RATE} bps.")
            time.sleep(1) # Um pequeno tempo para a serial estabilizar

            while True:
                print("\n----- MENU -----")
                print("1. Enviar um numero inteiro para o 28379D")
                print("2. Receber um numero inteiro do 28379D")
                print("3. Enviar uma Senoide")
                print("4. Receber uma Senoide")
                print("0. Sair")
                
                choice = input("Escolha uma opcao: ")

                if choice == '1':
                    send_int(ser)
                elif choice == '2':
                    receive_int(ser)
                elif choice == '3':
                    send_waveform(ser)
                elif choice == '4':
                    receive_waveform(ser)
                elif choice == '0':
                    print("Encerrando o programa.")
                    break
                else:
                    print("Opcao invalida. Tente novamente.")

    except serial.SerialException as e:
        print(f"\nERRO: Nao foi possivel abrir a porta serial '{SERIAL_PORT}'.")
        print(f"Detalhe: {e}")
        print("Verifique se a porta esta correta e se nenhum outro programa a esta usando.")

def send_int(ser_connection):
    """
    Pede um numero ao usuario, o empacota e envia para o microcontrolador.
    """
    try:
        num_str = input("Digite um numero inteiro para ENVIAR (entre -32768 e 32767): ")
        number_to_send = int(num_str)

        if not -32768 <= number_to_send <= 32767:
            print("ERRO: O numero esta fora do range permitido para um int16_t.")
            return

        # Empacota o COMANDO e o DADO em uma sequencia de bytes.
        # Formato: '<' (Little-endian), 'B' (byte, para o comando), 'h' (short, para o int16), 'h' para o tamanho do dado.
        packet_to_send = struct.pack('<Bhh', CMD_RECEIVE_INT, 2, number_to_send)
        
        print(f"\nEnviando pacote de {len(packet_to_send)} bytes: {packet_to_send.hex(' ')}")
        ser_connection.write(packet_to_send)
        print("Pacote enviado com sucesso.")

    except ValueError:
        print("ERRO: Entrada invalida. Por favor, digite um numero inteiro.")
    except Exception as e:
        print(f"Ocorreu um erro inesperado: {e}")

def receive_int(ser_connection):
    """
    Envia um comando para o microcontrolador solicitando um dado e depois o recebe.
    """
    try:
        # 1. Envia apenas o COMANDO para solicitar o dado.
        #    O pacote tera 3 bytes.
        request_packet = struct.pack('<Bh', CMD_SEND_INT, 0)

        print(f"\nEnviando comando de solicitacao (1 byte): {request_packet.hex(' ')}")
        ser_connection.write(request_packet)

        # 2. Aguarda a resposta do microcontrolador.
        #    O 28379D deve responder enviando apenas o dado (int16_t = 2 bytes).
        print("Aguardando resposta do 28379D...")
        response_data = ser_connection.read(2)
        ser_connection.flushInput()  # Limpa o buffer de entrada

        if not response_data or len(response_data) < 2:
            print("ERRO: Nao houve resposta do microcontrolador (timeout).")
            return

        # 3. Desempacota os bytes recebidos para um inteiro.
        #    Formato: '<' (Little-endian), 'h' (short, para o int16)
        received_number = struct.unpack('<h', response_data)[0]

        print(f"  -> Numero recebido do 28379D: {received_number}")

    except Exception as e:
        print(f"Ocorreu um erro inesperado: {e}")

def send_waveform(ser_connection):
    """
    Gera uma senoide, envia a quantidade de pontos para o microcontrolador,
    e em seguida envia cada valor separado em parte inteira e decimal (ambas com sinal).
    """
    try:
        freq = int(input("Digite a frequencia da Forma de onda (entre -32768 e 32767): "))
        amp = int(input("Digite a Amplitude da Forma de onda (entre -32768 e 32767): "))
        fase = float(input("Digite a Fase da Forma de onda (em radianos): "))
        fs = NUM_PONTOS_WAVEFORM
        duracao = 1

        # Tempo
        t = np.linspace(0, duracao, int(fs * duracao), endpoint=False)

        # Geração da senoide
        senoide = amp * np.sin(2 * np.pi * freq * t + fase)
        senoide_float = senoide.astype(np.float32)
        num_pontos = len(senoide_float)

        # Mostra a forma de onda
        plotagem(senoide_float)

        # Envia o cabeçalho com número de pontos
        # CMD_RECEIVE_WAVEFORM, data_len=4 (2 int16_t por ponto)
        header_packet = struct.pack('<Bhh', CMD_RECEIVE_WAVEFORM, 4, num_pontos)
        ser_connection.write(header_packet)
        time.sleep(0.01)

        # Envia cada valor da forma de onda como dois inteiros: parte inteira + parte decimal
        for valor in senoide_float:
            parte_inteira = int(valor)
            parte_decimal = int(abs(valor - parte_inteira) * 10000)

            # Aplica o mesmo sinal da parte inteira à parte decimal
            if valor < 0:
                parte_decimal = -parte_decimal

            data_packet = struct.pack('<hh', parte_inteira, parte_decimal)
            ser_connection.write(data_packet)
            time.sleep(0.001)

        print("Senoide enviada com sucesso como duas partes (int16).")

    except ValueError:
        print("ERRO: Entrada invalida. Por favor, digite um numero válido.")
    except Exception as e:
        print(f"Ocorreu um erro inesperado: {e}")




def receive_waveform(ser_connection):
    """
    Solicita todos os pontos da senoide ao microcontrolador via serial.
    Recebe dois inteiros por ponto: parte inteira e parte decimal (escalada por 10000).
    """
    try:
        num_points = NUM_PONTOS_WAVEFORM
        # Envia o comando solicitando os dados da senoide
        request_packet = struct.pack('<Bh', CMD_SEND_WAVEFORM, 0)
        print(f"Enviando comando de solicitação: {request_packet.hex(' ')}")
        ser_connection.write(request_packet)

        # Cada ponto é composto por 2 int16 => 4 bytes
        total_bytes = num_points * 4
        response_data = ser_connection.read(total_bytes)

        if len(response_data) != total_bytes:
            print(f"ERRO: Esperado {total_bytes} bytes, mas recebeu {len(response_data)}.")
            return None

        senoide_recebida = []
        for i in range(0, total_bytes, 4):
            parte_inteira, parte_decimal = struct.unpack('<hh', response_data[i:i+4])
            valor = parte_inteira + (parte_decimal / 10000.0)
            senoide_recebida.append(valor)

        print("Todos os dados foram recebidos com sucesso.")

        plotagem(senoide_recebida)

    except Exception as e:
        print(f"Ocorreu um erro inesperado: {e}")


   
def plotagem(senoide):
    plt.plot(senoide)
    plt.title("Forma de Onda Recebida do Microcontrolador")
    plt.xlabel("Amostra")
    plt.ylabel("Amplitude")
    plt.grid(True)
    plt.show()


if __name__ == "__main__":
    main()