#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"

using namespace std;

void usage()
{
     fprintf(stderr, "\nUsage: ./dict {insert|search|delete} <word>\n");
     fprintf(stderr, "insert : insert word in dictionary\n");
     fprintf(stderr, "delete : delete word in dictionary\n");
     fprintf(stderr, "search : search word in dictionary\n");
}

int main(int argc , char* argv[])
{
  string INSERT="insert";
  string SEARCH="search";
  string DELETE="delete";
  int opt;
  if(argc != 3) {
    usage();
    return 0;
  }

  Dictionary dict;
  dict.init();
  if(INSERT == argv[1]) {
    dict.insert(argv[2]);
  } else if(SEARCH == argv[1]) {
    dict.search(argv[2]);
  } else if(DELETE == argv[1]) {
    dict.remove(argv[2]);
  }

  return 0;
}
