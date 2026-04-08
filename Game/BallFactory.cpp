#include "BallFactory.h"

#include <random>

#include <SFML/Graphics/Color.hpp>
#include <math_functions.h>
#include <box2d.h>
#include <GameMode.h>
#include <PhysicsComponent.h>

#include "Ball.h"

BallFactory::BallFactory() : rng_(std::random_device{}())
{
}

void BallFactory::SpawnRandomizedBall(ni::GameMode& mode, b2Vec2 starting_position, b2Vec2 starting_velocity)
{
    std::uniform_real_distribution<float> dist(100, 255);

    float red   = dist(rng_);
    float green = dist(rng_);
    float blue  = dist(rng_);

    Ball ball(mode.CreateGameObject(), starting_position, mode.GetComponentStore(), mode.GetPhysicsEngine().GetWorldId(), sf::Color(red, green, blue), 0.5df);

    ni::PhysicsComponent* component = mode.GetComponentStore().GetPhysicsComponent(ball.GetId());

    if (component)
    {
        b2Body_SetLinearVelocity(component->GetBodyId(), starting_velocity);
    }
}
