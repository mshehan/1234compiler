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

/*helper functions*/ 
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

/*search functions*/
symbol* find_table_ident(symbol_table* table, astree* node){
    symbol* sym = nullptr; 
    if(table->count(node->lexinfo)!= 0){
        sym = table->find(node->lexinfo); 
    }
    return sym; 
}
symbol* find_stack_ident(astree* node){
    symbol* sym = nullptr; 
    for(auto i = symbol_stack.begin(); i != symbol_stack.end(); ++i){
        if(i != nullptr && !i->empty()){
            if( (sym = find_table_ident(i,node)) != nullptr ){
                return sym; 
            }
        }  
    }
    return sym; 
}

/*add attribute functions*/
void inherit_parent_type(astree* parent){
    
    if(!parent->children.empty()){
        for(auto i = parent->children.begin(); i != parent->children.end(); ++i){
            for(size_t j = 0; i < ATTR_function; j++){
                if(parent->attributes[j] == 1){
                    i->attributes.set(j); 
                }   
            }
        }
    }
     
}

void inherit_child_type(astree* parent){
    if(!parent->children.empty()){
        for(auto i = parent->children.begin(); i != parent->children.end(); ++i){
            for(size_t j = 0; i < ATTR_function; j++){
                if(i->attribute[j] == 1){
                    parent->attributes.set(j); 
                }   
            }
        }
    }
}

void inherit_all(astree* parent, astree*child){
    for(size_t i = 0; i < ATTR_bitset_size; ++i){
        if(child->attributes[i] == 1){
            parent->attributes[i].set(i); 
        }
    }
} 


/*check type functions*/
bool check_primative_type(astree* left, astree* right){
    for(size_t i = 0; i < ATTR_function; ++i){
        if(left->attributes[i] != right->attributes[i]){
            return false; 
        }
    }
    return true; 
}


void check_type(FILE* out_file, astree* node){
    
    symbol* sym; 
    

    //we must switch to handle for different symbols
    switch(node->symbol){    
        
        TOK_VOID:
            node->attributes.set(ATTR_void);
            break;
        //integers and characters are treated the same way
        '=':
            break;
        '-':
            break;
        '*':
            break;
        '/':
            break;
        '%':
            break;
        '!':
            break; 
        TOK_CHAR:
        TOK_INT:
            node->attribues.set(ATTR_int);
            inherit_parent_typ(node); 
            break;
        TOK_STRING:
            node->attributes.set(ATTR_string);
            inherit_parent_type(node); 
            break;
        TOK_NEWSTRING:
            node->attributes.set(ATTR_string);
            node->attribuites.set(ATTR_vreg); 
            break;
        TOK_STRUCT:
            node->attribute.set(ATTR_struct); 
            insert_symbol(types,node);
            // we need to add a print function here
            sym = find_table_ident(types,node);
            sym->fields = new symbol_table; 
            for(auto i = node->children.begin(); i != children.end(); ++i){
               insert_symbol(sym->fields,*i);
               //print here  
            }
            break;
        TOK_NULL:
            node->attributes.set(ATTR_null); 
            node->attributes.set(ATTR_const); 
            break;
        
        TOK_NEW:
            inherit_child_type(node); 
            break;
        
        TOK_ARRAY:
            break;
        TOK_DECLID: 
            break;
        TOK_VARDECL: 
            node->children[0].attributes.set(ATTR_lval);
            node->children[1].attributes.set(ATTR_variable);
            inherit_all(node,node->children[0]);
            if( stack_find_ident(node->children[0]) != nullptr){
                //print an error message
                // because the identifier exists on the stack
            }
            define_ident(node->children[0]);
            // print necessary information 
            break;
        TOK_GT:
        TOK_GE:
        TOK_LE:
        TOK_NE:
        TOK_LT:
        TOK_EQ: 
            if(check_primitive(node->children[0],node->children[1])){
                node->attributes.set(ATTR_int); 
                node->attributes.set(ATTR_vreg); 
            }else{
                //print error
            }

            break; 
        TOK_IDENT:
            sym = stack_find_ident(node);
            if(sym == nullptr){
                sym = table_find_ident(node); 
            }
            if(sym == nullptr){
                //error message
            }else{
                node->attributes = symbol->attributes; 
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
            node->attribute.set(ATTR_typeid); 
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