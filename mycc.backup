%{
#include <stdio.h>
#include <string.h>
#include "crude_symbols_list.h"

int yylex();

void init_variable_stack();
void reset_variable_stack();
void append_variable_stack(char* format, char* val);

void init_param_stack();
void reset_param_stack();
void append_param_stack(char* format, char* val);

void init_svar_stack();
void reset_svar_stack();
void append_svar_stack(char* format, char* val);

void init_struct_stack();
void reset_struct_stack();
void append_struct_stack(char* struct_name, int num_members, char** struct_members);

extern int yylineno;
extern char* yytext;
extern symbol_parse_list_t* spl;
extern char* input_comp_file;

// vars to keep track of variables during parsing
const int variable_stack_size = 253;
char** variable_stack;
int vstack_index = 0;

// vars to keep track of struct variables during parsing
const int svar_stack_size = 253;
char** svar_stack;
int svstack_index = 0;

// vars to keep track of struct list during parsing
const int struct_stack_size = 100;
struct_decl_symbol_t** struct_stack;
int struct_stack_index = 0;

// var to keep track of function name
char* function_name;

// var to keep track of parameters during parsing
const int param_stack_size = 253;
char** param_stack;
int pstack_index = 0;

// var to keep track of struct name
char* struct_name;

void yyerror(const char* mesg) {
     append_parse_error(spl, input_comp_file, yylineno, yytext, mesg);
     //fprintf(stderr, "Error near %s line %d text '%s'\n\t%s\n", input_comp_file, yylineno, yytext, mesg);
}
/*%define parse.trace*/
%}

%union {
     char* sval;
}

%token <sval> IDENT
%token TYPE CONST STRUCT INTCONST REALCONST STRCONST CHARCONST
%token FOR WHILE DO IF ELSE BREAK CONTINUE RETURN

%token LPAR RPAR LBRACKET RBRACKET LBRACE RBRACE
%token DOT COMMA SEMI QUEST COLON

%token PLUS MINUS STAR SLASH MOD TILDE
%token PIPE AMP BANG DPIPE DAMP

%token ASSIGN PLUSASSIGN MINUSASSIGN STARASSIGN SLASHASSIGN INCR DECR
%token EQUALS NEQUAL GT GE LT LE


%nonassoc WITHOUT_ELSE
%nonassoc ELSE

%left COMMA
%left DOT
%right ASSIGN PLUSASSIGN MINUSASSIGN STARASSIGN SLASHASSIGN
%right QUEST COLON
%left DPIPE
%left DAMP
%left PIPE
%left AMP
%left EQUALS NEQUAL
%left LT LE GT GE
%left PLUS MINUS
%left STAR SLASH MOD
%right BANG TILDE UMINUS DECR INCR TYPE
%left LPAR RPAR LBRACKET RBRACKET

%%

prog : 
     | com_unit prog
     ;

com_unit : glob_var_decl                                              {append_global_variables(spl,vstack_index,variable_stack);reset_variable_stack();}
         | glob_struct_def                                            {append_global_struct(spl,struct_stack_index,struct_stack);reset_struct_stack();}
         | func_proto                                                 {append_func_proto(spl,function_name,pstack_index,param_stack);reset_variable_stack(); reset_param_stack();}
         | func_def                                                   {append_func_decl(spl,function_name,pstack_index,param_stack,vstack_index,variable_stack,struct_stack_index,struct_stack);reset_param_stack();reset_variable_stack();reset_svar_stack();reset_struct_stack();}
         ;

glob_var_decl : var_decl;

glob_struct_def : struct_def;

func_proto : func_decl SEMI;

func_decl : TYPE IDENT LPAR formal_param_list RPAR                    {function_name = $2;}
          | TYPE IDENT LPAR RPAR                                      {function_name = $2;}
          | CONST TYPE IDENT LPAR formal_param_list RPAR              {function_name = $3;}
          | CONST TYPE IDENT LPAR RPAR                                {function_name = $3;}
          | TYPE CONST IDENT LPAR formal_param_list RPAR              {function_name = $3;}
          | TYPE CONST IDENT LPAR RPAR                                {function_name = $3;}
          | STRUCT IDENT IDENT LPAR formal_param_list RPAR            {function_name = $3;}
          | STRUCT IDENT IDENT LPAR RPAR                              {function_name = $3;}
          | CONST STRUCT IDENT IDENT LPAR formal_param_list RPAR      {function_name = $4;}
          | CONST STRUCT IDENT IDENT LPAR RPAR                        {function_name = $4;}
          | STRUCT IDENT CONST IDENT LPAR formal_param_list RPAR      {function_name = $4;}
          | STRUCT IDENT CONST IDENT LPAR RPAR                        {function_name = $4;}
          ;

formal_param_list : formal_param
                  | formal_param COMMA formal_param_list
                  ;

formal_param : TYPE IDENT                                             {append_param_stack("%s",$2);}
             | TYPE IDENT LBRACKET RBRACKET                           {append_param_stack("%s[]",$2);}
             | CONST TYPE IDENT                                       {append_param_stack("%s",$3);}
             | CONST TYPE IDENT LBRACKET RBRACKET                     {append_param_stack("%s[]",$3);}
             | TYPE CONST IDENT                                       {append_param_stack("%s",$3);}
             | TYPE CONST IDENT LBRACKET RBRACKET                     {append_param_stack("%s[]",$3);}
             | STRUCT IDENT IDENT                                     {append_param_stack("%s", $3);}
             | STRUCT IDENT IDENT LBRACKET RBRACKET                   {append_param_stack("%s[]", $3);}
             | CONST STRUCT IDENT IDENT                               {append_param_stack("%s", $4);}
             | CONST STRUCT IDENT IDENT LBRACKET RBRACKET             {append_param_stack("%s[]", $4);}
             | STRUCT IDENT CONST IDENT                               {append_param_stack("%s", $4);}
             | STRUCT IDENT CONST IDENT LBRACKET RBRACKET             {append_param_stack("%s", $4);}
             ;

func_def : func_decl LBRACE var_struct_decl_list stmt_list RBRACE            
         ;

struct_def : STRUCT IDENT LBRACE st_var_decl_list_na RBRACE SEMI      {append_struct_stack($2,svstack_index,svar_stack);reset_svar_stack();/*printf("FINISHED PARSING STRUCT [%s]\n",$2);*/}
           ;

stmts : stmt
      | stmt_block
      ;

stmt_block : LBRACE stmt_list RBRACE;

var_struct_decl_list :
                     | var_decl var_struct_decl_list
                     | struct_def var_struct_decl_list                
                     ;

st_var_decl_list_na : 
                    | st_var_decl_na st_var_decl_list_na
                    ;

st_var_decl_na : TYPE ident_list_na SEMI                              
               | CONST TYPE ident_list_na SEMI
               | TYPE CONST ident_list_na SEMI
               | STRUCT IDENT ident_list_na SEMI
               | CONST STRUCT IDENT ident_list_na SEMI
               | STRUCT IDENT CONST ident_list_na SEMI
               ; 

ident_list_na : ident_c
              | ident_c COMMA ident_list_na
              ;

ident_c : IDENT                                                       {append_svar_stack("%s",$1);}
        | IDENT LBRACKET INTCONST RBRACKET                            {append_svar_stack("%s[]",$1);}
        ;

stmt_list : 
          | stmt stmt_list
          ;

stmt : DO stmts WHILE LPAR expr RPAR SEMI
     | WHILE LPAR expr RPAR stmts
     | FOR LPAR opt_expr SEMI opt_expr SEMI opt_expr RPAR stmts
     | IF LPAR expr RPAR stmts %prec WITHOUT_ELSE
     | IF LPAR expr RPAR stmts ELSE stmts
     | RETURN opt_expr SEMI
     | BREAK SEMI
     | CONTINUE SEMI
     | expr SEMI
     | SEMI
     ;

opt_expr : 
         | expr
         ;

expr_list : 
          | nonzero_expr_list
          ;

nonzero_expr_list : expr
                  | nonzero_expr_list COMMA expr
                  ;

expr : LPAR expr RPAR
     | LPAR TYPE RPAR expr
     | expr QUEST expr COLON expr
     | bin_expr
     | un_expr
     | INCR l_val
     | DECR l_val
     | l_val INCR
     | l_val DECR
     | l_val assign_op expr %prec ASSIGN
     | l_val
     | IDENT LPAR expr_list RPAR
     | literal_val
     ;

l_val : IDENT
      | IDENT LBRACKET expr RBRACKET
      | l_val DOT l_val
      ;

un_expr : MINUS expr %prec UMINUS
        | BANG expr
        | TILDE expr
        ;

bin_expr : expr DPIPE expr
         | expr DAMP expr
         | expr PIPE expr
         | expr AMP expr
         | expr EQUALS expr
         | expr NEQUAL expr
         | expr GT expr
         | expr GE expr
         | expr LT expr
         | expr LE expr
         | expr PLUS expr
         | expr MINUS expr
         | expr STAR expr
         | expr SLASH expr
         | expr MOD expr
         ;

assign_op : ASSIGN
          | PLUSASSIGN
          | MINUSASSIGN
          | STARASSIGN
          | SLASHASSIGN
          ;

literal_val : INTCONST
            | REALCONST
            | STRCONST
            | CHARCONST
            ;

var_decl : TYPE ident_list SEMI                                       
         | CONST TYPE ident_list SEMI
         | TYPE CONST ident_list SEMI
         | STRUCT IDENT ident_list SEMI
         | CONST STRUCT IDENT ident_list SEMI
         | STRUCT IDENT CONST ident_list SEMI
         ; 

ident_list : ident_ca
           | ident_ca COMMA ident_list
           ;

ident_ca : IDENT ASSIGN expr                                          {append_variable_stack("%s",$1);}
         | IDENT LBRACKET INTCONST RBRACKET ASSIGN expr               {append_variable_stack("%s[]",$1);}
         | IDENT                                                      {append_variable_stack("%s",$1);}
         | IDENT LBRACKET INTCONST RBRACKET                           {append_variable_stack("%s[]",$1);}
         ;

%%
void init_variable_stack() {
     variable_stack = malloc(variable_stack_size * sizeof(char*));
     vstack_index = 0;
     for (int i = 0; i < variable_stack_size; i ++) {
          variable_stack[i] = NULL;
     }
}
void reset_variable_stack() {
     vstack_index = 0;
     for (int i = 0; i < variable_stack_size; i ++) {
          variable_stack[i] = NULL;
     }
}

void init_param_stack() {
     param_stack = malloc(param_stack_size * sizeof(char*));
     pstack_index = 0;
     for (int i = 0; i < param_stack_size; i ++) {
          param_stack[i] = NULL;
     }
}
void reset_param_stack() {
     pstack_index = 0;
     for (int i = 0; i < param_stack_size; i ++) {
          param_stack[i] = NULL;
     }
}

void init_svar_stack() {
     svar_stack = malloc(svar_stack_size * sizeof(char*));
     svstack_index = 0;
     for (int i = 0; i< svar_stack_size; i ++) {
          svar_stack[i] = NULL;
     }
}
void reset_svar_stack() {
     svstack_index = 0;
     for (int i = 0; i< svar_stack_size; i ++) {
          svar_stack[i] = NULL;
     }
}

void init_struct_stack() {
     struct_stack = malloc(struct_stack_size*sizeof(struct_decl_symbol_t*));
     struct_stack_index = 0;
     for (int i = 0; i < struct_stack_size; i ++) 
        struct_stack[i] = malloc(sizeof(struct_decl_symbol_t));
}
void reset_struct_stack() {
     struct_stack_index = 0;
     for (int i = 0; i < struct_stack_size; i ++) 
        struct_stack[i] = NULL;
}

void append_variable_stack(char* format, char* val) {
     if (vstack_index < variable_stack_size) {
          variable_stack[vstack_index] = malloc((strlen(format) + strlen(val))*sizeof(char));
          sprintf(variable_stack[vstack_index++], format, val);
     } else {
          fprintf(stderr, "stack overflow in variable stack\n");
     }
}
void append_param_stack(char* format, char* val) {
     if (pstack_index < param_stack_size) {
          param_stack[pstack_index] = malloc((strlen(format) + strlen(val))*sizeof(char));
          sprintf(param_stack[pstack_index++], format, val);
     } else {
          fprintf(stderr, "stack overflow in parameter stack\n");
     }
}
void append_svar_stack(char* format, char* val) {
     if (svstack_index < svar_stack_size) {
          svar_stack[svstack_index] = malloc((strlen(format) + strlen(val))*sizeof(char));
          sprintf(svar_stack[svstack_index++], format, val);
     } else {
          fprintf(stderr, "stack overflow in struct variable stack\n");
     }
}
void append_struct_stack(char* struct_name, int num_members, char** struct_members) {
     if (struct_stack_index < struct_stack_size) {
          struct_decl_symbol_t* temp = malloc(sizeof(struct_decl_symbol_t));
          temp->struct_name = struct_name;
          temp->num_members = num_members;
          for (int i = 0; i < num_members; i ++)
               temp->struct_members[i] = struct_members[i];
          
          struct_stack[struct_stack_index++] = temp;
     } else {
          fprintf(stderr, "stack overflow in struct stack\n");
     }
}