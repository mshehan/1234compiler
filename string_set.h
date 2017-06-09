// $Id: string_set.h,v 1.1 2016-10-06 16:15:22-07 - - $
/*
Matthew Shehan
Assignment 2
5/2/17
mshehan@ucsc.edu
*/
#ifndef __STRING_SET__
#define __STRING_SET__

#include <string>
#include <unordered_set>
using namespace std;

#include <stdio.h>
#include "auxlib.h"

class string_set {
public:
   string_set();
   static unordered_set<string> set;
   static const string* intern (const char*);
   static void dump (FILE*);

};

#endif
