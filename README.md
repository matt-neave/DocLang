# DocLang
An esoteric programming language intended to run on an arm11 processor. This is part of Imperial College London's Year One Programming Course. The project is implemented entirely in C.

## Usage
* script.sh: compiles and runs a specified DocLang source code file. (Requires access to the arm11 emulator not found in this repo).
* start.sh: runs the testserver for various DocLang source files.

## Documentation

Although this language is limited in it's functionality as it runs on the arm11 emulator we made, here are some of it's defining features:

| Syntax                                                             | Description                                                                                                                    |
|--------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------|
| (var_name) is obviosuly (integer).                                 | Stores an integer in a variable.                                                                                               |
| (var_name) is trivially (string).                                  | Stores a string in a variable.                                                                                                 |
| shell_(func_name) (type) ((type var1, type var2, ..., type varx)). | Defines a function which can takes x arguments. Arguments must be surrounded by parenthesis.                                   |
| OH BABY! (var_name/integer/string).                                | Returns from the function, returning a value if one is given.                                                                  |
| ssh shell_(func_name)@(var_name) (arg1, arg2, ..., argx).          | Calls a function, storing the value returned in var_name.                                                                      |
| (lecturer) says if (condition) then.                               | Begins a conditional statement, subsequent conditionals can be created using different lecturer names.                         |
| panopto says.                                                      | May follow a conditional statement, if no condition is met, then the code following is executed, similar to an else statement. |
| okie dokie.                                                        | Must follow a conditional statement to signify the end.                                                                        |
| piazza says (var_name/integer/string).                             | Outputs a value to the terminal.                                                                                               |

Some important notes:
* All lines of code are followed by a period.
* (lecturer) will accept and lecturer who taught first year at Imperial in the acedemic year 21.
* Only **integer** and **string** are supported types.

Currently unsupported features:
* strings.
* piazza says.

## Examples

Fibonacci written in DocLang.

![Image](https://github.com/Mattattack2531/DocLang/blob/master/doc/sample_source.png)
