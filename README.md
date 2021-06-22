# DocLang
An esoteric programming language intended to run on an arm11 processor.

## Usage
* script.sh: compiles and runs a specified DocLang source code file. (Requires access to the arm11 emulator not found in this repo).
* start.sh: runs the testserver for various DocLang source files.

## Documentation

Although this language is limited in it's functionality as it runs on the arm11 emulator we made, here are some of it's defining features:

* Typing

> **integer** and **string** are the only two supported types.

* Variables

> (var_name) is obviosuly (integer). Stores an integer in a variable.
> (var_name) is trivially (string). Stores a string in a variable.

* Functions

``` 
shell_(func_name) (type) ((type var1, type var2, ..., type varx)). Defines a function which can takes x arguments. Arguments must be surrounded by parenthesis.
OH BABY! (var_name/integer/string). Returns from the function, returning a value if one is given.
ssh shell_(func_name)@(var_name) (arg1, arg2, ..., argx). Calls a function, storing the value returned in var_name.
```
## Examples
