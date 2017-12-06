#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

struct {
  struct spinlock lock;
  struct shm_page {
    uint id;
    char *frame;
    int refcnt;
  } shm_pages[64];
} shm_table;

void shminit() {
  int i;
  initlock(&(shm_table.lock), "SHM lock");
  acquire(&(shm_table.lock));
  for (i = 0; i< 64; i++) {
    shm_table.shm_pages[i].id =0;
    shm_table.shm_pages[i].frame =0;
    shm_table.shm_pages[i].refcnt =0;
  }
  release(&(shm_table.lock));
}

int shm_open(int id, char **pointer) {

//you write this
	int flag = 0;

	acquire(&(shm_table.lock));

	for(int i = 0; i < shm_table.size(); i++)
	{
		if(id == shm_table.shm_pages[i].id)
		{
			flag = 1;
		}
	}
	
	if(flag == 0) //id not found in shm_table
	{
		//allocate a page and map it
		//store this info in shm_table
	}
	else
	{
		//increase reference count
		//use mappages to add the mapping between v address and p address.
		mappages(	
	}

	//in either case, return the virtual address through the second param of the system call

	//this is helpful somewhere
	//myproc()->sz += PGSIZE;

return 0; //added to remove compiler warning -- you should decide what to return
}


int shm_close(int id) {
//you write this too!




return 0; //added to remove compiler warning -- you should decide what to return
}
