/*
    Specifications File: test_flat.yaml
    Template File: ../template_module_flat_c
*/


#include "Python.h"
#include "structmember.h"

/*
#include <stddef.h>
#include <stdlib.h>
*/

#include <stdint.h>
#include "rc4.h"


/******************************************************************************
 rc4mod - Private definitions
******************************************************************************/

/* insert your code here */

/******************************************************************************
 rc4mod -  Public functions
******************************************************************************/

/* Function: init
   Prototype: init(key: bytes) -> None
   
   Init function 
*/

static PyObject *
rc4mod_init(PyObject *obj, PyObject *args, PyObject *kwds)
{
    Py_buffer key_buffer; /* input buffer */
    unsigned int key_maxlen;

    

    /* parse arguments */
    if (!PyArg_ParseTuple(
            args,
            "s*", /* FILL */
            &key_buffer,
            &key_maxlen)
        ) {
        PyErr_SetString(PyExc_AttributeError, "arguments must be FILL");
        return NULL;
    }
    
    /* FILL - This is a placeholder for the function logic */

    
    /* return None */
    Py_RETURN_NONE;
    
}


/* Function: next
   Prototype: next() -> int
   
   Next function, gives back the next RC4 byte 
*/

static PyObject *
rc4mod_next(PyObject *obj, PyObject *args, PyObject *kwds)
{
    Py_buffer key_buffer; /* input buffer */
    unsigned int key_maxlen;

    
    PyObject* retval; /* return value */
    

    /* parse arguments */
    if (!PyArg_ParseTuple(
            args,
            "s*", /* FILL */
            &key_buffer,
            &key_maxlen)
        ) {
        PyErr_SetString(PyExc_AttributeError, "arguments must be FILL");
        return NULL;
    }
    
    /* FILL - This is a placeholder for the function logic */

    
    /* or return our Python object */
    return retval;
    
}




/******************************************************************************
 Module Methods Array
******************************************************************************/
static PyMethodDef pyrc4_methods[] = {
    
    {"init", (PyCFunction)rc4mod_init,  METH_VARARGS, "Init function" },
    
    {"next", (PyCFunction)rc4mod_next,  METH_VARARGS, "Next function, gives back the next RC4 byte" },
    
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef rc4mod_module_definition = {
   PyModuleDef_HEAD_INIT,
   "pyrc4", /* name of module */
   "This module implements functions to wrap the RC4 C library", /* module documentation, may be NULL */
   -1,  /* size of per-interpreter state of the module,
           or -1 if the module keeps state in global variables. */
   pyrc4_methods
};


PyMODINIT_FUNC
PyInit_pyrc4(void)
{
    PyObject *m;

    /* création du module */
    m = PyModule_Create(&rc4mod_module_definition);
    if (m == NULL) {
        return NULL;
    }

    return m;
}

