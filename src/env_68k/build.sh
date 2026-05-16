#!/bin/bash

# Configurações
export PREFIX="/usr/local/m68k-elf"
export TARGET="m68k-elf"
export PATH="$PREFIX/bin:$PATH"

# Criar diretórios
mkdir -p lib include obj examples

# Compilar biblioteca
make lib

# Compilar exemplos
make examples

echo "Build completo! Biblioteca criada em: lib/libmc68000.a"
