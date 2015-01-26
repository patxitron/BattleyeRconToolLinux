# Simple Battleye RCON tool in modern C++ (C++11 and boost::asio)

This is a simple rcon tool for linux (actually it should be multiplattform).

Its usage is pretty similar to the Battleye's official one for windows but you
can specify ip/name and port of the server in command line.

Typical usage:

```
rconTool server.name.com 2302
```

At this moment, you can only exit by pressing Ctrl+C

## Dependencies

[Boost C++ libraries](http://www.boost.org/) are the only needed dependency.
Moreover only boost::asio and boost::system are used.

This project includes eclipse CDT project files, but eclipse is not needed to
build the project as it has a self-contained Makefile. So change to the
directory and type 'make' to build it.

## Lincense

This software is distributed under the terms of the MIT license.

License terms follow:

```
Copyright (c) 2015 Francisco J. Lazur

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```

