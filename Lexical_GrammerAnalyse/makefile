c--.tab.o : lex.yy.c c--.tab.c
	cc -c c--.tab.c -o ../c--.tab.o -ll -ly -w
c--.tab.c : c--.y
	bison -d c--.y
	flex c--.l
lex.yy.c : c--.l
	flex c--.l
clean : 
	rm lex.yy.c c--.tab.c
