# ASCII Snake In Console/Terminal Window

## Description
The snake is inspired on tutorial videos from https://www.youtube.com/@NVitanovic <br />
The changes I made are:
- enclosed the game in a C++ class (instead of using global variables and functions)
- added linux equivalents of `_kbhit()` and `_getch()` from `<conio.h>` for Linux with a healp of `#if defined()` macros. (The `<conio.h>` library is supported only on Windows)
- Created a makefile for linux build
- replaced raw c-style arrays with vectors
- added new features like multiple frogs spawning, different game modes and sprint kaey
- used my own coding style

The game has 4 modes: `IMMORTAL`, `EASY`, `MEDIUM` and `HARD`;
To select one you need to specify it in the game constructor like: 
```
SnakeGame game = { SnakeGame::GameMode::MEDIUM };
```
Each game mode has its own features:
<ul>
<li><b>IMMORTAL:</b><br />
You can't be killed by hitting the wall or biting your tail. (The only way to end the game is by clicking the `X` key.)
Walking into a wall will teleport the snake's head into the other side.
</li>
<li><b>EASY:</b><br />
You can die biting your tail, but walls will still teleport you to the other side.
</li>
<li><b>MEDIUM:</b><br />
You will die by bting your tail or hitting the wall.
</li>
<li><b>HARD:</b><br />
You will die by biting your tail or hitting the wall. On each eaten frog the snake will go faster (till a certain point). This should make the game increasingly harder
</li>
</ul>

Control snakes directions with `WASD` / `wasd` keys.
You can activate sprint with the `.` key. (You can't control directions while sprinting)
The sprint key can be changed in the code by modifying `_SPRINT_KEY` value.
To exit game press `X` key.

The goal is to consume frogs spawning on the screen. The number of spawned frogs on the screen can be changed by modifying the value of `_AMOUNT_OF_FROGS_SPAWNING`.

You can also make the game faster or slower by modifying the `START_REFRESH_RATE_MS` const value.


## Demo
<p align="center">
  <img src="Demo.gif">
</p>



