# Autotools skeleton

This is a project skeleton that uses criterion tests with the TAP test driver.

## How this works

Be sure to do the following to get similar setups to work:

1. Check for `Criterion` ([`configure.ac:5-7`](configure.ac#L5-L7)).
1. Substitute the flags needed to link against `Criterion` ([`configure.ac:9-10`](configure.ac#L9-L10)).
1. Check for `awk` ([`configure.ac:12`](configure.ac#L12)).
1. Check for `tap-driver.sh` ([`configure.ac:17`](configure.ac#L17)).
1. Set `LOG_DRIVER` to a command running `tap-driver.sh` with our found awk ([`Makefile.am:2-3`](Makefile.am#L2-L3)).
1. Set `AM_TESTS_ENVIRONMENT` to ensure that `Criterion` outputs its results using TAP ([`Makefile.am:6-8`](Makefile.am#L6-L8)).
1. Register your test program ([`Makefile.am:10-17`](Makefile.am#L10-L17)).

## Running the tests

The default setup assumes that criterion is installed on your system.

```
$ ./autogen.sh
$ mkdir build && cd build
$ ../configure
$ make check
```

## License

The project skeleton is licensed under the [wtfpl](http://www.wtfpl.net). Do
whatever you want with it.

### License clause

This program is free software. It comes without any warranty, to  
the extent permitted by applicable law. You can redistribute it  
and/or modify it under the terms of the Do What The Fuck You Want  
To Public License, Version 2, as published by Sam Hocevar. See  
http://www.wtfpl.net/ for more details.  
