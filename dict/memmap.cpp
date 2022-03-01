#include <iostream>
#include <string.h>
#include <cstring>

#include "memmap.h"

using namespace std;

int MapMemory::init(bool is_present) 
{

  sem_init(&mWrite, 1, 1);
  mReaderCnt=0;
  pthread_mutex_init(&mMutex, NULL);

  if(is_present == false) {
    for(int i=0; i<MAX_WORD_COUNT; i++) {
      mNode[i].mIsUsed = false;
    }
  }
  return 1;
}

int MapMemory::computeHash(const string &str) 
{
  /*This function PRIME number to calculate the hash and return the index in the range of 0-MAX_WORD_COUNT (1000000)
 */
  int hash = 0;
  for(int i=0; i<str.length(); i++) {
    hash = hash + (str[i] * PRIME_NUM);
  }
  return ( hash % MAX_WORD_COUNT);
}

int MapMemory:: getNextFreeNode(unsigned int id, const string &str) 
{
  int i = id;
  /*Here Collision is handle by searching next free node. 
 * This will have impactin search and delete as it need to check all the nodes
 * This can be improved by using other collision resolution mechanism or
 * Dividing the memory into smaller parts so search can be limited so one small part.
 * */
  while(( i < MAX_WORD_COUNT) && (mNode[i].mIsUsed == true)) {
    i++;
  }
  if(i == MAX_WORD_COUNT) {
    i=0;
    while(( i < id) && (mNode[i].mIsUsed == true)) {
      i++;
    }
    if(i == id) {
       cout << "Dictionary is full. no node is free." << endl;
      return -1;
    }
  }
  return i;
}

int MapMemory::insert(const string &str ) 
{
  int hash_idx = computeHash(str);
  
  if(search(str, true) == 1) {
    cout << "Word is already present" << endl;
    return 1;
  }
  // If word not already present
  sem_wait(&mWrite);
  /*get next free node from index (including) */
  int idx = getNextFreeNode(hash_idx, str);
  if(idx == -1) {
    cout << "No node is free."<< endl;
    return -1;
  }
  /*save the word */
  strncpy(mNode[idx].mWord, str.c_str(), MAX_WORD_LEN);
  mNode[idx].mIsUsed = true;
  
  sem_post(&mWrite);
  return 1;
}

int MapMemory::search (const string &str , bool is_internal) 
{
  pthread_mutex_lock(&mMutex);

  mReaderCnt++;
  if(mReaderCnt == 1) {
    sem_wait(&mWrite); //Block writer 
    
  }
  pthread_mutex_unlock(&mMutex);
  
  int hash_id = computeHash(str);
  int i = hash_id;
  while(i < MAX_WORD_COUNT)  {
    if((mNode[i].mIsUsed == true) &&  (str == mNode[i].mWord)) {
      if(is_internal == false)
        cout<< "Match found for word: " << str << endl;
      return 1;
    }
    i++;
  }
  i=0;
  while(i < hash_id) {
    if((mNode[i].mIsUsed == true) &&  (str == mNode[i].mWord)) {
      if(is_internal == false)
        cout<< "Match found for word: " << str << endl;
      return 1;
    }
    i++;
  }
  if(is_internal == false)
    cout << "Match is not found for word : " << str << endl;

  pthread_mutex_lock(&mMutex);
  mReaderCnt--;
  if(mReaderCnt == 0) {
    sem_post(&mWrite); //Wakeup writer
  }
  pthread_mutex_unlock(&mMutex);
  return 0;
}

int MapMemory::remove(const string &str ) 
{
  int hash_idx = computeHash(str);
  sem_wait(&mWrite);
  int i = hash_idx;
  while(i < MAX_WORD_COUNT)  {
    if((mNode[i].mIsUsed == true) &&  (str == mNode[i].mWord)) {
      mNode[i].mIsUsed = false;
      cout << "Word removed from dictionary." << endl;
      return 1;
    }
    i++;
  }
  i=0;
  while(i < hash_idx) {
    if((mNode[i].mIsUsed == true) &&  (str == mNode[i].mWord)) {
      mNode[i].mIsUsed = false;
      cout << "Word removed from dictionary." << endl;
      return 1;
    }
    i++;
  }
  sem_post(&mWrite);
  cout << "Word not found in dictionary" << endl;
  return 1;
}
