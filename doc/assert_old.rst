.. _old-assertions-ref:

Old assertion reference
=======================

.. note::
   This documents the old-but-stable assertion API. This API will eventually
   become deprecated then removed in Criterion 3.0.0. We suggest looking into
   using the new assertion API instead: :ref:`assertions-ref`.

This is an exhaustive list of all assertion macros that Criterion provides.

As each ``assert`` macros have an ``expect`` counterpart with the exact same
number of parameters and name suffix, there is no benefit in adding ``expect``
macros to this list. Hence only ``assert`` macros are represented here.

All ``assert`` macros may take an optional ``printf`` format string and
parameters.

Base Assertions
---------------

.. doxygengroup:: BaseAsserts

Common Assertions
-----------------

.. doxygengroup:: CommonBinAsserts
.. doxygengroup:: CommonUnaryAsserts
.. doxygengroup:: FloatAsserts

String Assertions
-----------------

.. doxygengroup:: StringAsserts

Wide String Assertions
----------------------

.. doxygengroup:: WideStringAsserts

Array Assertions
-----------------

.. doxygengroup:: ArrayAsserts
.. doxygengroup:: SafeArrCmpAsserts

Exception Assertions
--------------------

.. doxygengroup:: ExceptionAsserts

File Assertions
---------------

.. doxygengroup:: FileAsserts
