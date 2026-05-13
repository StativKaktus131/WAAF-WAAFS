<img width="295" height="230" alt="waaf_downscaled" src="https://github.com/user-attachments/assets/b09c37cf-5542-457e-b23c-8cc196ae5574" />

# WAAF Programming Language

WAAF is a programming language, which enables you to write simple code to analyze and manipulate .wav files.

## Supported formats

WAAF supports 16-, 24-, and 32-bit PCM wav files. Important: the your file is loaded onto the heap, watch out for big filesizes.

## Compilation and Running

Because I don't know how to use make and create makefiles, it's possible to compile this project with the gcc compiler like this:

_Shell:_
```shell
util_files="src/util/dbginfo.c src/util/strutils.c src/util/tinyexpr.c"
files="src/chunk.c src/instrhelp.c src/main.c src/sound.c src/variable.c src/wii.c"

gcc -o waaf $util_files $files -Iinclude
```

_Batch (not tested):_
```bat
set util_files = "src/util/dbginfo.c src/util/strutils.c src/util/tinyexpr.c"
set files = "src/chunk.c src/instrhelp.c src/main.c src/sound.c src/variable.c src/wii.c"

gcc -o waaf %util_files% %files% -Iinclude
```

To run your program you pass the arguments like so:
```
./waaf <Source File> <Output File> <WAAF File>
```


## Code
The WAAF interpreter generally works with single character instructions.

### Environment Variables
The WAAF programming language let's you retrieve data with multiple environment variables. To retrieve them you simply write them into expressions.
* **#DATA**: The given byte / sample at the current position
* **#N_SAMPLES**: The maximum number of samples in the file
* **#STEP**: The current step / position.
* **#PROGRESS**: The current progress through the file (from 0.0 to 1.0)
* **#BEAT**: The current beat (only works in sample mode and if bpm is defined, see 'SET')
* **#RANDOM**: A random integer value
* **#RANDOM_FLOAT**: A random Float value (from 0.0 to 1.0)


### Precompiler

You start a WAAF file with "/ WAAF" and end it with "/ ENDWAAF". There are different Precompiler flags you can change with the '/' instruction, such as:
* **ONCE**: only makes the code run once. Great for general console tests (Hello World)
* **MODE, [value]**: Changes the mode from byte manipulation to sample manipulation. Available values are: _BYTE, SAMPLE_MONO, SAMPLE_STEREO_. More on this later.


### Comments

Any lines that don't start with an instruction character, or closing curly bracket, are treated as comments.

To make readability of the file easier, by convention you would start a comment with '"' and close it as well.

```
this is a comment
" this is a comment as well
" this, also, is a comment "
```


### Set instruction

The character for the set instruction is '>'. You use it to either manipulate data or variables.
The schematic is

```
> target, value
```

To create variables, you declare the name with a preceding '$' and initialise it with the following value.
To manipulate data, you put 'DATA' as the target and feed it the given value.
To set the bpm of a track, you put 'BPM' as the target. It's important to note, that the bpm feature only works in sample mode.

**For example:**

```
> $foo, #DATA
> DATA, $foo
```
This code creates a variable called foo and initialises it with the current data value. Then it overwrites the data value with the value of the $foo variable.

This code does nothing.


### Conditions

A way to add conditions to your WAAF program is with the '?' instruction.

At this point in time, it's only possible to do one conditional check per instruction. 

All possible comparison operators are: _==, !=, >, <, >=, <=_ and follow the same logic as in other programming languages.

**For example:**
```
? #RANDOM_FLOAT < 0.5 {
    > DATA, 0
}
```
This code checks if a random float is greater than 0.5, so theres a 50% chance, that the current byte is flipped to zero.

This code results in a bit-crusher effect.


### Print instructions

There are two different type of print instructions, '.' and ':'. The dot prints a String to the console, while the double colon prints the evaluation of the given expression.

Print instructions slow the program down heavily, so it's best to either only use them in the Startup method or in combination with the "/ ONCE" precompiler flag

**For example:**
```
. Hello World!
. #DATA
: #DATA

> $foo, 5
. $foo * 2
: $foo * 2
```

This code prints (on every frame):
* "_Hello World!_" as a string
* "#DATA" as a string
* "123" (e.g. if #DATA evaluates to 123)
* "5 * 2" (because user variables concatenate into a string)
* "10" (because 5 * 2 evaluates to 10)


### Methods

You can declare methods with the '@' instruction and call them with the '!' instruction

To declare a method, you follow the '@' instruction with the method name and open a new block.

**For example:**
```
! test

@ test {
    . This is a test
}
```

This code declares a method called test and calls it, printing "This is a test" to the console. For demonstration purposes, the method is declared _after_ it is "called" in the code, because the precompiler gathers all methods and puts them a method register before the code actually runs.

The method name _STARTUP_ is reserved for the startup method that is called when the code runs for the first time. This is useful for creating "global" variables.

```
@ STARTUP {
    . This is the startup method
    > $foo, #RANDOM
}

. $foo
```

This code creates the _$foo_ variable and assigns it a random value, then for each step it prints exactly this value out (without reassigning it).


### Loops

You can define loops with the 'x' instruction, followed by the amount of times the loop runs.

```
> $i, 0

x 20 {
    > $i, $i + 1
    . This is iteration no.: $i
}
```

This code runs 20 times and increases the _$i_ variable by 1 each iteration.