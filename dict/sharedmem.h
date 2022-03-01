#ifndef __SHAREDMEM_H__
#define __SHAREDMEM_H__
#include <iostream>
#include <string.h>
#include <cstring>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;
const unsigned int SHARED_MEM_NAME_LEN = 20;

#define  SHM_MEM_NAME "edb_dict1"

class SharedMem {
  int mShmFd;
  char * mShmPtr;
  char mShmName[SHARED_MEM_NAME_LEN];
  unsigned long mShmMemSize;
public:
  SharedMem(unsigned long size) {
    mShmFd = -1;
    mShmPtr = NULL;
    mShmMemSize = size;
  }

  ~SharedMem() {
  }

  char * getSharedMem(bool &isExist)
  {
    //mShmFd = shm_open("edb_dict1", O_RDWR, S_IRUSR | S_IWUSR);
    mShmFd = shm_open(SHM_MEM_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    if(mShmFd != -1) {
      // shared memory is already present
      isExist = true;
    } else {
      //mShmFd = shm_open("edb_dict1", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
      mShmFd = shm_open(SHM_MEM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
      if(mShmFd == -1) {
        cout << "Shared memory open faile" << endl;;
        return NULL;
      }
      if(ftruncate(mShmFd, mShmMemSize) == -1) {
        cout << "Error in allocating the shared memory size." << endl;
        return NULL;
      }
    }
    mShmPtr = (char *)mmap(NULL, mShmMemSize, PROT_READ | PROT_WRITE, MAP_SHARED, mShmFd, 0);

    return mShmPtr;
  }

  void releaseSharedMem() {
    /*Release memory from current process */
    //shm_unlink("edb_dict1");
    shm_unlink(SHM_MEM_NAME);
  }
};

#endif
