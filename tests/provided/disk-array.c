#include "disk-array.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

extern int verbose;

struct disk_array {
  int ndisks;
  int nblocks;
  char * filename;
  struct disk * disks[0];
};

/*
Open and initialize an array of disks
*/

disk_array_t disk_array_create( const char *filename, 
				int disks, 
				int blocks )
{
  disk_array_t disk_array = NULL;
  int i;
  char tempbuf[strlen(filename)+6];
  
  if ((disks < 1) || (disks > MAX_DISKS)) {
    fprintf(stderr,"Disk_array_create: wrong number of disks %d\n",disks);
    goto Cleanup;
  }

  if ((blocks < 1) || (blocks > MAX_BLOCKS)) {
    fprintf(stderr,"Disk_array_create: wrong number of blocks %d\n",blocks);
    goto Cleanup;
  }

  disk_array = (disk_array_t) malloc(sizeof(struct disk_array) + (disks) * sizeof(struct disk *));
  if (disk_array == NULL) {
    goto Cleanup;
  }

  memset(disk_array, 0, sizeof(struct disk_array) + (disks) * sizeof(struct disk *));

  disk_array->ndisks = disks;
  disk_array->nblocks = blocks;
  disk_array->filename = strdup(filename);
  if (disk_array->filename == NULL) {
    goto Cleanup;
  }

  //
  // Initialize each disk separately
  //

  for (i = 0; i< disks; i++) {
    sprintf(tempbuf,"%s-%d",filename,i);
    disk_array->disks[i] = disk_open(tempbuf,blocks);
    if (disk_array->disks[i] == NULL) {
      goto Cleanup;
    }
  }
  return(disk_array);
  Cleanup:
    disk_array_close(disk_array);
    return(NULL);
}

/*
Write exactly BLOCK_SIZE bytes to a given block on the virtual disk array.
"d" must be a pointer to a virtual disk, "block" is the block number,
and "data" is a pointer to the data to write.
*/

int disk_array_write( disk_array_t da, int disk, int block, const char *data )
{
  if (verbose) {
    printf("DISK ARRAY: WRITE DISK %d BLOCK %d\n",disk,block);
  }
  if ((disk >= 0) && (disk < da->ndisks) && da->disks[disk]){
    disk_write(da->disks[disk],block,data);
    return(0);
  }
  return(-1);
}

/*
Read exactly BLOCK_SIZE bytes from a given block on the virtual disk.
"d" must be a pointer to a virtual disk, "block" is the block number,
and "data" is a pointer to where the data will be placed.
*/

int disk_array_read( disk_array_t da, int disk, int block, char *data )
{
  if (verbose) {
    printf("DISK ARRAY: READ DISK %d BLOCK %d\n",disk,block);
  }
  if ((disk >= 0) && (disk < da->ndisks) && da->disks[disk]){
    disk_read(da->disks[disk],block,data);
    return(0);
  }
  return(-1);
}

/*
Return the total number of blocks on each virtual disk in the array.
*/

int disk_array_nblocks( disk_array_t da ) 
{
  return(da->nblocks);
}

/*
Return the number of disks in the virtual disk array
*/
int disk_array_ndisks( disk_array_t da )
{
  return(da->ndisks);
}

/*
Close the virtual disk array.
*/

void disk_array_close( disk_array_t da )
{
  int i;
  if (da != NULL) {
    for (i = 0; i < da->ndisks; i ++) {
      if (da->disks[i] != NULL) {
	disk_close(da->disks[i]);
	da->disks[i] = 0;
      }
    }
    if (da->filename != NULL) {
      free(da->filename);
      da->filename = NULL;
    }
    free(da);
  }
}


/*
Mark a disk as failed; subsequent calls to read or write data will fail
 */

int disk_array_fail_disk( disk_array_t da, int disk)
{
  if ((disk >= 0) && (disk < da->ndisks) && da->disks[disk]) {
    if (verbose) {
      printf("DISK ARRAY: FAIL DISK %d\n",disk);
    }

    disk_close(da->disks[disk]);
    da->disks[disk] = NULL;
    return(0);
  }
  return(-1);

}

/*
Mark a disk as recovered; it will zero-out the contents of the disk
*/

int disk_array_recover_disk( disk_array_t da, int disk)
{
  if ((disk >= 0) && (disk < da->ndisks)) {
    if (da->disks[disk] == NULL) {
      char tempbuf[strlen(da->filename)+6];
      sprintf(tempbuf,"%s-%d",da->filename,disk);
      da->disks[disk] = disk_open(tempbuf,da->nblocks);

      if (verbose) {
	printf("DISK ARRAY: RECOVER DISK %d\n",disk);
      }
      if (da->disks[disk]) {
	return(0);
      }
    }
  }
  return(-1);
}


void disk_array_print_stats( disk_array_t da)
{
  int i;
  if (da != NULL) {
    for (i = 0; i < da->ndisks; i ++) {
      if (da->disks[i] != NULL) {
	printf("STATS FOR DISK %d\n",i);
	disk_print_stats(da->disks[i]);
	da->disks[i] = 0;
      }
    }
  }
}
