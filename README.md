# fsNotifierDaemon
This repository contains all the files needed by the daemon fsNotifier. 
This particular daemon is responsible for detecting the changes with inotify and sending notifications to [fsCheck](https://github.com/Aritzherrero4/fsCheckDaemon).

## Configuration
The configuration file fsNotifier.config allows the user to specify the base path that will be the root directory of the system. This file must be in the same directory of the executable. 

Apart of this, systemd and daemon specific configurations are managed though the file fsNotifier.service. There, the `ExecPath` and `WorkingDirectory` should be indicated in order to use it with systemctl. 
The `ExecPath` parameter must point to the executable file generated after the compilation and the `WorkingDirectory` should be the directory with the configuration file, usually the same directory that has the executable file.

## Dependencies

The code must be compiled with C++17, since it uses the implementation of the Filesystem library included in that release. Apart of that, one component must be installed before compiling:

* libsystemd-dev: used for sd_bus and logging.

## Building the program

With the repository, a makefile is included in order to facilitate the compilation process. To compile it, run:

```bash
make
```
### Other make options

* `make clean`: Clean all the files related to the program.
* `make fresh`: Shortcut for `make clean` and `make`.


### ARM support
The code is multiplatform ready and it works for ARM and x86.

## Executing the program

The program can be run as a normal program by just executing it in a shell:

```
./fsNotifierDaemon &
```

But it can also be run as a daemon. To do it, first build the program and indicate the correct path to the executable and the working directory in fsNotifier.service as indicated in the configuration section. 

Then add it with systemctl:
```
systemctl --user enable /path/to/the/daemon/fsNotifier.service

```

Now, the daemon can be managed as a normal daemon with systemctl:

```
systemctl --user start fsNotifier.service
systemctl --user stop fsNotifier.service
systemctl --user status fsNotifier.service
```

If this daemon is executed using systemctl, fsCheck should be available to be started via systemctl as it is a requirement indicated in the service file.
## Checking the logs

The program writes to the system log directly when it is completely initialized or every time there is a change. This logs can be checked directly with:

```
journalctl -n20 | grep fsNotifier
```

Or, if the program was launched with systemctl, in addition to that, systemctl can be used:

```
systemctl --user status fsNotifier.service
```

## Intellectual property
The code is copyright 2021 Aritz Herrero and released under MIT License.
For more information, check [LICENCE](https://github.com/Aritzherrero4/fsNotifierDaemon/blob/master/LICENSE) file.


