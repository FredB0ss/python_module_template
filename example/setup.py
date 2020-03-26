#!/usr/bin/env python3
#coding: utf-8

"""
lancer :
python3 setup.py build

puis :
cp build/lib.linux-x86_64-3.5/XXX.cpython-35m-x86_64-linux-gnu.so XXX.so

test :
python3 -m XXX

"""

import distutils.core

# compilation du .so
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

    version = "1.0",
    description = "Python Package with C Extension",
    url="https://github.com/FredB0ss",
    author="Fred",
    author_email="someone at nowhere dot com")
