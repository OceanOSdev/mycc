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

extern int yylineno;
extern char* yytext;
extern symbol_parse_list_t* spl;
char* filename = "BLANK";

// vars to keep track of variables during parsing
const int variable_stack_size = 253;
char** variable_stack;
int vstack_index = 0;

// var to keep track of function name
char* function_name;

// var to keep track of parameters during parsing
const int param_stack_size = 253;
char** param_stack;
int pstack_index = 0;

void yyerror(const char* mesg) {
     append_parse_error(spl, filename, yylineno, yytext, mesg);
     //fprintf(stderr, "Error near %s line %d text '%s'\n\t%s\n", filename, yylineno, yytext, mesg);
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

com_unit : glob_var_decl                                              {append_global_variables(spl, vstack_index,variable_stack); reset_variable_stack();}
         | func_proto                                                 {append_func_proto(spl,function_name,pstack_index,param_stack);reset_variable_stack(); reset_param_stack();}
         | func_def                                                   {append_func_decl(spl,function_name,pstack_index,param_stack,vstack_index,variable_stack);reset_param_stack();reset_variable_stack();}
         ;

glob_var_decl : var_decl;

func_proto : func_decl SEMI;

func_decl : TYPE IDENT LPAR formal_param_list RPAR                    {function_name = $2;}
          | TYPE IDENT LPAR RPAR                                      {function_name = $2; /*printf("function : %s()\n",$2);*/}
          ;

formal_param_list : formal_param
                  | formal_param COMMA formal_param_list
                  ;

formal_param : TYPE IDENT                                             {append_param_stack("%s",$2);}
             | TYPE IDENT "[]"                                        {append_param_stack("%s[]",$2);}
             ;

func_def : func_decl LBRACE var_decl_list stmt_list RBRACE            
         ;

stmts : stmt
      | stmt_block
      ;

stmt_block : LBRACE stmt_list RBRACE;

var_decl_list : 
              | var_decl var_decl_list
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

var_decl : TYPE ident_list SEMI                                       {/*printf(" Done with that list.\n");*/}
         ;

ident_list : ident_c
           | ident_c COMMA ident_list
           ;

ident_c : IDENT                                                       {append_variable_stack("%s",$1); /*printf("var %s,", $1);*/}
        | IDENT LBRACKET INTCONST RBRACKET                            {append_variable_stack("%s[]",$1);/*printf("var %s[],", $1);*/}
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