#ifndef FSATTR_H
#define FSATTR_H

#include <fatfs/ff.h>

typedef struct {
    char letra;
    BYTE bit;
} attr_map_t;

static const attr_map_t ATTR_TABLE[] = {
    { 'r', AM_RDO },
    { 'h', AM_HID },
    { 's', AM_SYS },
    { 'a', AM_ARC },
};
#define ATTR_TABLE_LEN (sizeof(ATTR_TABLE) / sizeof(ATTR_TABLE[0]))

static inline void fmt_attr_string(BYTE attr, char *out)
{
    uint8_t i;
    for (i = 0; i < ATTR_TABLE_LEN; i++) {
        out[i] = (attr & ATTR_TABLE[i].bit)
                     ? (char) (ATTR_TABLE[i].letra - 'a' + 'A')
                     : '-';
    }
    out[ATTR_TABLE_LEN] = '\0';
}

#endif
