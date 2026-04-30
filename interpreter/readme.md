# Interpreter example 2026

## Lexis
- numbers are variables
- numbers with leading zeros are function names
- numbers with _ (underscore) prefix are just numbers
- standalone underscore is dereference
- quote for borders of function definition
- math and logic operations + - * / = < > & | 
- unary operation !
- keywords 
  - @ is loop
  - \# is if
  - $ is return
  - % is write
  - ~ is read
- endline is separator
- EOF is end of program

## Syntax
\<program\> ::= (endline | \<function definition\>) \<program\> | \<main\>

\<function definition\> ::= quote function-name endline \<function body\> endline

\<function body\> ::= quote | \<line\> endline \<function body\>

\<main\> ::= EOF | (\<line\> endline \<main\>)

\<line\> ::= \<assignment\> | \<function call\> | \<loop\> | \<if\> | \<return\> | \<write\> | \<read\> 

\<assignment\> ::= var-name (\<function call\> | \<expression\>) 

\<function call\> ::= function-name \<args\>

\<args\> ::= endline | (\<value\> \<args\> )

\<value\> ::= number | var-name

\<expression\> ::= (\<value\> | unary-math-op \<value\>) (endline | binary-math-op \<expression\>)

\<loop\> ::= loop-keyword \<function call\>

\<if\> ::= if-keyword \<value\> (\<function call\> | \<assignment\> | \<return\>)

\<return\> ::= return-keyword (\<value\> | \<function call\>)

\<write\> ::= write-keyword (\<value\>)

\<read\> ::= read-keyword (\<value\>)