# alsa-volume-monitor

`alsa-volume-monitor` is a simple program written in C that listens to ALSA events and prints 'valueChanged' to stderr when something has been changed (e.g. volume).

It was created for use with Awesome WM in volume indicator widgets and had DBus integration, but now it just prints text to stderr.

### Usage
`./alsa-volume-monitor hw:0`, where `hw:0` is the name of your audio card.

`./alsa-volume-monitor default` also works fine.

### Compiling on Ubuntu

To build the app on Ubuntu, you need to install `libasound2-dev`.

To build the app on Gentoo, you need to install `media-libs/alsa-lib` from official repository.

### License
GPLv2
