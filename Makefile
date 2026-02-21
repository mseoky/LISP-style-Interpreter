all: main

main: y.tab.c lex.yy.c data.o env.o eval.o bop.o vop.o main.o
	gcc y.tab.c lex.yy.c data.o env.o eval.o bop.o vop.o main.o -o main

y.tab.c y.tab.h: ast.y data.h env.h eval.h
	bison -dy ast.y

lex.yy.c: token.l y.tab.h data.h
	flex token.l

data.o: data.c data.h
	gcc -c data.c

env.o: env.c env.h data.h
	gcc -c env.c

eval.o: eval.c eval.h env.h data.h
	gcc -c eval.c

bop.o: bop.c bop.h data.h
	gcc -c bop.c

vop.o: vop.c vop.h data.h
	gcc -c vop.c

main.o: main.c data.h env.h eval.h bop.h vop.h
	gcc -c main.c

test: main
	chmod +x run_demo.sh
	./run_demo.sh

clean:
	rm -f main main.exe y.tab.c y.tab.h lex.yy.c *.o main.zip

zip:
	zip main.zip Makefile README ast.y token.l \
	data.c data.h env.c env.h eval.c eval.h \
	bop.c bop.h vop.c vop.h main.c \
	*.lsp run_demo.sh
