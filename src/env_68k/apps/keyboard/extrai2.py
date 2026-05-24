#!/usr/bin/env python3
import sys
import re

def analisar_map(caminho_map, arquivo_objeto):
    # Expressão regular para pegar símbolos globais/locais: Endereço Hex -> Nome do Símbolo
    regex_simbolo = re.compile(r'^\s+(0x[0-9a-fA-F]{8})\s+([a-zA-Z_][a-zA-Z0-9_]*)')
    
    simbolos = []
    dentro_do_objeto = False

    try:
        with open(caminho_map, 'r') as f:
            for linha in f:
                # Detecta quando entramos em uma seção de código (.text) do objeto alvo
                if ".text" in linha and arquivo_objeto in linha:
                    dentro_do_objeto = True
                    continue
                
                # Se estamos dentro do objeto e a linha começa com outra seção (ex: .rodata, .data), 
                # ou mudou de arquivo objeto, fechamos a captura temporariamente
                if dentro_do_objeto and line_starts_new_section_or_object(linha, arquivo_objeto):
                    dentro_do_objeto = False
                
                # Se encontrar um símbolo na região do nosso objeto, captura endereço e nome
                if dentro_do_objeto:
                    match = regex_simbolo.match(linha)
                    if match:
                        end_hex = match.group(1)
                        nome_simbolo = match.group(2)
                        addr_int = int(end_hex, 16)
                        
                        # Evita duplicados idênticos de endereço/nome
                        if not simbolos or simbolos[-1][1] != addr_int:
                            simbolos.append((nome_simbolo, addr_int))
                            
    except FileNotFoundError:
        print(f"Erro: Arquivo '{caminho_map}' não foi encontrado.")
        return

    if not simbolos:
        print(f"Nenhum símbolo encontrado para o arquivo objeto '{arquivo_objeto}' no mapa.")
        return

    # Ordena os símbolos capturados por endereço (garante que a matemática funcione)
    simbolos.sort(key=lambda x: x[1])

    # Exibe o resultado formatado
    print(f"\n[ Análise de Memória: {arquivo_objeto} ]")
    print(f"{'Função / Símbolo':<28} {'Endereço Hex':<16} {'Tamanho (Bytes)'}")
    print("-" * 62)

    # Varre calculando a diferença entre o endereço atual e o próximo
    for i in range(len(simbolos)):
        nome = simbolos[i][0]
        end_atual = simbolos[i][1]
        
        if i < len(simbolos) - 1:
            end_proximo = simbolos[i+1][1]
            tamanho = end_proximo - end_atual
            tamanho_str = f"{tamanho} bytes"
        else:
            # Para a última função, não sabemos onde a próxima começa só olhando os símbolos internos
            tamanho_str = "Desconhecido (Última função do bloco)"

        print(f"{nome:<28} {hex(end_atual):<16} {tamanho_str}")

def line_starts_new_section_or_object(linha, arquivo_objeto):
    # Se a linha define uma nova seção padrão do linker ou cita outro arquivo .o, saímos do bloco
    if re.match(r'^\s+\.(text|rodata|data|bss|init|startup|stack)', linha):
        return True
    if ".o" in linha and arquivo_objeto not in linha:
        return True
    return False

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Uso correto: python3 analisar_map.py <arquivo.map> <arquivo_objeto.o>")
        print("Exemplo:     python3 analisar_map.py keyboard.map keyboard.o")
        sys.exit(1)
        
    analisar_map(sys.argv[1], sys.argv[2])

