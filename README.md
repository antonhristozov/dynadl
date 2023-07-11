# Dynamic Architecture Description Language

            _                           _____   _
           | |                   /\    |  __ \ | |
         __| | _   _  _ __      /  \   | |  | || |
        / _` || | | || |_ \    / /\ \  | |  | || |
       | (_| || |_| || | | |  / ____ \ | |__| || |____
        \__,_| \__, ||_| | | /_/    \_\|_____/ |______|
                __/ |
               |___/

## Overview
dynADL is a high level architecture definition language that supports dynamic behavior of the genrated systems and their components. It allows for the generation of system of systems.
For a simple example take a look at unit.expr file.
The language supports functions, variable defintions and some operators.
The Antlr grammar and the C++ parser is included in the project. This makes the language extensible.

## Prerequisites
dynADL depends on some popular, header only C++ libraries. They need to be installed first:

* nlohmann/json.hpp
* bitsery
* boost (message queue)

## Build
Using the Makefile one can execute: 
*make*       - to build
*make clean* - to clean the generated files.

## VS Code
There is a .vscode folder where the launch.json and tasks.json files reside.
To start Visual studio code, just type:

*code .*
Debugging of the grammar can be done thorugh the installation of the Antlr extension for VS Code.
Debugging of the generated C++ code can also be done within VS Code.
The launch.json file has two targets, one for the grammar and one for the C++ executable.

## Run
From the command line one can use the shell script run.sh

*./dynadl unit.expr [--generate | --runtime ] [posix | ros | px4]*


Depending on the structure of the desired architecture a tree will be generated that contains source code files:

<pre>
package1
└── system1
    ├── component11
    │   ├── action
    │   ├── config
    │   ├── diag
    │   ├── msg
    │   ├── srv
    │   └── state
    ├── component12
    │   ├── action
    │   ├── config
    │   ├── diag
    │   ├── msg
    │   ├── srv
    │   └── state
    └── component13
        ├── action
        ├── config
        ├── diag
        ├── msg
        ├── srv
        └── state
</pre>

Each component has the following files generated, which includes a Makefile.
Manual changes can be done based on this structure. 

<pre>
.
├── action
│   └── action1.action.json
├── component11
├── component11.cpp
├── component11.o
├── config
│   └── params.cfg.json
├── config.cpp
├── config.o
├── datatypes.h
├── diag
│   └── diagnostics1.diag.json
├── diagnostics.cpp
├── diagnostics.h
├── diagnostics.o
├── Makefile
├── msg
│   └── message1.msg.json
├── srv
│   └── service1.srv.json
├── state
│   └── state_variables.sta.json
├── state.cpp
├── state.h
└── state.o
</pre>

Example of code generation:

*./dynadl unit.expr --generate posix*

Example of running the generated and compiled code:

*./dynadl unit.expr --runtime posix*

<pre>
Author: Anton D. Hristozov 
Email: ahristoz@purdue.edu
</pre>

