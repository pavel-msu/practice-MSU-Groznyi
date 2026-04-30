lexer.o:
	g++ -c -g lexer.cpp -std=gnu++17

lexer: lexer.o
	g++ -o lexer lexer.o 

parser.o:
	g++ -c -g parser.cpp -std=gnu++17

parser: parser.o
	g++ -o parser parser.o 

test-lexer: lexer
	mkdir -p test-results
	./lexer < 'example/p1.c$$' > test-results/p1-lex.txt

test-parser: parser test-lexer
	./parser < test-results/p1-lex.txt > test-results/p1-prs.txt

clean:
	rm -f foo.o lexer.o lexer parser parser.o
