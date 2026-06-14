#ifndef __ATA_H__
#define __ATA_H__

int ata_detect(void);
int ata_read_sector(int sector, char *buffer);
int ata_read_sector_multi(int sector, char *buffer, int count);
int ata_write_sector(int sector, const char *buffer);
int ata_write_sector_multi(int sector, const char *buffer,int count);
int read_partition_table(char *buffer, struct partition *devices);
int ata_init(void);
int ata_disk_status(void);
int ata_disk_initialize(void);

#endif