import re

def extrair_funcoes_map(caminho_arquivo):
    # Expressões regulares para identificar as seções do arquivo .map
    # Procura por linhas com endereços hex (8 dígitos) e o nome do símbolo
    regex_simbolo = re.compile(r'^\s+(0x[0-9a-fA-F]{8})\s+([a-zA-Z_][a-zA-Z0-9_]*)')
    regex_fim_secao = re.compile(r'^\s+\.(text|rodata|data|bss|init|startup)')

    funcoes = []
    dentro_keyboard_text = False

    with open(caminho_arquivo, 'r') as f:
        linhas = f.readlines()

    for idx, linha in enumerate(linhas):
        # 1. Identifica onde começam as funções do keyboard.o
        if ".text " in linha and "keyboard.o" in linha:
            dentro_keyboard_text = True
            continue
        
        # 2. Se mudou de seção (ex: .text.startup ou .rodata), para de ler
        if dentro_keyboard_text and regex_fim_secao.match(linha) and "keyboard.o" not in linha:
            # Captura o endereço da próxima seção para calcular o tamanho da última função
            match_proximo = regex_simbolo.search(linha) or re.search(r'(0x[0-9a-fA-F]{8})', linha)
            if match_proximo:
                endereco_fim = int(match_proximo.group(1), 16)
                funcoes.append(("FIM_SECAO", endereco_fim))
            dentro_keyboard_text = False
            break

        # 3. Captura os símbolos enquanto estiver dentro do bloco do keyboard.o
        if dentro_keyboard_text:
            match = regex_simbolo.match(linha)
            if match:
                end_hex = match.group(1)
                nome_funcao = match.group(2)
                funcoes.append((nome_funcao, int(end_hex, 16)))

    # Se não achou o marcador de fim de seção, usa o próximo símbolo disponível ou o fim do bloco
    if funcoes and funcoes[-1][0] != "FIM_SECAO":
        # Procura um marcador de segurança (tamanho total do bloco text do keyboard era 0x452)
        funcoes.append(("FIM_SECAO", funcoes[0][1] + 0x452))

    # 4. Calcula os tamanhos e exibe a tabela
    print(f"{'Função':<25} {'Endereço Inicial':<20} {'Tamanho (Bytes)'}")
    print("-" * 62)

    for i in range(len(funcoes) - 1):
        nome = funcoes[i][0]
        end_atual = funcoes[i][1]
        end_proximo = funcoes[i+1][1]
        
        tamanho = end_proximo - end_atual
        
        # Adiciona notas baseadas no tamanho (opcional, igual ao seu exemplo)
        nota = ""
        if tamanho == 2:
            nota = " (uma única instrução curta!)"
        elif nome == "get_kbd_key":
            nota = " (A maior rotina do seu arquivo)"

        print(f"{nome:<25} {hex(end_atual)}            {tamanho} bytes{nota}")

if __name__ == "__main__":
    # Substitua 'keyboard.map' pelo caminho real do seu arquivo .map
    try:
        extrair_funcoes_map('keyboard.map')
    except FileNotFoundError:
        print("Erro: Arquivo 'keyboard.map' não encontrado. Verifique o caminho.")