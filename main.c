#include "helper.h"

int main(int argc, char** argv) {
    if (*(argv + 1) == NULL) {
        printf("You should provide name of the test file.\n");
        return 1;
    }
    void* ram = cse320_init(*(argv + 1));
    void* tmp_buf = cse320_tmp_buffer_init();
    int ret = 0;
    /*
     * You code goes below. Do not modify provided parts
     * of the code.
     */
    //Initial Errors Checks
    if(ram == NULL)
    {
      printf("INIT_ERROR \n");
      return errno;
    }
    if(cse320_sbrk(1024-128) == NULL)
    {
        printf("SBRK_ERROR \n");
        return errno;
    }

    //Flag ID Size of Block
    int flag;
    int id;
    int size;

    //Total Size of Blocks
    int totalSize = 0;

    //counters for flag and ID
    int idCounter = 1;
    int flagCounter = 1;
    int sizeCounter = 32;
    int sizeMoved = 0;

    //first_block
    uint64_t* block =  (uint64_t*) ram;
    flag = *block & 1;
    id = *block & 6;
    id = id >> 1;
    size = *block >> 3;
    size = size * 8;

    //This sorts the blocks by id and flags and size(size starts at 32 and goes to a max of 1024)
    while(idCounter < 4)
    {
      printf("IDCounter = %i and Flag is %i and size is %i \n" , idCounter, flagCounter, sizeCounter);
      sizeMoved = 0;
      block =  (uint64_t*) ram;
      flag = *block & 1;
      id = *block & 6;
      id = id >> 1;
      size = *block >> 3;
      size = size * 8;
      while(size != 0)
      {
        flag = *block & 1;
        id = *block & 6;
        id = id >> 1;
        size = *block >> 3;
        size = size * 8;
        sizeMoved = sizeMoved + size;
        if(id == idCounter && flag == flagCounter && sizeCounter == size)
        {
          printf("moved \n");
          memmove(tmp_buf, block, size);
          totalSize = totalSize + size;
          tmp_buf = tmp_buf + size;
        }
        ram = ram + size;
        block = (uint64_t*) ram;
      }
      if(sizeCounter == 1024)
      {
        if(flagCounter == 0)
        {
          ram = ram - sizeMoved;
          flagCounter = 1;
          sizeCounter = 32;
          idCounter++;
        }
        else
        {
          ram = ram - sizeMoved;
          sizeCounter = 32;
          flagCounter = 0;
        }
      }
      else
      {
        ram = ram - sizeMoved;
        sizeCounter = sizeCounter + 16;
      }
    }

    tmp_buf = tmp_buf - totalSize;

    memcpy(ram, tmp_buf, totalSize);

    

    /*
     * Do not modify code below.
     */
    cse320_check();
    cse320_free();
    return ret;
}

