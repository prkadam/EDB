#ifndef __NODEINFO_H__
#define __NODEINFO_H__
#include <string.h>
#include <cstring>
#include <semaphore.h>

const unsigned int MAX_WORD_LEN = 16;
const unsigned int MAX_WORD_COUNT = (1024 * 1024);

const unsigned int PRIME_NUM = 7979; //Using Prime number for calculating the hash

using namespace std;
/*Shared memory is divided into MAX_WORD_COUNT unit and each unit is represented by Node */
struct Node {
  char mWord[MAX_WORD_LEN];
  bool mIsUsed;
  Node() {
    mIsUsed = false;
  }
};

/* class MapMemory is used as Inmemory layout of array of Nodes to save the word.
 * */
class MapMemory {
  Node mNode[MAX_WORD_COUNT];
  sem_t mWrite;
  int mReaderCnt;
  pthread_mutex_t mMutex;

public:
  int init(bool is_present);

  int computeHash(const string &str);

  int getNextFreeNode(unsigned int id, const string &str);
  
  int insert(const string &str );

  int search (const string &str , bool is_internal=false);

  int remove(const string &str ) ;
};

#endif
