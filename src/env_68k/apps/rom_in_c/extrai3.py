#!/usr/bin/env python3
import sys
import re

def analisar_map(caminho_map, arquivos_objeto):
    # Captura endereços de 8 dígitos hexadecimais seguidos pelo nome do símbolo
    # Ex: 0x00008040    delay  ou  0x00008040        delay
    regex_simbolo = re.compile(r'^\s*(0x[0-9a-fA-F]{8})\s+([a-zA-Z_][a-zA-Z0-9_]*)')
    
    simbolos = []
    objeto_atual = None
    dentro_de_alvo = False

    try:
        with open(caminho_map, 'r') as f:
            for linha in f:
                # 1. Verifica se a linha menciona um dos nossos arquivos .o alvo
                # de forma direta e sem exigir estruturas complexas de caminhos
                for obj in arquivos_objeto:
                    if obj in linha:
                        # Mas garante que estamos lidando com a seção de código (.text)
                        if ".text" in linha or "text" in linha.lower():
                            dentro_de_alvo = True
                            objeto_atual = obj
                            break
                
                # 2. Se a linha começa com outra grande seção estrutural, desarmamos
                # para não pegar variáveis globais fora do bloco de funções
                if dentro_de_alvo and re.match(r'^\s*\.(rodata|data|bss|init|startup|stack)', linha):
                    dentro_de_alvo = False
                    objeto_atual = None

                # 3. Se o sinal estiver verde, tentamos casar o símbolo e o endereço
                if dentro_de_alvo:
                    match_simbolo = regex_simbolo.match(linha)
                    if match_simbolo:
                        end_hex = match_simbolo.group(1)
                        nome_simbolo = match_simbolo.group(2)
                        addr_int = int(end_hex, 16)
                        
                        # Filtra ruídos internos do GCC
                        if not nome_simbolo.startswith('.') and not nome_simbolo.startswith('__'):
                            # Evita duplicados adjacentes na captura
                            if not simbolos or simbolos[-1][1] != addr_int or simbolos[-1][0] != nome_simbolo:
                                simbolos.append((nome_simbolo, addr_int, objeto_atual))
                                
                    # Uma linha contendo outra menção de .o que NÃO é nosso alvo desarma a captura
                    elif ".o" in linha:
                        check_target = False
                        for obj in arquivos_objeto:
                            if obj in linha:
                                check_target = True
                        if not check_target:
                            dentro_de_alvo = False
                            objeto_atual = None
                            
    except FileNotFoundError:
        print(f"Erro: Arquivo '{caminho_map}' não foi encontrado.")
        return

    if not simbolos:
        print(f"Nenhum símbolo encontrado para os arquivos objeto fornecidos no mapa.")
        print("Pode ser que a formatação do .map seja diferente do esperado.")
        return

    # Ordena todos os símbolos por endereço na memória do PDS317
    simbolos.sort(key=lambda x: x[1])

    print(f"\n[ Análise de Memória Consolidada: {', '.join(arquivos_objeto)} ]")
    print(f"{'Função / Símbolo':<28} {'Origem .o':<15} {'Endereço Hex':<14} {'Tamanho (Bytes)'}")
    print("-" * 75)

    for i in range(len(simbolos)):
        nome = simbolos[i][0]
        end_atual = simbolos[i][1]
        origem = simbolos[i][2]
        
        if i < len(simbolos) - 1:
            end_proximo = simbolos[i+1][1]
            tamanho = end_proximo - end_atual
            tamanho_str = f"{tamanho} bytes"
        else:
            tamanho_str = "Desconhecido (Fim do bloco de análise)"

        print(f"{nome:<28} {origem:<15} {hex(end_atual):<14} {tamanho_str}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Uso correto: python3 extrai3.py <arquivo.map> <obj1.o> [obj2.o] ...")
        sys.exit(1)
        
    analisar_map(sys.argv[1], sys.argv[2:])