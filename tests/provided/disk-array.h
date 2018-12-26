
/*
Do not modify this file.
Make all of your changes to main.c instead.
*/

#ifndef DISK_ARRAY_H
#define DISK_ARRAY_H
#include "disk.h"

typedef struct disk_array * disk_array_t;

#define MAX_DISKS (100)
#define MAX_BLOCKS (1000000)

disk_array_t disk_array_create( const char *filename, int disks, int blocks );

/*
Write exactly BLOCK_SIZE bytes to a given block on the virtual disk array.
"d" must be a pointer to a virtual disk, "block" is the block number,
and "data" is a pointer to the data to write.
*/

int disk_array_write( disk_array_t da, int disk, int block, const char *data );

/*
Read exactly BLOCK_SIZE bytes from a given block on the virtual disk.
"d" must be a pointer to a virtual disk, "block" is the block number,
and "data" is a pointer to where the data will be placed.
*/

int disk_array_read( disk_array_t da, int disk, int block, char *data );


/*
Mark a disk as failed; subsequent calls to read or write data will fail
 */

int disk_array_fail_disk( disk_array_t da, int disk);

/*
Mark a disk as recovered; it will zero-out the contents of the disk
*/

int disk_array_recover_disk( disk_array_t da, int disk);

/*
Return the total number of blocks on each virtual disk in the array.
*/

int disk_array_nblocks( disk_array_t da );

/*
Return the number of disks in the virtual disk array
*/
int disk_array_ndisks( disk_array_t da );

/*
Close the virtual disk array.
*/

void disk_array_close( disk_array_t da );

/*
print number of reads and writes
*/

void disk_array_print_stats( disk_array_t da);

#endif // DISK_ARRAY_H
