%{
#include <stdio.h>

int yylex();

extern int yylineno;
extern char* yytext;

char* filename = "BLANK";

void yyerror(const char* mesg) {
    fprintf(stderr, "Error near %s line %d text '%s'\n\t%s\n", filename, yylineno, yytext, mesg);
}
/*%define parse.trace*/
%}



%token TYPE CONST STRUCT IDENT INTCONST REALCONST STRCONST CHARCONST
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
     | glob_var_decl 
     | func_proto
     | func_def {printf("bruhhhhh\n");}
     ;

glob_var_decl : var_decl;

func_proto : func_decl SEMI;

func_decl : TYPE IDENT LPAR formal_param_list RPAR;

formal_param_list : formal_param
                  | formal_param COMMA formal_param_list
                  ;

formal_param : TYPE IDENT
             | TYPE IDENT "[]"
             ;

func_def : func_decl LBRACE var_decl_list stmt_list RBRACE;

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
     | expr bin_op expr
     | un_op expr
     | INCR l_val
     | DECR l_val
     | l_val INCR
     | l_val DECR
     | l_val assign_op expr
     | l_val
     | IDENT LPAR expr_list RPAR
     | literal_val
     ;

l_val : IDENT
      | IDENT LBRACKET expr RBRACKET
      ;

un_op : MINUS %prec UMINUS
      | BANG
      | TILDE
      ;

bin_op : EQUALS
       | NEQUAL
       | GT
       | GE
       | LT
       | LE
       | PLUS
       | MINUS
       | STAR
       | SLASH
       | MOD
       | PIPE
       | AMP
       | DPIPE
       | DAMP
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

var_decl : TYPE ident_list SEMI;

ident_list : ident_c
           | ident_c COMMA ident_list
           ;

ident_c : IDENT
        | IDENT LBRACKET INTCONST RBRACKET
        ;

%%