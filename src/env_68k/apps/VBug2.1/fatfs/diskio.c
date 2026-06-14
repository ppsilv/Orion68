/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2025        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "ff.h"			/* Basic definitions of FatFs */
#include "diskio.h"		/* Declarations FatFs MAI */
#include "ata.h"

/* Example: Declarations of the platform and disk functions in the project */
#include "platform.h"
#include "storage.h"

/* Example: Mapping of physical drive number for each drive */
#define DEV_FLASH	0	/* Map FTL to physical drive 0 */
#define DEV_MMC		1	/* Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Map USB MSD to physical drive 2 */
#define DEV_MIDE    3   /* Solid State disk*

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_MIDE :
		result = ata_disk_status();
		return stat;

	case DEV_MMC :
		return stat;

	case DEV_USB :
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_MIDE :
		return ata_disk_initialize();
	case DEV_MMC :
		return stat;

	case DEV_USB :
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_MIDE :
		result = ata_read_sector_multi(sector, buff, count);
		return res;

	case DEV_MMC :
		return res;

	case DEV_USB :
		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_MIDE :
		result = ata_write_sector_multi(sector, buff, count);
		return res;

	case DEV_MMC :
		return res;

	case DEV_USB :
		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_MIDE :
		return res;

	case DEV_MMC :
		return res;

	case DEV_USB :
		return res;
	}

	switch (ctrl) {
	case CTRL_SYNC:			/* Nothing to do */
		res = RES_OK;
		break;

	case GET_SECTOR_COUNT:	/* Get number of sectors on the drive */
		*(LBA_t*)buff = Stat[pdrv].n_sectors;
		res = RES_OK;
		break;

	case GET_SECTOR_SIZE:	/* Get size of sector for generic read/write */
		*(WORD*)buff = Stat[pdrv].sz_sector;
		res = RES_OK;
		break;

	case GET_BLOCK_SIZE:	/* Get internal block size in unit of sector */
		*(DWORD*)buff = SZ_BLOCK;
		res = RES_OK;
		break;
	}




	return RES_PARERR;
}

