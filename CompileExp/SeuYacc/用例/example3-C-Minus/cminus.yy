%{
%}
%token IDENTIFIER FOR WHILE IF ELSE DO		
%token INT FLOAT LONG SHORT CHAR DOUBLE SIGNED UNSIGNED
%token EQ_OP L_OP G_OP NE_OP LE_OP GE_OP
%token NUM
%left  ADD SUB
%left  MUL DIV
%%
source_file : type function_name '(' parament_list ')' compound_statement | type function_name '(' ')' compound_statement
	    ;

type : INT
     | FLOAT
     | LONG
     | SHORT
     | CHAR
     | DOUBLE
     | SIGNED
     | UNSIGNED
     ;

function_name : identifier ; 

identifier : IDENTIFIER { insert(name,IDENTIFIER,0) }; 

parament_list : type identifier ',' parament_list
              | type identifier
              ;

compound_statement : '{' statement_list '}'
                   ;

statement_list : statement
               | statement_list statement   
                    ;

statement : assign_statement
          | selection_statement
          | iteration_statement
          ;

assign_statement : assign_expression ';'
                 | initial_expression ';'
                 ;

assign_expression : assign_expression ADD assign_expression { v[0]=v[1]+v[3] }
                 |  assign_expression SUB assign_expression { v[0]=v[1]-v[3] }
                 |  assign_expression MUL assign_expression { v[0]=v[1]*v[3] }
                 |  assign_expression DIV assign_expression { v[0]=v[1]/v[3] }
                 |  '(' assign_expression  ')'              { v[0]=v[2] }
                 | NUM                                      { v[0]=val }
                 | identifier operator identifier
                  ;

operator : ADD
         | SUB
         | MUL
         | DIV
         ;

initial_expression : type identifier   
                   | type identifier  '=' identifier           
                   | type identifier '=' assign_expression { symTable[lookup(name)].val=v[0] }
                   | identifier '=' assign_expression     
                   ;

selection_statement : IF '(' relate_expression ')' compound_statement
                    ;

relate_expression : identifier relation_operator identifier
		   | identifier relation_operator NUM
                   | NUM relation_operator identifier
                   | NUM relation_operator NUM
           ;

relation_operator : EQ_OP
                  | L_OP
                  | G_OP
                  | NE_OP
                  | GE_OP
                  | LE_OP
                  ;

iteration_statement : WHILE '(' relate_expression ')' compound_statement
                  |  DO compound_statement WHILE '(' relate_expression ')' ';'
                    ;
                  
%%
