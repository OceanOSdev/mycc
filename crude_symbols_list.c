#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crude_symbols_list.h"

#define DEFAULT_GLOB_VAR_STACK_SIZE 100

/*
 * Inserts a new symbol parse list node into the 
 * symbol parse list and returns a pointer to it.
 */
symbol_parse_list_node_t* insert_blank_parse_list_node(symbol_parse_list_t* spl) {
    if (spl->head == NULL) {
        spl->head = malloc(sizeof(symbol_parse_list_node_t));
        return spl->head;
    }
    symbol_parse_list_node_t* iter = spl->head;
    while (iter->next) iter = iter->next;
    iter->next = malloc(sizeof(symbol_parse_list_node_t));
    iter = iter->next;
    return iter;
}


/*
 * Creates a new symbol parse list.
 */
symbol_parse_list_t* create_symbol_parse_list() {
    symbol_parse_list_t* spl = malloc(sizeof(*spl));
    spl->global_variables = malloc(DEFAULT_GLOB_VAR_STACK_SIZE * sizeof(char*));
    for (int i = 0; i < DEFAULT_GLOB_VAR_STACK_SIZE; i ++)
        spl->global_variables[i] = NULL;
    spl->num_glob_vars = 0;
    return spl;
}

/*
 * Frees the memory used by the given symbol parse list.
 */
void unalloc_symbol_parse_list(symbol_parse_list_t* spl) {

}

/*
 * Appends the list of global variable symbols
 * to the symbol parse list.
 */
void append_global_variables(symbol_parse_list_t* spl, int num_vars, char** vars) {
    // For now, only add up to the default stack size
    long new_count = num_vars + spl->num_glob_vars;
    if (new_count > 100) new_count = 100;
    for (long i = 0; spl->num_glob_vars + i < new_count; i++)
        spl->global_variables[spl->num_glob_vars + i] = vars[i];
    spl->num_glob_vars = new_count;
}

/*
 * Creates and appends a new function declaration symbol
 * to the symbol parse list.
 */
void append_func_decl(symbol_parse_list_t* spl, char* func_name, int num_params, char* func_params[], int num_lvars, char* func_local_vars[]) {
    symbol_parse_list_node_t* node = insert_blank_parse_list_node(spl);
    node->symbol_type = FUNC_DECL_SYMBOL;
    node->symbol_data.fds_val = malloc(sizeof(func_decl_symbol_t));

    node->symbol_data.fds_val->func_name = func_name;
    
    for (int p_index = 0; p_index < 253; p_index++) {
        node->symbol_data.fds_val->func_params[p_index] = 
            p_index < num_params ? func_params[p_index] : NULL;
    }
    
    for (int v_index = 0; v_index < LOCAL_VAR_STACK_SIZE; v_index++) {
        node->symbol_data.fds_val->func_local_vars[v_index] = 
            v_index < num_lvars ? func_local_vars[v_index] : NULL;
    }
}

/*
 * Creates and appends a new function prototype symbol
 * to the symbol parse list.
 */
void append_func_proto(symbol_parse_list_t* spl, char* func_name, int num_params, char* func_params[]) {
    symbol_parse_list_node_t* node = insert_blank_parse_list_node(spl);
    node->symbol_type = FUNC_PROTO_SYMBOL;
    node->symbol_data.fps_val = malloc(sizeof(func_proto_symbol_t));

    node->symbol_data.fps_val->func_name = func_name;
    
    for (int p_index = 0; p_index < 253; p_index++) {
        node->symbol_data.fps_val->func_params[p_index] = 
            p_index < num_params ? func_params[p_index] : NULL;
    }
}

/*
 * Creates and appends a new parse error symbol
 * to the symbol parse list.
 */
void append_parse_error(symbol_parse_list_t* spl, char* filename, int line_number, char* text, char* msg) {
    symbol_parse_list_node_t* node = insert_blank_parse_list_node(spl);
    node->symbol_type = PARSE_ERROR_SYMBOL;
    node->symbol_data.pes_val = malloc(sizeof(parse_error_symbol_t));
}
