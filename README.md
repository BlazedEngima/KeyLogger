# KeyLogger
This is a short and simple project to get me into kernel programming
and learning about the Linux kernel tree from making LKMS.

In the future, this may get updated to cover more of these projects.

The keyloggers are of two types. One reads key presses from a keyboard
notifier and the other acts as a seperate interrupt driver for the keyboard
to get the key presses. Both log the key presses in the kernel logs.

## Building
To build, simply call `make` in the command line and it will build both of the
keyloggers.

## Installing
To install, call `sudo insmod keylogger_int_handler.ko` and `sudo insmod keylogger_notifier.ko`
and it will install the LKMs into the kernel module during runtime. Now when you type on the
keyboard typing `sudo dmesg` will show the logs of the key presses. It is recommended that you
only install one of them at a time.

Additionally to remove these kernel module, simply call `sudo rmmod keylogger_int_handler.ko`
or `sudo rmmod keylogger_notifier.ko`. To check if the kernel module is still installed in the
kernel, simply check via `sudo lsmod`.
