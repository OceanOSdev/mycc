#ifndef CRUDE_SYMBOLS_LIST_H
#define CRUDE_SYMBOLS_LIST_H

#define LOCAL_VAR_STACK_SIZE 253 

/*
 * Stores info from parser on structs.
 */
typedef struct struct_decl_symbol {
    char* struct_name;
    int num_members;
    char* struct_members[253]; // keeping with the theme
} struct_decl_symbol_t;

/* 
 * Stores info from parser on function such as:
 * function name
 * list of parameters
 * list of local variables
 */ 
typedef struct func_decl_symbol {
    char* func_name;
    int num_params; // how many params are actually stored
    char* func_params[253]; // C standard
    int num_lvars; // how many local variables are actually stored
    char* func_local_vars[LOCAL_VAR_STACK_SIZE]; // Not from C standard 
    int num_structs;
    struct_decl_symbol_t* structs[253];
} func_decl_symbol_t;

/* 
 * Stores info from parser on function prototype such as:
 * function name
 * list of parameters
 */ 
typedef struct func_proto_symbol {
    char* func_name;
    int num_params; // how many params are actually stored
    char* func_params[253]; // C standard
} func_proto_symbol_t;

/* 
 * Stores info about where parsing errors happen
 * This is just in here so that the stuff that
 * parsed correctly gets shown before printing the error
 */
typedef struct parse_error_symbol { 
    char* filename;
    int line_number;
    char* text;
    char* msg;
} parse_error_symbol_t;

/* 
 * Union that allows me to make a 
 * (somewhat (i.e. not really)) type safe
 * list of the parsed symbols for printing
 */ 
typedef union symbol_data {
    func_decl_symbol_t* fds_val;
    func_proto_symbol_t* fps_val;
    parse_error_symbol_t* pes_val;
} symbol_data_t;

/* 
 * Just an enum that allows me to tag
 * each entry in the symbol parse list with
 * what kind it should be resolved to when
 * printing
 */ 
typedef enum symbol_type {
    FUNC_DECL_SYMBOL,
    FUNC_PROTO_SYMBOL,
    PARSE_ERROR_SYMBOL
} symbol_type_t;

/* 
 * Type that defines the parse data
 * to be printed
 */
typedef struct symbol_parse_list_node {
    symbol_type_t symbol_type;
    symbol_data_t symbol_data;

    struct symbol_parse_list_node* next;
} symbol_parse_list_node_t;

/* 
 * This allows us to keep track of the
 * global variables separately so that
 * when printing we can get all of the
 * global variables in one pass through
 */
typedef struct symbol_parse_list {
    symbol_parse_list_node_t* head;
    struct_decl_symbol_t** glob_structs;
    int num_glob_structs;
    char** global_variables;
    long num_glob_vars; 
} symbol_parse_list_t;

/*
 * Creates a new symbol parse list.
 */
symbol_parse_list_t* create_symbol_parse_list();

/*
 * Frees the memory used by the given symbol parse list.
 */
void unalloc_symbol_parse_list(symbol_parse_list_t* spl);

/*
 * Appends the list of global variable symbols
 * to the symbol parse list.
 */
void append_global_variables(symbol_parse_list_t* spl, int num_vars, char** vars);

/*
 * Creates and appends a new global struct symbol
 * to the symbol parse list.
 */
void append_global_struct(symbol_parse_list_t* spl, int num_structs, struct_decl_symbol_t** structs);

/*
 * Creates and appends a new function declaration symbol
 * to the symbol parse list.
 */
void append_func_decl(symbol_parse_list_t* spl, char* func_name, 
                      int num_params, char* func_params[], 
                      int num_lvars, char* func_local_vars[],
                      int num_structs, struct_decl_symbol_t* structs[]);

/*
 * Creates and appends a new function prototype symbol
 * to the symbol parse list.
 */
void append_func_proto(symbol_parse_list_t* spl, char* func_name, int num_params, char* func_params[]);

/*
 * Creates and appends a new parse error symbol
 * to the symbol parse list.
 */
void append_parse_error(symbol_parse_list_t* spl, char* filename, int line_number, char* text, const char* msg);


#endif