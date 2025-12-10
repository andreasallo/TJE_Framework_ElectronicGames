LAB 2 - ELECTRONIC GAMES

Name: Andrea Salló Ribas NIA:268587 email: andrea.sallo01@estudiant.upf.edu 
Name: Carles Marin Maestro NIA: 252841 email: carles.marin01@estudiant.upf.edu

Enllaç a Youtube:

https://youtu.be/cBwWQu9lzcg

--

////GAMEPLAY MECHANICS////

**Player & Camera Movement**

	Author: Andrea & Carles
	Files: player.h/cpp world.h/cpp

    Implemented the full movement system of the player with boundary clamping.	Created the updateCamera function to implement a 3rd person camera that follows the player using interpolation. As well as the turbo mechanic for accelerating.

**Map & Procedural Generation System**

	Author: Andrea
	Files: asteroid.h/cpp, asteroidControl.h/cpp, ring.h/cpp, ringControl.h/cpp, 	chunkGenerator.h/cpp

    Developed the procedural generation system to spawn the level segments. Created different difficulty patterns, so it gets more difficult while you are playing (Soft Rain->Wall->Rain->Sinusoide->End planet animation).Implemented the logic for spawning, moving, and cleaning up asteorids and rings.

USE OF AI: Used AI for the function generateChunk(), used to know how to make all the different types of patterns for the level. Used to make the general structure of ChunkGenerator. Used for the final planet animation to connect ChunkGenerator with World.

**Combat System**
	Author: Andrea
	Files: projectile.h/cpp

	Shooting mechanic, including projectile movement and collision logic against 	asteroids. Implemented the destruction logic that removes both the 	projectile and the target upon impact.

**Collision Detection**
	Author: Andrea

    Integrated the collision system (sphere vs sphere) for all the dynamic 	entities (Player vs. Asteroids/Rings, Projectiles vs. Asteroids).

////RENDER & VISUAL EFFECTS////

**Particle System**

	Author: Andrea
	Files: world.h/.cpp

    Integrated the ParticleEmitter system to create visual feedback:
		->Explosions: Burst effect when asteroids are destroyed.
		->Engine Trail: Continues fire effect behind the player.
		->Damage Smoke: Smoke Trail when asteroids are destroyed.

USE OF AI: Used to know which parameters to set to generate certain colors and effects. Used for the position of the damage smoke behind the spaceship. Mix between class code and slides and AI.


**Post-processing Shaders**

	Author: Andrea & Carles
	Files: stage.cpp, world.cpp

    Implemented:
	->Vignette effect: Darkening effect/tunel effect that appears during turbo 	mode. A red effect appearing when the Player is running low on Health.
	->Chromatic Aberration: Distortion effect when the player takes damage.

////UI & HUD////

**Title Screen Graphics & Logic**
	Author: Carles
	Files: stage.h/cpp, game.h/cpp, entityUI.h/cpp

    Used quads to render 2D textures that make up the title screen, as well as using the skybox as a background to it. Implmented buttons to start or exit the game, usable with the mouse, they show feedback by slightly changing their color when the mouse is used on them.

**Tutorial Screen Graphics & Logic**
	Author: Carles
	Files: stage.h/cpp, game.h/cpp, entityUI.h/cpp

     Used quads to render 2D textures that compose the lore text, as well as the ones in the tutorial, used the skybox as the background.

**Play Stage HUD & Logic**
	Author: Carles
	Files: stage.h/cpp, game.h/cpp, entityUI.h/cpp, player.h/cpp

    Used quads to render 2D textures that make up the Player HUD. Implemented the logic necessary for the Turbo bar and Health bar to react to changes in the player's state.

**End Screen Graphics & Logic**
	Author: Carles
	Files: stage.h/cpp, game.h/cpp, entityUI.h/cpp, player.h/cpp

    Used quads to render 2D textures that make up the EndStage result screen as well as the score that the Player has collected by he end of the game.

////MENU LOGIC & WORLD////

**World Management**
	Authors: Carles & Andrea
	Files: world.h/cpp, stage.h/cpp, game.h/cpp, chunkGenerator.h/cpp, projectile.h/cpp

    Implemented logic for matches, after finishing a game by either finishing the level, dying or exiting it, the next time you play, the world is generated again from scratch. Also loading everything else that isn't part of the UI.

**Menu Logic**
	Authors: Carles
	Files: stage.h/cpp, player.h/cpp, asteroidController.h/cpp, game

    Implemented the necessary code to move from Stage to Stage, from menu to menu and into the game and out of it. Implemented a pause menu in order to exit the game while in the Play Stage, returning to the Title Screen.

--

Before you Play: Press 'Z' on your keyboad to advance after the menu. You can also Press 'S' to skip to the tutorial screen. While in the Play stage, press the 'P' key to open the Pause Menu.

