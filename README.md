# Scientific Calculator

## 1. Demo

## 2. Introduction

This is the course project for `Data Structure`.

In this project, I designed a text-parsing framework that is able to handle most of the usual operators and functions in the mathematical expressions and calculate the value of the expressions.

The general text-parsing process of my scientific calculator is as follows:

To match the strings/characters of the operators and the function names.
In my scientific calculator, there is a built-in binary tree whose leaf nodes store all information for the supported functions. If the name matching process is failed, an exception will be thrown and the information of the gramatical error will be printed on the screen. If the name matching process is successful, then we will get the leaf node that store the information of the corresponding function.

To get the parameters according to the type of the function.
The leaf nodes store two key components: the type of the function and the function pointer. We learned how many parameters a function takes and the types of the parameters (double or int) from the type of the function. For example, int (int,int) is a type of the function, which means that the function takes two integer parameters and returns an integer as the result. It is obvious that all the functions of the same type share the same parsing process.

After we get the type of the function from the leaf nodes, it is the function getParameters()'s duty to get the parameters of the function. The function getParameters() will take care of the comma-delimited numbers in the brackets and check the type and number of the parameters.

To calculate the value. Since the function getParameters() have got all the needed parameters of the function, it is easy to pass the parameters to the function and call the function using the function pointer. Hence, we are able to calculate the value of the function.

Recurrence. By 1 to 3 we are able to calculate the value of a sub-expression. Repeat the process 1~3 recurrently until we get the value of the whole expression.

As long as we give the correct type and pointer of the function and rebuild the binary tree, the calculator is able to process arbitrary self-defined functions.

In the advanced edition, I tried to implement the functions of numerical integration and statistics. For example, we can input function_name(lower_bound:upper_bound:step) to calculate the numerical integration approximately and we can input sd/var/avg(variable1,variable2,...) to calculate the standard deviation/variance/average of the data.
