/*
test program for the RAID project.
*/

#include "disk-array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
 
int verbose = 1;

int main( int argc, char *argv[] )
{
	if(argc!=4) {
	  printf("use: %s <ndisks> <nblocks> <niter>\n",argv[0]);
		return 1;
	}

	int ndisks = atoi(argv[1]);
	int nblocks = atoi(argv[2]);
	int niter = atoi(argv[3]);

	disk_array_t da = disk_array_create("myvirtualdisk",ndisks,nblocks);
	if(!da) {
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}

	//
	// Randomly read/write to disks
	//
	char buffer[BLOCK_SIZE];
	char cmpbuffer[BLOCK_SIZE];
	char zerobuffer[BLOCK_SIZE];
	int i;
	int failed_disk = -1;

	memset(zerobuffer,0,sizeof(zerobuffer));

#define PROB_FAIL 10
#define PROB_RECOVER 10
#define NUM_OPS 2
#define OP_READ 0
#define OP_WRITE 1
	
	for (i = 0; i < niter; i++) {
	  if (failed_disk != -1) {
	    if ((rand() % PROB_RECOVER) == 0) {
	      disk_array_recover_disk(da,failed_disk);
	      failed_disk = -1;
	      continue;
	    }
	  } else if ((rand() % PROB_FAIL) == 0) {
	      failed_disk = rand() % ndisks;
	      disk_array_fail_disk(da,failed_disk);
	      continue;
	  }
	  int disk = rand() % ndisks;
	  int block = rand() % nblocks;
	  switch (rand() % NUM_OPS) {
	  case OP_READ:
	    if (disk_array_read(da, disk, block, buffer) == 0) {
	      memset(cmpbuffer,(disk+1)*(block+1),sizeof(cmpbuffer));
	      if ((memcmp(cmpbuffer,buffer,sizeof(buffer)) != 0) &&
		  (memcmp(zerobuffer,buffer,sizeof(buffer)) != 0))
		printf("Error: mismatch disk %d block %d value %d should be %d or zero\n",
		       disk,
		       block,
		       buffer[0],
		       cmpbuffer[0]);
	    }
	    break;
	  case OP_WRITE:
	    memset(buffer,(disk+1)*(block+1),sizeof(buffer));
	    disk_array_write(da, disk, block, buffer);
	    printf("Wrote disk %d block %d value %d\n",
		   disk, block, buffer[0]);

	    break;		       
	  default:
	    printf("Illegal operation value\n");
	    break;
	  }
	}

	disk_array_close(da);

	return 0;
}
