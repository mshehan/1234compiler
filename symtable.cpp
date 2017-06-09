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

string get_attributes(attr_bitset attr) {
    vector<string> attributes; 
    string formatted = "";

    if(attr[ATTR_void] == 1)attributes.push_back('void');
    if(attr[ATTR_int] == 1)attributes.push_back('int');
    if(attr[ATTR_null] == 1)attributes.push_back('null');
    if(attr[ATTR_string] == 1)attributes.push_back('string');
    if(attr[ATTR_struct] == 1)attributes.push_back('struct');
    if(attr[ATTR_array] == 1)attributes.push_back('array');
    if(attr[ATTR_function] == 1)attributes.push_back('function');
    if(attr[ATTR_variable] == 1)attributes.push_back('variable');
    if(attr[ATTR_field] == 1)attributes.push_back('field');
    if(attr[ATTR_typeid] == 1)attributes.push_back('typeid');
    if(attr[ATTR_param] == 1)attributes.push_back('param');
    if(attr[ATTR_lval] == 1)attributes.push_back('lval');
    if(attr[ATTR_const] == 1)attributes.push_back('const');
    if(attr[ATTR_vreg] == 1)attributes.push_back('vreg');
    if(attr[ATTR_vaddr] == 1)attributes.push_back('vaddr');

    for(auto i = attributes.begin(); i != attributes.end(); ++i){
        if(i != attributes.end()-1 ){
            formatted += *i + " ";
        } else{
            formatted += *i;
        }
    } 
    return formatted; 
}
astree* current = nullptr;

void print_newsymbol(FILE* out, astree* node){
    auto var_name = node->lexinfo->c_str(); 
    auto line_num = node->linenr; 
    auto file_num = node->filenr;
    auto offset = node->offset;
    auto block_num = node->blocknum;
    auto attributes = (get_attributes(node->attributes)).c_str(); 
    if(node->attributes[ATTR_struct]){
        fprintf(out, "\n");
    }else{
        fprintf(out,"\t"); 
    }
    
    if(node->attributes[ATTR_struct]){
        current = node; 
        fprintf(out,"%s (%zu.%zu.%zu) {%zu} struct \"%s\" ",
            var_name, file_num, line_num, offset, 
            block_num, current->lexinfo); 
    }
    if(node->attributes[ATTR_field]){
        fprintf(out,"%s (%zu.%zu.%zu) field {%s} ", 
        var_name, file_num, line_num, offset, current->lexinfo);
    }

    fprintf (outfile, "%s\n", get_attributes(node->attributes));
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

void inherit_child_type(astree* parent, astree* child) {
    for(size_t i = 0; i < ATTR_function; i++) {
         if (child->attribute[i] == 1) {
             parent->attributes.set(i); 
        }   
     }

}
void inherit_children_type(astree* parent){
    if(!parent->children.empty()){
        for(auto i = parent->children.begin(); i != parent->children.end(); ++i){
            for(size_t j = 0; j < ATTR_function; j++){
                if(i->attributes[j] == 1){
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

bool check_base_type(astree* node) {
        if(node->attributes[ATTR_int] || node->attributes[ATTR_struct]) {
            return false; 
        }
    return true;
}

void check_block() {
    if (node->symbol == TOK_BLOCK) {
        enter_block();
    }

    node->block_nr = next_block;

    for (auto child = node->children.begin(); 
        child != node->children.end(); ++child) {
        check_block(child);
    }
}

void handle_function(FILE* out, astree* node) {
    node->attributes.set(ATTR_function);

    /*which part of the symbol stack?*/
    insert_symbol(symbol_stack[0], node->children[0]->children[0]);
    
    auto param = node->children[1];

    for (auto c = param.begin(); i != param.end(); ++param) {
        c->children[0]->attributes.set(ATTR_variable);
        c->children[0]->attributes.set(ATTR_lval);
        c->children[0]->attributes.set(ATTR_param);
    }

}
void check_type(FILE* out_file, astree* node){
    
    symbol* sym; 
    

    //we must switch to handle for different symbols
    switch(node->symbol){    
        
        TOK_VOID:
            node->attributes.set(ATTR_void);
            break;
        //integers and characters are treated the same way
        '+':
        '-':
            node->attributes.set(ATTR_int);
            node->attributes.set(ATTR_vreg);

            if (node->children.size() == 1) {
                if (!node->children[0].attributes[ATTR_int]) {
                    //print error message
                }
            }

            if (node->children.size() == 2) {
                if (!node->children[0].attributes[ATTR_int] ||
                    !node->children[1].attributes[ATTR_int]) {
                    //print an error message
                }
            }
            break;  
        '*':
        '/':
        '%':
            node->attributes.set(ATTR_int);
            node->attributes.set(ATTR_vreg);

            if (node->children.size() == 2) {
                if (!node->children[0].attributes[ATTR_int] ||
                    !node->children[1].attributes[ATTR_int]) {
                    //print an error message
                }
            }
            break;
        '!':
            node->attributes.set(ATTR_int);
            node->attributes.set(ATTR_vreg);

            if (node->children.size() == 1) {
                if (!node->children[0].attributes[ATTR_int]) {
                    //print error message
                }
            }
            break;
        '=':
            if (node->children.size() == 2) {
                if (node.children[0].attributes[ATTR_lval] && 
                    node.children[1].attributes[ATTR_vreg]) {
                    inherit_child_type(node, node->children[0]);
                    node->attributes.set(ATTR_vreg);
                }
            }

            break;
        TOK_CHAR:
        TOK_INT:
            node->attributes.set(ATTR_int);
            inherit_parent_type(node); 
            break;
        TOK_STRING:
            node->attributes.set(ATTR_string);
            inherit_parent_type(node); 
            break;
        TOK_NEWSTRING:
            node->attributes.set(ATTR_string);
            node->attributes.set(ATTR_vreg); 
            break;
        TOK_STRUCT:
            node->attributes.set(ATTR_struct); 
            insert_symbol(types,node);
            print_newsymbol(symfile, node);
            sym = find_table_ident(types,node);
            sym->fields = new symbol_table; 
            for(auto i = node->children.begin(); i != children.end(); ++i){
               insert_symbol(sym->fields,*i);
               print_newsymbol(symfile, node);
            }
            break;
        TOK_NULL:
            node->attributes.set(ATTR_null); 
            node->attributes.set(ATTR_const); 
            break;
        
        TOK_NEW:
            inherit_children_type(node); 
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
            if(check_primitive_type(node->children[0],node->children[1])){
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
            if(!node->attributes[ATTR_int]){
                //print an error 
            }
            break;
        TOK_RETURN:
            inherit_child_type(node, node->children[0]);
            break;  
        TOK_RETURNVOID:
            node->attributes.set(ATTR_void);
            break;
        TOK_NEWARRAY:
            node->attributes.set(ATTR_vreg);
            
            if (node->children[0].attributes[ATTR_string]) {
                node->attributes.set(ATTR_int); 
                node->attributes.set(ATTR_string);
            }  

            if (check_base_type(node->children[0])) {
                 inherit_child_type(node, node->children[0]);
                 node->attributes.set(ATTR_array);
            }
            break;
        TOK_TYPEID:
            node->attributes.set(ATTR_typeid); 
            break;
        TOK_BLOCK: 
            check_block(node);
            exit_block();
            break;
        TOK_INDEX:
            node->attributes.set(ATTR_lval);
            node->attributes.set(ATTR_vaddr);

            if (node->children[0].attributes[ATTR_string]) {
                node->attributes.set(ATTR_int); 
                node->attributes.set(ATTR_string);
            }  
            break;
        TOK_POS:
            break;
        TOK_NEG:
            break;
        TOK_CALL: 
            break;
        TOK_ARRAY:
            node->attributes.set(ATTR_vaddr);
            node->attributes.set(ATTR_lval);
            break;
        TOK_DECLID: 
            break;
        TOK_FIELD:
            break;
        TOK_FUNCTION:
            break;
        TOK_PARAMLIST: 
            break;
        TOK_PROTOTYPE: 
            break; 
        
    }

}