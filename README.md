# python_module_template

This script renders a skeleton of a C file for a Python module, based on a specifications written in YAML.

PyYAML brings the specifications support, thank you:
https://pyyaml.org/
https://github.com/yaml/pyyaml

JINJA2 offers the templating & rendering mechanisms, kudos:
https://jinja.palletsprojects.com/

Both are indexed in the PyPi: https://pypi.org

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

## Common Fields

The YAML specifications must contain the following fields:

MODEL: *model*

*model* is either:

- flat: module with global functions only
- class: module implementing a class
- class_iter: module implementing an iterator kind of a class

MODULE_NAME: *name*, module name as seen by the Python interpreter.

MODULE_DOC: *string*, description of the module, highly recommended.

MODULE_NAMESPACE: *name*, This name is solely used in the C implementation.

OWN_INCLUDES: *string*, list of needed C includes - formatted as C lines

Example:
```YAML
OWN_INCLUDES: "#include <stdio.h>\n#include <stdlib.h>\n#include \"rc4.h\"\n"
```

## Flat Module

A flat module is described by a list of C functions.

MODULE_FUNCTIONS: *dict*

This dictionary provides the list of the functions to template, for each function the prototype under the form of an annotated Python function declaration and a documentation string are provided:

*funcname*: [ *prototype*, *docstring* ]

Example:
```YAML
  MODULE_FUNCTIONS:
    "init": [ "init(key: bytes) -> None", "Init function" ]
    "next": [ "next() -> int", "Next function, gives back the next RC4 byte" ]
```

### Flat Module Specifications Example

```YAML
MODEL: flat
MODULE_NAME: pyrc4
MODULE_DOC: This module implements functions to wrap the RC4 C library
MODULE_NAMESPACE: rc4mod
OWN_INCLUDES: "#include <stdint.h>\n#include \"rc4.h\"\n"
MODULE_FUNCTIONS:
  "init": [ "init(key: bytes) -> None", "Init function" ]
  "next": [ "next() -> int", "Next function, gives back the next RC4 byte" ]
```

## Class module

Additionaly to the above common fields a class is described by the list of its methods:

CLASS_METHODS: *dict*, a dictionary of functions, same as MODULE_FUNCTIONS above.

### Class Module Specifications Example

```YAML
MODEL: class
MODULE_NAME: pyrc4
MODULE_DOC: This module implements a Python class to wrap the RC4 C library
CLASS_NAME: Rc4
MODULE_NAMESPACE: rc4mod
OWN_INCLUDES: "#include <stdio.h>\n#include <stdlib.h>\n"
CLASS_METHODS:
  "key": [ "key(key: bytes) -> None", "Key Init function" ]
  "doit": [ "doit() -> int", "Gives an RC4 byte" ]
```

## Iterator Class module

Additionaly to the above common fields an iterator is described by its next function. It is up to the developper to implement the init function in the resulting generated C file :

NEXT_METHOD: *dict*, a dictionary of a function, with one single entry describing the "next" function of the iterator. Though unecessary this keeps the same syntax as CLASS_METHODS

Example:

```YAML
  NEXT_METHOD:
    "my_next": [ "my_next() -> long", "Gives next RC4 byte" ]
```

Optionaly the iterator class may be augmented with others methods, using the following field:

CLASS_METHODS: *dict*, a dictionary of functions, same as above.

### Iterator Class Module Specifications Example

```YAML
MODEL: class_iter
MODULE_NAME: pyrc4
MODULE_DOC: This module implements an iterator class to wrap the RC4 C library
CLASS_NAME: Rc4
MODULE_NAMESPACE: rc4iter
OWN_INCLUDES: "#include <stdio.h>\n#include <stdlib.h>\n"
NEXT_METHOD:
  "my_next": [ "my_next() -> long", "Gives next RC4 byte" ]
```

# Tests

  cd tests/ && bash do_tests.sh


# Complete Example

In the directory example/ a complete example is provided. The objective is to build an iterator class module which produces RC4 bytes. The RC4 implementation is provided by a disctinct C file, rc4.c. The YAML specifications for the iterator are available as rc4.yaml. The rendering is based on a dedicated template, template_module_class_iter_rc4_c.

In the end the proposed module is an iterator roughly equivalent to the following pure Python code:

```Python
class Rc4:
    def __init__(self, key):
        count = 0
        rc4_init(key)
    def __next__(self):
        return rc4_next()
    def __iter__(self):
        return self
```

The steps to build the module are:

- In the directory example:

  cd example/
  
- Build the RC4 C library & archive:

  gcc -Wall -Wextra -o rc4.o -c rc4.c
  ar -crs  librc4.a rc4.o
  
- Render the template for an iterator class into a temporary C file:

python3 ../render.py -t template_module_class_iter_rc4_c rc4.yaml > pyrc4.c.tmp

- Manually edit the C file, fill the blanks and save it as pyrc4.c (final version is provided)

- A setup.py is provided to build everything, it refers to both librc4.a and pyrc4.c files:

```Python
import distutils.core
my_module = distutils.core.Extension(
    "pyrc4",
    sources = ["pyrc4.c"],
    include_dirs=[".", ],
    libraries=["rc4"],
    library_dirs=["."],
)
distutils.core.setup(
    name = "PyRC4",
    ext_modules = [my_module],
)
```

- Build the Python module:

  python3 setup.py build
  
- Retrieve the .so module:

  cp build/lib.*/pyrc4.*.so pyrc4.so
  
- You may now use it everywhere!:

  cp pyrc4.so /tmp
  cd /tmp && python3

- Use it in Python:

```Python
Python 3.7.6 (default, Dec 19 2019, 09:25:23) 
[GCC 9.2.1 20191130] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import pyrc4
>>> dir(pyrc4)
['Rc4', '__doc__', '__file__', '__loader__', '__name__', '__package__', '__spec__']
>>> o = pyrc4.Rc4(b"Fred")
>>> type(o)
<class 'pyrc4.Rc4'>
>>> o.count
0
>>> o.__next__()
195
>>> o.count
1
>>> for i, x in enumerate(o):
...     print(i, hex(x))
...     if 10 == i: break
... 
0 0x1d
1 0x11
2 0xfe
3 0x73
4 0x6a
5 0x59
6 0x15
7 0xce
8 0xba
9 0x70
10 0x73
```

- The Python script test.py shows how two such iterators have distinct contexts:

```Python
import pyrc4

it1 = pyrc4.Rc4(b"Fred")

for x in it1:
    print(it1.count, hex(x))

    it2 = pyrc4.Rc4(b"B0ss")

    for y in it2:
        print("\t", it2.count, hex(y))
        if 4 == it2.count:
            break

    if 10 == it1.count:
        break

it1 = pyrc4.Rc4(b"Fred")
for x in it1:
    print(it1.count, hex(x))
    if 10 == it1.count:
        break
```

```shell
$ python3 test.py
1 0xc3
	 1 0x3b
	 2 0xb1
	 3 0x0
	 4 0xa7
2 0x1d
	 1 0x3b
	 2 0xb1
	 3 0x0
	 4 0xa7
3 0x11
	 1 0x3b
	 2 0xb1
	 3 0x0
	 4 0xa7
4 0xfe
	 1 0x3b
	 2 0xb1
	 3 0x0
	 4 0xa7
5 0x73
	 1 0x3b
	 2 0xb1
	 3 0x0
	 4 0xa7
6 0x6a
	 1 0x3b
	 2 0xb1
	 3 0x0
	 4 0xa7
7 0x59
	 1 0x3b
	 2 0xb1
	 3 0x0
	 4 0xa7
8 0x15
	 1 0x3b
	 2 0xb1
	 3 0x0
	 4 0xa7
9 0xce
	 1 0x3b
	 2 0xb1
	 3 0x0
	 4 0xa7
10 0xba
	 1 0x3b
	 2 0xb1
	 3 0x0
	 4 0xa7
1 0xc3
2 0x1d
3 0x11
4 0xfe
5 0x73
6 0x6a
7 0x59
8 0x15
9 0xce
10 0xba
```
