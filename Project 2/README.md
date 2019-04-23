# Operating-systems-project-2-starter
[![Python Status](https://img.shields.io/pypi/pyversions/django.svg)](https://veveusa.com)
[![Build Status](https://travis-ci.org/Microsoft/TypeScript.svg?branch=master)](https://veveusa.com)
[![VSTS Build Status](https://dev.azure.com/typescript/TypeScript/_apis/build/status/Typescript/node10)](https://veveusa.com) 
[![GitHub license](https://img.shields.io/badge/license-MIT-lightgrey.svg)](https://veveusa.com)
[![Logo](https://cl.ly/ae0979e60b70/Image%202019-03-20%20at%201.15.02%20PM.png)](https://trujamal.com)

CS4328 Project 2

## Installation
We recommend you clone the repo for easiest implementation:

```
git clone https://github.com/trujamal/operating-systems.git
```

Navigate to the Project 2 folder within the repository

How to run the file os_project_2.cpp:
1. On a Linux command line, run the following command  
    
```
gcc os_project_2.cpp -lpthread
```

2. Once that is proccessed then run the next following command

```
    ./a.out 1
```

3. Repalce the number one with any  number as that is your random seed number

### Detailed  

1. Open an instance of the Linux command line
2. Change to the directory that contains the file os_project_2.cpp
3. Type the following on the command line: gcc os_project_2.cpp -lpthread 
4. Press 'Enter' 
    * the program will compile and produce a new file in the directory
    * the new file will be called a.out 
5. Type the following on the command line: ./a.out #
    * where # is a non-negative integer value 
    * the value will be used to seed rand() to produce a random card deck
    * Example 1: ./a.out 123
    * Example 2: ./a.out 5
    * (note: if you do not provide an integer, the program will use 0 by default)
6. Press 'Enter'
    * the program will run
    * results will print to the console
    * a log file called 'report.txt' will appear in the directory

## Usage

Check the section above regarding the different conditions of the command line arguments.

### Setup Keys
The only required setup keys are just regarding compiling the code using the gcc compiler, if for any reason you don't have that compiler click [here](https://gcc.gnu.org/releases.html)


### Use the whole directory as needed
If you find a better solution to what we have created weather in C++, or python, or even java create a pull request and we will add it to the repository, to keep expanding this concenpt known as the Sauce.

[![Sauce It Up](https://66.media.tumblr.com/97ea15cac4dd904859e7dd8fdaeb6149/tumblr_ncaqtbSLeA1qj8u1do6_r1_400.gif)](https://veveusa.com)

## Requirements
* Unix machine
* Windows Machine
* MacOS Machine

## Documentation

Take a look at the [documentation table of contents](dist/doc/TOC.md).
This documentation is bundled with the project which makes it
available for offline reading and provides a useful starting point for
any documentation you want to write about your project.


## License

The code is available under the [MIT license](LICENSE.txt).
