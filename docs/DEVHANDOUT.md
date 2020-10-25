# Setting up your dev env

To develop efficiently you are going to need to setup your env to have the following behaviors:

- code prediction
- error prediction
- syntax highlighting
- automated build system
- automated debug execution

## Visual Studio Code

### extensions

- [C/C++ by Microsoft](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) For error prediction, syntax highlighting, pretty colors, and debugging!
- [C++ Intellisense by austin](https://marketplace.visualstudio.com/items?itemName=austin.code-gnu-global) Optional: For code prediction and auto-completion, use this if the Intellisense portion of Microsoft's extension doesnt seem to be working very well.
- [markdownlint by David Anson](https://marketplace.visualstudio.com/items?itemName=DavidAnson.vscode-markdownlint) (If you will be adding markdown docs, like me :D)

### .vscode files

Create a folder called .vscode in your root directory if you don't already have one.  In those folders you will add 3 files:

- c_cpp_properties.json:

```JSON
    {
        "configurations": [
            {
                "name": "Linux",
                "includePath": [
                    "${workspaceFolder}/src/**",
                    "/usr/include/SDL2"
                ],
                "defines": [],
                "compilerPath": "/usr/bin/gcc",
                "cStandard": "gnu11",
                "cppStandard": "gnu++14",
                "intelliSenseMode": "${default}"
            }
        ],
        "version": 4
    }
```

We use this file to configure our C/C++ by Microsoft extension.

- tasks.json:

```JSON
    {
        "version": "2.0.0",
        "tasks": [
            {
                "type": "shell",
                "label": "compile-client",
                "command": "make -f Makefile_client",
                "options": {
                    "cwd": "${workspaceFolder}"
                },
                "problemMatcher": [
                    "$gcc"
                ],
                "group": {
                    "kind": "build",
                    "isDefault": true
                }
            },
            {
                "type": "shell",
                "label": "compile-server",
                "command": "make -f Makefile_server",
                "options": {
                    "cwd": "${workspaceFolder}"
                },
                "problemMatcher": [
                    "$gcc"
                ],
                "group": "build"
            }
        ]
    }
```

This is for building our C++ applications.

- launch.json

```JSON
    {
        "version": "0.2.0",
        "configurations": [
            {
                "name": "Client",
                "type": "cppdbg",
                "request": "launch",
                "program": "${workspaceFolder}/bin/Speedrun",
                "args": [],
                "stopAtEntry": false,
                "cwd": "${workspaceFolder}/bin",
                "environment": [],
                "externalConsole": false,
                "MIMode": "gdb",
                "setupCommands": [
                    {
                        "description": "Enable pretty-printing for gdb",
                        "text": "-enable-pretty-printing",
                        "ignoreFailures": true
                    }
                ],
                "preLaunchTask": "compile-client"
            },
            {
                "name": "Server",
                "type": "cppdbg",
                "request": "launch",
                "program": "${workspaceFolder}/bin/SpeedrunServer",
                "args": [],
                "stopAtEntry": false,
                "cwd": "${workspaceFolder}/bin",
                "environment": [],
                "externalConsole": true,
                "MIMode": "gdb",
                "setupCommands": [
                    {
                        "description": "Enable pretty-printing for gdb",
                        "text": "-enable-pretty-printing",
                        "ignoreFailures": true
                    }
                ],
                "preLaunchTask": "compile-server"
            }
        ]
    }
```

This file describes how to launch our programs.  It is also configured to build an updated version automatically for you.  These launch configurations are executed using the debug button in VSCode and selecting the proper config in the dropdown.  The server configuration spawns a new system console, while the client application uses the vscode terminal for output and, obviously, a window for rendering the game application.  
