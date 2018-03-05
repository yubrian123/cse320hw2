#include "helper.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

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
    int numberOfBlocks = 0;
    int pointerThruRam = 0;

    //counters for flag and ID
    int idCounter = 1;
    int flagCounter = 1;
    int sizeCounter = 32;

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
      pointerThruRam = 0;
      block =  (uint64_t*) ram;
      flag = *block & 1;
      id = *block & 6;
      id = id >> 1;
      size = *block >> 3;
      size = size * 8;
      while(pointerThruRam < 1024)
      {
        flag = *block & 1;
        id = *block & 6;
        id = id >> 1;
        size = *block >> 3;
        size = size * 8;
        if(id == idCounter && flag == flagCounter && sizeCounter == size)
        {
          memmove(tmp_buf, block, size);
          numberOfBlocks++;
          totalSize = totalSize + size;
          pointerThruRam = pointerThruRam + size;
          tmp_buf = tmp_buf + size;
          ram = ram + size;
          block = (uint64_t*) ram;
        }
        else
        {
          pointerThruRam = pointerThruRam + 8;
          size = 8;
          ram = ram + size;
          block = (uint64_t*) ram;
        }
      }
      if(sizeCounter == 1024)
      {
        if(flagCounter == 0)
        {
          ram = ram - 1024;
          flagCounter = 1;
          sizeCounter = 32;
          idCounter++;
        }
        else
        {
          ram = ram - 1024;
          sizeCounter = 32;
          flagCounter = 0;
        }
      }
      else
      {
        ram = ram - 1024;
        sizeCounter = sizeCounter + 16;
      }
    }
    
    (*(char*)tmp_buf) = 16;
    tmp_buf = tmp_buf + 8;
    (*(char*)tmp_buf) = 16;

    tmp_buf = tmp_buf - totalSize - 8;

    memcpy(ram, tmp_buf, 1024);

    uint64_t* pointerOne =  (uint64_t*) tmp_buf;
    int pointOneFlag = *pointerOne & 1;
    int pointerOneID = *pointerOne & 6;
    pointerOneID = pointerOneID >> 1;
    int pointerOneSize = *pointerOne >> 3;
    pointerOneSize = pointerOneSize * 8;

    tmp_buf = tmp_buf + pointerOneSize;

    uint64_t* pointerTwo =  (uint64_t*) tmp_buf;
    int pointTwoFlag = *pointerTwo & 1;
    int pointerTwoID = *pointerTwo & 6;
    pointerTwoID = pointerTwoID >> 1;
    int pointerTwoSize = *pointerTwo >> 3;
    pointerTwoSize = pointerTwoSize * 8;

    while(pointerTwoSize != 0)
    {
      if(pointOneFlag == 0 && pointTwoFlag == 0 && pointerOneID == pointerTwoID)
      {

        pointOneFlag = *pointerOne & 1;
        pointerOneID = *pointerOne & 6;
        pointerOneID = pointerOneID >> 1;
        pointerOneSize = *pointerOne >> 3;
        pointerOneSize = pointerOneSize * 8;
        (*(uint64_t*)pointerOne) += pointerTwoSize;
        tmp_buf = tmp_buf + pointerTwoSize - 8;
        uint64_t* footerOfCoalesce =  (uint64_t*) tmp_buf;
        (*(uint64_t*)footerOfCoalesce) += pointerOneSize;
        pointOneFlag = *footerOfCoalesce & 1;
        pointerOneID = *footerOfCoalesce & 6;
        pointerOneID = pointerOneID >> 1;
        pointerOneSize = *footerOfCoalesce >> 3;
        pointerOneSize = pointerOneSize * 8;
        tmp_buf = tmp_buf + 8;

        //moving Pointer 2 To next Block
        pointerTwo = (uint64_t*) tmp_buf;
        pointTwoFlag = *pointerTwo & 1;
        pointerTwoSize = *pointerTwo >> 3;
        pointerTwoSize = pointerTwoSize * 8;
        pointerTwoID = *pointerTwo & 6;
        pointerTwoID = pointerTwoID >> 1;
      }
      else
      {
        pointerOne = pointerTwo;
        pointOneFlag = *pointerOne & 1;
        pointerOneSize = *pointerOne >> 3;
        pointerOneSize = pointerOneSize * 8;
        pointerOneID = *pointerOne & 6;
        pointerOneID = pointerOneID >> 1;

        tmp_buf = tmp_buf + pointerTwoSize;

        pointerTwo = (uint64_t*) tmp_buf;
        pointTwoFlag = *pointerTwo & 1;
        pointerTwoSize = *pointerTwo >> 3;
        pointerTwoSize = pointerTwoSize * 8;
        pointerTwoID = *pointerTwo & 6;
        pointerTwoID = pointerTwoID >> 1;
      }
    }

    tmp_buf = tmp_buf - totalSize - 16;
    memcpy(ram, tmp_buf, totalSize + 16);

    /*
     * Do not modify code below.
     */
    cse320_check();
    cse320_free();
    return ret;
}

