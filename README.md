![2048 logo](https://upload.wikimedia.org/wikipedia/commons/8/8a/2048_logo.png)

# 2048

Welcome to 2048, a game written in C!

**_I made this game as a project for my C programming course at the University of Milan._**

## Features

- Uses a database to store a leaderboard
- You can save your progress during the game and resume later
- Debug mode available by running the game with the `-d` parameter

## Requirements

- Linux-based operating system
- [libsqlite3-dev](https://www.sqlite.org/download.html)

## Installation

1. Install libsqlite3-dev:

```bash
sudo apt-get install libsqlite3-dev
```

2. Compile the game:

```bash
gcc main.c -o 2048 -lsqlite3
```

## Usage

To start the game, run:
    
```bash
./2048
```


To enable debug mode, run:

```bash
./2048 -d
```

To save your progress during the game, or load it see the first menu when you start the game.


## License

This project is licensed under the terms of the [MIT license](LICENSE).

