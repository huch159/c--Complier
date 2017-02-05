#ifndef __gram__
#define __gram__
#include <stdio.h>
#define float_size 4
#define int_size 4
#define address_size 4
typedef enum Tag{type_strong,address,gram,array,structure,type_ID,type_int,type_float,type_INT,type_FLOAT,type_RELOP,lex,err_type}tag;
typedef struct Array_node{
	tag kind;
	//char id[52];
	int id_struct;
	int dimension;
	int* each_dimen;
}*array_node;
typedef struct Para_node{
	tag kind;
	char name[52];
	int left_value;//1表示可以为左值
	union {
		int id_struct;
		array_node info_array;
	};
}*para_node;
typedef struct lex_gram{
	tag kind;
	union {
		struct Node *kid;
		char type_id[52];
		int type_int;
		float type_float;
	};
}*plex_gram;			
typedef struct Node{
	int num;
	int row;
	char name[52];//name of ID
	struct Para_node para;
	char type_id[52];	
	plex_gram kids;
}*pNode;
typedef struct Struct_node{
	char name[52];
	int id_struct;
	int num_para;
	int size;
	para_node para;
	struct Struct_node* kid;
	struct Struct_node* next;
	//struct Struct_node* domain;
}*struct_node;
typedef struct Row_node
{
	int row;
	struct  Row_node* next;
}*row_node;
typedef struct Func_node{
	int decl;//为0表示只进行过声明,为1表示进行了定义
	para_node re_turn;
	row_node use_row;
	//char id[52];
	char name[52];
	int num_para;
	para_node para;
	struct Func_node* next;
}*func_node;
typedef struct New_id
{
	tag kind;
	int size;
	union {
		char id[52];
		int type_int;
		float type_float;
	};
}*new_id;
typedef struct Hash_node{
	tag kind;
	char name[52];
	new_id id;
	union {
		int id_struct;
		array_node info_array;
	};
	struct Hash_node* next;
	struct Hash_node* domain;
}*hash_node;
typedef struct Domain_hashnode
{
	hash_node first;
	struct Domain_hashnode* next;
}*domain_hashnode;
typedef struct Domain_structnode
{
	int num;
	struct Domain_structnode* next;
}*domain_structnode;
typedef struct Tar_node
{
	char name[52];
	int kind;	//1 if in stack 相对于$sp,2 if global
	int add;
	struct Tar_node *next;
}*tar_node;
/*typedef struct Sum_head_node
{
	tar_node first;
	struct Sum_head_node* next;
}*sum_head_node;*/
void exchange_code();
void if_func(char* arg,int n);
void calculate_tar(char* arg1,char *arg4);
int count_chr(char* arg);
void print_id(new_id arg);
void copy_code(new_id arg);
void id_create(char* arg);
void label_create(char* arg);
struct New_id id_create_from_null(tag arg,int size);
new_id id_create_from_hashnode(hash_node arg);
int size_of_array(array_node arg);
struct_node search_structid(struct_node arg,int num);
void Comp_St(pNode arg1,para_node arg2,int);
void Var_List_func(func_node arg1,pNode arg2);
void Stmt_List(pNode arg1,para_node arg2);
void St_mt(pNode arg1,para_node arg2);
void print_func_para_fault(func_node arg1,pNode arg2);
struct Para_node ID_Args(pNode arg1,pNode arg2);
struct Para_node Exp_DOT_ID(pNode arg);
int is_type(tag kind);
struct Para_node ID_type(char* name);
void display(struct Node* pNode,int layer);
int Struct_Spec(pNode arg);
void Ext_DefList(FILE* arg);
struct Para_node Speci_fier(pNode arg);
void Spec_ExtD_func(pNode arg);
int Def_List(pNode arg,int);
struct Para_node Var_Dec(pNode arg);
void insert_structnode(struct_node newnode);
void delete_structnode();
void delete_struct(struct_node arg);
struct Para_node search_attr(struct_node arg,char *name,int arr_str);
void copy_struct(struct_node arg1,struct_node arg2);
int has_define_structdomain(char* name);
int has_define_hashdomain(char* name);
void delete_domain_struct();
void delete_domain_hash();
void insert_structdomain(struct_node newnode);
void insert_hashdomain(hash_node newnode);
void creat_structlist();
void creat_hashlist();
void delete_array(array_node arg);
void copy_array(array_node arg1,array_node arg2,int num);
void delete_hashtable();
void delete_hashnode(hash_node arg);
void initial_hashtable();
hash_node search_hashtable(char* name);
void delete_hash(hash_node newnode);
void insert_hash(hash_node newnode);
hash_node makehash_array(tag kind,char* name,array_node info_array);
hash_node makehash_type(tag kind,char* name);
hash_node makehash_struct(char *name,int id);
hash_node makehash_structarray(char *name,int id,array_node info_array);
int search_table(char *name);
void insert_funcnode(func_node newnode);
int search_func_para(func_node arg,int num,...);
func_node search_funcnode(char* id);
void delete_func_para(func_node arg);
void delete_func();
#endif

