# serialjs

~~A JavaScript framework for Serial communication.~~

A little program to transfer Xbox 360 gamepad input data to virtual
COM port for use by development board controlling a robot. It is designed
to convert gamepad input into commands like setting motors' speed or toggling
a certain servo. I am using `xboxdrv` userspace driver.

# Installation

```bash
git clone https://github.com/foxpy/serialjs
cd serialjs
make
sudo make install
```

# Serial protocol for data transfer

<img src="https://raw.github.com/foxpy/serialjs/master/commands.svg?sanitize=true">
