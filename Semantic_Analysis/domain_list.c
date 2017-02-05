#include "node.h"
domain_hashnode  head_domain_hash=NULL;
domain_structnode  head_domain_struct=NULL;
extern struct_node head_struct;
void creat_hashlist()
{
	if(head_domain_hash==NULL)
	{
		head_domain_hash=(domain_hashnode)malloc(sizeof(struct Domain_hashnode));
		head_domain_hash->first=NULL;
		head_domain_hash->next=NULL;
	}
	else
	{
		domain_hashnode flag=(domain_hashnode)malloc(sizeof(struct Domain_hashnode));
		flag->first=NULL;
		flag->next=head_domain_hash;
		head_domain_hash=flag;
	}
}
void creat_structlist()
{
	if(head_domain_struct==NULL)
	{
		head_domain_struct=(domain_structnode)malloc(sizeof(struct Domain_structnode));
		head_domain_struct->num=0;
		head_domain_struct->next=NULL;
	}
	else
	{
		domain_structnode flag=(domain_structnode)malloc(sizeof(struct Domain_structnode));
		flag->num=0;
		flag->next=head_domain_struct;
		head_domain_struct=flag;
	}
}
void insert_hashdomain(hash_node newnode)
{
	if(head_domain_hash->first==NULL)
	{
		head_domain_hash->first=newnode;
		newnode->domain=NULL;
	}
	else
	{
		hash_node flag=head_domain_hash->first;
		head_domain_hash->first=newnode;
		newnode->domain=flag;
	}
}
void insert_structdomain(struct_node newnode)
{
	insert_structnode(newnode);
	head_domain_struct->num++;
}
void delete_domain_hash()
{
	if(head_domain_hash!=NULL)
	{
		hash_node flag=head_domain_hash->first;
		domain_hashnode flag_node=head_domain_hash->next;
		//head_domain_hash=head_domain_hash->first;
		free(head_domain_hash);
		head_domain_hash=flag_node;
		while(flag!=NULL)
		{	
			hash_node flag_next=flag->domain;;
			delete_hash(flag);
			flag=flag_next;
		}
	}
}
void delete_domain_struct()
{
	if(head_domain_struct!=NULL)
	{
		while(head_domain_struct->num--)
		{
			delete_structnode();
		}
		domain_structnode flag=head_domain_struct->next;
		free(head_domain_struct);
		head_domain_struct=flag;	
	}
}
int has_define_hashdomain(char* name)
{
	if(head_domain_hash!=NULL)
	{
		hash_node flag=head_domain_hash->first;
		while(flag!=NULL)
		{
			if(!strcmp(flag->name,name))
				return 1;
			flag=flag->domain;
		}
	}
	return 0;
}
int has_define_structdomain(char* name)
{
	if(head_domain_struct!=NULL)
	{
		int i=head_domain_struct->num;
		struct_node flag=head_struct;
		while(flag!=NULL&&i--)
		{
			if(!strcmp(flag->name,name))
				return 1;
			flag=flag->next;
		}
	}
	return 0;
}


