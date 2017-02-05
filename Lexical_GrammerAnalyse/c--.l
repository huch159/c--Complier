%option yylineno
%{
#include <string.h>
#include <stdio.h>
#include "c--.tab.h"
typedef union{
	int type_int;
	float type_float;
	char type_id[52];
}YYLVAL;
#define YYSTYPE YYLVAL
int yycolumn =1;
#define YY_USER_ACTION \
	(yylloc.first_line)=yylloc.last_line=yylineno; \
	yylloc.first_column=yycolumn; \
	yylloc.last_column=yycolumn+yyleng-1;\
	yycolumn+=yyleng;
%}
int [0-9]+
float ([0-9]*\.?[0-9]+|[0-9]+\.)([eE][+-]?[0-9]+)?  
id [a-zA-Z_][a-zA-Z_0-9]*		
%%
"/*" { 
	char a=input();
	char b=input();
	while(a!='*'&&b!='/')
	{
		a=b;
		b=input();
	}  
}
"//" { 
	char c=input();
	while(c!='\n')
		c=input();  
}
"int"|"float" {strcpy(yylval.type_id, yytext);return TYPE;}
"struct" {return STRUCT;}
"if" {return IF;}
"else" {return ELSE;}
"while" {return WHILE;}
"return" {return RETURN;}
{id} {strcpy(yylval.type_id, yytext);return ID;}
";" {;return SEMI;}
"," {return COMMA; }
"=" {return ASSIGNOP; }
">"|"<"|"<="|">="|"=="|"!=" {strcpy(yylval.type_id, yytext);return RELOP;}
"+" {return PLUS;}
"-" {return MINUS;}
"*" {return STAR;}
"/" {return DIV;}
"&&" {return AND;}
"||" {return OR;}
"." {return DOT;}
"!" {return NOT;}
"(" {return LP;}
")" {return RP;}
"[" {return LB;}
"]" {return RB;}
"{" {return LC;}
"}" {return RC;}
{int} {yylval.type_int=atoi(yytext);;return INT;}
{float} {yylval.type_float=atof(yytext);return FLOAT;}
[ \t]* {}
\n {yycolumn=1;}
. {printf("error type A at line [%d]\n",yylineno);  }
%%
/*int main(int argc,char** argv)
{
	if(argc>1){
		if(!(yyin=fopen(argv[1],"r"))){
			perror(argv[1]);
			return 1;
		}
	yylex();
	}
	return 0;
}*/
