# No-bullshit game framework

Follow these instructions and you will be making games in no time. They work on Windows, Mac (with automatic scaling on Retina display) and Linux, as well as the browser when compiled with [Emscripten](https://emscripten.org/).

The game built from the sample code in this repo can be played at http://diablesdelescorts.cat/gaem2020_template/

## Project structure

  - `bin`: This is what you will end up shipping. Both the Makefile and Visual Studio solution are configured to output its binaries here, and the DLLs needed in Windows are also bundled.
  - `bin/data`: This is where you put your assets. This way, the binary are gonna find them both when doing development and when the game is shipped.
  - `engine`: All the helper classes that will make your life so much easier when creating your game. You shouldn't need to change the code here, but feel free to do so.
  - `src`: Your game code goes here. More on how to organize it in a second.
  - `vendor`: Dependencies I decided to include in the repo because C++ is from 1985, internet didn't exist, and there's not propper dependency management for the language.
  - `asset_src`: This is meant for you to put the Gimp files, Audacity projects, etc. for the assets in your game. By default, I included a [Tiled](https://www.mapeditor.org/) file and a python script that can generate C++ code from a Tiled map to use in your game.

## Assets

All assets are loaded from the beginning of the game, stored as globals and never unloaded, because your game is so small and your RAM so big they all fit in memory. Declare them in [src/assets.h](src/assets.h) and load them in [src/assets.cpp](src/assets.cpp).

Supported assets are:
  - PNG images, loaded as [GPU_Image*](engine/asset_load.h) textures. If you want to load PSDs or any other shit, you can use [this library](https://unity.com/).
  - OGG files, that can be loaded as [Mix_Music*](engine/asset_load.h) for buffered background music or as the [Sound](engine/sound.h) class for SFX. WAV is also supported if you like to waste disk space.
  - TTF fonts, which can be loaded as [TTF_Font*](engine/asset_load.h) and used to render texts using the [Text](engine/text.h) class.
  - Shaders, using the [Shader](engine/shader.h) class. A limitation for shaders is that they can't be applied full screen, only to a given texture.

## Scenes

The entrypoint to your awesome new game is [SceneMain](src/scene_main.h). The `Update` and `Draw` functions are called in a loop as long as the scene is active and I bet you can tell when the `EnterScene` and `ExitScene` functions are called. Fill these functions with your code. If you need more than one scene in your game, you can switch between them using the [SceneManager](engine/scene_manager.h) static methods.

In most cases, you will be creating [Entities](engine/entity.h) in your scenes, updating them around and then drawing them at the end of the frame.

## Creating and finding entities

In the sample scene in this project, you can see that `Bullet`s and `Alien`s are created but not stored anywhere:

```
for (int angle = 0; angle < 360; angle += 360/currentLevel) {
  new Alien(angle, Random::rollf(250,350));
}
```

This is not a plain stupid memory leak. The [Alien](src/alien.h) class inherits from the [SelfRegister](engine/selfregister.h) class, which means that at any point in our code we can find them by using the `Alien::GetAll()` function. Eg:

```
for (Alien* a : Alien::GetAll()) {
  a->Update(dt);
}
```

The underlying vector of `Alien`s is kept up-to-date automatically as new `Alien`s are created and destroyed, and it's globally accessible so any other class can iterate over all the objects if it needs to.

In the sample scene, this is used both to update and draw all `Bullet`s and `Alien`s.

### Destroying entities

Calling `delete` mid-frame can be problematic (eg: the player is carrying a bomb and stores a pointer to it, the bomb explodes and gets deleted, now the player is carrying an ACTUAL bomb: an invalid pointer). To prevent this, entities contain an `alive` boolean. An entity that must be destroyed can set its `alive` boolean to false, so other entities can react to that before the entity is actually gone. Using the `SelfRegister` magic, we can delete all non-alive entities at once at the end of the frame, like this:

```
Bullet::DeleteNotAlive();
```

## Collisions between entities

Box-Box, Box-Circle and Circle-Circle collsisions are implemented for you. Make your entities be one of `BoxEntity` or `CircleEntity` and you can now ask for collisions between them using the `Collide` function in [collide.h](engine/collide.h). This code here computes if any `Bullet` collides with any `Alien` (again, using `SelfRegister` magic):

```
for (Alien* a : Alien::GetAll()) {
  for (Bullet* b : Bullet::GetAll()) {
    if (Collide(a,b)) {
      // Collision handling here
    }
  }
}
```

## Drawing stuff on screen

The [Window](engine/window.h) class lets you draw a texture (`GPU_Image*`) anywhere in the game world, with a single-liner:

```
Window::Draw(Assets::aNiceTexture, pos);
```

If you want to transform what you are drawing, you can concatenate any transformations like this:

```
Window::Draw(Assets::aNiceTexture, pos)
  .withRotation(180)
  .withScale(2,1);
```

Since not only the `Draw` function is global but also all the assets are, you can Draw anything from anywhere without having to carry around `Sprite` classes, `RenderTarget`s or anything of what you would do in other frameworks. Also, the resulting code is quite compact since you only pass the transformations you want when you want them.

The [Camera](engine/camera.h) class can be moved arround and zoomed in and out, and defines the viewport that will be drawn on screen in the end. As it should be, all `Camera` functions are global too.

## Input

The awesome [Input](engine/input.h) class centralizes input events from both gamepads and the keyboard, so your players can use whatever they chose or have handy. If you want to explicitly read input from a specific input source, you can import [raw_input.h](engine/raw_input.h) and use the `Keyboard`, `GamePad` or `Mouse` classes directly.

All `Input` functions are global of course, so you can use them from wherever you please.

To remap the keys or add new actions to the Input, you will need to tweak [input.h](engine/input.h) and [input.cpp](engine/input.cpp), as an exception to not having to modify things inside `engine`.

## Debugging

- `ImGUI` is included by default, because that's what you are going to use the most to debug.
- Use `Debug::out << "My debug message";` to print your shit. It's better than `std::out` because it adds a newline at the end automatically. Also, if your game ever gets serious, you can redirect `Debug::out` to a log file. Some common classes like `vec` know how to write themselves to a C++ stream, so you will see a nice `(x,y)` output if you stream them directly.
- Pressing F1 during the game will toggle frame-by-frame mode. Press E while in this mode to advance a frame.
- Press F2 anytime and you will enable graphical debug mode. In debug mode, the global `Debug::Draw` boolean is true, so you can put all your debug code (eg: drawing boundaries, invisible objects, etc) surrounded by `if (Debug::Draw)` and toggle it on and off during the game.
- It's handy to draw primitives in debug mode, you can do so using `Window::DrawPrimitive::Line(...)`, `Window::DrawPrimitive::Circle(...)`, etc.

## More features

Some useful stuff you can include from the `engine` folder:

- A [vec](engine/vec.h) class for your `x,y` coordinates. Its name is short because you will use it a lot. It contains all kinds of useful methods, like normalize, calculate the lenght, rotate around a point, etc.
- The [Bounds](engine/bounds.h) and CircleBounds classes, are also handy to store (drum roll) bounds of things. Also come packed with useful stuff like intersection functions or debug draw methods.
- A [Random](engine/random.h) namespace with functions to generate random numbers, plus a `GoodRandom` namespace with functions to generate better (but slower) random numbers.
- A [Mates](engine/mates.h) class with some trigonometric constants and common operations like clamping a value, rounding, etc.
- A [PartSys](engine/partsys.h) class to create particle systems. Use the built-in `DrawImGUI` function to show an interactive editor and tweak the parameters, then set them in code once you are satisfied with the result.
- An [Animation](engine/animation.h) class that can reproduce animations defined in code in [src/anim_lib.h](src/anim_data.h).
- A [TileMap](engine/tilemap.h) class that can draw your tile-based map in a very efficient way. In `asset_src` you have a script that will convert Tiled projects into code that can be loaded using the TileMap class.
