////GAMEPLAY MECHANICS////

**Player & Camera Movement**

	Author:Andrea
	Files: player.h/cpp world.h/cpp

    Implemented the full movement system of the player with boundary clamping.	Created the updateCamera function to implement a 3rd person camera that follows the player using interpolation.

**Map & Procedural Generation System**

	Author:Andrea
	Files:asteroid.h/cpp, asteroidControl.h/cpp, ring.h/cpp, ringControl.h/cpp, 	chunkGenerator.h/cpp

    Developed the procedural generation system to spawn the level segments. Created different difficulty patterns, so it gets more difficult while you are playing (Soft Rain->Wall->Rain->Sinusoide->End planet animation).Implemented the logic for spawning, moving, and cleaning up asteorids and rings.

**Combat System**
	Author:Andrea
	Files:projectile.h/cpp

	Shooting mechanic, including projectile movement and collision logic against 	asteroids. Implemented the destruction logic that removes both the 	projectile and the target upon impact.

**Collision Detection**
	Author:Andrea

    Integrated the collision system (sphere vs sphere) for all the dynamic 	entities (Player vs. Asteroids/Rings, Projectiles vs. Asteroids).

////RENDER & VISUAL EFFECTS////

**-Particle System**

	Author:Andrea
	Files:world.h/.cpp

    Integrated the ParticleEmitter system to create visual feedback:
		->Explosions: Burst effect when asteroids are destroyed.
		->Engine Trail: Continues fire effect behind the player.
		->Damage Smoke: Smoke Trail when asteroids are destroyed.


**-Post-processing Shaders**

	Author:Andrea
	Files:stage.cpp, world.cpp

    Implemented:
	->Vignette effect: Darkening effect/tunel effect that appears during turbo 	mode.
	->Chromatic Aberration: Distortion effect when the player takes damage.

////UI////


////GAME LOGIC & WORLD////
**World Management**
    Authors: Carles \& Andrea

    Game loop