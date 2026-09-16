#ifndef __SYSCALL_H__
#define __SYSCALL_H__


// 0x0000 - 0x000F = tasks
#define SYS_EXIT    0x0000
#define SYS_SPAWN   0x0001
#define SYS_SLEEP   0x0002
#define SYS_YIELD   0x0003
#define SYS_00_0f4  0x0004
#define SYS_00_0f5  0x0005
#define SYS_00_0f6  0x0006
#define SYS_00_0f7  0x0007
#define SYS_00_0f8  0x0008
#define SYS_00_0f9  0x0009
#define SYS_00_0fA  0x000A
#define SYS_00_0fB  0x000B
#define SYS_00_0fC  0x000C
#define SYS_00_0fD  0x000D
#define SYS_00_0fE  0x000E
#define SYS_00_0fF  0x000F

//0x0010 - 0x001F = VFS
#define SYS_OPEN     0x0010
#define SYS_CLOSE    0x0011
#define SYS_READ     0x0012
#define SYS_WRITE    0x0013
#define SYS_SINC     0x0014
#define SYS_LSEEK    0x0015
#define SYS_IOCTL    0x0016
#define SYS_DUP      0x0017
#define SYS_UNLINK   0x0018
#define SYS_CHMOD    0x0019
#define SYS_TELL     0x001A
#define SYS_SIZE     0x001B
#define SYS_EOF      0x001C
#define SYS_RENAME   0x001D
#define SYS_STAT     0x001E
#define SYS_OPENDIR  0x001F
#define SYS_10_1f6   0x0006
#define SYS_10_1f7   0x0007
#define SYS_10_1f8   0x0008
#define SYS_10_1f9   0x0009
#define SYS_10_1fA   0x000A
#define SYS_10_1fB   0x000B
#define SYS_10_1fC   0x000C
#define SYS_10_1fD   0x000D
#define SYS_10_1fE   0x000E
#define SYS_10_1fF   0x000F

//0x0020 - 0x002F = VFS
#define SYS_READDIR   0x0020
#define SYS_MOUNT     0x0021
#define SYS_CLOSEDIR  0x0022
#define SYS_FMKDIR    0x0023
#define SYS_UNMOUNT   0x0024
#define SYS_FINDFIRST 0x0025
#define SYS_FINDNEXT  0x0026
#define SYS_20_2f7    0x0027
#define SYS_20_2f8    0x0028
#define SYS_20_2f9    0x0029
#define SYS_20_2fA    0x002A
#define SYS_20_2fB    0x002B
#define SYS_20_2fC    0x002C
#define SYS_20_2fD    0x002D
#define SYS_20_2fE    0x002E
#define SYS_20_2fF    0x002F

//0x0030 - 0x003F = VIDEO
#define SYS_VGA_PUTCHAR  0x0030
#define SYS_GOTOXY       0x0031
#define SYS_GOHOME       0x0032
#define SYS_SETTEXTCOLOR 0x0033
#define SYS_CLRSCR       0x0034
#define SYS_30_3f5       0x0035
#define SYS_30_3f6       0x0036
#define SYS_30_3f7       0x0037
#define SYS_30_3f8       0x0038
#define SYS_30_3f9       0x0039
#define SYS_30_3fA       0x003A
#define SYS_30_3fB       0x003B
#define SYS_30_3fC       0x003C
#define SYS_30_3fD       0x003D
#define SYS_30_3fE       0x003E
#define SYS_30_3fF       0x003F

//0x0040 - 0x004F = GVIDEO
#define SYS_DRAWVLINE         0x0041
#define SYS_DRAWHLINE         0x0042
#define SYS_DRAWLINE          0x0043
#define SYS_DRAWCIRCLE        0x0044
#define SYS_DRAWCIRCLEHELPER  0x0045
#define SYS_FILLCIRCLE        0x0046
#define SYS_FILLCIRCLEHELPER  0x0047
#define SYS_DRAWROUNDRECT     0x0048
#define SYS_FILLROUNDRECT     0x0049
#define SYS_FILLRECT          0x004A
#define SYS_40_4fB            0x004B
#define SYS_40_4fC            0x004C
#define SYS_40_4fD            0x004D
#define SYS_40_4fE            0x004E
#define SYS_40_4fF            0x004F

//0x0050 - 0x005F = RTC
#define SYS_READTIME          0x0050
#define SYS_WRITETIME         0x0051
#define SYS_EEPROMREADBYTE    0x0052
#define SYS_EEPROMWRITEBYTE   0x0053
#define SYS_50_5f4            0x0054
#define SYS_50_5f5            0x0055
#define SYS_50_5f6            0x0056
#define SYS_50_5f7            0x0057
#define SYS_50_5f8            0x0058
#define SYS_50_5f9            0x0059
#define SYS_50_5fA            0x005A
#define SYS_50_5fB            0x005B
#define SYS_50_5fC            0x005C
#define SYS_50_5fD            0x005D
#define SYS_50_5fE            0x005E
#define SYS_50_5fF            0x005F

//0x0060 - 0x006F = keyboard
#define SYS_RINGBUFFERREAD    0x0060
#define SYS_RINGBUFFERWRITE   0x0061
#define SYS_60_6f4            0x0062
#define SYS_60_6f4            0x0063
#define SYS_60_6f4            0x0064
#define SYS_60_6f5            0x0065
#define SYS_60_6f6            0x0066
#define SYS_60_6f7            0x0067
#define SYS_60_6f8            0x0068
#define SYS_60_6f9            0x0069
#define SYS_60_6fA            0x006A
#define SYS_60_6fB            0x006B
#define SYS_60_6fC            0x006C
#define SYS_60_6fD            0x006D
#define SYS_60_6fE            0x006E
#define SYS_60_6fF            0x006F

//0x0070 - 0x007F = duart
#define SYS_DUARTACFG         0x0070
#define SYS_DUARTAREAD        0x0071
#define SYS_DUARTAWRITE       0x0072
#define SYS_DUARTBCFG         0x0073
#define SYS_DUARTBREAD        0x0074
#define SYS_DUARTBWRITE       0x0075
#define SYS_70_7f6            0x0076
#define SYS_70_7f7            0x0077
#define SYS_70_7f8            0x0078
#define SYS_70_7f9            0x0079
#define SYS_70_7fA            0x007A
#define SYS_70_7fB            0x007B
#define SYS_70_7fC            0x007C
#define SYS_70_7fD            0x007D
#define SYS_70_7fE            0x007E
#define SYS_70_7fF            0x007F

//0x0080 - 0x008F = ppi
#define SYS_PPIACFG           0x0080
#define SYS_PPIPORTAREAD      0x0081
#define SYS_PPIPORTAWRITE     0x0082
#define SYS_PPIBCFG           0x0083
#define SYS_PPIPORTBREAD      0x0084
#define SYS_PPIPORTBWRITE     0x0085
#define SYS_PPICCFG           0x0086
#define SYS_PPIPORTCREAD      0x0087
#define SYS_PPIPORTCWRITE     0x0088
#define SYS_80_8f9            0x0089
#define SYS_80_8fA            0x008A
#define SYS_80_8fB            0x008B
#define SYS_80_8fC            0x008C
#define SYS_80_8fD            0x008D
#define SYS_80_8fE            0x008E
#define SYS_80_8fF            0x008F

//0x0090 - 0x009F = Sockets
#define SYS_CONNECT           0x0090
#define SYS_BIND              0x0091
#define SYS_LISTEN            0x0092
#define SYS_ACCEPT            0x0093
#define SYS_SETSOCKOPT        0x0094
#define SYS_90_9f9            0x0095
#define SYS_90_9f9            0x0096
#define SYS_90_9f9            0x0097
#define SYS_90_9f9            0x0098
#define SYS_90_9f9            0x0099
#define SYS_90_9fA            0x009A
#define SYS_90_9fB            0x009B
#define SYS_90_9fC            0x009C
#define SYS_90_9fD            0x009D
#define SYS_90_9fE            0x009E
#define SYS_90_9fF            0x009F
 
  
 const sys_call_t sys_call_table[] = {
    (sys_call_t) sys_fput,      // Syscall 0
    (sys_call_t) sys_getpid,    // Syscall 1
  
};
  
  
  
 