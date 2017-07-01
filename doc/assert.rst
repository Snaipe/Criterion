.. _assertions-ref:

Assertion reference
===================

This is an exhaustive list of all assertion macros that Criterion provides.

As each ``assert`` macros have an ``expect`` counterpart with the exact same
number of parameters and name suffix, there is no benefit in adding ``expect``
macros to this list. Hence only ``assert`` macros are represented here.

All ``assert`` macros may take an optional ``printf`` format string and
parameters.

Assertion API
-------------

.. doxygengroup:: AssertAPI

Assertion Criteria
------------------

.. doxygengroup:: Criteria
.. doxygengroup:: TaggedCriteria

Tags
----

.. doxygengroup:: Tags
