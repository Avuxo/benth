# Benth

![Benth logo](https://i.imgur.com/6dPBL2p.png)

## A compiled minimalist FORTH derivative taken to the benth degree to target the BVM

Benth (like FORTH) is a stack-based, concatenative programming language that compiles down to [BVM](https://github.com/avuxo/bvm) bytecode.

### Programs

Benth programs are made of whitespace-seperated calls to BVM bytecode. Programs are written using "reverse polish notation", where first values are passed onto the stack and then an operator is called which then uses the values on the stack. To add 3 and 5 in standard mathematical notaiton, you'd write `3 + 5`. However, in RPN you would pass the numbers first and then pass the mathematical operator, so it'd be `3 5 +` (which is valid Benth code).

Programs are written in postfix notation by passing "words" onto the stack. A word is effectively a macro that operates as a subroutine. There are pre-defined words such as the simple arithmetic functions `+`,`-`,`*`, etc. However, you can define words yourself using the `: ;` syntax. Example: `: add-two 2 + ;` is a program that adds 2 to whatever value is on the top of the stack.

You can use words as functions by passing the parameters onto the stack before calling a function like in the above example.

In Benth, newlines are ignored by the compiler, so words can stretch over multiple lines and can be indented as you please.

Some example code can be seen in the examples/ folder of this repository (Benth programs end in .bnth).



### Styling

Words should be short and concise. They should be no longer than a line in most cases to make them more readable. To make it easier to understand your code, you should add a short comment after the name labeling the parameters and return value of the function. Using the previous example of `add-two` you would change it to `add-two (n -- n) 2 + ;`

Multi-line words should be written with only the first line and the parameter comment on the same line as the `:` Example:
```
: mul-and-add (n1 n2 n3 -- res)
  *
  +
;
```
  