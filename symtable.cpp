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

/*symbol stack functions */ 
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

symbol* find_ident(astree* node){
    symbol* sym = nullptr
    if(!symbol_stack.empty()){
        
        for(auto table = symbol_stack.begin(); table != symbol_stack.end(); ++table){
            if(*table != nullptr ){
                if(table->count(node->lexinfo) > 0 ){
                    sym = table->find(node->lexinfo)->second; 
                }
            }
        }
    }
    return sym; 
}
/**/ 
bool check_primative_type(astree* left, astree* right){
    return left->attributes == right->attributes; 
}
void inherit_child_type(astree* parent, astree* child){
    for(auto i = 0; i < ATTR_function; i++){
        if(child->attributes[i] == 1){
            parent->attributes.set(i); 
        }
    } 
}


void check_type(FILE* out_file, astre* node){

    astree* right;
    astree* left;
    symbol* sym; 
     if (node->children.size() > 0) {
        left = node->children[0];
    }
    if (node->children.size() > 1) {
        right = node->children[1];
    }

    //we must switch to handle for different symbols
    switch(node->symbol){    
        TOK_VOID:
            left->attributes.set(ATTR_void);
            break;
        //integers and characters are treated the same way
        '=':
        
        TOK_CHAR:
        TOK_INT:
            if(left != nullptr){
                left->attributes.set(ATTR_int);
                inherit_child_type(node,left); 
            }
             break;
        TOK_STRING:
            if(left != nullptr){
                left->attributes.set(ATTR_string);
                inherit_child_type(node,left); 
            }
            break;
        TOK_NEWSTRING:
            node->attributes.set(ATTR_string);
            node->attribuites.set(ATTR_vreg); 
            break;
        TOK_STRUCT:
            left->attributes.set(ATTR_struct);
            insert_symbol(types,left);
            symbol* sym = find_ident(left);
            if(sym == nullptr){
                sym->fields = new symbol_table;
                for(auto iter = node->children.begin()+1; iter != node->children.end(); ++iter){
                    sym->fields.insert_symbol(*iter);
                }
            } 
            break;
        TOK_NULL:
            node->attributes.set(ATTR_null); 
            node->attributes.set(ATTR_const); 
            break;
        TOK_NEW:
            inherit_child_type(node,left);
            break;
        
        TOK_ARRAY:
            
            break;
        TOK_DECLID:
            break;
        TOK_VARDECL: 
            break;
        TOK_GT:
        TOK_GE:
        TOK_LE:
        TOK_NE:
        TOK_LT:
        TOK_EQ: 
            if(check_primitive(left,right)){
                node->attributes.set(ATTR_int); 
                node->attributes.set(ATTR_vreg); 
            }else{
                //print error
            }

            break; 
        TOK_IDENT:
            
            symbol* sym = find_ident(node);
            if(sym == null){
                insert_symbol(types,node); 
            } 
            break;
        INTCON:
        CHARCON: 
            node->attributes.set(ATTR_int); 
            node->attributes.set(ATTR_const); 
            break; 
        TOK_STRINGCON:
            node->attributes.set(ATTR_string);
            node->attributes.set(ATTR_const); 
            break;  
        TOK_IF:
        TOK_IFELSE:
        TOK_WHILE:
            if(!left.attributes[ATTR_int]){
                //print an error 
            }
            break;
        TOK_RETURN:
            
            //???????
            break;  
        TOK_ELSE:
            break; 
        TOK_BLOCK: 
            break;
        TOK_CALL: 
            break;
        TOK_INITDECL:
            break;
        TOK_RETURNVOID:
            break;
        TOK_INDEX:
            break;
        TOK_POS:
            break;
        TOK_NEG:
            break;
        TOK_NEWARRAY:
            node->attributes.set(ATTR_array);
            node->attributes.set(ATTR_vreg);
            inherit_child_type(node,left); 
            break;
        TOK_TYPEID:
            break;
        TOK_FIELD:
            break;
        TOK_ORD:
            break;
        TOK_CHR:
            break;
        TOK_ROOT: 
            break;
        TOK_FUNCTION:
            break;
        TOK_PARAMLIST: 
            break;
        TOK_PROTOTYPE: 
            break; 
        
        


    }

}