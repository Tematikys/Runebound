# Runebound

* [About](#About)
* [Screenshots](#Screenshots)
* [Installation](#Installation)

## About
Runebound is a popular strategic tabletop game in the fantasy genre for 2-4 players, previously not adapted for PC. The action takes place in a fantasy world. Each player goes through various trials during the game. The goal of the game may vary depending on the chosen scenario. The main menu screen includes game creation and joining an existing one. The main screen is the playing field where players or objects in the world are located. During the game, when completing specific tasks, the player has access to the store, battles, and inventory screen.

## Screenshots
TODO

## Installation
Requirements:
1. [MSYS2 (minGW toolchain)](https://www.msys2.org/)
2. [SDL2](https://github.com/libsdl-org/SDL/releases/tag/release-2.26.4)
3. [SDL image](https://github.com/libsdl-org/SDL_image/releases)
4. [SDL2 gfx](https://sourceforge.net/projects/sdl2gfx/)
5. [SDL ttf](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.20.2)
6. [Boost](https://packages.msys2.org/package/mingw-w64-x86_64-boost)
7. [CMake](https://cmake.org/download/)
8. [nlohmann::json](https://github.com/nlohmann/json)
   
and put SDL `.dll` files in root dir of this repository.
To build executables write this command in console
```powershell
  cmake . --build
```
then `graphics_client.exe` and `network_server.exe` will appear in the root directory. 
