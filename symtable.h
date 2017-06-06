
#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include <string>
#include <vector>
#include <bitset>

#include "auxlib.h"
#include "lyutils.h"
#include "astree.h"

using namespace std;

//global counter 
extern size_t next_block;

struct symbol {
	attr_bitset attributes;
	symbol_table* fields;
	size_t filenr, linenr, offset;
	size_t block_nr;
	vector<symbol*>* parameters;
}

using symbol_entry = symbol_table::valuetype;