# Contributing

Contributions are welcomed, but must follow a simple set of rules in order to
be merged.

**Please follow these conventions if you want your pull request(s) accepted.**

## General

* Try to respect a 80 chars column limit. Not mandatory, but appreciated.
* No trash files. Trash files are by-products of the compilation process, or
  generated files that does not need to be under version control.
* Pull requests must pass the style check. See "Passing the style check" section below.
* Pull requests must compile and work properly.
* Pull requests must pass all tests.
* Pull requests must be mergeable without conflicts.
* Number of commits in a pull request should be kept to one commit and all
  additional commits must be squashed.
* You may have more than one commit in a pull request if the commits are
  separate changes, otherwise squash them.
* Keep your commit messages simple and concise. Good commit messages start
  with a verb ('Add', 'Fix', ...).
* Commit messages should follow the format `topic: message`, e.g. `cli: Added --foo flag`.
  In cases where multiple topics are relevant, use a comma-separated list of topics, e.g.
  `api,float: Fixed printf format specifiers for long double`

## Passing the style check

This project use an automated style checking process for pull requests defined as
an uncrustify configuration file named `.uncrustify.cfg`.

To ease the development process, a pre-commit hook is installable by running
`.githooks/install.sh` from the project root directory. This pre-commit hook
checks for style, outputs a diff of the changes that need to be done, and
produces a patch to fix them automatically.

As the automatic style checker is bound to have errors, parts of the code
that make the checker fail because of their tricky grammar (e.g. Duff devices)
may be enclosed by `/* *INDENT-OFF* */` and `/* *INDENT-ON* */` to disable
the style checker on the region. Note that this feature should be used
*exceptionally*, and that the maintainer will always have the last word regarding
stylistic choices that cannot pass the style check.

## Testing

Testing your changes is important. When you submit a pull request, the CI system
will trigger a build to test your changes on various platforms; however, to speed
up the initial development process, it's best to test it locally at first.

The following commands should get you going with a workable test system:

```
$ mkdir -p build && cd $_
$ cmake -DDEV_BUILD=1 -DCTESTS=1 ..
$ cmake --build . --target criterion_tests
$ ctest
```

Make sure you have cram 0.7 installed before running ctest; Criterion uses cram
internally for its system tests:

```
$ pip install --user cram==0.7

or alternatively

$ sudo pip install cram==0.7
```

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
    |- dev/: Developer files
    |- include/criterion/: Public API
    |- src/: Sources for libcriterion
    |   |- compat/: Cross-platform abstractions for platform-dependent code
    |   |- core/: Core mechanisms used to run the tests
    |   |- entry/: Entry-point related sources, and default main function
    |   |- io/: IO related functions, redirections
    |   |- log/: Output providers, all the output logic in general
    |   `- string/: String manipulation functions, i18n
    |- po/: Translation files, i18n stuff
    |- test/: Unit tests for libcriterion
    `- samples/: Sample files
        |- outputs/: Expected output files for the current samples
        `- tests/: Internal regression tests
            `- outputs/: Expected output files for the regression tests

## Release Checklist

* [ ] Make sure `bleeding` builds on all platforms and all tests passes.
* [ ] `git checkout master`
* [ ] `git merge bleeding --no-commit --no-ff`
* [ ] Bump version
* [ ] Update cram tests
* [ ] Update changelog
* [ ] `git commit -m "vX.Y.Z: merging bleeding back to master"`
* [ ] `git tag -s vX.Y.Z`
* [ ] `git branch -f bleeding`
* [ ] `git push origin master bleeding vX.Y.Z`
