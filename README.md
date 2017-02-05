# c--Complier
A complier of a programming language which is similar to c language
# directory
	|Lexical_GrammerAnalyse
		|c--.l
		|c--.y
		|display.c
		|makefile
		|node.h
	|Semantic_Analyse
		|array.c
		|domain_list.c
		|func_list.c
		|gtree.c
		|hash.c
		|other.c
		|struct_list.c
	|Intermediate_CodeCreate
		|intermedite_code_create.c
	|Object_CodeCreate
		|Object_code.c
	|Test
		|test.c--
	|makefile
	|README.md
# usage
	running "make" to compile the project,then you can find g-- and t-- in directory "Test".
	changing the directory to Test,you can use the following command:
		./g-- inputfile [-o outputfile] 
	(it's used to generate the intermediate code.if you want to see grammer tree ,you can add the "-d" option.AND
	if want to specify the name of outputfile("a.tem"default),you can add the "-o filename" option.)
		./t-- inputfile [-o outputfile]
	(it's used to convert the intermediate code to object code,using "-o filename" to specify the name of 
	outputfile("a.out"default).)
	But if you want to generate the object code directly by using g--,you can use "-d 1" option.For example:
	./g-- inputfile -d 1.In this case,the default name of object file is "a.tem".
	
	
