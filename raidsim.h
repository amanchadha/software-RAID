#ifndef RAIDSIM_H

/*Striping for RAID Level 0: Calculate the disk and block number from the LBA*/

void stripe_address_0(int ndisks, int lba, int strip, int *disk_to_use, int *address_on_disk);

/*Striping for RAID Level 4: Calculate the disk and block number from the LBA*/

void stripe_address_4(int ndisks, int lba, int strip, int *disk_to_use, int *address_on_disk);

/*Striping for RAID Level 5: Calculate the disk and block number from the LBA*/

void stripe_address_5(int ndisks, int lba, int strip, int *disk_to_use, int *address_on_disk);

/*Striping for RAID Level 10: Calculate the disk and block number from the LBA*/

void stripe_address_10(int ndisks, int lba, int strip, int *disk_to_use, int *address_on_disk);

/*Recover Disk for RAID Level 4 and 5: Open a new disk and zero it out*/

void recover_disk_4_5(disk_array_t *da, int nblocks, int ndisks, int disk);

/*Recover Disk for RAID 10: Open a new disk and zero it out*/

void recover_disk_10(disk_array_t *da, int disk_no, int disk_size, int strip, int ndisks);

/*Read RAID Level 0: Print out the first 4 bytes off 'size' number of blocks after striping*/

int read_0(int ndisks, int strip, int size, int lba, disk_array_t *da);

/*Read RAID Level 4: Print out the first 4 bytes off 'size' number of blocks after striping*/

int read_4(int ndisks, int strip, int size, int lba, disk_array_t *da);

/*Read RAID Level 5: Print out the first 4 bytes off 'size' number of blocks after striping*/

int read_5(int ndisks, int strip, int size, int lba, disk_array_t *da);

/*Read RAID Level 10: Print out the first 4 bytes off 'size' number of blocks after striping*/

int read_10(int ndisks, int strip, int size, int lba, disk_array_t *da);

/*Write RAID Level 0: Write 1024B sized 'buffer' to 'size' number of blocks*/

int write_0(int ndisks, int strip, int size, int lba, disk_array_t *da, char *buffer);

/*Write RAID Level 4: Write 1024B sized 'buffer' to 'size' number of blocks*/

int write_4(int ndisks, int strip, int size, int lba, disk_array_t *da, char *buffer);

/*Write RAID Level 5: Write 1024B sized 'buffer' to 'size' number of blocks*/

int write_5(int ndisks, int strip, int size, int lba, disk_array_t *da, char *buffer);

/*Write RAID Level 10: Write 1024B sized 'buffer' to 'size' number of blocks*/

int write_10(int ndisks, int strip, int size, int lba, disk_array_t *da, char *buffer);

#endif // RAIDSIM_H
