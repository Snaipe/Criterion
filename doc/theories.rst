Using theories
==============

`Theories`_ are a powerful tool for test-driven development, allowing you
to test a specific behaviour against all permutations of a set of user-defined
parameters known as "data points".

.. _Theories: http://web.archive.org/web/20110608210825/http://shareandenjoy.saff.net/tdd-specifications.pdf

Adding theories
---------------

.. doxygengroup:: TheoryBase

Adding theories is done by defining data points and a theory function:

.. code-block:: c

    #include <criterion/theories.h>

    TheoryDataPoints(suite_name, test_name) = {
        DataPoints(Type0, val0, val1, val2, ..., valN),
        DataPoints(Type1, val0, val1, val2, ..., valN),
        ...
        DataPoints(TypeN, val0, val1, val2, ..., valN),
    }

    Theory((Type0 arg0, Type1 arg1, ..., TypeN argN), suite_name, test_name) {
    }

``suite_name`` and ``test_name`` are the identifiers of the test suite and
the test, respectively. These identifiers must follow the language
identifier format.

``Type0/arg0`` through ``TypeN/argN`` are the parameter types and names of theory
theory function and are available in the body of the function.

Datapoints are declared in the same number, type, and order than the parameters
inside the ``TheoryDataPoints`` macro, with the ``DataPoints`` macro.
Beware! It is undefined behaviour to not have a matching number and type of
theory parameters and datatypes.

Each ``DataPoints`` must then specify the values that will be used for the
theory parameter it is linked to (``val0`` through ``valN``).

Assertions and invariants
-------------------------

You can use any ``cr_assert`` or ``cr_expect`` macro functions inside the body of a
theory function.

Theory invariants are enforced through the ``cr_assume(Condition)`` macro function:
if ``Condition`` is false, then the current theory iteration aborts without
making the test fail.

On top of those, more ``assume`` macro functions are available for common operations:

.. doxygengroup:: TheoryInvariants

Configuring theories
--------------------

Theories can optionally recieve configuration parameters to alter the behaviour
of the underlying test; as such, those parameters are the same ones as the ones
of the ``Test`` macro function (c.f. :ref:`test-config-ref`).

Full sample & purpose of theories
---------------------------------

We will illustrate how useful theories are with a simple example using Criterion:

The basics of theories
~~~~~~~~~~~~~~~~~~~~~~

Let us imagine that we want to test if the algebraic properties of integers,
and specifically concerning multiplication, are respected by the C language:

.. code-block:: c

    int my_mul(int lhs, int rhs) {
        return lhs * rhs;
    }

Now, we know that multiplication over integers is commutative, so we first test
that:

.. code-block:: c

    #include <criterion/criterion.h>

    Test(algebra, multiplication_is_commutative) {
        cr_assert_eq(my_mul(2, 3), my_mul(3, 2));
    }

However, this test is imperfect, because there is not enough triangulation to
insure that my_mul is indeed commutative. One might be tempted to add more
assertions on other values, but this will never be good enough: commutativity
should work for *any* pair of integers, not just an arbitrary set, but, to be
fair, you cannot just test this behaviour for every integer pair that exists.

Theories purposely bridge these two issues by introducing the concept of
"data point" and by refactoring the repeating logic into a dedicated function:

.. code-block:: c

    #include <criterion/theories.h>

    TheoryDataPoints(algebra, multiplication_is_commutative) = {
        DataPoints(int, [...]),
        DataPoints(int, [...]),
    };

    Theory((int lhs, int rhs), algebra, multiplication_is_commutative) {
        cr_assert_eq(my_mul(lhs, rhs), my_mul(rhs, lhs));
    }

As you can see, we refactored the assertion into a theory taking two unspecified
integers.

We first define some data points in the same order and type the parameters have,
from left to right: the first ``DataPoints(int, ...)`` will define the set of values passed
to the ``int lhs`` parameter, and the second will define the one passed to ``int rhs``.

Choosing the values of the data point is left to you, but we might as well use
"interesting" values: ``0``, ``-1``, ``1``, ``-2``, ``2``, ``INT_MAX``, and ``INT_MIN``:

.. code-block:: c

    #include <limits.h>

    TheoryDataPoints(algebra, multiplication_is_commutative) = {
        DataPoints(int, 0, -1, 1, -2, 2, INT_MAX, INT_MIN),
        DataPoints(int, 0, -1, 1, -2, 2, INT_MAX, INT_MIN),
    };

Using theory invariants
~~~~~~~~~~~~~~~~~~~~~~~

The second thing we can test on multiplication is that it is the inverse function
of division. Then, given the division operation:

.. code-block:: c

    int my_div(int lhs, int rhs) {
        return lhs / rhs;
    }

The associated theory is straight-forward:

.. code-block:: c

    #include <criterion/theories.h>

    TheoryDataPoints(algebra, multiplication_is_inverse_of_division) = {
        DataPoints(int, 0, -1, 1, -2, 2, INT_MAX, INT_MIN),
        DataPoints(int, 0, -1, 1, -2, 2, INT_MAX, INT_MIN),
    };

    Theory((int lhs, int rhs), algebra, multiplication_is_inverse_of_division) {
        cr_assert_eq(lhs, my_div(my_mul(lhs, rhs), rhs));
    }

However, we do have a problem because you cannot have the theory function divide
by 0. For this purpose, we can ``assume`` than ``rhs`` will never be 0:

.. code-block:: c

    Theory((int lhs, int rhs), algebra, multiplication_is_inverse_of_division) {
        cr_assume(rhs != 0);
        cr_assert_eq(lhs, my_div(my_mul(lhs, rhs), rhs));
    }

``cr_assume`` will abort the current theory iteration if the condition is not
fulfiled.

Running the test at that point will raise a big problem with the current
implementation of ``my_mul`` and ``my_div``:

.. code-block:: none

    [----] theories.c:24: Assertion failed: (a) == (bad_div(bad_mul(a, b), b))
    [----]   Theory algebra::multiplication_is_inverse_of_division failed with the following parameters: (2147483647, 2)
    [----] theories.c:24: Assertion failed: (a) == (bad_div(bad_mul(a, b), b))
    [----]   Theory algebra::multiplication_is_inverse_of_division failed with the following parameters: (-2147483648, 2)
    [----] theories.c:24: Unexpected signal caught below this line!
    [FAIL] algebra::multiplication_is_inverse_of_division: CRASH!

The theory shows that ``my_div(my_mul(INT_MAX, 2), 2)`` and ``my_div(my_mul(INT_MIN, 2), 2)``
does not respect the properties for multiplication: it happens that the
behaviour of these two functions is undefined because the operation overflows.

Similarly, the test crashes at the end; debugging shows that the source of the
crash is the divison of INT_MAX by -1, which is undefined.

Fixing this is as easy as changing the prototypes of ``my_mul`` and ``my_div``
to operate on ``long long`` rather than ``int``.

What's the difference between theories and parameterized tests ?
----------------------------------------------------------------

While it may at first seem that theories and parameterized tests are the same,
just because they happen to take multiple parameters does not mean that they
logically behave in the same manner.

Parameterized tests are useful to test a specific logic against a fixed, *finite*
set of examples that you need to work.

Theories are, well, just that: theories. They represent a test against an
universal truth, regardless of the input data matching its predicates.

Implementation-wise, Criterion also marks the separation by the way that both
are executed:

Each parameterized test iteration is run in its own test; this means that
one parameterized test acts as a collection of many tests, and gets reported
as such.

On the other hand, a theory act as one single test, since the size and contents
of the generated data set is not relevant. It does not make sense to say that
an universal truth is "partially true", so if one of the iteration fails, then
the whole test fails.
