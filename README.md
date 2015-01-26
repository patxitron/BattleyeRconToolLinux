# Simple Battleye RCON tool in modern C++ (C++11 and boost::asio)

This is a simple rcon tool for linux (actually it should be multiplattform).

Its usage is pretty similar to the Battleye's official one for windows but you
can specify ip/name and port of the server in command line.

Typical usage:

```
rconTool server.name.com 2302
```

At this moment, you can only exit by pressing Ctrl+C
