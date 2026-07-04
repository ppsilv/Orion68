/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdio.h>

#include "orion68.h"
#include "libs/fatfs/ff.h"	/* Obtains integer types */

#include "libs/fatfs/diskio.h"		            /* Declarations of disk functions */
#include "ata.h"
#include "rtc.h"

#include "interrupt.h"

#define log_notice printf
#define log_info printf

DSTATUS g_drive_status[FF_VOLUMES];
FATFS g_fat_fs_workarea[FF_VOLUMES];

ata_identify_info drive_id;

#define LOCK(saved) {                   \
    asm("move.w %%sr, %0\n" : "=dm" ((saved))); \
    m68k_disable_all_interrupts();                 \
}
#define UNLOCK(saved) {                     \
    m68k_enable_all_interrupts(); \
}
#define ATA_DELAY(x) { for (int delay = 0; delay < (x); delay++) { asm volatile(""); } }
#define ATA_WAIT()	 { ATA_DELAY(4); while (ATA_REG_STATUS & ATA_ST_BUSY) { } }
#define DEBUG_PRINT


static const char *g_fatfs_errmsg[20] =
{
    /* 0  */ "Succeeded",
    /* 1  */ "A hard error occurred in the low level disk I/O layer",
    /* 2  */ "Assertion failed",
    /* 3  */ "The physical drive is not operational",
    /* 4  */ "Could not find the file",
    /* 5  */ "Could not find the path",
    /* 6  */ "The path name format is invalid",
    /* 7  */ "Access denied due to prohibited access or directory full",
    /* 8  */ "Access denied due to prohibited access",
    /* 9  */ "The file/directory object is invalid",
    /* 10 */ "The physical drive is write protected",
    /* 11 */ "The logical drive number is invalid",
    /* 12 */ "The volume has no work area",
    /* 13 */ "There is no valid FAT volume",
    /* 14 */ "The f_mkfs() aborted due to any parameter error",
    /* 15 */ "Could not get a grant to access the volume within defined period",
    /* 16 */ "The operation is rejected according to the file sharing policy",
    /* 17 */ "LFN working buffer could not be allocated",
    /* 18 */ "Number of open files > _FS_LOCK",
    /* 19 */ "Given parameter is invalid"
};



/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (BYTE pdrv)
{
    if (pdrv >= FF_VOLUMES)
        return STA_NOINIT;

    return g_drive_status[pdrv];
}


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (BYTE pdrv)
{
    return ata_disk_initialize();
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count)
{
    if (pdrv >= FF_VOLUMES)
        return RES_PARERR;

    if (g_drive_status[pdrv] & (STA_NOINIT | STA_NODISK))
        return RES_NOTRDY;

    while (count > 0)
    {
        uint8_t *read_buff = (uint8_t *)buff;
        uint32_t sec_count = 1;
        if (ata_read_sector(sector, read_buff))
            return RES_ERROR;

        sector++;
        count--;
        buff += 512;
    }

	return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0
DRESULT disk_write (BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count)
{
    //printf("DiskIO: drive_write called. pdrv = %d, sector = %d, count = %d\n", (int)pdrv, (int)sector, (int)count);

    if (pdrv >= FF_VOLUMES)
        return RES_PARERR;

    if (g_drive_status[pdrv] & (STA_NOINIT | STA_NODISK))
        return RES_NOTRDY;

    while (count > 0)
    {
        uint8_t *read_buff = (uint8_t *)buff;
        uint32_t sec_count = 1;
        if (ata_write_sector(sector, read_buff))
            return RES_ERROR;

        sector++;
        count--;
        buff += 512;
    }

    return RES_OK;
}
#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
long get_sector_count(BYTE pdrv)
{
#ifdef DEBUG_PRINT
    printf("DiskIO: get_sector_count called. pdrv = %d\n", (int)pdrv);;
#endif
    return drive_id.current_log_sects_per_track;

}

DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void *buff)
{
    uint32_t count;
#ifdef DEBUG_PRINT
    printf("DiskIO: disk_ioctrl called. pdrv = %d cmd = %d\n", (int)pdrv, (int)cmd);;
#endif
    if (pdrv >= FF_VOLUMES)
        return RES_PARERR;

    if (g_drive_status[pdrv] & (STA_NOINIT | STA_NODISK))
        return RES_NOTRDY;

    switch (cmd)
    {
        case CTRL_SYNC:
        case CTRL_TRIM:
            return RES_OK;

        case GET_SECTOR_SIZE:
            *((long *)buff) = 512;
            return RES_OK;

        case GET_SECTOR_COUNT:
            count = get_sector_count(pdrv);
            *((uint32_t *)buff) = count;

            if (count == -1)
                return RES_ERROR;

            return RES_OK;

        default:
            return RES_PARERR;
    }
}

void printerro(int eno)
{
    if (eno <= 19)
        printf("%s\n", g_fatfs_errmsg[eno]);
    else
        printf("Unknown error %d\n", eno);
}

/*-------------------------------------------------------------------*/
/* User Provided RTC Function for FatFs module                       */
/*-------------------------------------------------------------------*/
/* This is a real time clock service to be called from FatFs module. */
/* This function is needed when FF_FS_READONLY == 0 and FF_FS_NORTC == 0 */
DWORD get_fattime(void)
{
    rtc_date_t tm;

    tm.tm_year = 1980;
    tm.tm_mon  = 1;
    tm.tm_day  = 1;
    tm.tm_hour = 0;
    tm.tm_min  = 0;
    tm.tm_sec  = 0;

    DWORD time = (tm.tm_year - 1980) << 25 | tm.tm_mon << 21 | tm.tm_day << 16 | tm.tm_hour << 11 | tm.tm_min << 5 | tm.tm_sec >> 1;
	return time;
}


/*
int ata_read_identity(void)
{

#ifdef DEBUG_PRINT
    printf("ata_read_identity: function entry\n");
#endif

	if (ata_wait_not_busy())
    {
#ifdef DEBUG_PRINT
        printf("ata_read_identity: ata_wait_for_data (1) error\n");
#endif
        return 1;
    }

	ATA_REG_SECTOR_COUNT = 1;
	ATA_REG_COMMAND = ATA_CMD_IDENTIFY;
	if (ata_wait_for_data())
    {
#ifdef DEBUG_PRINT
        printf("ata_read_identity: ata_wait_for_data (1) error\n");
#endif
        return 1;
    }

	uint8_t status = ATA_REG_STATUS;
	if (status & ATA_STATUS_ERROR)
    {
#ifdef DEBUG_PRINT
        printf("ata_read_identity: disk status error - status = %02X\n", status);
#endif
        return 1;
    }

	if (ata_wait_not_busy())
    {
#ifdef DEBUG_PRINT
        printf("ata_read_identity: ata_wait_not_busy (2) error\n");
#endif
        return 1;
    }

#ifdef DEBUG_PRINT
	printf("ata_read_identity: reading data....");
#endif

    uint8_t *ptr8 = (uint8_t *)&drive_id;

    for (int i = 0; i < 512; i++)
    {
        // Lê os dois bytes consecutivos do hardware
        ptr8[i]= ATA_REG_DATA_BYTE;
        ATA_WAIT();

        // Faz o Swap gravando invertido na memória do M68K (Big Endian)
    //    ptr8[i]     = byte2;
    //    ptr8[i + 1] = byte1;
    }
    for (int i = 0; i < 512; i += 2)
    {
        uint8_t tmp = ptr8[i];
        ptr8[i]     = ptr8[i + 1];
        ptr8[i + 1] = tmp;
    }

    
    //dump_memory((void *)ptr8, 512);
#ifdef DEBUG_PRINT
    printf("done\n");
#endif
	return 0;
}
*/