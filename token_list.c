#include "token_list.h"
#include <stdlib.h>

token_list_node_t* init_token_list() {
    token_list_node_t* head = malloc(sizeof(token_list_node_t));
    head->text = "HEAD";
    head->filename = "HEAD";
    return head;
}

void add_token(token_list_node_t* head, 
               char* filename, 
               int linenum, 
               char* text, 
               enum yytokentype token) {
    
    token_list_node_t* cur = head;
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = malloc(sizeof(token_list_node_t));
    cur = cur->next;
    cur->filename = filename;
    cur->lineNum = linenum;
    cur->text = text;
    cur->token = token;
    cur->isError = 0;
}

void add_token_err(token_list_node_t* head, 
                   char* errMsg, 
                   char* filename, 
                   int linenum, 
                   char* text) {

    token_list_node_t* cur = head;
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = malloc(sizeof(token_list_node_t));
    cur = cur->next;
    cur->filename = filename;
    cur->lineNum = linenum;
    cur->text = text;
    cur->isError = 1;
    cur->errDesc = errMsg;
}

void unalloc_token_list(token_list_node_t* head) {
    token_list_node_t* temp;
    while (head) {
        temp = head;
        head = head->next;
        if (temp->errDesc)
            free(temp->errDesc);
        free(temp->filename);
        free(temp->text);
        free(temp);
    }
}