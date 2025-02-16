# PRooFPS-dd

<p align="center">
  <a href="http://www.youtube.com/watch?feature=player_embedded&v=XPMMzPYjR98" target="_blank"><img src="PR00FPS-dd-logo.png" alt="Click to see the video!"/></a>
</p>

- Table of Contents [(generated by markdown-toc)](http://ecotrust-canada.github.io/markdown-toc/)
  * [Summary](#summary)
  * [Try the Game](#try-the-game)
  * [FAQ](#faq)
  * [Build](#build)

## Summary

This game is the successor of the [original PR00FPS made in 2007](https://github.com/proof88/PR00FPS).  
It uses my open-source [game engine](https://github.com/proof88/PGE).

This game is still **under development**.

**[Version History](HISTORY.md)**  
**[Known Bugs](https://github.com/proof88/PRooFPS-dd/labels/bug)**  
**[Known Network Performance Issues](https://github.com/proof88/PRooFPS-dd/issues?q=is%3Aopen+is%3Aissue+milestone%3A%22Networking+%28Public+Beta%29%22+label%3Aoptimization+sort%3Aupdated-desc)**  
**[Other Performance Issues](https://github.com/proof88/PRooFPS-dd/issues?q=is%3Aopen+is%3Aissue+-milestone%3A%22Networking+%28Public+Beta%29%22+label%3Aoptimization+sort%3Aupdated-desc+)**  
**[Full Backlog](https://github.com/proof88/PRooFPS-dd/milestones?direction=asc&sort=title&state=open)**

You can follow the project on my **[facebook page](https://www.facebook.com/whiskhyll)**.

## Try the Game

The latest available download version [v0.1.6.1 Private Beta is here](https://drive.google.com/file/d/1ruZMjLmeU571It93LEwDnRKt72csj2tv/view?usp=drive_link).  
If the game cannot start due to missing DLL files, [this Visual C++ Redistributable Package](https://drive.google.com/file/d/1B61VzifHvK-wTNGUai4HaEeik2cXLRHH/view?usp=share_link) should be installed.

[Version history is here.](HISTORY.md)

### Run as Server

To start as server, simply run `PRooFPS-dd.exe`.

### Run as Client

To start as client, simply run the convenient:
 - `PRooFPS-dd-as-client.bat` (Windows) or
 - `PRooFPS-dd-as-client.sh` (Linux).

To specify IP-address of the server for the client, change `cl_server_ip` CVAR in `gamedata/profiles/default/default.cfg`.  

Alternatively, you can use command line arguments as well, e.g.:  
`./PRooFPS-dd.exe --net_server=false --cl_server_ip=192.168.1.100`

### Keyboard, Mouse

 - **A/D, LEFT/RIGHT:** move the player (strafe);
 - **LSHIFT:** toggle move speed: walking or running;
 - **SPACE:** jump;
 - **LCTRL:** crouch/duck;
 - **R**: reload current weapon;
 - **2,3 / Mouse Wheel**: change to pistol or machine gun;
 - **Mouse Click**: attack (fire weapon);
 - **TAB:** show frag table;
 - **M:** switch to next map based on mapcycle.txt;
 - **BACKSPACE:** show GUI demo;
 - **ENTER/RETURN:** if `testing=true` CVAR is defined, it makes a dump of testing data into `RegTestDumpServer.txt` or `RegTestDumpClient.txt`;
 - **T**: if instance is server, it respawns the player to a random spawnpoint and dumps debug data to console window.

### Debugging

If you have any problem with the game, it is always recommended to save the latest generated log file(s) from the game's main directory.  
Their file name format is: `log_<hostname>_<date>_<time>.html` .  
Since they are saved in HTML format, it is easy to open and read them in your favourite web browser, but what is better is if you send them to the developer.

If you have performance issues and you are running the server instance of the game, pressing 'T' key anytime during the game saves extra information into the HTML log file.  

If you have network related issue, you can check real-time network statistics by displaying the frag table by pressing the 'TAB' key.  
Note that some network statistics are also saved into the HTML log file whenever a client- or server instance disconnects.

## FAQ

### Mouse Cursor is Not Visible in Fullscreen Menu when OBS Studio is Running, How to Fix It?

This seems to be an issue which does not happen on all computers but when it happens then **most probably you have a "Display Capture" set as Source** in OBS Studio.  
Sometimes simply **switching to another application and then switching back to the game (ALT+TAB, then ALT+TAB again)** resolves the issue, so this should be the first thing you try.  
As a next idea to resolve this issue, go to the Properties of your "Display Capture" Source and try to **change the Capture Method to "DXGI Desktop Duplication"**, also make sure **"Capture Mouse" is checked**.  
If this does not help, **remove "Display Capture"** from Sources, start the game, then switch to OBS Studio (ALT+TAB), add a **"Window Capture" Source** and select the game window. You might also need to change the Capture Method too.  
If the issue still persists, try to upgrade OBS Studio, your video card driver, or just simply turn Fullscreen mode off in game settings when you are using OBS Studio in parallel.

### Why the Compressed Release is So Big?

At this point of development, I'm not really concerned about the disk space usage of my game. We are talking about a ZIP file less than 10 MiBytes.  
I'm also fully aware of the existence of a big UNUSED texture file in map_warhouse directory, and I also know that textures could be downscaled a bit.  
Believe it or not but there is also a build configuration issue that results in unnecessary big executable size.  
Look at the version number and you will understand why disk space usage is really not my number 1 priority at this point.

## Build

You must have the Visual Studio solution file including other relevant projects as well in [PGE-misc](https://github.com/proof88/PGE-misc) repo.  
**Follow the build instructions** in [PGE-WoW.txt](https://github.com/proof88/PGE-misc/blob/master/src/PGE-WoW.txt).  
As described in that file, you need to build other projects before PRooFPS-dd can be built.
