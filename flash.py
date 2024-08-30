import subprocess
import os


# Configurações
MCU = "attiny13"
HEX_FILE = "files/emissor.hex"
BAUD_RATE = 19200
ARDUINO_PORT = "COM20"


# Caminho para o toolchain
TOOLCHAIN_PATH = "C:/Users/Pedro Coutinho/AVR/avrdude"
AVRDUDE = os.path.join(TOOLCHAIN_PATH, "avrdude")


def upload_to_attiny13():
    """Programa o ATtiny13 usando avrdude e um Arduino como programador."""
    try:
        # Comando para carregar o arquivo HEX no ATtiny13
        upload_command = [
            AVRDUDE,
            "-c", "arduino",
            "-p", MCU,
            "-P", ARDUINO_PORT,
            "-b", str(BAUD_RATE),
            "-U", "flash:w:{}:i".format(HEX_FILE)
        ]
        subprocess.run(upload_command, check=True)
        print("Programação concluída com sucesso.")

    except subprocess.CalledProcessError as e:
        print(f"Erro durante a programação: {e}")


if __name__ == "__main__":
    upload_to_attiny13()
