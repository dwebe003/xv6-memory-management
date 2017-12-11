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
	acquire(&(shm_table.lock));
	int flag = -1;
	int i = 0;

        for(i = 0; i < 64; i++)
        {
                if(id == shm_table.shm_pages[i].id)
                {
                        flag = i;
                }
        }

        if(flag == -1) //id not found in shm_table
        {
		i = 0;	
		for(i = 0; i < 64; i++)
		{
			if(shm_table.shm_pages[i].id == 0)
			{
			  flag = i;
		
			  shm_table.shm_pages[i].frame = kalloc();

			  memset(shm_table.shm_pages[i].frame, 0, PGSIZE);

			  mappages(myproc()->pgdir, (void *)myproc()->sz, PGSIZE, V2P((shm_table.shm_pages[i].frame)), PTE_W|PTE_U);
			  *pointer = (char *)myproc()->sz;
			  myproc()->sz += PGSIZE;
			  shm_table.shm_pages[i].id = id;
                	  shm_table.shm_pages[i].refcnt += 1;
			  break;
			}
		}
		//kmalloc a page and store its address in frame. set refcnt to 1. map page
                //allocate a page and map it
                //store this info in shm_table
        }
        else
        {
		shm_table.shm_pages[flag].refcnt++;
		mappages(myproc()->pgdir, (void *)myproc()->sz, PGSIZE, V2P((shm_table.shm_pages[flag].frame)), PTE_W|PTE_U);
		*pointer = (char *)myproc()->sz;
		myproc()->sz += PGSIZE;
                //increase reference count
                //use mappages to add the mapping between v address and p address.      
        }

        //in either case, return the virtual address through the second param of the system call

        //this is helpful somewhere
        //myproc()->sz += PGSIZE;
	if(flag > -1)
	{
		release(&(shm_table.lock));
		return 1;
	}

        return id; //added to remove compiler warning -- you should decide what to return
}

int shm_close(int id) {
//you write this too!
	acquire(&(shm_table.lock));
	int flag = 99;
	int i = 0;

	for(i = 0; i < 64; i++)
	{
	  if(id == shm_table.shm_pages[i].id)
	  {
		//shm_table.shm_pages[i].refcnt -= 1;
		if(shm_table.shm_pages[i].refcnt > 0)
		{
			flag = i;
			shm_table.shm_pages[i].refcnt--;
			if(shm_table.shm_pages[i].refcnt == 0)
			{
		  		shm_table.shm_pages[i].id = 0;
		 		 shm_table.shm_pages[i].frame = 0;
			}
		}
	  }
	}

	if(flag != 99)
	{		
		release(&(shm_table.lock));
		return 0;
	}
	

release(&(shm_table.lock));
return 0; //added to remove compiler warning -- you should decide what to return
}
