#include <Python.h>
#include <math.h>

struct Point {
    double x, y;
};


double distance(struct Point *p1, struct Point *p2) {
  return hypot(p1->x - p2->x, p1->y - p2->y);
}

static void del_point(PyObject *obj) {
  free(PyCapsule_GetPointer(obj, "Point"));
}

static struct Point *PyPoint_AsPoint(PyObject *obj) {
  return (struct Point *) PyCapsule_GetPointer(obj, "Point");
}

static PyObject *PyPoint_FromPoint(struct Point *p, int must_free) {
  return PyCapsule_New(p, "Point", must_free ? del_point : NULL);
}

static PyObject *py_Point(PyObject *self, PyObject *args) {
  struct Point *p;
  double x, y;
  if(!PyArg_ParseTuple(args, "dd", &x, &y)) {
    return NULL;
  }

  p = (struct Point *) malloc(sizeof(struct Point));
  p->x = x;
  p->y = y;

  return PyPoint_FromPoint(p, 1);
}

static PyObject *py_distance(PyObject *self, PyObject *args) {
  struct Point *p1, *p2;
  PyObject *py_p1, *py_p2;
  double result;

  if(!PyArg_ParseTuple(args, "OO", &py_p1, &py_p2)) {
    return NULL;
  }
  if(!(p1 = PyPoint_AsPoint(py_p1))) {
    return NULL;
  }
  if(!(p2 = PyPoint_AsPoint(py_p2))) {
    return NULL;
  }

  result = distance(p1, p2);
  return Py_BuildValue("d", result);
}


static PyMethodDef SpamMethods[] = {
    {"Point", py_Point, METH_VARARGS, "Point Struct"},
    {"distance", py_distance, METH_VARARGS, "distance between two points"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initspam(void)
{
    (void) Py_InitModule("spam", SpamMethods);
}
