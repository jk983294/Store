%{
%}
%token NUM IDENTIFIER EQ
%left  ADD SUB
%left  MUL DIV	
%%
S : identifier EQ E  { symTable[lookup(name)].val=v[0] }
     ;
identifier : IDENTIFIER { insert(name,IDENTIFIER,0) }
     ; 
E : E ADD E  { v[0]=v[1]+v[3] }
  | E SUB E  { v[0]=v[1]-v[3] }
  | E MUL E  { v[0]=v[1]*v[3] }
  | E DIV E  { v[0]=v[1]/v[3] }
  | '(' E  ')'    { v[0]=v[2] }
  | NUM            { v[0]=val }
             ;
%%
