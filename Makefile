lexer.o:
	g++ -c -g lexer.cpp

lexer: lexer.o
	g++ -o lexer lexer.o

test-lexer: lexer
	mkdir -p lexer-test-results
	./lexer < 'example/p1.c$$' > lexer-test-results/p1.txt

clean:
	rm -f foo.o lexer.o lexer
