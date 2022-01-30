# Total-order Forward Decomposition Algorithm: an HTN Planner library

This library provides the TFD algorithm - an HTN Planner so that developers can focus on writing Planning Domain and Planning Problem for their applications.

## Book and Python Implementation

The implementation of this algorithm is based on Total-order STN Planning in "Automated Planning: Theory & Practice" [book](https://www.amazon.sg/Automated-Planning-Practice-Malik-Ghallab/dp/1558608567).

This library is a C++17 version of [Pyhop](https://bitbucket.org/dananau/pyhop/src/master/) implementation.

The application that uses this library must implement the following:

- Planning Domain which has all methods and operators
- Planning Problem which has the initial state of the world and the task to be solved

If you're interested in understanding the concepts and algorithm you can read the blog post [here](https://towardsdatascience.com/total-order-forward-decomposition-an-htn-planner-cebae7555fff).
