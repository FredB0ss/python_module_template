/*
    Specifications File: rc4.yaml
    Template File: template_module_class_iter_rc4_c
*/


#include "Python.h"
#include "structmember.h"

#include <stdio.h>
#include <stdlib.h>
#include "rc4.h"


/******************************************************************************
 rc4iter - attributes
******************************************************************************/
typedef struct {
    PyObject_HEAD
    
    /* private attributes if not declared in rc4iter_class_members[] below */
    rc4_ctxt_t ctxt;

    uint32_t count;

} rc4iter_class_t;


static void
rc4iter_class_dealloc(rc4iter_class_t* self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
rc4iter_class_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    rc4iter_class_t *self;

    self = (rc4iter_class_t *)type->tp_alloc(type, 0);
    if (self)
    {
        self->count = 0;
    }

    return (PyObject *)self;
}

/*******************************************************************************
 Class __init__() function

 You may fill the blank if needed
*******************************************************************************/
static int
rc4iter_class_init(rc4iter_class_t *self, PyObject *args, PyObject *kwds)
{
    Py_buffer buffer; /* input buffer */
    unsigned int maxlen;

    /* Example, expect a Bytes sequence which is the key */
    if (!PyArg_ParseTuple(
            args,
            "s*",
            &buffer,
            &maxlen)
        ) {
        PyErr_SetString(PyExc_AttributeError, "key argument must be a bytes()");
        return -1;
    }

    /* call to proper init function */
    rc4_init(buffer.buf, buffer.len, &self->ctxt);

    /* OK */
    return 0;
}

/******************************************************************************
 rc4iter Class Public Attributes
******************************************************************************/
static PyMemberDef rc4iter_class_members[] = {
    {"count",  T_ULONG,     offsetof(rc4iter_class_t, count),   0, "rc4iter number of generated RC4 bytes"},

    {NULL}  /* Sentinel */
};

/******************************************************************************
 Public methods
******************************************************************************/

/******************************************************************************
rc4iter Class Public Methods
******************************************************************************/

/* Function: my_next
   Prototype: my_next() -> long
   
   Gives next RC4 byte 
*/

/* Iter next() method takes only 1 argument = self */
static PyObject *
rc4iter_class_my_next(PyObject *obj)
{
    rc4iter_class_t *self = (rc4iter_class_t *)obj;
    PyObject* retval; /* return value */
    long r;
    
    r = (long)rc4_next(&self->ctxt);

    /* keep track of generated bytes count */
    self->count++;
    
    retval = PyLong_FromLong(r);

    return retval;
}



static PyMethodDef rc4iter_class_methods[] = {
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


static PyTypeObject rc4iter_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyrc4.Rc4",               /* tp_name */
    sizeof(rc4iter_class_t),           /* tp_basicsize */
    0,                                              /* tp_itemsize */
    (destructor)rc4iter_class_dealloc, /* tp_dealloc */
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
    "rc4iter class",                   /* tp_doc */
    0,                                              /* tp_traverse */
    0,                                              /* tp_clear */
    0,                                              /* tp_richcompare */
    0,                                              /* tp_weaklistoffset */
    PyObject_SelfIter,                              /* tp_iter */
    rc4iter_class_my_next,/* tp_iternext */
    rc4iter_class_methods,             /* tp_methods */
    rc4iter_class_members,             /* tp_members */
    0,                                              /* tp_getset */
    0,                                              /* tp_base */
    0,                                              /* tp_dict */
    0,                                              /* tp_descr_get */
    0,                                              /* tp_descr_set */
    0,                                              /* tp_dictoffset */
    (initproc)rc4iter_class_init,      /* tp_init */
    0,                                              /* tp_alloc */
    rc4iter_class_new,                 /* tp_new */
};


/******************************************************************************
 Les méthodes du module
******************************************************************************/
static PyMethodDef pyrc4_methods[] = {
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef rc4iter_module_definition = {
   PyModuleDef_HEAD_INIT,
   "pyrc4", /* name of module */
   "This module implements classes to wrap rc4iter library services", /* module documentation, may be NULL */
   -1,  /* size of per-interpreter state of the module,
           or -1 if the module keeps state in global variables. */
   pyrc4_methods
};


PyMODINIT_FUNC
PyInit_pyrc4(void)
{
    PyObject *m;

    /* type creation method */
    if (PyType_Ready(&rc4iter_type) < 0)
        return NULL;

    /* création du module */
    m = PyModule_Create(&rc4iter_module_definition);
    if (m == NULL) {
        return NULL;
    }

    /* ajout du type au module */
    Py_INCREF(&rc4iter_type);
    PyModule_AddObject(
        m,
        "Rc4",  // nom de la classe "Rc4", utilisée par ex. m = pyrc4.Rc4()
        (PyObject *)&rc4iter_type);

    return m;
}

