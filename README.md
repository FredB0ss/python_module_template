# python_module_template
This script renders a skeleton of a C file for a Python module, based on a specifications written in YAML.

The rendering

# Discussion

There exists several ways to binds Python & C code each with its own merits:

- SWIG
- ctypes
- etc.

The reference interpreter is developped in C and the developpers designed a native interface between Python & C: Python.h

It is explained in the embedded documentation:

- file:///usr/share/doc/python3/html/extending/index.html
- https://docs.python.org/3/extending/index.html

The needed work to write the C code to create one's own module may be seen has an obstacle. The purpose of the present Python script is to automate as much as possible of this writing effort.

The idea is to KISS hence this script stops where things get too complicated. It is up to the human to do the remaining bits and to fill in the blanks which might hopefully be the most interesting part.

Three models are available:
- flat: a Python module implemented by a list of C functions
- class: a module implementing a Python object class
- class_iter: a module implementing a particular case of an iterator class

# Installation

No installation required per se. Just copy the directory containing the render.py script and its associated templates.

# Usage

- write your YAML specifications. Example are given in the tests/ directory.
- render a C skeleton:
    python3 render.py specs.yaml > file.c
- edit file.c, implement functions bodies
- write your setup.py

# YAML specifications

The YAML specifications contains the common following fields:

MODEL: <model>

model is either:

- flat: module with global functions only
- class: module implementing a class
- class_iter: module implementing an iterator kind of a class

MODULE_NAME: <name> module name as seen by the Python interpreter.

MODULE_DOC: <string> description of the module, highly recommended.

MODULE_NAMESPACE: <name> This name is solely used in the C implementation.

OWN_INCLUDES: <string> list of needed C includes - formatted as C lines

## flat module

MODULE_FUNCTIONS: <dict>
This dictionary provides the list of the functions to template, for each function the prototype under the form of an annotated Python function declaration and a documentation string are provided:
<funcname>: [ <prototype>, <docstring>]

Example:
  MODULE_FUNCTIONS:
    "init": [ "init(key: bytes) -> None", "Init function" ]
    "next": [ "next() -> int", "Next function, gives back the next RC4 byte" ]

## class module

CLASS_METHODS: <dict>
A dictionary of functions, same as above.

## class_iter module

NEXT_METHOD:
A dictionary of a function, with one single entry describing the "next" function of the iterator.

Example:

  NEXT_METHOD:
    "my_next": [ "my_next() -> long", "Gives next RC4 byte" ]


Optionaly the iterator class may be augmented with others methods, using the following field:

CLASS_METHODS: <dict>
A dictionary of functions, same as above.


# Tests

  cd tests/ && bash do_tests.sh