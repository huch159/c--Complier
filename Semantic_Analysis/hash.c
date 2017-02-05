#include <string.h>
#include "node.h"
#define hashtable_size 1000
hash_node static hash_table[hashtable_size];
hash_node makehash_array(tag kind,char* name,array_node info_array)
{
	hash_node hash_flag=(hash_node)malloc(sizeof(struct Hash_node));
	hash_flag->kind=array;
	strcpy(hash_flag->name,name);
	hash_flag->info_array=info_array;
	info_array->kind=kind;
	return hash_flag;
}
hash_node makehash_type(tag kind,char* name)
{
	hash_node hash_flag=(hash_node)malloc(sizeof(struct Hash_node));
	hash_flag->kind=kind;
	strcpy(hash_flag->name,name);
	return hash_flag;
}
hash_node makehash_struct(char *name,int id)
{
	hash_node hash_flag=(hash_node)malloc(sizeof(struct Hash_node));
	hash_flag->kind=structure;
	strcpy(hash_flag->name,name);
	hash_flag->id_struct=id;
	return hash_flag;
}
hash_node makehash_structarray(char *name,int id,array_node info_array)
{
	hash_node hash_flag=(hash_node)malloc(sizeof(struct Hash_node));
	hash_flag->kind=array;
	strcpy(hash_flag->name,name);
	hash_flag->info_array=info_array;
	info_array->kind=structure;
	info_array->id_struct=id;
	return hash_flag;
}
int search_table(char *name)
{
	unsigned int val=0,i;
	for(;*name;++name)
	{
		val=(val<<2)+*name;
		if(i=val &~hashtable_size)
			val=(val^(i>>12))&hashtable_size;
	}
	return val;
}
void insert_hash(hash_node newnode)
{
	int val=search_table(newnode->name);
	if(hash_table[val]->next==NULL)
	{
		(hash_table[val])->next=newnode;
		newnode->next=NULL;		
	}
	else
	{
		hash_node flag=(hash_table[val])->next;
		(hash_table[val])->next=newnode;
		newnode->next=flag;
	}
}
void delete_hash(hash_node newnode)
{
	int val=search_table(newnode->name);
	hash_node flag=(hash_table[val])->next;
	(hash_table[val])->next=flag->next;
	if(flag->kind==array)
	{
		delete_array(flag->info_array);
	}
	free(flag);
}
hash_node search_hashtable(char* name)
{
	int val=search_table(name);
	hash_node flag=(hash_table[val])->next;
	while(flag!=NULL)
	{
		if(!strcmp(flag->name,name))
			return flag;
		else
			flag=flag->next;
	}
	return NULL;
}
void initial_hashtable()
{
	int i;
	for(i=0;i<hashtable_size;i++)
	{
		hash_table[i]=(hash_node)malloc(sizeof(struct Hash_node));
		hash_table[i]->next=NULL;
	}
}
void delete_hashnode(hash_node arg)
{
	/*if(arg->next!=NULL)
		delete_hashnode(arg->next);
	else
		free(arg);*/
}
void delete_hashtable()
{
	int i;
	for(i=0;i<hashtable_size;i++)
	{
		free(hash_table[i]);
	}
}


