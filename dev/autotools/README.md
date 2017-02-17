# Autotools skeleton

This is a project skeleton that uses criterion tests with the TAP test driver.

## How this works

Be sure to do the following to get similar setups to work:

1. copy `tap-driver.sh` from your automake installation ([`autogen.sh:4-7`](autogen.sh#L4-L7)).
2. create a wrapper script that contains the following ([`autogen.sh:12-15`](autogen.sh#L12-L15)):

  ```bash
  #!/bin/sh
  $1 -Otap:- --always-succeed 2>&1 >/dev/null
  ```

3. Check for criterion ([`configure.ac:5-7`](configure.ac#L5-L7)).
4. Check for awk ([`configure.ac:9`](configure.ac#L9)).
5. Check for `tap-driver.sh` ([`configure.ac:14`](configure.ac#L14)).
6. Set `LOG_COMPILER` to the path of the wrapper script created at step 2 ([`Makefile.am:2`](Makefile.am#L2)).
7. Set `LOG_DRIVER` to a command running `tap-driver.sh` with our found awk ([`Makefile.am:5-6`](Makefile.am#L5-L6)).
8. Register your test programs ([`Makefile.am:8-14`](Makefile.am#L8-L14)).

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
