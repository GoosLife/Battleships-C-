#include <SDL.h>

#include "Enemy.h"

Enemy::Enemy(const LoaderParams* pParams) : SDLGameObject(pParams)
{
	m_velocity.setY(2);
	m_velocity.setX(0.001);
}

void Enemy::draw()
{
	SDLGameObject::draw();
}

void Enemy::update()
{
	// m_velocity = Vector2D(1.0f, 1.0f); // enemy will move down and right
	m_currentFrame = int(((SDL_GetTicks() / 100) % 3)); // Animate enemy

	if (m_position.getY() < 0)
		m_velocity.setY(2);

	if (m_position.getY() > 400)
		m_velocity.setY(-2);
	
	SDLGameObject::update();
}

void Enemy::clean() {}
