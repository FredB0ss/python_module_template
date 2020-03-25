#!/usr/bin/env python3
#coding: utf-8

"""
This script renders a C file for a Python module, based on a specifications
written in YAML.

The idea is to ease the development of a C module for Python. This script takes
care of rendering a skeleton that the user completes. The intended module is
describes by a YAML specifications which is documented thanks to comments.

The idea is to KISS hence the script stops where things get too complicated.
The human does the rest which might hopefully be the most interesting part.

PyYAML brings the specifications support:
https://pyyaml.org/
https://github.com/yaml/pyyaml

JINJA2 offers the templating & rendering mechanisms:
https://jinja.palletsprojects.com/

Both are indexed in the PyPi: https://pypi.org
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
def load_template(template):

    filename = os.path.join(os.path.dirname(__file__), template)

    with open(filename) as f:
        t = jinja2.Template(f.read())

    return t


###############################################################################
def render_function(arguments, funcname, funcspecs):
    y = arguments.y.copy()
    y["FUNCTION_NAME"] = funcname
    y["FUNCTION_HINT"] = funcspecs[0]
    y["FUNCTION_DOC"] = funcspecs[1]
    t = load_template(DEFAULT_FUNCTION_TEMPLATE)
    s = t.render(y)
    return s

###############################################################################
def render(arguments):

    if arguments.template and len(arguments.template):
        template = arguments.template[0]
    else:
        #print("y=", arguments.y)
        model = arguments.y["MODEL"]
        template = template_from_model(model)

    print("/*\nUsing\n   Specifications File: %s\n    Template File: %s\n*/\n" % (arguments.specs[0], template))
        
    # 1st render functions
    funcs = arguments.y["MODULE_FUNCTIONS"]

    rf = []
    for f in funcs:
        print("Function", f, funcs[f])
        rf.append(render_function(arguments, f, funcs[f]))
        
    print("Functions Bodies:", rf)
    arguments.y["MODULE_FUNCTIONS"] = [f for f in funcs]
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

    ap.add_argument("-t", "--template", type=str, nargs="?", default=None, help="Enforce a template file name, when not provided the template file is choosen according to the model name (MODEL field in YAML specs) and is among %s" % DEFAULT_TEMPLATE_FILENAME_LIST_S)

    ap.add_argument("specs", type=str, nargs=1, default=None, help="mandatory YAML specifications file")

    # parse command line
    arguments = ap.parse_args(sys.argv[1:])

    # call our function
    main(arguments)
