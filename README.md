# pf
A simple pixelflut client for linux.

## But... Why?
Originally, I hacked together a pretty simple python script to get some pixels painted on a [pixelflut](https://wiki.cccgoe.de/wiki/Pixelflut) screen.
Then someone told me about `sendfile (2)` and its [python bindings](https://docs.python.org/3/library/socket.html#socket.socket.sendfile).
So I figured I might get away with just libpng and a bunch of linux syscalls (plus plain old threads later on, maybe).
Having worked with cmake in the past, I also wanted to try meson and get my hands dirty with some programming on a lower level than I'm used to.

## What does it do?
Read a PNG file, convert it to pixelflut commands and send those to a target host/port forever.
Usage is pretty straight forward (see `pf -h`):
```
Usage: pf [-H <host>] [-p <port>] <image>
```

## How do I build this again?
```
# Set up meson build directory
meson setup builddir && cd builddir

# Compile executable
meson compile

# Run unit tests
meson test
``` 

## Is this any good?
Probably not, but I have fun hacking on it.
