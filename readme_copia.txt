LAB 2 - ELECTRONIC GAMES

Name: Andrea Salló Ribas NIA:268587 email: andrea.sallo01@estudiant.upf.edu 
Name: Carles Marin Maestro NIA: 252841 email: carles.marin01@estudiant.upf.edu

Enllaç a Youtube:

asdasd

--

////GAMEPLAY MECHANICS////

**Player & Camera Movement**

	Author: Andrea
	Files: player.h/cpp world.h/cpp

    Implemented the full movement system of the player with boundary clamping.	Created the updateCamera function to implement a 3rd person camera that follows the player using interpolation.

**Map & Procedural Generation System**

	Author: Andrea
	Files: asteroid.h/cpp, asteroidControl.h/cpp, ring.h/cpp, ringControl.h/cpp, 	chunkGenerator.h/cpp

    Developed the procedural generation system to spawn the level segments. Created different difficulty patterns, so it gets more difficult while you are playing (Soft Rain->Wall->Rain->Sinusoide->End planet animation).Implemented the logic for spawning, moving, and cleaning up asteorids and rings.

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

////GAME LOGIC & WORLD////

**World Management**
	Authors: Carles & Andrea
	Files: 

    asd 

**Game loop**
	Authors: Carles & Andrea
	Files: 

    asd 

**Menu Logic** (Pause menu??)
	Authors: Carles
	Files: 

    asd 

--

Before you Play: Press 'Z' on your keyboad to advance after the menu. You can also Press 'S' to skip to the tutorial screen.
