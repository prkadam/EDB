#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__
#include "sharedmem.h"
#include "memmap.h"

using namespace std;

class Dictionary {

  SharedMem *mSharedMem;
  MapMemory *mMap;
public:
  Dictionary() {
    mSharedMem = NULL;
    mMap = NULL;
  }
  
  ~Dictionary() {
    delete mSharedMem;
  }
  int init();

  int insert(const string& str);
  
  int search(const string& str);

  int remove(const string &str);
};
#endif
