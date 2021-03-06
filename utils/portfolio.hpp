#ifndef CPORTFOLIO_HPP
#define CPORTFOLIO_HPP

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string>

int PortFolio(const std::vector<std::string>& stocks,
              const std::string& dataSource,
              const std::string& start,
              const std::string& end,
              const std::string& filePath = "portfolio.csv")
{
  PyObject *pName, *pModule, *pFunc;
  PyObject *pArgs, *pValue;
  int i;

  Py_Initialize();
  PyRun_SimpleString("import sys");
  PyRun_SimpleString("sys.path.append(\".\")");
  PyRun_SimpleString("sys.path.append(\"/srv/conda/envs/notebook/include/\")");
  pName = PyUnicode_DecodeFSDefault("portfolio");

  pModule = PyImport_Import(pName);
  Py_DECREF(pName);

  if (pModule != NULL)
  {
    pFunc = PyObject_GetAttrString(pModule, "cportfolio");
    size_t numStocks = stocks.size();
    if (pFunc && PyCallable_Check(pFunc))
    {
      pArgs = PyTuple_New(numStocks + 4);

      for(size_t idx = 0; idx < numStocks; ++idx)
      {
        PyObject* pValueCurrentStock = PyUnicode_FromString(stocks[idx].c_str());
        PyTuple_SetItem(pArgs, idx, pValueCurrentStock);
      }

      PyObject* pValueSource= PyUnicode_FromString(dataSource.c_str());
      PyTuple_SetItem(pArgs, numStocks, pValueSource);

      PyObject* pValueStart = PyUnicode_FromString(start.c_str());
      PyTuple_SetItem(pArgs, numStocks + 1, pValueStart);

      PyObject* pValueEnd = PyUnicode_FromString(end.c_str());
      PyTuple_SetItem(pArgs, numStocks + 2, pValueEnd);

      PyObject* pValueFilePath = PyUnicode_FromString(filePath.c_str());
      PyTuple_SetItem(pArgs, numStocks + 3, pValueFilePath);

      pValue = PyObject_CallObject(pFunc, pArgs);
      Py_DECREF(pArgs);
      if (pValue != NULL)
      {
        Py_DECREF(pValue);
      }
      else
      {
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        PyErr_Print();
        fprintf(stderr,"Call failed\n");
        return 1;
      }
    }
    else
    {
      if (PyErr_Occurred())
        PyErr_Print();
    }

    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
  }
  else
  {
    PyErr_Print();
    return 1;
  }

  return 0;
}

#endif