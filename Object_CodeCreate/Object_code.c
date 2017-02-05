#include "node.h"
#include <string.h>
static FILE* mid_file,*tar_file;
tar_node head_tar;
tar_node now_tar;
int para_num=0;
int acpara_num=0;
static int now_esp=0;
void delete_tar()
{
	while(head_tar!=NULL)
	{
		tar_node flag=head_tar;
		head_tar=head_tar->next;
		free(flag);
	}
}
int main(int argc,int **argv)
{
	if(argc<2)
	{
		perror("parameter error\n");
		exit(-1);
	}
	char filename[52]="a.out";
	for(int i=0;i<argc;i++)
		if(!strcmp(argv[i],"-o")&&i<argc-1)
			strcpy(filename,argv[i+1]);
	if((mid_file=fopen(argv[1],"r"))==NULL)
	{
		perror("open file filed\n");
		exit(-1);
	}
	if((tar_file=fopen(filename,"w+"))==NULL)
	{
		perror("open file filed\n");
		exit(-1);
	}
	head_tar=(tar_node)malloc(sizeof(struct Tar_node));
	now_tar=head_tar;
	now_tar->next=NULL;
	exchange_code();
	fclose(mid_file);
	fclose(tar_file);
	delete_tar();
	return 0;
}
tar_node search_tar(char *arg)
{
	tar_node flag=head_tar->next;
	while(flag!=NULL)
	{
		if(!strcmp(flag->name,arg))
			return flag;
		flag=flag->next;
	}
	return NULL;
}
void exchange_code()
{
	fprintf(tar_file,"%s\n",".data");
	fprintf(tar_file,"%s\n","_prompt: .asciiz \"Enter an integer:\"");
	fprintf(tar_file,"%s\n","_ret: .asciiz \"\\n\"");
	char code[52];
	char var_code[10][50];
	fgets(code,52,mid_file);
	while(!feof(mid_file))
	{
		if(code[0]=='g')
		{
			sscanf(code,"%s%s%s",var_code,var_code+1,var_code+2);
			fprintf(tar_file,"%s: .space %s\n",var_code+1,var_code+2);
			now_tar->next=(tar_node)malloc(sizeof(struct Tar_node));
			now_tar->next->next=NULL;
			now_tar=now_tar->next;
			strcpy(now_tar->name,var_code+1);
			now_tar->kind=2;
		}
		else
			break;
		fgets(code,52,mid_file);
	}
	
	fprintf(tar_file,".globl %s\n","main");
	fprintf(tar_file,".%s\n","text");
	fprintf(tar_file,"%s:\n","read");
	fprintf(tar_file,"%s\n","	li $v0,4\n	la $a0,_prompt\n	syscall\n	li $v0,5\n	syscall\n	jr $ra\n");
	fprintf(tar_file,"%s:\n","write");
	fprintf(tar_file,"%s\n","	li $v0,1\n	syscall\n	li $v0,4\n	la $a0,_ret\n	syscall\n	move $v0,$0\n	jr $ra\n");
	
	while(!feof(mid_file))
	{
		
		switch((int)(code[0]))
		{
			case 'F':
			{
				para_num=0;
				now_esp=0;
				sscanf(code,"%s%s%s",var_code,var_code+1,var_code+2);
				fprintf(tar_file,"%s:\n",var_code+1);	
				/*if(strcmp(var_code+1,"main"))
				{
					fprintf(tar_file,"	%s\n","addi $sp,$sp,-4");
					fprintf(tar_file,"	%s\n","sw $fp,0($sp)");
				}*/
				fprintf(tar_file,"	%s\n","move $fp,$sp");
				break;
			}	
			case 'P':
			{
				para_num++;
				sscanf(code,"%s%s",var_code,var_code+1);
				now_tar->next=(tar_node)malloc(sizeof(struct Tar_node));
				now_tar->next->next=NULL;
				now_tar=now_tar->next;
				now_tar->kind=1;
				strcpy(now_tar->name,var_code+1);
				now_tar->add=(para_num+1)*4;
				break;
			}
			case 'L':
			{
				sscanf(code,"%s%s",var_code,var_code+1,var_code+2);
				fprintf(tar_file,"	%s:\n",var_code+1);
				break;
			}
			case 'G':
			{
				sscanf(code,"%s%s",var_code,var_code+1);
				fprintf(tar_file,"	j %s\n",var_code+1);
				break;
			}
			case 'W':
			{
				sscanf(code,"%s%s",var_code,var_code+1);
				if(((char*)(var_code+1))[0]=='#')
				{
					fprintf(tar_file,"	li $a0,%s\n",((char*)(var_code+1))+1);
				}
				else if(((char*)(var_code+1))[0]=='*')
				{
					tar_node flag_tem=search_tar(((char*)(var_code+1))+1);
					if(flag_tem->kind==1)
					{
						fprintf(tar_file,"	lw $s0,%d($fp)\n",flag_tem->add);
						fprintf(tar_file,"	lw $a0,0($s0)\n");
					}
					else
					{
						fprintf(tar_file,"	la $s0,%s\n",flag_tem->name);
						fprintf(tar_file,"	lw $a0,0($s0)");
					}
				}
				else if(((char*)(var_code+1))[0]=='&')
				{
					tar_node flag_tem=search_tar(((char*)(var_code+1))+1);
					if(flag_tem->kind==1)
						fprintf(tar_file,"	addi $a0,$fp,%d\n",flag_tem->add);
					else
						fprintf(tar_file,"	la $a0,%s\n",flag_tem->name);
				}
				else
				{
					tar_node flag_tem=search_tar(((char*)(var_code+1)));
					fprintf(tar_file,"	lw $a0,%d($fp)\n",flag_tem->add);
				}
				fprintf(tar_file,"	addi $sp,$sp,-4\n");
				fprintf(tar_file,"	sw $ra,0($sp)\n");
				//fprintf(tar_file,"	addi $sp,$sp,-4\n");
				//fprintf(tar_file,"	sw $fp,0($sp)\n");
				fprintf(tar_file,"	jal write\n");
				//fprintf(tar_file,"	move $sp,$fp\n");
				//fprintf(tar_file,"	lw $fp,0($sp)\n");
				fprintf(tar_file,"	lw $ra,0($sp)\n");
				fprintf(tar_file,"	addi $sp,$sp,4\n");
				break;
			}
			case 'R':
			{
				sscanf(code,"%s%s",var_code,var_code+1);	
				if(!strcmp((char*)(var_code),"READ"))
				{
					tar_node flag_tem=search_tar(var_code+1);
					if(flag_tem==NULL)
					{
						flag_tem=(tar_node)malloc(sizeof(struct Tar_node));
						strcpy(flag_tem->name,var_code+1);
						flag_tem->kind=1;		
						fprintf(tar_file,"	%s\n","addi $sp,$sp,-4");
						now_esp+=4;
						flag_tem->add=-now_esp;
						now_tar->next=flag_tem;
						now_tar=flag_tem;
						now_tar->next=NULL;
					}
					fprintf(tar_file,"	addi $sp,$sp,-4\n");
					fprintf(tar_file,"	sw $ra,0($sp)\n");
					fprintf(tar_file,"	jal read\n");
					fprintf(tar_file,"	lw $ra,0($sp)\n");
					fprintf(tar_file,"	addi $sp,$sp,4\n");
					fprintf(tar_file,"	sw $v0,%d($fp)\n",flag_tem->add);
					break;
					
				}
			}
			//case 'R':
			{	
				if(((char*)(var_code+1))[0]=='#')
					fprintf(tar_file,"	li $v0,%s\n",(char*)(var_code+1)+1);
				else if(((char*)(var_code+1))[0]=='*')
				{
					tar_node flag_tem=search_tar((char*)(var_code+1)+1);
					if(flag_tem->kind==1)
					{
						fprintf(tar_file,"	lw $t0,%d($fp)\n",flag_tem->add);
						fprintf(tar_file,"	lw $v0,%d($t0)\n",0);
					}
					else
					{
						fprintf(tar_file,"	la $t0,%s\n",flag_tem->name);
						fprintf(tar_file,"	lw $v0,%d($t0)\n",0);
					}
				}
				else if(((char*)(var_code+1))[0]=='&')
				{
					tar_node flag_tem=search_tar((char*)(var_code+1)+1);
					if(flag_tem->kind==1)
						fprintf(tar_file,"	addi $v0,&fp,%d\n",flag_tem->add);
					else
						fprintf(tar_file,"	la $v0,%s\n",flag_tem->name);
				}
				else
				{
					tar_node flag_tem=search_tar((char*)(var_code+1));
					if(flag_tem->kind==1)
						fprintf(tar_file,"	lw $v0,%d($fp)\n",flag_tem->add);
					else
						fprintf(tar_file,"	lw $v0,%s\n",flag_tem->name);
				}
				fprintf(tar_file,"	jr %s\n","$ra");
				break;
			}
			case 'A':
			{
				acpara_num++;
				sscanf(code,"%s%s",var_code,var_code+1);
				fprintf(tar_file,"	%s\n","addi $sp,$sp,-4");
				if(((char*)(var_code+1))[0]=='#')
				{
					fprintf(tar_file,"	li $t0,%s\n",(char*)(var_code+1)+1);
					fprintf(tar_file,"	sw %s,0($sp)\n","$t0");
				}
				else if(((char*)(var_code+1))[0]=='*')
				{
					tar_node flag_tem=search_tar((char*)(var_code+1)+1);
					if(flag_tem->kind==1)
					{
						fprintf(tar_file,"	lw $t0,%d($fp)\n",flag_tem->add);
						fprintf(tar_file,"	lw $t0,%d($t0)\n",0);
						fprintf(tar_file,"	sw %s,0($sp)\n","$t0");
					}
					else
					{
						fprintf(tar_file,"	la $t0,%s\n",flag_tem->name);
						fprintf(tar_file,"	sw $t0,%d($t0)\n",0);
						fprintf(tar_file,"	sw %s,0($sp)\n","$t0");
					}
				}
				else if(((char*)(var_code+1))[0]=='&')
				{
					tar_node flag_tem=search_tar((char*)(var_code+1)+1);
					if(flag_tem->kind==1)
					{
						fprintf(tar_file,"	lw $t0,%d($fp)\n",flag_tem->add);
						fprintf(tar_file,"	sw %s,0($sp)\n","$t0");
					}
					else
					{
						fprintf(tar_file,"	la $t0,%s\n",flag_tem->name);
						fprintf(tar_file,"	sw %s,0($sp)\n","$t0");
					}
				}
				else
				{
					tar_node flag_tem=search_tar((char*)(var_code+1));
					if(flag_tem->kind==1)
					{
						fprintf(tar_file,"	lw $t0,%d($fp)\n",flag_tem->add);
						fprintf(tar_file,"	sw %s,0($sp)\n","$t0");
					}
					else
					{
						fprintf(tar_file,"	lw $t0,%s\n",flag_tem->name);
						fprintf(tar_file,"	sw %s,0($sp)\n","$t0");
					}
				}
				break;
				
			}
			case 'D':
			{
				sscanf(code,"%s%s",var_code,var_code+1,var_code+2);
				now_tar->next=(tar_node)malloc(sizeof(struct Tar_node));
				now_tar->next->next=NULL;
				now_tar=now_tar->next;
				strcpy(now_tar->name,var_code+1);
				now_esp+=atoi(var_code+2);
				fprintf(tar_file,"	addi $sp,$sp,-%d\n",atoi(var_code+2));
				now_tar->kind=1;
				now_tar->add=-now_esp;
				break;
			}
			case 'I':
			{
				sscanf(code,"%s%s%s%s%s%s",var_code,var_code+1,var_code+2,var_code+3,var_code+4,var_code+5);
				if(!strcmp(var_code+2,"!="))
				{
					if_func(var_code+1,0);
					if_func(var_code+3,1);
					fprintf(tar_file,"	bne $t0,$t1,%s\n",var_code+5);
				}
				else if(!strcmp(var_code+2,"=="))
				{
					if_func(var_code+1,0);
					if_func(var_code+3,1);
					fprintf(tar_file,"	beq $t0,$t1,%s\n",var_code+5);
				}
				else if(!strcmp(var_code+2,">"))
				{
					if_func(var_code+1,0);
					if_func(var_code+3,1);
					fprintf(tar_file,"	bgt $t0,$t1,%s\n",var_code+5);
				}
				else if(!strcmp(var_code+2,"<"))
				{
					if_func(var_code+1,0);
					if_func(var_code+3,1);
					fprintf(tar_file,"	blt $t0,$t1,%s\n",var_code+5);
				}
				else if(!strcmp(var_code+2,">="))
				{
					if_func(var_code+1,0);
					if_func(var_code+3,1);
					fprintf(tar_file,"	bge $t0,$t1,%s\n",var_code+5);
				}
				else if(!strcmp(var_code+2,"<="))
				{
					if_func(var_code+1,0);
					if_func(var_code+3,1);
					fprintf(tar_file,"	ble $t0,$t1,%s\n",var_code+5);
				}
				break;
			}
			default:
			{
				int num_chr=count_chr(code);
				if(num_chr==3)				
				{
					sscanf(code,"%s%s%s",var_code,var_code+1,var_code+2);
					if_func(var_code+2,0);
					if(((char*)var_code)[0]=='*')
					{
						tar_node flag_tem=search_tar(((char*)var_code)+1);
						if(flag_tem->kind==1)
							fprintf(tar_file,"	sw $t0,%d($fp)\n",flag_tem->add);
						else
							fprintf(tar_file,"	sw $t0,%s\n",flag_tem->name);
					}
					else
					{
						tar_node flag_tem=search_tar(((char*)var_code));
						if(flag_tem==NULL)
						{
							flag_tem=(tar_node)malloc(sizeof(struct Tar_node));
							strcpy(flag_tem->name,var_code);
							flag_tem->kind=1;
							
							fprintf(tar_file,"	%s\n","addi $sp,$sp,-4");
							now_esp+=4;
							flag_tem->add=-now_esp;
							now_tar->next=flag_tem;
							now_tar=flag_tem;
							now_tar->next=NULL;
						}
						if(flag_tem->kind==1)
							fprintf(tar_file,"	sw $t0,%d($fp)\n",flag_tem->add);
						else
							fprintf(tar_file,"	sw $t0,%s\n",flag_tem->name);
					}
				}
				else if(num_chr==5)
				{
					sscanf(code,"%s%s%s%s%s",var_code,var_code+1,var_code+2,var_code+3,var_code+4);
					if_func(var_code+2,0);
					if_func(var_code+4,1);
					char type[6];
					if(!strcmp(var_code+3,"/"))
						strcpy(type,"div");
					else if(!strcmp(var_code+3,"+"))
						strcpy(type,"add");
					else if(!strcmp(var_code+3,"-"))
						strcpy(type,"sub");
					else
						strcpy(type,"mul");
					calculate_tar(var_code,type);
				}
				else if(num_chr==4)
				{
					sscanf(code,"%s%s%s%s",var_code,var_code+1,var_code+2,var_code+3);
					fprintf(tar_file,"	addi $sp,$sp,-4\n");
					fprintf(tar_file,"	sw $ra,0($sp)\n");
					fprintf(tar_file,"	addi $sp,$sp,-4\n");
					fprintf(tar_file,"	sw $fp,0($sp)\n");
					fprintf(tar_file,"	jal %s\n",var_code+3);
					fprintf(tar_file,"	move $sp,$fp\n");
					fprintf(tar_file,"	lw $fp,0($sp)\n");
					fprintf(tar_file,"	lw $ra,4($sp)\n");
					fprintf(tar_file,"	addi $sp,$sp,%d\n",acpara_num*4+8);
					if(((char*)var_code)[0]=='*')
					{
						tar_node flag_tem=search_tar(((char*)var_code)+1);
						if(flag_tem->kind==1)
							fprintf(tar_file,"	sw $v0,%d($fp)\n",flag_tem->add);
						else
							fprintf(tar_file,"	sw $v0,%s\n",flag_tem->name);
					}
					else
					{
						tar_node flag_tem=search_tar(((char*)var_code)+1);
						if(flag_tem==NULL)
						{
							flag_tem=(tar_node)malloc(sizeof(struct Tar_node));
							strcpy(flag_tem->name,var_code);
							flag_tem->kind=1;		
							fprintf(tar_file,"	%s\n","addi $sp,$sp,-4");
							now_esp+=4;
							flag_tem->add=-now_esp;
							now_tar->next=flag_tem;
							now_tar=flag_tem;
							now_tar->next=NULL;
						}
						fprintf(tar_file,"	sw $v0,%d($fp)\n",flag_tem->add);
					}
				}
			}
		};
			fgets(code,52,mid_file);
	}
}
void if_func(char* arg,int n)
{
	if(arg[0]=='#')
		fprintf(tar_file,"	li $t%d,%s\n",n,arg+1);
	else if(arg[0]=='&')
	{
		tar_node flag_tem=search_tar(arg+1);
		if(flag_tem->kind==1)
			fprintf(tar_file,"	addi $t%d,$fp,%d\n",n,flag_tem->add);
		else
			fprintf(tar_file,"	la $t%d,%s\n",n,flag_tem->name);
	}
	else if(arg[0]=='*')
	{
		tar_node flag_tem=search_tar(arg+1);
		if(flag_tem->kind==1)
		{
			fprintf(tar_file,"	lw $s0,%d($fp)\n",flag_tem->add);
			fprintf(tar_file,"	lw $t%d,0($s0)\n",n);
		}
		else
		{
			fprintf(tar_file,"	la $s0,%s\n",flag_tem->name);
			fprintf(tar_file,"	lw $t%d,0($s0)",n);
		}
	}
	else
	{
		tar_node flag_tem=search_tar(arg);
		fprintf(tar_file,"	lw $t%d,%d($fp)\n",n,flag_tem->add);
	}
}
int count_chr(char* arg)
{
	int num=1;
	int i=0;
	int l=strlen(arg);
	while(i<l-1)
	{
		if((arg[i]==' ')&&(i<(l-2)))
			num++;
		i++;
	}
	return num;
}
void calculate_tar(char* arg1,char *arg4)
{
	if(arg1[0]=='*')
	{
		tar_node flag_tem=search_tar(arg1+1);
		if(!strcmp(arg4,"div"))
		{
			fprintf(tar_file,"	%s $t0,$t1\n","div");
			fprintf(tar_file,"	mlfo $s0\n");
			if(flag_tem->kind==1)
				fprintf(tar_file,"	sw $s0,%d($fp)\n",flag_tem->add);
			else
				fprintf(tar_file,"	sw $s0,%s\n",flag_tem->name);
		}
		else
		{
			fprintf(tar_file,"	%s $s0,$t0,$t1\n",arg4);
			if(flag_tem->kind==1)
				fprintf(tar_file,"	sw $s0,%d($fp)\n",flag_tem->add);
			else
				fprintf(tar_file,"	sw $s0,%s\n",flag_tem->name);
		}
	}
	else
	{
		tar_node flag_tem=search_tar(arg1);
		if(flag_tem==NULL)
		{
			flag_tem=(tar_node)malloc(sizeof(struct Tar_node));
			strcpy(flag_tem->name,arg1);
			flag_tem->kind=1;		
			fprintf(tar_file,"	%s\n","addi $sp,$sp,-4");
			now_esp+=4;
			flag_tem->add=-now_esp;
			now_tar->next=flag_tem;
			now_tar=flag_tem;
			now_tar->next=NULL;
		}
		if(!strcmp(arg4,"div"))
		{
			fprintf(tar_file,"	%s $t0,$t1\n","div");
			fprintf(tar_file,"	mlfo $s0\n");
			if(flag_tem->kind==1)
				fprintf(tar_file,"	sw $s0,%d($fp)\n",flag_tem->add);
			else
				fprintf(tar_file,"	sw $s0,%s\n",flag_tem->name);
		}
		else
		{
			fprintf(tar_file,"	%s $s0,$t0,$t1\n",arg4);
			if(flag_tem->kind==1)
				fprintf(tar_file,"	sw $s0,%d($fp)\n",flag_tem->add);
			else
				fprintf(tar_file,"	sw $s0,%s\n",flag_tem->name);
		}
	}
}
