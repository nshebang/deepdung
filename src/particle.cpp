#include"particle.hpp"

void
Particle::Behavior(bool isPlayerAttacking, Camera* camera,
	Image* map, Inventory* inventory)
{
	this->queuedForDeletion = (GetTime() - this->creationTime) > 0.5;
}

