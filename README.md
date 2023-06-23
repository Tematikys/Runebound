# Runebound

* [About](#About)
* [Installation](#Installation)
* [Screenshots](#Screenshots)

## About
Runebound is a popular strategic tabletop game in the fantasy genre for 2-4 players, previously not adapted for PC. The action takes place in a fantasy world. Each player goes through various trials during the game. The goal of the game may vary depending on the chosen scenario. The main menu screen includes game creation and joining an existing one. The main screen is the playing field where players or objects in the world are located. During the game, when completing specific tasks, the player has access to the store, battles, and inventory screen.

## Screenshots
TODO

## Installation
This software is developed only for the Windows, so you will need to install minGW compiler, CMake, boost by yourself. Also you wil need nlohmann::json, but is already included in this reposotory, and finally SDL2. Here are the links of source files, which should be placed into tpl/SDL2 folder:
1. https://github.com/libsdl-org/SDL/releases/tag/release-2.26.4
2. https://github.com/libsdl-org/SDL_image/releases
3. https://sourceforge.net/projects/sdl2gfx/
4. https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.20.2
   
and put .dll files in root dir of this repository.
To build executables write this comman in console
```powershell
  cmake . --build
```
then `graphics_client.exe` and `network_server.exe` will appear in root directory. 
