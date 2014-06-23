%{
%}
%token EQ STAR I	
%%
S : L EQ R | R
   ;
L : STAR R | I
   ;
R : L
   ;
%%
