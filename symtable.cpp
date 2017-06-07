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
    if(sym_table != nullptr && node != nullptr){
        sym = create_symbol(node); 
        sym_table->insert(symbol_entry(node->lexinfo,sym));
    }
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




void check_type(FILE* out_file, astre* node){

    astree* = right;
    astree* = left;
    symbol* = sym; 
     if (node->children.size() > 0) {
        left = node->children[0];
    }
    if (node->children.size() > 1) {
        right = node->children[1];
    }
    switch(node->symbol){
        
        TOK_VOID:
            left->attribute.set(ATTR_void);
            break;

        TOK_INT:
            if(left == nullptr){
                left->attribute.set(ATTR_int);
            }
            inhert_type(node,left); 
             break;
        TOK_GT:
        TOK_GE:
        TOK_LE:
        TOK_LT:
        TOK_EQ: 
            if(check_primitive(left,right)){
                node->attribute.set(ATTR_int); 
            }else{
                //print error
            }

            break; 
        TOK_NULL:
            node->attribute.set(ATTR_null); 
            node->attribute.set(ATTR_const); 
            break;
        
        
        INTCON:
        CHARCON: 
            node->attribute.set(ATTR_int); 
            node->attribute.set(ATTR_const); 
            break; 
        STRINGCON:
            node->attribute.set(ATTR_string);
            node->attribute.set(ATTR_const); 

       


    }

}