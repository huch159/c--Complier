#include "node.h"
#include <string.h>
struct Para_node Var_Dec(pNode arg);
static int noname_structnum=1; 
extern domain_hashnode head_domain_hash;
extern domain_structnode head_domain_struct;
extern struct_node head_struct;
static int struct_num=1;
extern FILE* file;
char code_tem[52];
struct New_id tem_id;
/******************************************
	parameter:ExtDefList
	return:void
********************************************/
void Ext_DefList(FILE* arg)
{
	if(head_domain_hash!=NULL)
	{
		hash_node flag=head_domain_hash->first;
		while(flag!=NULL)
		{
			switch(flag->kind)
			{
				case err_type:break;
				case type_int:fprintf(arg,"global %s %d\n",flag->id->id,int_size);break;
				case type_float:fprintf(arg,"global %s %d\n",flag->id->id,float_size);break;
				case address:fprintf(arg,"global %s %d\n",flag->id->id,address_size);break;
				case array:fprintf(arg,"global %s %d\n",flag->id->id,size_of_array(flag->info_array));break;
				case structure:fprintf(arg,"global %s %d\n",flag->id->id,search_structid(head_struct,flag->id_struct)->size);break;
			}
			flag=flag->domain;
		}
	}
		
}
/******************************************
   parameter:specifier  
   return:para_node
	   type:int\float\structure\err_type
	   struct:id_struct
******************************************/
struct Para_node Speci_fier(pNode arg)
{
	struct Para_node flag; 
	if((arg->kids)->kid==NULL)
		flag.kind=(arg->kids)->kind;
	else
	{
		int num=Struct_Spec(arg->kids->kid);
		if(num!=0)
		{
			flag.kind=structure;
			flag.id_struct=num;
		}
		else
			flag.kind=err_type;
	}
	return flag;
}
/***********************************************
	paramater:ExtDef
	return：
		define variable
**************************************************/
void Spec_ExtD_func(pNode arg)
{
	struct Para_node now_para=Speci_fier(arg->kids->kid);
	if(now_para.kind!=err_type)
		if(now_para.kind==type_int||now_para.kind==type_float) //not structure
		{
			pNode flag=(arg->kids+1)->kid;
			pNode flag_help=flag;
			do
			{
				flag=flag_help;
				struct Para_node Var_para=Var_Dec(flag->kids->kid);
				if(!has_define_hashdomain(Var_para.name))
				{
					hash_node new_hashnode;
					if(Var_para.kind==type_ID)
						new_hashnode=makehash_type(now_para.kind,Var_para.name);
					else
						new_hashnode=makehash_array(now_para.kind,Var_para.name,Var_para.info_array);
					new_hashnode->id=id_create_from_hashnode(new_hashnode);
					insert_hash(new_hashnode);
					insert_hashdomain(new_hashnode);
				}
				else
					printf("Error type 3 at Line %d :Redefined variable \"%s\"\n",flag->row,Var_para.name);
				flag_help=(flag->kids+2)->kid;
			}while(flag->num!=1);
		}
		else   //define structure
		{
			pNode flag=(arg->kids+1)->kid;
			pNode flag_help=flag;
			do
			{
				flag=flag_help;
				struct Para_node Var_para=Var_Dec(flag->kids->kid);
				if(!has_define_hashdomain(Var_para.name))
				{
					hash_node new_hashnode;
					if(Var_para.kind==type_ID)
						new_hashnode=makehash_struct(Var_para.name,now_para.id_struct);
					else
						new_hashnode=makehash_structarray(Var_para.name,now_para.id_struct,Var_para.info_array);
					new_hashnode->id=id_create_from_hashnode(new_hashnode);
					insert_hash(new_hashnode);
					insert_hashdomain(new_hashnode);
				}
				else
					printf("Error type 3 at Line %d :Redefined variable \"%s\"\n",flag->row,Var_para.name);
				if(flag->num!=1)
					flag_help=(flag->kids+2)->kid;
			}while(flag->num!=1);
		}
}
/******************************************
   parameter:structspecifier  
   return:
	0 if id not exit
	else id_struct of struct
******************************************/
int Struct_Spec(pNode arg) //arg:structspecifier  return:name of structure
{
	if(arg->num==2)
	{
		struct_node new_flag_struct=search_structnode((arg->kids+1)->kid->kids->type_id);
		if(new_flag_struct!=NULL)
			return new_flag_struct->id_struct;			
		else
		{
			printf("Error type 17 at Line %d :Undefined structure \"%s\"\n",(arg->kids+1)->kid->row,(arg->kids+1)->kid->kids->type_id);
			
			return 0;	
		}		
	}
	else
	{
		
		if(((arg->kids+1)->kid)!=NULL&&has_define_structdomain((arg->kids+1)->kid->kids->type_id))
		{
			printf("Error type 16 at Line %d :Duplicated name \"%s\"\n",(arg->kids+1)->kid->row,(arg->kids+1)->kid->kids->type_id);
			
			return 0;
		}
		else
		{
			creat_hashlist();
			creat_structlist();
			pNode Def_flag=(arg->kids+3)->kid;
			struct_node new_node=(struct_node)malloc(sizeof(struct Struct_node));
			new_node->size=0;
			//new_node->num_para=0;
			new_node->kid=NULL;
			new_node->num_para=Def_List(Def_flag,1);
			new_node->para=(para_node)malloc(new_node->num_para*sizeof(struct Para_node));
			hash_node hash_list=head_domain_hash->first;
			int i;
			for(i=new_node->num_para-1;i>=0;i--)
			{
				switch(hash_list->kind)
				{
					case type_float:			
					case type_int:
						(new_node->para+i)->kind=hash_list->kind;
						strcpy((new_node->para+i)->name,hash_list->name);
						if(hash_list->kind==type_int)
							new_node->size+=int_size;
						else
							new_node->size+=float_size;
						break;
					case array:
						(new_node->para+i)->kind=array;
						strcpy((new_node->para+i)->name,hash_list->name);				
						(new_node->para+i)->info_array=(array_node)malloc(sizeof(struct Array_node));
						copy_array((new_node->para+i)->info_array,hash_list->info_array,hash_list->info_array->dimension);
						new_node->size+=size_of_array(hash_list->info_array);
						break;
					case structure:
						(new_node->para+i)->kind=structure;
						strcpy((new_node->para+i)->name,hash_list->name);
						(new_node->para+i)->id_struct=hash_list->id_struct;
						new_node->size+=(search_structid(head_struct,hash_list->id_struct))->size;
						break;					
				}
				hash_list=hash_list->domain;
			}
			i=head_domain_struct->num;
			if(i)
			{
				struct_node new,struct_next;
				if(new_node->kid==NULL)
				{
					new_node->kid=(struct_node)malloc(sizeof(struct Struct_node));
					new_node->kid->kid=NULL;
					new_node->kid->next=NULL;
					struct_next=(head_struct->next);
					head_struct->next=NULL;
					copy_struct(new_node->kid,head_struct);
					head_struct->next=struct_next;
					new=new_node->kid;
				}
				for(i=i-1;i>0;i--)
				{
					new->next=(struct_node)malloc(sizeof(struct Struct_node));
					new->next->kid=NULL;
					new->next->next=NULL;
					struct_node next=struct_next->next;
					struct_next->next=NULL;
					copy_struct(new->next,struct_next);
					struct_next->next=next;
					struct_next=next;
					new=new->next;
				}
			}	
			delete_domain_hash();
			delete_domain_struct();	
			if(((arg->kids+1)->kid)!=NULL)
			{
				strcpy(new_node->name,(arg->kids+1)->kid->kids->type_id);
			}
			else
			{
				sprintf(new_node->name,"$$$_$$$%d",noname_structnum++);
			}
			new_node->id_struct=struct_num++;
			insert_structdomain(new_node);
			return new_node->id_struct;	
		}	
	}

}
/**********************************
	parameter:
		arg:DefList
		i_id:1 if struct else 0
	return:num of varivable
**************************************/
int Def_List(pNode arg,int i_id)
{
	if(arg!=NULL)
	{
		int num_para=0;
		struct Para_node now_para=Speci_fier(arg->kids->kid->kids->kid);
		if(now_para.kind!=err_type)
			if(now_para.kind==type_int||now_para.kind==type_float) //not structure
			{
				pNode flag=(arg->kids->kid->kids+1)->kid;  //flag=DecList
				pNode flag_help=flag;
				do
				{
					flag=flag_help;
					struct Para_node Var_para=Var_Dec(flag->kids->kid->kids->kid);
					if(i_id&&flag->kids->kid->num==3)
						printf("Error type 15 at Line %d :variable \"%s\" in structure is Initialized\n",flag->kids->kid->kids->kid->row,Var_para.name);
					else if(flag->kids->kid->num==3)
					{
						Exp((flag->kids->kid->kids+2)->kid,NULL,NULL,0);
						if(!is_type(((flag->kids->kid->kids+2)->kid->para).kind))
						{
							printf("Error type 5 at Line %d :Type mismatched for assignment\n",flag->kids->kid->kids->kid->row);
						}
					}
					if(!has_define_hashdomain(Var_para.name))
					{
						hash_node new_hashnode;
						if(Var_para.kind==type_ID)
							new_hashnode=makehash_type(now_para.kind,Var_para.name);
						else
							new_hashnode=makehash_array(now_para.kind,Var_para.name,Var_para.info_array);
						num_para++;
						if(!i_id)
						{
							new_hashnode->id=id_create_from_hashnode(new_hashnode);
							if(Var_para.kind!=type_ID)
								fprintf(file,"DEC %s %d\n",new_hashnode->id->id,size_of_array(new_hashnode->info_array));
							if(flag->kids->kid->num==3)
							{
								fprintf(file,"%s := ",new_hashnode->id->id);
								print_id(&tem_id);
								fprintf(file,"%s\n"," ");
							}
						}
						else
							new_hashnode->id=NULL;
						insert_hash(new_hashnode);
						insert_hashdomain(new_hashnode);
					}
					else
					{
						if(i_id)
							printf("Error type 15 at Line %d :Redefined variable \"%s\"\n",flag->kids->kid->kids->kid->row,Var_para.name);
						else
							printf("Error type 3 at Line %d :Redefined variable \"%s\"\n",flag->row,Var_para.name);
					}
					if(flag->num!=1)
						flag_help=(flag->kids+2)->kid;
				}while(flag->num!=1);
			}
			else   //define structure
			{
				pNode flag=(arg->kids->kid->kids+1)->kid;
				pNode flag_help=flag;
				do
				{
					flag=flag_help;
					struct Para_node Var_para=Var_Dec(flag->kids->kid->kids->kid);
					if(i_id&&flag->kids->kid->num==3)
						printf("Error type 15 at Line %d :variable \"%s\" in structure is Initialized\n",flag->kids->kid->kids->kid->row,Var_para.name);
					else if(flag->kids->kid->num==3)
					{
						Exp((flag->kids->kid->kids+2)->kid,NULL,NULL,0);
						if((((flag->kids->kid->kids+2)->kid->para).kind!=structure||((flag->kids->kid->kids+2)->kid->para).id_struct!=now_para.id_struct))
							printf("Error type 5 at Line %d :Type mismatched for assignment\n",flag->kids->kid->kids->kid->row);
					}
					if(!has_define_hashdomain(Var_para.name))
					{
						hash_node new_hashnode;
						if(Var_para.kind==type_ID)
							new_hashnode=makehash_struct(Var_para.name,now_para.id_struct);
						else
							new_hashnode=makehash_structarray(Var_para.name,now_para.id_struct,Var_para.info_array);
						num_para++;
						if(!i_id)
						{
							new_hashnode->id=id_create_from_hashnode(new_hashnode);
							if(Var_para.kind!=type_ID)
								fprintf(file,"DEC %s %d\n",new_hashnode->id->id,size_of_array(new_hashnode->info_array));
							else
								fprintf(file,"DEC %s %d\n",new_hashnode->id->id,(search_structid(head_struct,new_hashnode->id_struct))->size);
							if(flag->kids->kid->num==3)
							{
								fprintf(file,"%s := ",new_hashnode->id->id);
								print_id(&tem_id);
								fprintf(file,"%s\n"," ");
							}
						}
						else
							new_hashnode->id=NULL;
						insert_hash(new_hashnode);
						insert_hashdomain(new_hashnode);
					}
					else
					{
						if(i_id)
							printf("Error type 15 at Line %d :Redefined variable \"%s\" in structure\n",flag->kids->kid->kids->kid->row,Var_para.name);
						else
							printf("Error type 3 at Line %d :Redefined variable \"%s\"\n",flag->row,Var_para.name);
					}
					if(flag->num!=1)
						flag_help=(flag->kids+2)->kid;
				}while(flag->num!=1);
			}
		//if(i_id)
		return num_para+Def_List((arg->kids+1)->kid,i_id);	
	}
	return 0;
}
/*********************************************
	paramter:VarDec
	return:Para_node
		ID,name
		array,info_array
***********************************************/
struct Para_node Var_Dec(pNode arg)
{
	struct Para_node flag;
	if(arg->num==1)
	{	
		flag.kind=type_ID;
		strcpy(flag.name,arg->kids->type_id);
	}
	else
	{
		flag.kind=array;
		array_node info_array=(array_node)malloc(sizeof(struct Array_node));
		struct Para_node flag_son=Var_Dec(arg->kids->kid);
		strcpy(flag.name,flag_son.name);
		if(flag_son.kind==type_ID)
			info_array->dimension=1;
		else
			info_array->dimension=1+(flag_son.info_array)->dimension;
		info_array->each_dimen=(int *)malloc(info_array->dimension*sizeof(int));
		int i;
		for(i=0;i<info_array->dimension-1;i++)
			(info_array->each_dimen)[i]=(flag_son.info_array->each_dimen)[i];
		if(flag_son.kind!=type_ID)
			delete_array(flag_son.info_array);
		(info_array->each_dimen)[i]=(arg->kids+2)->type_int;;
		flag.info_array=info_array;
	}	
	return flag;
}
/************************************
	parameter:Exp
	return:struct Para_node of Exp
*************************************/
/*struct Para_node Exp_DOT_ID(pNode arg)
{
	struct Para_node flag;
	if((arg->kids->kid->para).kind!=structure)
	{
		printf("Error type 13 at Line %d :Illegal use of \".\"\n",arg->row);
		flag.kind=err_type;
	}
	else
	{
		flag=search_attr(search_structid(head_struct,(arg->kids->kid->para).id_struct),(arg->kids+2)->type_id);
		if(flag.kind==err_type)
			printf("Error type 14 at Line %d :Non-exidtent field \"%s\"\n",arg->row,(arg->kids+2)->type_id);
		//return flag;
	}
	return flag;
}*/
/************************************
	parameter:
		arg1:func_node
		arg2:Args or NULL
	return:wether parameter if right
************************************/
int Arg_Exp(func_node arg1,pNode arg2)
{
	if(arg1->num_para==0&&arg2==NULL)
	{	
		if(!strcmp(arg1->name,"read"))
		{
			struct New_id flag_id_new=id_create_from_null(type_int,int_size);
			fprintf(file,"%s ","READ");
			print_id(&flag_id_new);
			fprintf(file,"%s\n"," ");
			tem_id=flag_id_new;
		}	
		return 1;
	}
	else if(arg1->num_para!=0&&arg2==NULL)
	{
		print_func_para_fault(arg1,arg2);
		return 0;
	}
	else if(arg1->num_para==0&&arg2!=NULL)
	{
		print_func_para_fault(arg1,arg2);
		return 0;
	}
	else
	{
		pNode flag=arg2;
		int i,j=0;
		char code[arg1->num_para][52];
		for(i=0;i<arg1->num_para;i++)
		{
			j=1;
			Exp(flag->kids->kid,NULL,NULL,0);
			copy_code(&tem_id);
			if(((flag->kids->kid->para).kind==array||(flag->kids->kid->para).kind==structure)&&tem_id.kind!=address)
				sprintf(code[i],"&%s",code_tem);
			else
				sprintf(code[i],"%s",code_tem);
			if(!strcmp(arg1->name,"write"))
				continue;
			if(((arg1->para+i)->kind==(flag->kids->kid->para).kind)||((is_type((arg1->para+i)->kind))&&is_type((flag->kids->kid->para).kind)))
			{
				if(((arg1->para+i)->kind==array)&&(!same_type_array((arg1->para+i)->info_array,(flag->kids->kid->para).info_array)))
				{
					print_func_para_fault(arg1,arg2);
					return 0;
				}
				else if(((arg1->para+i)->kind==structure)&&((arg1->para+i)->id_struct!=(flag->kids->kid->para).id_struct))
				{	
					print_func_para_fault(arg1,arg2);
					return 0;
				}
				else if(flag->num==1&&(i!=(arg1->num_para-1)))
				{
					print_func_para_fault(arg1,arg2);
					return 0;
				}
				else if(flag->num!=1)
				{
					flag=(flag->kids+2)->kid;	
					j=0;
				}	
			}
			else
			{
				print_func_para_fault(arg1,arg2);
				return 0;
			}
		}
		if(flag->num!=1||!j)
		{
			print_func_para_fault(arg1,arg2);
			return 0;
		}
		if(!strcmp(arg1->name,"write"))
		{
			fprintf(file,"%s ","WRITE");
			fprintf(file,"%s\n",code[0]);
		}
		else if(!strcmp(arg1->name,"read"))
		{
			struct New_id flag_id_new=id_create_from_null(type_int,int_size);
			fprintf(file,"%s ","READ");
			print_id(&flag_id_new);
			fprintf(file,"%s\n"," ");
			tem_id=flag_id_new;
		}
		else
		{
			for(i=arg1->num_para-1;i>=0;i--)
				fprintf(file,"ARG %s\n",code[i]);
			struct New_id flag_id_new=id_create_from_null(arg1->re_turn->kind,int_size);
			print_id(&flag_id_new);
			fprintf(file," := CALL %s\n",arg1->name);
			tem_id=flag_id_new;
		}
	}	
	return 1;
}
/*********************************
	paramerer: 
		arg1:function name :Exp
		arg2:parameter of function :Args
			or NULL
	return: struct Para_node
*********************************/
struct Para_node ID_Args(pNode arg1,pNode arg2)
{
	struct Para_node flag;
	func_node flag_func=search_funcnode(arg1->kids->type_id);
	if(flag_func==NULL)
	{
		printf("Error type 11 at Line %d :Undefined function \"%s\"\n",arg1->row,arg1->kids->type_id);
		flag.kind=err_type;
	}
	else 
	{
		if(flag_func->decl==0)
		{
			if(flag_func->use_row==NULL)
			{
				flag_func->use_row=(row_node)malloc(sizeof(struct Row_node));
				flag_func->use_row->row=arg1->row;
				flag_func->use_row->next=NULL;
			}
			else
			{
				row_node flag_row=flag_func->use_row;
				while(flag_row->next!=NULL)
				{
					flag_row=flag_row->next;
				}		
				flag_row->next=(row_node)malloc(sizeof(struct Row_node));
				flag_row->next->row=arg1->row;
				flag_row->next->next=NULL;
			}
		}
		Arg_Exp(flag_func,arg2);
		if(!strcmp(flag_func->name,"read")||!strcmp(flag_func->name,"write"))
			flag.kind=type_int;
		else
			flag.kind=flag_func->re_turn->kind;
		if(flag.kind==array)
			flag.info_array=flag_func->re_turn->info_array;
		else if(flag.kind==structure)
			flag.id_struct=flag_func->re_turn->id_struct;
	}
	return flag;
}
/*************************************
	parameter:ExtDef  if declar CompSt=NULL;
	return:void
**************************************/
void Spec_Fun_Com(pNode arg)
{
	char *s=(arg->kids+1)->kid->kids->type_id;
	func_node flag_sear=search_funcnode(s);
	func_node flag_help;
	if(flag_sear==NULL||flag_sear->decl==0||(arg->kids+2)->kind==lex)
	{
		func_node flag=(func_node)malloc(sizeof(struct Func_node));
		flag->use_row=NULL;
		flag_help=flag;
		flag->re_turn=(para_node)malloc(sizeof(struct Para_node));
		*flag->re_turn=Speci_fier(arg->kids->kid);
		strcpy(flag->name,s);
		if((arg->kids+1)->kid->num==4)
			Var_List_func(flag,((arg->kids+1)->kid->kids+2)->kid);
		else
			Var_List_func(flag,NULL);
		if((arg->kids+2)->kind!=lex)
			flag->decl=1;
		else
			flag->decl=0;
	}
	if(flag_sear!=NULL)
	{
		if(flag_sear->decl==1&&(arg->kids+2)->kind!=lex)
		{
			printf("Error type 4 ar Line %d :Redefined function \"%s\"\n",(arg->kids+1)->kid->row,s);
		}
		else 
		{
			if(!is_same_func(flag_help,flag_sear))
			{
				printf("Error type 19 at Line %d :Inconsistent declararion of function \"%s\"\n",(arg->kids+1)->kid->row,s);
			}
			else if((arg->kids+2)->kind!=lex)
				flag_sear->decl=1;
			delete_func_para(flag_help);
		}
	}
	else
		insert_funcnode(flag_help);
	if(flag_sear!=NULL)
		flag_help=flag_sear;
	if((arg->kids+2)->kind!=lex)
	{				//define function
		fprintf(file,"FUNCTION %s :\n",flag_help->name);
		creat_hashlist();
		creat_structlist();
		int i;
		for(i=0;i<flag_help->num_para;i++)
		{
			fprintf(file,"%s","PARAM ");
			hash_node test_hash;
			if((flag_help->para+i)->kind==structure)
				test_hash=makehash_struct((flag_help->para+i)->name,(flag_help->para+i)->id_struct);
			else if((flag_help->para+i)->kind==array)
			{
				if((flag_help->para+i)->info_array->kind==structure)
					test_hash=makehash_structarray((flag_help->para+i)->name,(flag_help->para+i)->info_array->id_struct,(flag_help->para+i)->info_array);
				else
					test_hash=makehash_array((flag_help->para+i)->info_array->kind,(flag_help->para+i)->name,(flag_help->para+i)->info_array);
				test_hash->info_array=(array_node)malloc(sizeof(struct Array_node));
				copy_array(test_hash->info_array,(flag_help->para+i)->info_array,(flag_help->para+i)->info_array->dimension);
			}
			else
				test_hash=makehash_type((flag_help->para+i)->kind,(flag_help->para+i)->name);
			test_hash->id=id_create_from_hashnode(test_hash);
			if(test_hash->kind==array||test_hash->kind==structure)
				test_hash->id->kind=address;
			fprintf(file,"%s\n",test_hash->id->id);
			insert_hash(test_hash);
			insert_hashdomain(test_hash);	
		}
		Comp_St((arg->kids+2)->kid,flag_help->re_turn,1);
	}
			
}
/******************************************
	parameter:
		arg1:func_node 
		arg2: VarList (if no NULL)
	return:
		void
******************************************/
void Var_List_func(func_node arg1,pNode arg2)
{
	if(arg2==NULL)
	{
		arg1->num_para=0;
	}
	else
	{
		arg1->num_para=1;
		pNode flag=arg2;
		while(flag->num!=1)
		{
			flag=(flag->kids+2)->kid;
			(arg1->num_para)++;
		}
		arg1->para=(para_node)malloc((arg1->num_para)*sizeof(struct Para_node));
		flag=arg2;
		int i;
		for(i=0;i<arg1->num_para;i++)
		{
			struct Para_node flag_spec=Speci_fier(flag->kids->kid->kids->kid);
			struct Para_node flag_para=Var_Dec((flag->kids->kid->kids+1)->kid);
			strcpy((arg1->para+i)->name,flag_para.name);
			if(flag_para.kind==type_ID)
			{
				(arg1->para+i)->kind=flag_spec.kind;
				if(flag_spec.kind==structure)
				{
					(arg1->para+i)->id_struct=flag_spec.id_struct;
				}	
			}
			else
			{
				(arg1->para+i)->kind=array;
				(arg1->para+i)->info_array=flag_para.info_array;
				(arg1->para+i)->info_array->kind=flag_spec.kind;
				if(flag_spec.kind==structure)
				{
					(arg1->para+i)->info_array->id_struct=flag_spec.id_struct;
				}
			}
			if(i!=(arg1->num_para-1))
				flag=(flag->kids+2)->kid;
		}
	}
}
/************************************
	parameter:
		arg1:CompSt
		arg2:para_node  return of function
		i: 1 if function
	return 0;
*************************************/
void Comp_St(pNode arg1,para_node arg2,int i_num)
{
	if(!i_num)
	{
		creat_hashlist();
		creat_structlist();
	}			
	Def_List((arg1->kids+1)->kid,0);
	Stmt_List((arg1->kids+2)->kid,arg2);

	delete_domain_hash();
	delete_domain_struct();
}
/*************************************
	parameter:
		arg1:StmtList
		arg2:para_node  return of function
	return : void
**************************************/
void Stmt_List(pNode arg1,para_node arg2)
{
	if(arg1!=NULL)
	{
		St_mt(arg1->kids->kid,arg2);
		Stmt_List((arg1->kids+1)->kid,arg2);
	}
}
/***************************************
	parameter:
		arg1:Stmt
		arg2:para_node  return of function
	return : void
*****************************************/
void St_mt(pNode arg1,para_node arg2)
{
	switch(arg1->num)
	{
		//case 2: Exp(arg1->kids->kid);break;
		case 1:Comp_St(arg1->kids->kid,arg2,0);break;
		case 3:
			Exp((arg1->kids+1)->kid,NULL,NULL,0);
			if(!is_same_para(arg2,&((arg1->kids+1)->kid->para)))
				printf("Error type 8 at Line %d :Type midmatched for return.\n",arg1->row);
			else
			{
				fprintf(file,"%s ","RETURN");
				print_id(&tem_id);
				fprintf(file,"%s\n"," ");
			}
			break;
		case 5:
		{
			char flag_mid[52];
			if(!strcmp(arg1->kids->type_id,"WHILE"))
			{
				label_create(flag_mid);
				fprintf(file,"LABEL %s :\n",flag_mid);
			}
			char flag_true[52];
			label_create(flag_true);
			char flag_false[52];
			label_create(flag_false);
			Exp((arg1->kids+2)->kid,flag_true,flag_false,0);
			if(!is_type(((arg1->kids+2)->kid->para).kind))
				printf("Error type 7 at Line %d :Type midmatched for operands(not int or float)\n",(arg1->kids+2)->kid->row);
			fprintf(file,"LABEL %s :\n",flag_true);
			St_mt((arg1->kids+4)->kid,arg2);
			if(!strcmp(arg1->kids->type_id,"WHILE"))
				fprintf(file,"GOTO %s\n",flag_mid);
			fprintf(file,"LABEL %s :\n",flag_false);
			break;
		}
		case 2:
			Exp(arg1->kids->kid,NULL,NULL,0);
			break;
		default:
		{
			char flag_true[52];
			label_create(flag_true);
			char flag_false[52];
			label_create(flag_false);
			char flag_mid[52];
			label_create(flag_mid);
			Exp((arg1->kids+2)->kid,flag_true,flag_false,0);
			if(!is_type(((arg1->kids+2)->kid->para).kind))
				printf("Error type 7 at Line %d :Type midmatched for operands(not int or float)\n",(arg1->kids+2)->kid->row);
			fprintf(file,"LABEL %s :\n",flag_true);
			St_mt((arg1->kids+4)->kid,arg2);
			fprintf(file,"GOTO %s\n",flag_mid);
			fprintf(file,"LABEL %s :\n",flag_false);
			St_mt((arg1->kids+6)->kid,arg2);
			fprintf(file,"LABEL %s :\n",flag_mid);
			break;
		}
	}
}
/*********************************
	parameter:Exp
	srr_str:if  1 array else 2 structure else 3
	return : void
*********************************/
void Exp(pNode arg,char* true_label,char* false_label,int arr_str)
{
	switch(arg->num)
	{
		case 3:
		{
			if((arg->kids+2)->kind==gram)
			{
				if(!strcmp((arg->kids+1)->type_id,"ASSIGNOP"))
				{
					Exp((arg->kids+2)->kid,NULL,NULL,0);
					struct New_id flag_new_id=tem_id;
					Exp(arg->kids->kid,NULL,NULL,0);
					
					if((arg->kids->kid->para).left_value!=1)
					{
							printf("Error type 6 at Line %d :The left_hand side of an assignment must be a variable.\n",arg->row);
							(arg->para).kind=err_type;
							(arg->para).left_value=1;
					}
					else if(((arg->kids->kid)->para).kind!=err_type&&(((arg->kids+2)->kid)->para).kind!=err_type&&!is_same_para(&((arg->kids->kid)->para),&(((arg->kids+2)->kid)->para)))
					{
						printf("Error type 5 at Line %d :Type mismatched for assignment\n",arg->row);
						(arg->para).kind=err_type;
						(arg->para).left_value=1;
					}
					else
					{
						if(flag_new_id.kind!=err_type&&tem_id.kind!=err_type)
						{
							print_id(&tem_id);
							fprintf(file," %s ",":=");
							print_id(&flag_new_id);
							fprintf(file,"%s\n"," ");
							if(true_label!=NULL)
							{
								fprintf(file,"IF %s != #0 GOTO %s\n",tem_id.id,true_label);
								fprintf(file,"GOTO %s\n",false_label);
							}
						}
						(arg->para).left_value=1;
						strcpy(arg->name,arg->kids->kid->name);
						copy_para_withoutname(&(arg->para),&(((arg->kids+2))->kid->para));
					}
				}
				else if(!strcmp((arg->kids+1)->type_id,"AND")||(arg->kids+1)->kind==type_RELOP||!strcmp((arg->kids+1)->type_id,"OR"))
				{
					if(!strcmp((arg->kids+1)->type_id,"AND"))
					{
						if(true_label==NULL)
						{
							char flag_true[52];
							label_create(flag_true);
							char flag_false[52];
							label_create(flag_false);
							char flag_mid[52];
							label_create(flag_mid);
							char flag_mid2[52];
							label_create(flag_mid2);
							Exp(arg->kids->kid,flag_mid,flag_false,0);
							fprintf(file,"LABEL %s :\n",flag_mid);
							Exp((arg->kids+2)->kid,flag_true,flag_false,0);
							fprintf(file,"LABEL %s :\n",flag_true);
							tem_id=id_create_from_null(type_int,int_size);
							fprintf(file,"%s := #1\n",tem_id.id);
							fprintf(file,"GOTO %s\n",flag_mid2);
							fprintf(file,"LABEL %s :\n",flag_false);
							fprintf(file,"%s := #0\n",tem_id.id);
							fprintf(file,"LABLE %s :\n",flag_mid2);
						}
						else
						{
							char flag_mid[52];
							label_create(flag_mid);
							Exp(arg->kids->kid,flag_mid,false_label,0);
							fprintf(file,"LABEL %s :\n",flag_mid);
							Exp((arg->kids+2)->kid,true_label,false_label,0);
						}
					}
					else if(!strcmp((arg->kids+1)->type_id,"OR"))
					{
						if(true_label==NULL)
						{
							char flag_true[52];
							label_create(flag_true);
							char flag_false[52];
							label_create(flag_false);
							char flag_mid[52];
							label_create(flag_mid);
							char flag_mid2[52];
							label_create(flag_mid2);
							Exp(arg->kids->kid,flag_true,flag_mid,0);
							fprintf(file,"LABEL %s :\n",flag_mid);
							Exp((arg->kids+2)->kid,flag_true,flag_false,0);
							fprintf(file,"LABEL %s :\n",flag_true);
							tem_id=id_create_from_null(type_int,int_size);
							fprintf(file,"%s := #1\n",tem_id.id);
							fprintf(file,"GOTO %s\n",flag_mid2);
							fprintf(file,"LABEL %s :\n",flag_false);
							fprintf(file,"%s := #0\n",tem_id.id);
							fprintf(file,"LABLE %s :\n",flag_mid2);
						}
						else
						{
							char flag_mid[52];
							label_create(flag_mid);
							Exp(arg->kids->kid,true_label,flag_mid,0);
							fprintf(file,"LABEL %s :\n",flag_mid);
							Exp((arg->kids+2)->kid,true_label,false_label,0);
						}
					}
					else if((arg->kids+1)->kind==type_RELOP)
					{
						if(true_label==NULL)
						{
							char flag_true[52];
							label_create(flag_true);
							char flag_false[52];
							label_create(flag_false);
							char flag_mid[52];
							label_create(flag_mid);
							Exp(arg->kids->kid,NULL,NULL,0);
							struct New_id flag_new_id=tem_id;
							Exp((arg->kids+2)->kid,NULL,NULL,0);
							fprintf(file,"%s ","IF");
							print_id(&flag_new_id);
							fprintf(file," %s ",(arg->kids+1)->type_id);
							print_id(&tem_id);
							fprintf(file," GOTO %s\n",flag_true);
							fprintf(file,"GOTO %s\n",flag_false);
							tem_id=id_create_from_null(type_int,int_size);
							fprintf(file,"LABEL %s :\n",flag_true);
							fprintf(file,"%s := #1\n",tem_id.id);
							fprintf(file,"GOTO %s\n",flag_mid);
							fprintf(file,"LABEL %s :\n",flag_false);
							fprintf(file,"%s := #0\n",tem_id.id);
							fprintf(file,"LABLE %s :\n",flag_mid);
						}
						else
						{
							Exp(arg->kids->kid,NULL,NULL,0);
							struct New_id flag_new_id=tem_id;
							Exp((arg->kids+2)->kid,NULL,NULL,0);
							fprintf(file,"%s ","IF");
							print_id(&flag_new_id);
							fprintf(file," %s ",(arg->kids+1)->type_id);
							print_id(&tem_id);
							fprintf(file," GOTO %s\n",true_label);
							fprintf(file,"GOTO %s\n",false_label);
						}
					}
					if(((arg->kids->kid)->para).kind!=err_type&&(((arg->kids+2)->kid)->para).kind!=err_type&&(!is_type(((arg->kids->kid)->para).kind)||!is_type((((arg->kids+2)->kid)->para).kind)))
					{
						printf("Error type 7 at Line %d :Type mismatched for operands\n",arg->row);
						(arg->para).kind=err_type;
						(arg->para).left_value=0;
					}
					else
					{
						(arg->para).left_value=0;	
						(arg->para).kind=type_int;
					}
				}
				else
				{
					Exp((arg->kids+2)->kid,NULL,NULL,0);
					struct New_id flag_new_id=tem_id;
					Exp(arg->kids->kid,NULL,NULL,0);
					struct New_id flag_new_id2=tem_id;
					if(((arg->kids->kid)->para).kind!=err_type&&(((arg->kids+2)->kid)->para).kind!=err_type)
					{
						if(((arg->kids->kid)->para).kind==type_int||((arg->kids->kid)->para).kind==type_INT)
							tem_id=id_create_from_null(type_int,int_size);
						else
							tem_id=id_create_from_null(type_float,float_size);
						print_id(&tem_id);
						fprintf(file," %s ",":=");
						print_id(&flag_new_id2);
						fprintf(file," %s ",(arg->kids+1)->type_id);
						print_id(&flag_new_id);
						fprintf(file," \n"," ");
						if(true_label!=NULL)
						{	
							fprintf(file,"IF %s != #0 GOTO %s\n",tem_id.id,true_label);
							fprintf(file,"GOTO %s\n",false_label);
						}
					}
					if(((arg->kids->kid)->para).kind!=err_type&&(((arg->kids+2)->kid)->para).kind!=err_type&&(!is_type(((arg->kids->kid)->para).kind)||!is_type((((arg->kids+2)->kid)->para).kind)))
					{
						printf("Error type 7 at Line %d :Type mismatched for operands\n",arg->row);
						(arg->para).kind=err_type;
						(arg->para).left_value=0;
					}
					else
					{
						(arg->para).left_value=0;
						(arg->para).kind=((arg->kids->kid)->para).kind;
					}
				}
			}
			else if(arg->kids->kind==lex)
			{
					Exp((arg->kids+1)->kid,NULL,NULL,0);
					copy_para_withoutname(&(arg->para),&(((arg->kids+1))->kid->para));
					if((arg->para).left_value==1&&(arg->para).kind!=err_type)
						strcpy(arg->name,(arg->kids+1)->kid->name);
			}
			else if((arg->kids+2)->kind==lex)
			{
					arg->para=ID_Args(arg,NULL);	
			}
			else 
			{
				Exp((arg->kids)->kid,NULL,NULL,2);
				if((arg->kids->kid->para).kind!=structure)
				{
					printf("Error type 13 at Line %d :Illegal use of \".\"\n",arg->row);
					(arg->para).kind=err_type;
				}
				else
				{
					struct_node flag_struct_now=search_structid(head_struct,(arg->kids->kid->para).id_struct);
					if(arg->kids->kid->num==1&&(search_hashtable(arg->kids->kid->kids->type_id))->id->kind!=address)
						arg->para=search_attr(flag_struct_now,(arg->kids+2)->type_id,2);
					else
						arg->para=search_attr(flag_struct_now,(arg->kids+2)->type_id,0);
					if((arg->para).kind==err_type)
						printf("Error type 14 at Line %d :Non-exidtent field \"%s\"\n",arg->row,(arg->kids+2)->type_id);
				}
				//if((arg->para).kind!=err_type)
				(arg->para).left_value=1;
			}
			break;
		}
		case 2:
		{
			if(true_label==NULL)
			{
				if(!strcmp(arg->kids->type_id,"NOT"))
				{
					char flag_true[52];
					label_create(flag_true);
					char flag_false[52];
					label_create(flag_false);
					char flag_mid[52];
					label_create(flag_mid);
					Exp((arg->kids+1)->kid,flag_true,flag_false,0);
					fprintf(file,":LABEL %s :\n",flag_true);
					struct New_id flag_new_id=id_create_from_null(type_int,int_size);
					print_id(&flag_new_id);
					fprintf(file," := #%d\n",1);
					fprintf(file,"GOTO %s\n",flag_mid);
					fprintf(file,":LABEL %s :\n",flag_false);
					print_id(&flag_new_id);
					fprintf(file," := #%d\n",0);
					tem_id=flag_new_id;
				}
				else
				{
					Exp((arg->kids+1)->kid,NULL,NULL,0);
					int size;
					tag new_kind;
					if((((arg->kids+1)->kid)->para).kind==type_int||(((arg->kids+1)->kid)->para).kind==type_INT)
					{
						size=int_size;
						new_kind=type_int;
					}
					else
					{
						size=float_size;
						new_kind=type_float;
					}
					struct New_id flag_new_id=id_create_from_null(new_kind,size);
					print_id(&flag_new_id);
					fprintf(file," := #%d - ",0);
					print_id(&tem_id);
					fprintf(file,"%s\n"," ");
					tem_id=flag_new_id;
				}
			}
			else
			{
				if(!strcmp(arg->kids->type_id,"NOT"))
				{
					Exp((arg->kids+1)->kid,true_label,false_label,0);
				}
				else
				{
					Exp((arg->kids+1)->kid,NULL,NULL,0);
					fprintf(file,"%s ","IF");
					print_id(&tem_id);
					fprintf(file," != #0 GOTO %s\n",true_label);
					fprintf(file,"GOTO %s\n",false_label);
				}
			}
			if((((arg->kids+1)->kid)->para).kind!=err_type&&!is_type((((arg->kids+1)->kid)->para).kind))
			{
				printf("Error type 7 at Line %d :Type mismatched for operands\n",arg->row);
				(arg->para).kind==err_type;
			}
			if(!strcmp((arg->kids)->type_id,"NOT"))
				(arg->para).kind=type_int;
			else
				(arg->para).kind=((arg->kids+1)->kid->para).kind;
			(arg->para).left_value=0;
			break;
		}
		case 1:
		{
			if(arg->kids->kind==type_ID)
			{
				hash_node id_hash=search_hashtable(arg->kids->type_id);
				if(id_hash==NULL)
				{
					printf("Error type 1 at Line %d : Undefined variable \"%s\".\n",arg->row,arg->kids->type_id);
					(arg->para).kind=err_type;
					tem_id.kind=err_type;
				}
				else
				{
					(arg->para).kind=id_hash->kind;
					strcpy(arg->name,arg->kids->type_id);
					if(id_hash->kind==array)
						(arg->para).info_array=id_hash->info_array;
					else if(id_hash->kind==structure)
						(arg->para).id_struct=id_hash->id_struct;
					(arg->para).left_value=1;
					tem_id=*(id_hash->id);
					if(true_label!=NULL)
					{
						fprintf(file,"IF %s != #0 GOTO %s\n",tem_id.id,true_label);
						fprintf(file,"GOTO %s\n",false_label);
					}
				}	
						
			}
			else
			{
				tem_id.kind=arg->kids->kind;
				if(arg->kids->kind==type_FLOAT)
				{
					tem_id.type_float=arg->kids->type_float;
					tem_id.size=float_size;
				}
				else
				{
					tem_id.type_int=arg->kids->type_int;
					tem_id.size=int_size;
				}
				if(true_label!=NULL)
				{
					fprintf(file,"%s ","IF");
					print_id(&tem_id);
					fprintf(file," != #0 GOTO %s\n",true_label);
					fprintf(file,"GOTO %s\n",false_label);
				}
				(arg->para).kind=arg->kids->kind;
			}
			break;
		}
		case 4:
		{
			if(arg->kids->kind==type_ID)
			{
				arg->para=ID_Args(arg,(arg->kids+2)->kid);
			}
			else
			{
				Exp((arg->kids+2)->kid,NULL,NULL,0);
				struct New_id flag_new_id=tem_id;
				Exp((arg->kids)->kid,NULL,NULL,1);
				if(((arg->kids->kid)->para).kind!=array)
				{
					printf("Error type 10 at Line %d :not an aray.\n",arg->row);
					(arg->para).kind=err_type;
				}
				else if(((arg->kids+2)->kid->para).kind!=type_int&&((arg->kids+2)->kid->para).kind!=type_INT)
				{
					printf("Error type 12 at Line %d :not an integer.\n",arg->row);
					(arg->para).kind=err_type;
				}
				else
				{
					(arg->para).left_value=1;
					array_node flag_array=(arg->kids->kid->para).info_array;
					int size;
					if(flag_array->kind==type_int)
						size=int_size;
					else if(flag_array->kind==type_float)
						size=float_size;
					else if(flag_array->kind==structure)
						size=search_structid(head_struct,flag_array->id_struct)->size;
					if(flag_array->dimension==1)
					{	
						(arg->para).kind=flag_array->kind;
						if(!(flag_new_id.kind==type_INT&&flag_new_id.type_int==0))
						{
							struct New_id flag_new_id2=id_create_from_null(address,address_size);
							print_id(&flag_new_id2);
							fprintf(file," %s ",":=");
							print_id(&flag_new_id);
							fprintf(file," * #%d\n",size);
							struct New_id flag_new_id3=id_create_from_null(address,address_size);
							print_id(&flag_new_id3);
							fprintf(file," %s ",":=");
							if(arg->kids->kid->num==1&&tem_id.kind!=address)
								fprintf(file,"%s","&");
							print_id(&tem_id);
							fprintf(file," %s ","+");
							print_id(&flag_new_id2);
							fprintf(file,"%s\n"," ");
							tem_id=flag_new_id3;
						}		
						else if(arg->kids->kid->num==1&&tem_id.kind!=address)
						{
							
							struct New_id flag_new_id2=id_create_from_null(address,address_size);
							print_id(&flag_new_id2);
							fprintf(file," := %s","&");
							print_id(&tem_id);
							fprintf(file,"%s\n"," ");
							tem_id=flag_new_id2;
						}	
						if(arg->kids->kid->num==1)
						{
							tem_id.kind=address;
							tem_id.size=address_size;
						}	
						if(flag_array->kind==structure)
						{
							(arg->para).id_struct=flag_array->id_struct;
						}
						else
						{
							tem_id.kind=type_strong;
							tem_id.size=size;
						}						
					}
					else
					{	
						(arg->para).kind=array;
						(arg->para).info_array=(array_node)malloc(sizeof(struct Array_node));
						copy_array((arg->para).info_array,flag_array,flag_array->dimension-1);
						int array_now_size=size_of_array((arg->para).info_array);
						if(!(flag_new_id.kind==type_INT&&flag_new_id.type_int==0))
						{
							struct New_id flag_new_id2=id_create_from_null(address,address_size);
							print_id(&flag_new_id2);
							fprintf(file," %s ",":=");
							print_id(&flag_new_id);
							fprintf(file," * #%d\n",size_of_array((arg->para).info_array));
							struct New_id flag_new_id3=id_create_from_null(address,address_size);
							print_id(&flag_new_id3);
							fprintf(file," %s ",":=");
							if(arg->kids->kid->num==1&&tem_id.kind!=address)
								fprintf(file,"%s","&");
							print_id(&tem_id);
							fprintf(file," %s ","+");
							print_id(&flag_new_id2);
							fprintf(file,"%s\n"," ");
							tem_id=flag_new_id3;
						}
						else if(arg->kids->kid->num==1&&tem_id.kind!=address)
						{
							
							struct New_id flag_new_id2=id_create_from_null(address,address_size);
							print_id(&flag_new_id2);
							fprintf(file," := %s","&");
							print_id(&tem_id);
							fprintf(file,"%s\n"," ");
							tem_id=flag_new_id2;
						}
						if(arg->kids->kid->num==1)
						{
							tem_id.kind=address;
							tem_id.size=address_size;
						}		
					}
					
				}
			}
		}
	}
}
