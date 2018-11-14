# Data-Structure-Project-2

This is a calculator application named mycalc to evaluate multiple arithmetic expressions. 
The expressions, provided through a text file, are to be processed for correctness, get evaluated.

## Prerequisites

All the files including Makefile, mycalc.cpp, operator.cpp, operator.h, and expression files should be in the same directory
Sorted words in dictionary files, each word per line.

## Getting Started

This instruction will get you a copy of the project up and running on your local machine for development and testing purposes. 
./mycalc -i <inputfile> -o <outputfile>>
The following is an example of running the program via command line.

## Algorithmic Analysis

In the Main.() function, reading from the expression file has O(1) running time.
In the Operator class, postfix evaluation via stacks has O(n) running time. 

## Authors

* **Julie Liu** 
* **Romeno Wenogk Fernando** 

## Acknowledgments

* The project is finished under the guidance of Professor Stavros Kolliopoulos and instructor Khalid Mengal in New York University Abu Dhabi Computer Science Department

