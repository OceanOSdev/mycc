#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H
#include "mycc.tab.h"

typedef struct token_list_node {
    char* filename;
    int lineNum;
    char* text;
    enum yytokentype token;
    int isError;
    char* errDesc;
    
    struct token_list_node* next;

} token_list_node_t;

token_list_node_t* init_token_list();

void add_token(token_list_node_t* head, 
               char* filename, 
               int linenum, 
               char* text, 
               enum yytokentype token);

void add_token_err(token_list_node_t* head, 
                   char* errMsg, 
                   char* filename, 
                   int linenum, 
                   char* text);

void unalloc_token_list(token_list_node_t* head);

#endif