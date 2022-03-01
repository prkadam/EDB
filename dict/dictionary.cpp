#include "dictionary.h"


int Dictionary::init()
{
  bool is_already_present = false;
  mSharedMem = new SharedMem(sizeof(MapMemory));
  char *ptr =  mSharedMem->getSharedMem(is_already_present);
  if(ptr == NULL) {
    return 0;
  }

  if(is_already_present) {
    mMap = (MapMemory*) ptr;
  } else {
    mMap = new(ptr) MapMemory();
  }
  mMap->init(is_already_present);

  return 1;
}

int Dictionary::insert(const string &str)
{
    return mMap->insert(str);
}

int Dictionary::search(const string &str)
{
    return mMap->search(str);
}

int Dictionary::remove(const string &str)
{
    return mMap->remove(str);
}
