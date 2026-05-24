#!/usr/bin/env python3
import sys
import re

def gerar_arquivos(caminho_map, arquivos_objeto):
    regex_simbolo = re.compile(r'^\s*(0x[0-9a-fA-F]{8})\s+([a-zA-Z_][a-zA-Z0-9_]*)')
    simbolos = []
    objeto_atual = None
    dentro_de_alvo = False

    try:
        with open(caminho_map, 'r') as f:
            for linha in f:
                for obj in arquivos_objeto:
                    if obj in linha:
                        if ".text" in linha or "text" in linha.lower():
                            dentro_de_alvo = True
                            objeto_atual = obj
                            break
                
                if dentro_de_alvo and re.match(r'^\s*\.(rodata|data|bss|init|startup|stack)', linha):
                    dentro_de_alvo = False
                    objeto_atual = None

                if dentro_de_alvo:
                    match_simbolo = regex_simbolo.match(linha)
                    if match_simbolo:
                        end_hex = match_simbolo.group(1)
                        nome_simbolo = match_simbolo.group(2)
                        addr_int = int(end_hex, 16)
                        
                        if not nome_simbolo.startswith('.') and not nome_simbolo.startswith('__'):
                            if not simbolos or simbolos[-1][1] != addr_int or simbolos[-1][0] != nome_simbolo:
                                simbolos.append((nome_simbolo, addr_int))
                                
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
        print("Nenhum símbolo encontrado para gerar os arquivos.")
        return

    # Ordena por endereço
    simbolos.sort(key=lambda x: x[1])

    # ---- GERAR ARQUIVO PARA ASSEMBLY (pds317_rom.inc) ----
    with open("pds317_rom.inc", "w") as f_asm:
        f_asm.write("* ===================================================================\n")
        f_asm.write("*             MAPA DE SIMBOLOS DA ROM DO COMPUTADOR PDS317            \n")
        f_asm.write("*              (Gerado automaticamente via gera_includes.py)         \n")
        f_asm.write("* ===================================================================\n\n")
        for nome, addr in simbolos:
            # Formato clássico: nome   EQU   $endereço
            f_asm.write(f"{nome:<28} EQU   ${addr:04X}\n")
    print("Sucesso: Arquivo 'pds317_rom.inc' (Assembly) gerado.")

    # ---- GERAR ARQUIVO PARA C (pds317_rom.h) ----
    with open("pds317_rom.h", "w") as f_c:
        f_c.write("/* =================================================================== */\n")
        f_c.write("/*             MAPA DE PONTEIROS DA ROM DO COMPUTADOR PDS317          */\n")
        f_c.write("/*              (Gerado automaticamente via gera_includes.py)         */\n")
        f_c.write("/* =================================================================== */\n\n")
        f_c.write("#ifndef PDS317_ROM_H\n")
        f_c.write("#define PDS317_ROM_H\n\n")
        
        for nome, addr in simbolos:
            # Mapeia como macros de ponteiros para funções que retornam int e aceitam parâmetros indefinidos
            # Isso evita que o GCC reclame de tipos ao fazer chamadas rápidas
            f_c.write(f"#define ROM_{nome:<24} ((int (*)(void))0x{addr:04X})\n")
            
        f_c.write("\n#endif /* PDS317_ROM_H */\n")
    print("Sucesso: Arquivo 'pds317_rom.h' (C) gerado.")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Uso: python3 gera_includes.py <arquivo.map> <obj1.o> [obj2.o] ...")
        sys.exit(1)
        
    gerar_arquivos(sys.argv[1], sys.argv[2:])