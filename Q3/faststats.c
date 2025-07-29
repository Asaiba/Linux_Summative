#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <math.h>
#include <stdlib.h>

// Helper: Compare function for qsort
int cmp_func(const void *a, const void *b) {
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    return (fa > fb) - (fa < fb);
}

// sum_values: Computes the total sum of the input float array
static PyObject* sum_values(PyObject* self, PyObject* args) {
    PyObject* input_list;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &input_list)) {
        return NULL;
    }

    Py_ssize_t len = PyList_Size(input_list);
    double sum = 0.0;
    for (Py_ssize_t i = 0; i < len; i++) {
        PyObject* item = PyList_GetItem(input_list, i);  // Borrowed reference
        sum += PyFloat_AsDouble(item);
    }
    return PyFloat_FromDouble(sum);
}

// mean_values: Calculates the average of the float list
static PyObject* mean_values(PyObject* self, PyObject* args) {
    PyObject* input_list;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &input_list)) {
        return NULL;
    }

    Py_ssize_t len = PyList_Size(input_list);
    if (len == 0) return PyFloat_FromDouble(0.0);

    double sum = 0.0;
    for (Py_ssize_t i = 0; i < len; i++) {
        sum += PyFloat_AsDouble(PyList_GetItem(input_list, i));
    }

    return PyFloat_FromDouble(sum / len);
}

// std_dev: Calculates sample-based standard deviation
static PyObject* std_dev(PyObject* self, PyObject* args) {
    PyObject* input_list;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &input_list)) {
        return NULL;
    }

    Py_ssize_t len = PyList_Size(input_list);
    if (len <= 1) return PyFloat_FromDouble(0.0);

    double sum = 0.0, sq_sum = 0.0, val;
    for (Py_ssize_t i = 0; i < len; i++) {
        val = PyFloat_AsDouble(PyList_GetItem(input_list, i));
        sum += val;
        sq_sum += val * val;
    }

    double mean = sum / len;
    double variance = (sq_sum - len * mean * mean) / (len - 1);
    return PyFloat_FromDouble(sqrt(variance));
}

// count_values: Returns the count of values in the list
static PyObject* count_values(PyObject* self, PyObject* args) {
    PyObject* input_list;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &input_list)) {
        return NULL;
    }
    return PyLong_FromSsize_t(PyList_Size(input_list));
}

// mode_value: Computes the mode (most frequent value)
static PyObject* mode_value(PyObject* self, PyObject* args) {
    PyObject* input_list;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &input_list)) {
        return NULL;
    }

    Py_ssize_t len = PyList_Size(input_list);
    if (len == 0) return PyFloat_FromDouble(0.0);

    float* values = malloc(len * sizeof(float));
    if (!values) return PyErr_NoMemory();

    for (Py_ssize_t i = 0; i < len; i++) {
        values[i] = (float)PyFloat_AsDouble(PyList_GetItem(input_list, i));
    }

    qsort(values, len, sizeof(float), cmp_func);

    float mode = values[0];
    int max_count = 1, count = 1;

    for (Py_ssize_t i = 1; i < len; i++) {
        if (fabs(values[i] - values[i-1]) < 1e-6) {
            count++;
        } else {
            if (count > max_count) {
                max_count = count;
                mode = values[i - 1];
            }
            count = 1;
        }
    }

    if (count > max_count) mode = values[len - 1];
    free(values);
    return PyFloat_FromDouble(mode);
}

// Method Definitions
static PyMethodDef FastStatsMethods[] = {
    {"sum_values", sum_values, METH_VARARGS, "Returns the sum of float values"},
    {"mean_values", mean_values, METH_VARARGS, "Returns the mean of float values"},
    {"std_dev", std_dev, METH_VARARGS, "Returns the sample standard deviation"},
    {"mode_value", mode_value, METH_VARARGS, "Returns the mode of float values"},
    {"count_values", count_values, METH_VARARGS, "Returns the number of float values"},
    {NULL, NULL, 0, NULL}
};

// Module Definition
static struct PyModuleDef faststatsmodule = {
    PyModuleDef_HEAD_INIT,
    "faststats",
    "Efficient statistical computations in C",
    -1,
    FastStatsMethods
};

PyMODINIT_FUNC PyInit_faststats(void) {
    return PyModule_Create(&faststatsmodule);
}
