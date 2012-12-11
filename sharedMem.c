#include <stdio.h>
#include <sys/mman.h>
#include <netdb.h>

#define NUM_CORES 8

uint32_t * memAddr;
size_t sharedMemSize = NUM_CORES;
int status[NUM_CORES] = {0};

void sharedMemInit()
{
  memAddr = (uint32_t *)409600000;
  memAddr = mmap(memAddr, sharedMemSize,
                        PROT_WRITE|PROT_READ, MAP_SHARED|MAP_ANON|MAP_FIXED, -1, 0);
}

int sharedMemWrite(int dest, uint32_t * data)
{
  if (status[dest] == 1)
    return 0;
  else
  {
    *(memAddr + dest) = *data;
    status[dest] = 1;
    return 1;
  }
}

int sharedMemRead(int src, uint32_t * data)
{
  if (status[src] == 0)
    return 0;
  else
  {
    *data = *(memAddr + src);
    status[src] = 0;
    return 1;
  }
}


int main()
{ 
    uint32_t write_data = 9999;
    uint32_t read_data;

    sharedMemInit();

    if (sharedMemWrite(3,&write_data))
      printf("Write Success \n");

    if (sharedMemRead(3,&read_data))
      printf("Read Success %d \n",(int) read_data);
    return 0;
} 
