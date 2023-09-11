# PuzzleCrushGame

A multiplayer game, similar to the famous game Candy Crush, where the players need to shoot to a pyramid and get more points to win.

## Version

Unreal Engine 5.2.1

## How to test it?

### PIE 
- The default map is GameMap, make sure you are in that map and press play with net mode StandAlone/Client/ListenerServer (The game should work in those scenarios)

### Run a game instance in command line
You can use the .bat files in the root of the project.
The first time you run those .bat, they will ask for the Unreal engine source path. Normally the path location is `C:\Program Files\Epic Games\UE_5.2`. If you need to change the path location or if you accidentally mistype it. You can simply modify the `UEPath.txt` file that will be created after you type the path.

#### .bat files
- `RunServer.bat`: this game instance will simulate a dedicate server build.
- `RunListenServer.bat`: this game instance will simulate a listen server build.
- `RunClient.bat`: this game instance will simulate a client or standalone build.

NOTE: Both servers .bat should run first before running the game client.


## Class Diagram

![266845137-a0e173ba-9f4a-483e-833b-2840d74b9e4c](https://github.com/ingridwarrior2008/PCGame/assets/1762283/9d073ca7-0dab-46a3-a26f-899a4dad33bb)


# Debug

## UPCGCheatManager
A manager class for testing and debugging code. If you need to test gameplay functionality, press a key to access the terminal and find the cheat code you need.

Make sure before using the following cheats to activate in the terminal `EnableCheats`.

I added the following cheats 

- `Server_PrintPyramidAdjacentBlockInLocation` : Given a location, this function will print the adjacent grid block on the screen or log on the server. (for command line builds only can be use on listen server)
- `PrintPyramidAdjacentBlockInLocation` : It will print on the owning client the adjacent grid block in a given location.
- `PrintFibonacciScoreSequence` : It will print on screen or log a fibonnaci sequence
- `Server_DestroyBlockInLocation` : Given a location, It will destroy a grid block and the adjacents on the server and replicated to the clients.
- `Server_FindNearestUpperBlockFromLocation` : Given a location, It will find the nearest upper grid block on server.

# Data table Definitions
## DT_PyramidColorDefinition

In this data table, you can add the colors of the pyramids grid. The color will be chosen randomly. 

![image](https://github.com/ingridwarrior2008/PCGame/assets/1762283/9a7396b4-ad25-4459-a0fe-d532c205b4e0)


## DT_PyramidDefinition

The data table for the definition of the pyramid system. you can add the list of the color of the previous definition the grid class to spawn and the number of rows in the pyramid.

![image](https://github.com/ingridwarrior2008/PCGame/assets/1762283/58d55a86-2d2c-4ae8-a876-3d6bef0276ef)


# Multiple pyramid system
The project allow you to test multiple pyramids, try the map `GameMap_MultiplePyramids` to test it.

