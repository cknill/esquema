# Esquema

It's Spanish for scheme, or in this case Scheme.  Say the letters 'S' 'K' and then Ma with stress on 'K'. This is a *muy*, *muy*, *muy*, simple implementation of a calculator that uses the usual Lispy syntax. Think of it as an over-glorified reverse Polish notation calculator with some additional features. I wrote this just as a code sample for job interviews, please for *el amor de dios*, fork this at your own risk and please don't submit pull requests because I won't be looking.

## A sample

    esquema> 2
    2
    esquema> (+ 40 2)
    42
    esquema> (define answer (+ 40 2))
    Nil
    esquema> answer
    42
    esquema> 
You can escape from the loop by typing Ctrl + C (I don't remember what that is in Mac)

## Compiling from source
As if there's any other way to get programs.  You will need a C++ compiler that speaks C++20, sorry, but as of writing this it's 2024 going on 2025.  Time to upgrade.  You will also need Git, and an internet connection, which if you're getting the source then that shouldn't be an issue. Esquema uses CMake to generate builds. Make sure you've got at least version 3.18.

Find a cozy place in your filesystem and type the following:

    git clone https://cknill/esquema.git
    cd esquema
    mkdir build
    cd build
    
Now you're ready to configure the build

    cmake -GNinja -DCMAKE_BUILD_TYPE=Release -DESQUEMA_BUILD_TESTS=Off -DCMAKE_INSTALL_PREFIX=/usr/local

You don't have to use Ninja, but if you have it you certainly can. By default Esquema builds the tests and it uses googletest for no specific reason. If you like to ship to production on Fridays then set ESQUEMA_BUILD_TESTS=Off and then *vaya con dios*.

Now type this and marvel:

    cmake --build . --parallel

The parallel is for not-Ninja builds but it doesn't hurt with Ninja builds either. After a few minutes hopefully the compiler has not spewed on you. If so, throw this defective product away, I am so sorry for having wasted your time.

Testing the binary you should see something akin to:

    ./esquema
    esquema> 42
    42
    esquema> 
If you're happy with the purchase and you want to keep this little binary, issue this command:

    cmake --build . --target install

you may have to do this if you're installing somewhere official

    sudo cmake -build . --target install
Be sure that your OS knows how to find Esquema or else it just won't work.  Do some web searches about setting your Path variable if you aren't sure.

## How Esquema compares to Scheme
It doesn't.  Esquema isn't a fully fledged Scheme interpreter, that would be awfully *chismoso* of me. Esquema does some of the things Scheme does and that's good enough.
 
### Primitive Types
Also known as atoms, these are irreducible in the interpreter. If you type them in you get them right back.
| Name | Example |Description|
|---------|-----------|------|
|Bool     |#t, #f, #T, #F|True or False|
|Symbol|x, y, The-Answer, +, <=|These are names that can resolve to variables or built-in procedures|
|Number|3.14, -.1, +4|What we humans consider numbers|

### Compound Types
Just the list for Esquema, Scheme has more and I'm really proud of Scheme for that. Lists are also how we group operations on either atoms or other lists e.g.

   esquema> (+ (+ 2 2) (- 2 2))

The outer most list has two members that are lists and one member that is a symbol. It's the same for the inner lists.  The interpreter will reduce each inner list to an atom and then use those atoms to reduce the outer list. The symbols map to a primitive operation that is applied to the atoms.

### Primitive Operations
Here's a listing of the primitive (read builtin) operations that the Esquema interpreter knows about
|Symbols|Arguments|Returns|Description|
|----|---------|-------|-------|
|+,-,*,/|A list of elements that evaluate to numbers|A number|The usual simple arithmetic operators addition, subtraction, multiplication, and division|
|<, <=, >, >=|A list of at least two elements that evaluates to numbers|A boolean|The usual relational operators. Esquema will start with the first two elements compare them and then do pairwise comparisons until something evaluates to false otherwise returns true|
|not|A single thing|A boolean|Any type in Scheme can be an argument for not. Non-boolean values always evaluate to true so applying not to them returns false|
|eqv?|Two things|A boolean|Scheme has a complicated way to calculate if two things are equivalent. [Go here](https://conservatory.scheme.org/schemers/Documents/Standards/R5RS/HTML/r5rs-Z-H-9.html#%_sec_6.1) to see all that. Esquema attempts to do it.|
|pi and e|Nothing|A number|Not procedures but rather the mathematical constants|
|define|A symbol and a value|Nil|Binds the symbol to the value and then you can use the symbol as a synonym for the value|
|begin|A non-empty list|The last element of that list|Evaluates each member of the list and then returns the last element|
|if|A condition that evaluates to a boolean, an argument to evaluate on true and optionally an argument to evaluate on false|When true the true argument, when false and there's a false argument that false argument otherwise Nil|It's the classic if statement, except now it's an expression so you can use it in operations and store it|

## Miscellanea 
I built and tested Esquema on Linux Mint 23 with gcc 13.1.0. I used cmake version 3.22.1.  I used cpp-linenoise to do the REPL because it was a happy C++ wrapper of liblinenoise.  As I have stated earlier this is only meant as a code sample for prospective employers, so I won't be looking at PRs I have no doubt that there are plenty of bugs, defects, and poor design decisions. Fork at your own risk, and please don't laugh too hard at my C++. I do what I can.

