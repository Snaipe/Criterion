.. _assertions-ref:

Assertion reference
===================

This is an exhaustive list of all assertion macros that Criterion provides.

.. attention::
   This documents the new (but experimental) assertion API. To see the
   documentation of the old API, see: :ref:`old-assertions-ref`.

The new assertions API is centered around the use of criteria pseudo-functions
to check various properties of the values being tested. Of note, there no longer
are ``cr_assert_<xyz>`` macros, and instead all functionality has been merged into
the ``cr_assert`` and ``cr_expect`` macros. For instance, instead of using
``cr_assert_eq(1, 2)`` one must use instead ``cr_assert(eq(int, 1, 2))``, which
involves the ``eq`` criterion and the ``int`` type tag.

To use the new assertion API, one must include the ``<criterion/new/assert.h>`` header.

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
