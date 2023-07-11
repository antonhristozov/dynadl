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
dynADL is a high-level architecture definition language that supports dynamic behavior of the generated systems and their components. It allows for the generation of code for system of systems (SoS) and single systems.
For a simple example of the syntax, take a look at unit.expr file or any of the .expr files.
The language supports functions, variable definitions, and some operators.
The Antlr grammar and the C++ parser are included in the project. This makes the language extensible for the curious developer.

## Prerequisites
dynADL depends on some popular, header-only C++ libraries. They need to be installed first before attempting to build the code:

* nlohmann/json.hpp
* bitsery
* boost (message queue)

## The Build
The instructions are for Linux platforms and, more specifically, have been tested on Ubuntu 20.04.
Using the Makefile, one can execute the following: 
<pre>
make       - to build
make clean - to clean the generated files.            
</pre>            
This will create the dynADL parser, code generator, and run-time functionality.

## VS Code for Development
There is a .vscode folder where the launch.json and tasks.json files reside.
To start Visual Studio code, just type:

*code .*
Debugging of the grammar can be done by installing the Antlr extension for VS Code.
Debugging of the generated C++ code can also be done within VS Code.
The launch.json file has two targets, one for the grammar and one for the C++ executable.
The grammar is in dynadl.g4 file.

## Files 
<ul>
  <li>dynadl.cpp - main file</li>
  <li>dynadlWalkListener.h and .cpp - parse tree walker files </li>
  <li>dynadlDataManager.h and .cpp - utility files for the implementation of grammatical constructs </li>
  <li>dataTypes.h - Common data types file</li>
  <li>dynadl.g4 - Antlr grammar file</li>
</ul> 

## Running
The general syntax of the dynadl executable is given below:

*./dynadl unit.expr [--generate | --runtime ] [posix | ros | px4]*

Currently, the Posix platform is fully supported, and ROS and PX4 are not yet added as platforms.
Extensions for ROS and PX4 are very feasible.

Depending on the structure of the desired architecture, a tree will be generated that contains source code files:

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

The two main uses of the tool are to:

<ul>
  <li>Generate Posix compliant code</li>
  <li>Execute the generated code</li>
</ul> 

Example of code generation:

*./dynadl unit.expr --generate posix*

Example of running the generated and compiled code:

*./dynadl unit.expr --runtime posix*

The run.sh script is also available, as well as several files that have a .expr extension and contain valid dynADL code.

## Portability

The language was developed and tested in Linux. However, the dependencies on header-only libraries listed above make it possible to port it to other platforms, such as Mac and Windows.

## Citation

If you plan to cite this work, here is a BibTeX entry:

<pre>
@article{Hristozov2023Dissertation,
	title={Resilient Architectures Through Dynamic Management of Software Components},
	author={Hristozov, Anton},
	journal={Purdue},
	volume={},
	pages={1 - 202},
	year={2023}
}
</pre>

A more thorough discussion can be found in the cited work above, especially in the chapter devoted to dynADL and the appendices.

## License

The license used for this project is GPL 3.0, which is the most restrictive of the licenses of the used components. This is determined by the license used by the Boost libraries.

## Disclaimer

The code in this repository is experimental. Please be sure to use caution and do not expect commercial-grade software quality. This was done as experimental development while writing one of the chapters of my dissertation at Purdue University.

## Author

<pre>
Anton D. Hristozov 
Email: ahristoz@purdue.edu
Pittsburgh, PA
</pre>

