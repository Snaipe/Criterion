# CMake skeleton

This is a project skeleton that uses criterion for testing.

The tests are intentionally failing for the demonstration,
by including a buggy implementation of a few string.h functions.

## Running the tests

The default setup assumes that criterion is installed on your system.

```
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
$ ctest
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
