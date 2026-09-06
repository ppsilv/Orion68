#  /                       (raiz do FatFS, montada pelo orioncore no boot)
#  ├── KERNEL.BIN          # imagem do kernel, carregada pelo orioncore
#  │
#  ├── BIN/                # executáveis .elf prontos pra rodar
#  │   ├── SHELL.ELF
#  │   ├── DUART.ELF
#  │   └── ...
#  │
#  ├── DEV/        # arquivos de METADADO de device (não são "arquivos" de verdade)
#  │   ├── SERIAL0         # baudrate=9600 / databits=8 / parity=none...
#  │   ├── RTC0
#  │   └── VIDEO0
#  │
#  ├── ETC/                # configuração do sistema
#  │   ├── BOOT.CFG        # o que o orioncore/kernel carrega automaticamente
#  │   └── AUTOEXEC        # comandos rodados pelo shell no login/boot
#  │
#  ├── HOME/               # área do usuário
#  │
#  └── TMP/


touch kernel.bin
mkdir bin
touch shell.elf
touch duart.elf
mkdir dev
touch dev/serial0
touch dev/rtc0
touch dev/video0
mkdir etc
touch etc/boot.cfg
touch etc/autoexec
mkdir -p home/pdsilva
mkdir tmp
