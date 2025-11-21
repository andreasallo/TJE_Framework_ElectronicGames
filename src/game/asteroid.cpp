#include "game/asteroid.h"
#include "game/game.h"
#include "framework/world.h"

void Asteroid::update(float dt)
{
    // Els meteorits es mouen cap al jugador → direcció -Z
    model.translate(0, 0, -speed * dt);

    // Si passen del jugador → destruir
    if (model.getTranslation().z < -10.0f) {
        World::instance->destroyEntity(this);
        toDelete = true;
        return;
    }
}
