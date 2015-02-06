
Criterion
=========

## Installation

```bash
$ git clone https://github.com/Snaipe/Criterion.git
$ cd Criterion
$ ./autogen.sh && ./configure && make && sudo make install
```

## Usage

Given a test file named test.c, compile it with `-lcriterion`:

```bash
$ gcc -o test test.c -lcriterion
```

## Samples

Sample tests can be found in the [sample directory](https://github.com/Snaipe/Criterion/tree/master/samples).

* [A simple test](https://github.com/Snaipe/Criterion/blob/master/samples/simple.c)
* [Using multiple suites](https://github.com/Snaipe/Criterion/blob/master/samples/suites.c)
* [Tests with signals](https://github.com/Snaipe/Criterion/blob/master/samples/signal.c)
* [Using report hooks](https://github.com/Snaipe/Criterion/blob/master/samples/report.c)

## F.A.Q.

**Q. What's wrong with other test frameworks?**
A. I worked with CUnit and Check, and I must say that they do their job
   very well -- the only thing that bugs me is that setting up a test
   suite from scratch is a pain, it should really be simpler. Most
   (if not all) high-level languages have test frameworks with automatic
   test registration, but all the ones for C require you to set up a
   main, manually register suites, then tests. Criterion tries to
   fix these shortcomings.

**Q. Where has this been tested?**
A. Currently, on Linux 2.6.32 and Linux 3.15.7, although it should work on
   most \*nix systems. More tests will be added on the build matrix.

**Q. Will this work under Windows/MSVC?**
A. Windows support with MinGW is coming, but MSVC is a bit of a lost cause
   for the C language. The project internally uses c99 features and gnu
   extensions, and MSVC is stuck at supporting c89.
