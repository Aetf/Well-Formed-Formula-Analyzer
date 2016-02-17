# Propositional logic calculator

This is a little program which can calculate simple [propositional logic](https://en.wikipedia.org/wiki/Propositional_calculus)
statements and check whether a statement is valid.
If so, it will print the truth table and generate [Disjunctive Normal Form](https://en.wikipedia.org/wiki/Disjunctive_normal_form) and [Conjunctive Normal Form](https://en.wikipedia.org/wiki/Conjunctive_normal_form) of the statement.

I am sure there must be planty of mistakes or bugs in my code, so please let me know it if you kindly tested my program and found something wrong!

### Compile
Type the following into your shell, and after the compilation, you'll get a single executable called `wffanalyze`.

```shell
$ git clone https://github.com/Aetf/Well-Formed-Formula-Analyzer.git
$ cd Well-Formed-Formula-Analyzer
$ mkdir build && cd build
$ cmake ..
$ make
```

### Usage
Pass in the propositional statement as command line argument. The program will analyze it and print relevant informations about it. There are many variations on logical notation. The following table shows the notation this analyzer accepts.

| meaning | notation |
|---|---|
| propositional variable | One letter, e.g. P, Q, x, y |
| logical NOT | !P |
| logical AND | P&&Q |
| logical OR  | P||Q |
| logical implication | P->Q |
| logical bi-implication | P<->Q |
| parenthesis | (P||Q) |

###### Note
- No space is allowed in the statements
- There should be no more than 32 different variables in one statement
- You must quote the statement in the command line or the shell will screw things up

###### Example
```shell
$ ./wffanalyze '!((Q->R)||(Q->P))<->!(!(P||R)||Q)'
The expression is valid. Print the truth table:
P  Q  R  !((Q->R)||(Q->P))<->!(!(P||R)||Q)
F  F  F  T
T  F  F  F
F  T  F  F
T  T  F  T
F  F  T  F
T  F  T  F
F  T  T  T
T  T  T  T
The main DNF is:
(!P&&!Q&&!R)||(P&&Q&&!R)||(!P&&Q&&R)||(P&&Q&&R)
The main CNF is:
(!P||Q||R)&&(P||!Q||R)&&(P||Q||!R)&&(!P||Q||!R)
```

### Version History
- 2016-2: major rewrite and fixed various bugs
- 2012-3: initial version

### Story
I wrote this to practice what I have learned in my Discrete Mathematics course during my undergraduate and to improve my C++ programming skills. The project was abandoned for a long time after that course. But then I'm back and find it useful in my AI course which also involves propositional logic. It's quite interesting to review the code you wrote years ago, especally when it was the first project I had on GitHub, and to see how things changed. I used to hate spaces in the code and it's really odds now reading those 'compact' code :-).

On the other hand, of course, I've gained much more understanding over the years about the whole algorithm and the language therefore I'm able to improve it now. Thus a major rewrite. There are also many fixes for bugs I found when using this little program to do my AI homework.

I'm not sure where this project will go. But it did witness my growth as a programmer. And I'm looking forward to see what this will look like when I return to it some years later.