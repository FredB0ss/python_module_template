/*
    Specifications File: test_class.yaml
    Template File: ../template_module_class_c
*/


#include "Python.h"
#include "structmember.h"

#include <stdio.h>
#include <stdlib.h>


/******************************************************************************
 rc4mod - attributes
******************************************************************************/
typedef struct {
    PyObject_HEAD
    
    /* example how to define a public attribute */
    uint8_t key;

} rc4mod_class;


static void
rc4mod_class_dealloc(rc4mod_class* self)
{
    

    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
rc4mod_class_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    rc4mod_class *self;

    self = (rc4mod_class *)type->tp_alloc(type, 0);
    if (self != NULL) {
        
    }

    return (PyObject *)self;
}

/*******************************************************************************
 Class __init__() function

 You may fill the blank if needed
*******************************************************************************/
static int
rc4mod_class_init(rc4mod_class *self, PyObject *args, PyObject *kwds)
{
    if (!PyArg_ParseTuple(
            args,
            "" // No argument expected
       )
        ) {
        return -1;
    }

    /* FILL */

    /* GOOD */
    return 0;
}

/******************************************************************************
 rc4mod Class Public Attributes
******************************************************************************/
static PyMemberDef rc4mod_class_members[] = {
    {"key",  T_ULONG,     offsetof(rc4mod_class, key),   0, "rc4mod key"},
    
    {NULL}  /* Sentinel */
};

/******************************************************************************
  Private Functions
******************************************************************************/
static int rc4mod_my_private_method_param(rc4mod_class* self, uint32_t key)
{
    /* store in class context */
    self->key = (uint8_t)key;
    printf("Key 0x%.2x\n", self->key);
    
    return self->key;
}

/******************************************************************************
rc4mod Class Public Methods
******************************************************************************/

/* Function: key
   Prototype: key(key: bytes) -> None
   
   Key Init function 
*/

static PyObject *
rc4mod_key(PyObject *obj, PyObject *args, PyObject *kwds)
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


/* Function: doit
   Prototype: doit() -> int
   
   Gives an RC4 byte 
*/

static PyObject *
rc4mod_doit(PyObject *obj, PyObject *args, PyObject *kwds)
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




static PyMethodDef rc4mod_class_methods[] = {
    
    {"key", (PyCFunction)rc4mod_key,  METH_VARARGS, "Key Init function" },
    
    {"doit", (PyCFunction)rc4mod_doit,  METH_VARARGS, "Gives an RC4 byte" },
    
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


static PyTypeObject rc4mod_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyrc4.Rc4",               /* tp_name */
    sizeof(rc4mod_class),             /* tp_basicsize */
    0,                                              /* tp_itemsize */
    (destructor)rc4mod_class_dealloc, /* tp_dealloc */
    0,                                              /* tp_print */
    0,                                              /* tp_getattr */
    0,                                              /* tp_setattr */
    0,                                              /* tp_reserved */
    0,                                              /* tp_repr */
    0,                                              /* tp_as_number */
    0,                                              /* tp_as_sequence */
    0,                                              /* tp_as_mapping */
    0,                                              /* tp_hash  */
    0,                                              /* tp_call */
    0,                                              /* tp_str */
    0,                                              /* tp_getattro */
    0,                                              /* tp_setattro */
    0,                                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,       /* tp_flags */
    "rc4mod class",                   /* tp_doc */
    0,                                              /* tp_traverse */
    0,                                              /* tp_clear */
    0,                                              /* tp_richcompare */
    0,                                              /* tp_weaklistoffset */
    0,                                              /* tp_iter */
    0,                                              /* tp_iternext */
    rc4mod_class_methods,             /* tp_methods */
    rc4mod_class_members,             /* tp_members */
    0,                                              /* tp_getset */
    0,                                              /* tp_base */
    0,                                              /* tp_dict */
    0,                                              /* tp_descr_get */
    0,                                              /* tp_descr_set */
    0,                                              /* tp_dictoffset */
    (initproc)rc4mod_class_init,      /* tp_init */
    0,                                              /* tp_alloc */
    rc4mod_class_new,                 /* tp_new */
};


/******************************************************************************
 Les méthodes du module
******************************************************************************/
static PyMethodDef pyrc4_methods[] = {
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef rc4mod_module_definition = {
   PyModuleDef_HEAD_INIT,
   "pyrc4", /* name of module */
   "This module implements classes to wrap rc4mod library services", /* module documentation, may be NULL */
   -1,  /* size of per-interpreter state of the module,
           or -1 if the module keeps state in global variables. */
   pyrc4_methods
};


PyMODINIT_FUNC
PyInit_pyrc4(void)
{
    PyObject *m;

    /* type creation method */
    if (PyType_Ready(&rc4mod_type) < 0)
        return NULL;

    /* création du module */
    m = PyModule_Create(&rc4mod_module_definition);
    if (m == NULL) {
        return NULL;
    }
    
    /* ajout du type au module */
    Py_INCREF(&rc4mod_type);
    PyModule_AddObject(
        m,
        "Rc4",  // nom de la classe "Rc4", utilisée par ex. m = pyrc4.Rc4()
        (PyObject *)&rc4mod_type);

    return m;
}

