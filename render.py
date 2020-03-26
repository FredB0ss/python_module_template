#!/usr/bin/env python3
#coding: utf-8

"""
This script renders a C file for a Python module, based on a specifications
written in YAML.

The idea is to ease the development of a C module for Python. This script takes
care of rendering a skeleton that the user completes. The intended module is
describes by a YAML specifications which is documented thanks to comments.

The idea is to KISS hence the script stops where things get too complicated.
The human does the remaining bits which might hopefully be the most interesting
part.

PyYAML brings the specifications support:
https://pyyaml.org/
https://github.com/yaml/pyyaml

JINJA2 offers the templating & rendering mechanisms:
https://jinja.palletsprojects.com/

Both are indexed in the PyPi: https://pypi.org

Basic examples of YAML specifications are provided in the tests directory.

How to produce a C file which the user has to edit it and fill in it with her own C code ?
$ python3 render.py specs.yaml > file.c
"""

import os
import sys
import argparse
import yaml

import jinja2

# The template files must be kept in the same directory as the present Python file.
MODEL_LIST = [ "flat", "class", "class_iter", ]
# The order of the template files must be the same as the models in the above list.
DEFAULT_TEMPLATE_FILENAME_LIST_S = "template_module_flat_c template_module_class_c  template_module_class_iter_c"
DEFAULT_FUNCTION_TEMPLATE = "template_function_c"

###############################################################################
def load_specs(yamlfile) :

    with open(yamlfile, "rb") as f :
        y = yaml.load(f, Loader=yaml.FullLoader)

    return y


###############################################################################
def template_from_model(model):
    "Returns the template file corresponding "
    d_model = { MODEL_LIST[i]: template for i, template in enumerate(DEFAULT_TEMPLATE_FILENAME_LIST_S.split()) }

    return d_model[model]


###############################################################################
def load_template(filename):

    with open(filename) as f:
        t = jinja2.Template(f.read())

    return t


###############################################################################
def render_function(arguments, funcname, funcspecs):

    hint = funcspecs[0]
    doc = funcspecs[1]

    # Get return type # FFS parameters handling
    l = hint.split("->")
    if len(l) > 1:
        rettype = l[1].strip()

    y = arguments.y.copy()
    y["FUNCTION_NAME"] = funcname
    y["FUNCTION_HINT"] = hint
    y["FUNCTION_DOC"] = doc
    y["FUNCTION_RETTYPE"] = rettype
    
    t = load_template(os.path.join(os.path.dirname(__file__), DEFAULT_FUNCTION_TEMPLATE))
    s = t.render(y)
    return s

###############################################################################
def render(arguments):

    model = arguments.y["MODEL"]
    
    if arguments.template and len(arguments.template):
        template = arguments.template[0]
    else:
        template = os.path.join(os.path.dirname(__file__), template_from_model(model))

    print("/*\n    Specifications File: %s\n    Template File: %s\n*/\n" % (arguments.specs[0], template))
        
    # 1st render functions/methods
    rf = []

    if "flat" == model:
        funcs = []
        if "MODULE_FUNCTIONS" in arguments.y:
            funcs = arguments.y["MODULE_FUNCTIONS"]
        else:
            raise ValueError("Flat model but no functions declared")
        
        for f in funcs:
            #print("Function", f, funcs[f])
            rf.append(render_function(arguments, f, funcs[f]))

    elif "class" == model:
        meths = []
        if "CLASS_METHODS" in arguments.y:
            meths = arguments.y["CLASS_METHODS"]
        else:
            raise ValueError("Class model but no methods declared")
        for m in meths:
            rf.append(render_function(arguments, m, meths[m]))

    elif "class_iter" == model:
        if "NEXT_METHOD" in arguments.y:
            ns = arguments.y["NEXT_METHOD"]
            # consider it is the only one or the first of the dict
            n = list(ns.keys())[0]
            # we add it to our y argument in order to simplify the template
            arguments.y["NEXT_METHOD_NAME"] = n
            rf = [render_function(arguments, n, ns[n])]
        else:
            raise ValueError("Class Iter model but no next method declared")

        # Optional methods
        if "CLASS_METHODS" in arguments.y:
            meths = arguments.y["CLASS_METHODS"]
            for m in meths:
                rf.append(render_function(arguments, m, meths[m]))
        
    #print("Functions Bodies:", rf)
    arguments.y["MODULE_FUNCTIONS_BODY"] = rf
    
    # render whole module
    #print("Using template:", template)
    t = load_template(template)
    s = t.render(arguments.y)
    print(s)
    
    
###############################################################################
def main(arguments) :
    "our logic"

    debug = arguments.debug

    if debug :
        print("Arguments are :")
        print("debug= %s" % arguments.debug)
        print("verbose= %s" % arguments.verbose)
        print("template= %s" % arguments.template)
        print("specs= %s" % arguments.specs)
        print("")

    # load YAML specifications
    arguments.y = load_specs(arguments.specs[0])

    if debug:
        print("y=", arguments.y)
    
    render(arguments)


###############################################################################
if "__main__" == __name__ :

    ap = argparse.ArgumentParser(description=__doc__)

    ap.add_argument("-d", "--debug", action="store_true", help="debug infos")

    ap.add_argument("-v", "--verbose", action="store_true", help="verbose mode")

    ap.add_argument("-t", "--template", type=str, nargs=1, default=None, help="Enforce a template file name, when not provided the template file is choosen according to the model name (MODEL field in YAML specs) and is among %s" % DEFAULT_TEMPLATE_FILENAME_LIST_S)

    ap.add_argument("specs", type=str, nargs=1, default=None, help="mandatory YAML specifications file")

    # parse command line
    arguments = ap.parse_args(sys.argv[1:])

    # call our function
    main(arguments)
