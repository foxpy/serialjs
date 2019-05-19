# serialjs

~~A JavaScript framework for Serial communication.~~

A little program to transfer Xbox 360 gamepad input data to virtual
COM port for use by development board controlling a robot. It is designed
to convert gamepad input into commands like setting motors' speed or toggling
a certain servo. I am using `xboxdrv` userspace driver.

## Installation

```bash
git clone https://github.com/foxpy/serialjs
cd serialjs
make release -j$(nproc)
sudo make install
```

## How to use

```bash
serialjs [joystick] [acm device]
```

Usually your gamepad should be e. g. `/dev/input/js0`, but, actually, you are
able to use any file. To record 100 gamepad input events and use them in tests:

```bash
JS_STRUCT_SIZE=8 # the size of js event struct, consult kernel documentation:
# /path/to/your/kernel//build/Documentation/input/joydev/joystick-api.html
packets=100
dd if=/dev/input/js0 of=samples/my_sample bs=$JS_STRUCT_SIZE count=$packets status=progress
serialjs samples/my_sample
```

ACM device is any USB virtual COM port device attached to your PC/SBC/etc.
Usually, it is `/dev/ttyACM0`. You can use udev to run this program every time
a particular device is attached.

`/etc/udev/rules.d/99-serialjs.rules`:
```
SUBSYSTEM=="tty", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="5740", RUN += "/usr/bin/bash -c 'killall serialjs; systemd-run serialjs /dev/input/js0 /dev/ttyACM%n'"
```

For testing, you can specify arbitrary file as a second argument, the only
requirement is to use debug binary. Release binary will refuse to write to a
non-character file for safety reasons.

## Serial protocol for data transfer

<img src="https://raw.github.com/foxpy/serialjs/master/commands.svg?sanitize=true">
