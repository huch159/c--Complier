%{
	#include <string.h>
	#include <stdlib.h>
	#include "node.h"
	#include "math.h"
	extern int yylineno;
	FILE* file;
	struct Node* dis_node=NULL;
%}
%union{
	int type_int;
	float type_float;
	char type_id[52];
	struct Node* pNode;
	struct lex_gram* plex_gram;
};
%type <pNode> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier OptTag Tag VarDec FunDec VarList ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args Specifier_sec StructSpecifier_sec
%token <type_int> INT 
%token <type_float> FLOAT
%token <type_id> TYPE  RELOP ID
%token SEMI COMMA STRUCT PLUS MINUS STAR DIV AND OR DOT NOT LP RP LB RB LC RC RETURN IF ELSE WHILE ASSIGNOP
%right ASSIGNOP 
%left OR
%left AND
%left RELOP
%left PLUS MINUS 
%left STAR DIV 
%right NOT
%left DOT LP RP LB RB
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%%
Program : ExtDefList {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"Program");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		if($1==NULL) {free $$;$$=NULL;}
		dis_node=$$;
	}
	;
ExtDefList : ExtDef ExtDefList {$$=(pNode)malloc(sizeof(struct Node));$$->num=2;$$->row=(@$).first_line;strcpy($$->type_id,"ExtDefList");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
		if($1==NULL&&$2==NULL) {free $$;$$=NULL;
		
		}
	}
	| {$$=NULL;}
	;
ExtDef : Specifier ExtDecList SEMI {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"ExtDef");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
		($$->kids+2)->kind=lex;strcpy((($$->kids+2))->type_id,"SEMI");
		Spec_ExtD_func($$);
	}
	|Specifier SEMI {$$=(pNode)malloc(sizeof(struct Node));$$->num=2;$$->row=(@$).first_line;strcpy($$->type_id,"ExtDef");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"SEMI");
		Speci_fier($1);
	}
	|Specifier FunDec CompSt {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"ExtDef");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
		if($1==NULL&&$2==NULL&&$3==NULL) {free $$;$$=NULL;}
		Spec_Fun_Com($$);
		
	}
	|Specifier FunDec SEMI{$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"ExtDef");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
		($$->kids+2)->kind=lex;strcpy((($$->kids+2))->type_id,"SEMI");
		Spec_Fun_Com($$);
	}
	;
ExtDecList : VarDec {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"ExtDecList");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		if($1==NULL) {free $$;$$=NULL;}
	}
	| VarDec COMMA ExtDecList {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"ExtDecList");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"COMMA");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;	
	}
	;
Specifier : TYPE {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"Specifier");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		if(!strcmp("int",$1))
			($$->kids)->kind=type_int;
		else
			($$->kids)->kind=type_float;
		(($$->kids))->kid=NULL;
		if($1==NULL) {free $$;$$=NULL;}
	}
	| StructSpecifier {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"Specifier");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		
	}
	;
StructSpecifier : STRUCT OptTag LC DefList RC {$$=(pNode)malloc(sizeof(struct Node));$$->num=5;$$->row=(@$).first_line;strcpy($$->type_id,"StructSpecifier");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=lex;strcpy((($$->kids))->type_id,"STRUCT");
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
		($$->kids+2)->kind=lex;strcpy((($$->kids+2))->type_id,"LC");
		($$->kids+3)->kind=gram;(($$->kids+3))->kid=$4;
		($$->kids+4)->kind=lex;strcpy((($$->kids+4))->type_id,"RC");
	}
	| STRUCT Tag {$$=(pNode)malloc(sizeof(struct Node));$$->num=2;$$->row=(@$).first_line;strcpy($$->type_id,"StructSpecifier");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=lex;strcpy((($$->kids))->type_id,"STRUCT");
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
	}
	;
OptTag : ID {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"OptTag");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=type_ID;strcpy((($$->kids))->type_id,$1);
	}
	| {$$=NULL;}
	;
Tag : ID {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"Tag");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=type_ID;strcpy((($$->kids))->type_id,$1);
	}
	;
VarDec : ID {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"VarDec");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=type_ID;strcpy((($$->kids))->type_id,$1);
	}
	| VarDec LB INT RB {$$=(pNode)malloc(sizeof(struct Node));$$->num=4;$$->row=(@$).first_line;strcpy($$->type_id,"VarDec");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"LB");
		($$->kids+2)->kind=type_INT;(($$->kids+2))->type_int=$3;
		($$->kids+3)->kind=lex;strcpy((($$->kids+3))->type_id,"RB");
	}
	;
FunDec : ID LP VarList RP {$$=(pNode)malloc(sizeof(struct Node));$$->num=4;$$->row=(@$).first_line;strcpy($$->type_id,"FunDec");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=type_ID;strcpy((($$->kids))->type_id,$1);
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"LP");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
		($$->kids+3)->kind=lex;strcpy((($$->kids+3))->type_id,"RP");
	}
	|ID LP RP {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"FunDec");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=type_ID;strcpy((($$->kids))->type_id,$1);
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"LP");
		($$->kids+2)->kind=lex;strcpy((($$->kids+2))->type_id,"RP");
	}
	;
VarList : ParamDec COMMA VarList {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"VarList");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"COMMA");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
	}
	|ParamDec {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"VarList");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		if($1==NULL) {free $$;$$=NULL;}
	}
	;
ParamDec : Specifier_sec VarDec {$$=(pNode)malloc(sizeof(struct Node));$$->num=2;$$->row=(@$).first_line;strcpy($$->type_id,"ParamDec");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
		if($1==NULL&&$2==NULL) {free $$;$$=NULL;}
	}
	;
Specifier_sec : TYPE {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"Specifier_sec");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		if(!strcmp("int",$1))
			($$->kids)->kind=type_int;
		else
			($$->kids)->kind=type_float;
		(($$->kids))->kid=NULL;
		if($1==NULL) {free $$;$$=NULL;}
	}
	| StructSpecifier_sec {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"Specifier_sec");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		
	}
	;
StructSpecifier_sec :STRUCT Tag {$$=(pNode)malloc(sizeof(struct Node));$$->num=2;$$->row=(@$).first_line;strcpy($$->type_id,"StructSpecifier_sec");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=lex;strcpy((($$->kids))->type_id,"STRUCT");
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
	}
	;
CompSt : LC DefList StmtList RC {$$=(pNode)malloc(sizeof(struct Node));$$->num=4;$$->row=(@$).first_line;strcpy($$->type_id,"CompSt");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=lex;strcpy((($$->kids))->type_id,"LC");
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
		($$->kids+3)->kind=lex;strcpy((($$->kids+3))->type_id,"RC");
	}
	;
StmtList :  Stmt StmtList {$$=(pNode)malloc(sizeof(struct Node));$$->num=2;$$->row=(@$).first_line;strcpy($$->type_id,"StmtList");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
		if($1==NULL&&$2==NULL) {free $$;$$=NULL;}
	}
	| {$$=NULL;}
	;
Stmt : Exp SEMI {$$=(pNode)malloc(sizeof(struct Node));$$->num=2;$$->row=(@$).first_line;strcpy($$->type_id,"Stmt");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"SEMI");
	}
	|CompSt {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"Stmt");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		if($1==NULL) {free $$;$$=NULL;}
	}
	|RETURN Exp SEMI {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"Stmt");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=lex;strcpy((($$->kids))->type_id,"RETURN");
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
		($$->kids+2)->kind=lex;strcpy((($$->kids+2))->type_id,"SEMI");
	}
	|IF LP Exp RP Stmt {$$=(pNode)malloc(sizeof(struct Node));$$->num=5;$$->row=(@$).first_line;strcpy($$->type_id,"Stmt");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=lex;strcpy((($$->kids))->type_id,"IF");
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"LP");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
		($$->kids+3)->kind=lex;strcpy((($$->kids+3))->type_id,"RP");
		($$->kids+4)->kind=gram;(($$->kids+4))->kid=$5;
	}
	|IF LP Exp RP Stmt ELSE Stmt {$$=(pNode)malloc(sizeof(struct Node));$$->num=7;$$->row=(@$).first_line;strcpy($$->type_id,"Stmt");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=lex;strcpy((($$->kids))->type_id,"IF");
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"LP");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
		($$->kids+3)->kind=lex;strcpy((($$->kids+3))->type_id,"RP");
		($$->kids+4)->kind=gram;(($$->kids+4))->kid=$5;
		($$->kids+5)->kind=lex;strcpy((($$->kids+5))->type_id,"ELSE");
		($$->kids+6)->kind=gram;(($$->kids+6))->kid=$7;
	}
	|WHILE LP Exp RP Stmt {$$=(pNode)malloc(sizeof(struct Node));$$->num=5;$$->row=(@$).first_line;strcpy($$->type_id,"Stmt");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=lex;strcpy((($$->kids))->type_id,"WHILE");
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"LP");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
		($$->kids+3)->kind=lex;strcpy((($$->kids+3))->type_id,"RP");
		($$->kids+4)->kind=gram;(($$->kids+4))->kid=$5;
	}
	;
DefList : Def DefList {$$=(pNode)malloc(sizeof(struct Node));$$->num=2;$$->row=(@$).first_line;strcpy($$->type_id,"DefList");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
		if($1==NULL&&$2==NULL) {free $$;$$=NULL;}
	}
	| {$$=NULL;}
	;
Def : Specifier DecList SEMI {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"Def");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
		($$->kids+2)->kind=lex;strcpy((($$->kids+2))->type_id,"SEMI");
	}
	;
DecList : Dec {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"DecList");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		if($1==NULL) {free $$;$$=NULL;}
	}
	| Dec COMMA DecList {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"DecList");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"COMMA");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
	}
	;
Dec : VarDec {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"Dec");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		if($1==NULL) {free $$;$$=NULL;}
	}
	|VarDec ASSIGNOP Exp {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"Dec");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"ASSIGNOP");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
	}
	;
Exp : Exp ASSIGNOP Exp {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"ASSIGNOP");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
		
	}
	|Exp AND Exp {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"AND");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
		
	}
	|Exp OR Exp {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"OR");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
	}
	|Exp RELOP Exp {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=type_RELOP;strcpy((($$->kids+1))->type_id,$2);
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
	}
	|Exp PLUS Exp {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"+");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
		
	}
	|Exp MINUS Exp {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"-");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
	}
	|Exp STAR Exp {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"*");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
	}
	|Exp DIV Exp {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"/");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
	}
	|LP Exp RP {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=lex;strcpy((($$->kids))->type_id,"LP");
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
		($$->kids+2)->kind=lex;strcpy((($$->kids+2))->type_id,"RP");
	}
	|MINUS Exp {$$=(pNode)malloc(sizeof(struct Node));$$->num=2;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=lex;strcpy((($$->kids))->type_id,"-");
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
	}
	|NOT Exp {$$=(pNode)malloc(sizeof(struct Node));$$->num=2;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=lex;strcpy((($$->kids))->type_id,"NOT");
		($$->kids+1)->kind=gram;(($$->kids+1))->kid=$2;
	}
	|ID LP Args RP {$$=(pNode)malloc(sizeof(struct Node));$$->num=4;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=type_ID;strcpy((($$->kids))->type_id,$1);
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"LP");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
		($$->kids+3)->kind=lex;strcpy((($$->kids+3))->type_id,"RP");
	}
	|ID LP RP {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=type_ID;strcpy((($$->kids))->type_id,$1);
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"LP");
		($$->kids+2)->kind=lex;strcpy((($$->kids+2))->type_id,"RP");
	}
	|Exp LB Exp RB {$$=(pNode)malloc(sizeof(struct Node));$$->num=4;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"LB");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
		($$->kids+3)->kind=lex;strcpy((($$->kids+3))->type_id,"RB");
	}
	|Exp DOT ID {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"DOT");
		($$->kids+2)->kind=type_ID;strcpy((($$->kids+2))->type_id,$3);
	}
	|ID {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=type_ID;strcpy((($$->kids))->type_id,$1);
	}
	|INT {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=type_INT;(($$->kids))->type_int=$1;
	}
	|FLOAT {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"Exp");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=type_FLOAT;(($$->kids))->type_float=$1;
	}
	;
Args : Exp COMMA Args {$$=(pNode)malloc(sizeof(struct Node));$$->num=3;$$->row=(@$).first_line;strcpy($$->type_id,"Args");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		($$->kids+1)->kind=lex;strcpy((($$->kids+1))->type_id,"COMMA");
		($$->kids+2)->kind=gram;(($$->kids+2))->kid=$3;
	}
	|Exp {$$=(pNode)malloc(sizeof(struct Node));$$->num=1;$$->row=(@$).first_line;strcpy($$->type_id,"Args");
		$$->kids=(plex_gram)malloc($$->num*sizeof(struct lex_gram));
		($$->kids)->kind=gram;(($$->kids))->kid=$1;
		if($1==NULL) {free $$;$$=NULL;}
	}
	;
%%
#include "lex.yy.c"
int main(int argc,char *argv[])
{
	if(argc<=1)	
		return 1;
	if(!(yyin=fopen(argv[1],"r")))
	{
		perror(argv[1]);
		return 1;
	}
	char filename[52];
	creat_hashlist();
	creat_structlist();
	func_node read_func=(func_node)malloc(sizeof(struct Func_node));  //定义read函数
	strcpy(read_func->name,"read");
	read_func->decl=1;
	read_func->num_para=0;
	read_func->re_turn=NULL;
	insert_funcnode(read_func);
	func_node write_func=(func_node)malloc(sizeof(struct Func_node));  //定义write函数
	strcpy(write_func->name,"write");
	write_func->decl=1;
	write_func->num_para=1;
	write_func->re_turn=NULL;
	insert_funcnode(write_func);
	sprintf(filename,"%s","a.tem");
	for(int i=0;i<argc;i++)
		if(!strcmp(argv[i],"-o")&&(i<argc-1))
			sprintf(filename,"%s",argv[i+1]);
	file=fopen(filename,"w");		//打开文件
	initial_hashtable();
	yyparse();
	search_func_nodefine_use();
	char exc[1024];
	int num_rea;
	FILE* tmp=fopen("tem.in","w");
	Ext_DefList(tmp);
	fclose(file);
	FILE* tmp_flag=fopen(filename,"r");
	do{
		num_rea=fread(exc,1,1024,tmp_flag);
		fwrite(exc,1,num_rea,tmp);
	}while(num_rea==1024);
	fclose(tmp_flag);
	fclose(tmp);
	remove(filename);
	int mode_flag=0;
	for(int i=0;i<argc;i++)
		if(!strcmp(argv[i],"-m")&&(i<argc-1)&&!strcmp(argv[i+1],"1")){
			char inter_obj[100];
			sprintf(inter_obj,"./t-- tem.in -o %s",filename);
			system(inter_obj);
			remove("tem.in");
			mode_flag=1;
			break;
		}
	if(!mode_flag)
		rename("tem.in",filename);
	for(int i=0;i<argc;i++)
		if(!strcmp(argv[i],"-d"))
		{
			display(dis_node,0);
			delete_tree(dis_node);
		}
	//回收链表空间
	delete_domain_hash();
	delete_domain_struct();	
	delete_hashtable();
	delete_func();
	return 0;
}
yyerror(char* msg)
{
	fprintf(stderr,"error(%d):%s\n",yylineno,msg);
}
