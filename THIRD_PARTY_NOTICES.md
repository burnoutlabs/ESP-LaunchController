# Third-party notices

## ESPUI

`src/main.cpp` started as the ESPUI example
`examples/completeLambda/completeLambda.ino` (author Ian Gray, @iangray1000)
from <https://github.com/s00500/ESPUI>, and has been modified since. ESPUI
itself is pulled in as a PlatformIO dependency (`lib_deps` in
`platformio.ini`) and is not vendored here.

ESPUI is distributed under the following licence, reproduced as published in
its repository:

```text
The MIT License (MIT)

Copyright (c) 2017 Lukas Bachschwell (s00500)
Authors: Lukas Bachschwell

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## `src/rlht.cpp` and `src/rlht.h`

The origin of these files has not been confirmed. They are an I2C client for a
relay-heater/thermocouple controller and are unrelated to launch control.
Until their origin is recorded here, do not assume the repository licence
covers them.
