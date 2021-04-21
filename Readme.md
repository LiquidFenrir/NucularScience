# NucularScience

Management/design game for [GBA Jam 2021](https://itch.io/jam/gbajam21) inspired by `Reactor Incrememental`, written in C++20 using the [butano engine](https://github.com/GValiente/butano) by [GValiente](https://github.com/GValiente).  

## Story

Create a reactor that won't overheat, leave it running for a night, sell the power it produced, then upgrade your components and do it all again!  
Your goal is to unlock the ultimate power source and use it, just once is fine to beat the game.  
Depending on your luck, you can get stuck without money! Just leave the reactor empty for the day to go look for cash elsewhere.  

## Usage

### Runnning

### Compiling

1. Follow [butano's Getting Started guide](https://gvaliente.github.io/butano/getting_started.html) up to and including the `Python` step
2. Download butano from the releases page (last tested with 6.10.0)
3. Clone this repository in a folder, and extract butano to that same folder, then rename its versioned folder to `butano`
4. Run `make` in this repository's folder using the msys2 shell on Windows, or your preferred terminal emulator on *nix

At the end of compilation, your project folder tree should look like this
```
- Your Folder
  - butano
    - ...
  - NucularScience
    - build
    - ...
    - NucularScience.gba
```

## License

This game, its code (`src/`, `include/`) and its assets (`graphics/`, `audio/`) are licensed under the zlib license unless specified otherwise.  
Specifically, the `graphics/butano_background.bmp`, `graphics/butano_big_silhouette.bmp` and `graphics/butano_big_sprite.bmp` images from butano-fighter are adapted from [Sun Ladder's work](https://commons.wikimedia.org/wiki/File:ButaneGasCylinder_WhiteBack.jpg) and licensed under the Creative Commons Attribution-Share Alike 3.0 Unported license  

## Credits

- devkitPro for the devkitARM toolchain making GBA homebrew possible  
- GValiente for butano, and thus all the third party libraries it uses as well (check out its readme!), the examples and butano-fighter which were great help for figuring out how things worked.  
- Tomas Pettersson for [sfxr](http://www.drpetter.se/project_sfxr.html), neat tool used to make the `.wav` files in `audio/`  
- the gbadev discord guild people for hosting GBA Jam 2021  