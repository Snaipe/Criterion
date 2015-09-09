# Contributing

Contributions are welcomed, but must follow a simple set of rules in order to
be merged.

**Please follow these conventions if you want your pull request(s) accepted.**

## General

* Use 4 (four) spaces for indentation.
* No trailing whitespaces.
* 80 chars column limit.
* No trash files. Trash files are by-products of the compilation process, or
  generated files that does not need to be under version control.
* Pull requests must compile and work properly.
* Pull requests must pass all tests.
* Pull requests must be mergeable automatically.
* Number of commits in a pull request should be kept to one commit and all
  additional commits must be squashed.
* You may have more than one commit in a pull request if the commits are
  separate changes, otherwise squash them.

## Translations

* You can contribute new translation files for output messages, on the
  condition that you are fluent with the language itself.
* Each correction on existing translations must be followed by a
  rationale ("why would the translation be better if the change is applied?")

## Roadmap

    .
    |- .cmake/: CMake modules
    |- dependencies/: dependencies for building libcriterion
    |- doc/: Sphinx documentation files
    |- include/criterion/: Public API
    |- src/: Sources for libcriterion
    |   `- log/: Output providers, all the output logic in general
    |- po/: Translation files, i18n stuff
    |- test/: Unit tests for libcriterion
    `- samples/: Sample files
        `- tests/: Internal regression tests
