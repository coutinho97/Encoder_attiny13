import subprocess
import os


# Configurações
C_FILE = "files/emissor.c"
MCU = "attiny13"

# Caminho para o toolchain
TOOLCHAIN_PATH = "C:/Users/Pedro Coutinho/AVR/avr8-gnu-toolchain/bin"
AVR_GCC = os.path.join(TOOLCHAIN_PATH, "avr-gcc")
AVR_OBJCOPY = os.path.join(TOOLCHAIN_PATH, "avr-objcopy")


def compile_c_file(c_file):

    """Compila o arquivo C para um arquivo HEX."""
    try:
        # Deriva o nome base do arquivo (sem extensão)
        base_name = os.path.splitext(c_file)[0]
        output_elf = base_name + ".elf"
        output_hex = base_name + ".hex"

        # Comando para compilar o arquivo C
        compile_command = [
            AVR_GCC,
            "-mmcu={}".format(MCU),
            "-Os",  # Otimização para tamanho
            "-o", output_elf,
            c_file
        ]
        subprocess.run(compile_command, check=True)
        print(f"Compilação concluída com sucesso. ELF gerado: {output_elf}")

        # Comando para converter o ELF para HEX
        objcopy_command = [
            AVR_OBJCOPY,
            "-O", "ihex",
            output_elf,
            output_hex
        ]
        subprocess.run(objcopy_command, check=True)
        print(f"Arquivo HEX gerado com sucesso: {output_hex}")

    except subprocess.CalledProcessError as e:
        print(f"Erro durante a compilação: {e}")


def compile_to_assembly(c_file):
    """Compila o arquivo C para um arquivo de código assembly."""
    try:
        # Deriva o nome base do arquivo (sem extensão)
        base_name = os.path.splitext(c_file)[0]
        output_asm = base_name + ".s"

        # Comando para compilar o arquivo C para código assembly
        compile_command = [
            AVR_GCC,
            "-mmcu={}".format(MCU),
            "-S",  # Gera o código assembly
            "-o", output_asm,
            c_file
        ]
        subprocess.run(compile_command, check=True)
        print(f"Compilação para assembly concluída com sucesso. Assembly gerado: {output_asm}")

    except subprocess.CalledProcessError as e:
        print(f"Erro durante a compilação para assembly: {e}")


if __name__ == "__main__":
    compile_c_file(C_FILE)