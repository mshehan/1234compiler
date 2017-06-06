#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <bitset>

#include "lyutils.h"
#include "astree.h"
#include "symtable.h"

extern FILE* symfile;
extern vector<symbol_table*> symbol_stack;
extern symbol_table* types;


vector<symbol_table*> symbol_stack = new vector<symbol_table*>;
symbol_table* types = new symbol_table;
symbol_stack.push_back (new symbol_table);

symbol* create_symbol(astree* node){
    symbol* sym = new symbol();
    sym->attributes = node->attributes;
    sym->filenr = node->lloc.filenr;
    sym->linenr = node->lloc.linenr;
    sym->offset = node->lloc.offset;
    sym->block_nr = node->blocknum;

    sym->fields = nullptr;
    sym->parameters = nullptr; 

    return sym; 
}

insert_symbol(symbol_table* sym_table, astree* node){

    sym_table->insert(symbol_entry(node->lexinfo,node->symbol))
}

void enter_block(){
    next_block++;
    symbol_stack.push_back(nullptr);  
}

void exit_block(){
    next_block--;
    symbol_stack.pop_back(); 
}

void define_ident(*astree node){
    if(symbol_stack.back() == nullptr){
        symbol_stack.back() = new symbol_table; 
    }
    insert_symbol(symbol_table.back(), node); 
}