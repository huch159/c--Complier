ALL : g-- t--
.PHONY : ALL
CFLAGS  = -I Lexical_GrammerAnalyse
VPATH = Semantic_Analysis Intermediate_CodeCreate Lexical_GrammerAnalyse Object_CodeCreate
IntermediateObject = c--.tab.o display.o domain_list.o func_list.o struct_list.o hash.o array.o other.o gtree.o Intermediate_code_create.o
Object = Object_code.o
g-- : $(IntermediateObject)
	cc $(CFLAGS) -o Test/g-- $(IntermediateObject) -ll -ly;
t-- : $(Object)
	cc $(CFLAGS) -o Test/t-- $(Object)
c--.tab.o : Lexical_GrammerAnalyse/c--.y Lexical_GrammerAnalyse/c--.l
	$(MAKE) -C Lexical_GrammerAnalyse
clean:
	rm *.o Test/g-- Test/t-- Lexical_GrammerAnalyse/lex.yy.c Lexical_GrammerAnalyse/c--.tab.c Lexical_GrammerAnalyse/c--.tab.h
