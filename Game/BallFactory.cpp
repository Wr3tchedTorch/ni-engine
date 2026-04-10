#include "BallFactory.h"

#include <math_functions.h>
#include <box2d.h>

#include <random>
#include <cstdint>

#include <SFML/Graphics/Color.hpp>
#include <NiEngine/GameMode.h>
#include <NiEngine/PhysicsComponent.h>

#include "Ball.h"
#include <NiEngine/BodyPhysicsComponent.h>

BallFactory::BallFactory() : rng_(std::random_device{}())
{
}

void BallFactory::SpawnRandomizedBall(ni::GameMode& mode, b2Vec2 starting_position, b2Vec2 starting_velocity)
{
    std::uniform_real_distribution<float> dist(100, 255);

    uint8_t red   = dist(rng_);
    uint8_t green = dist(rng_);
    uint8_t blue  = dist(rng_);

    Ball ball(mode.CreateGameObject(), starting_position, mode.GetComponentStore(), mode.GetPhysicsEngine().GetWorldId(), sf::Color(red, green, blue), 0.5f);

    auto component = dynamic_cast<ni::BodyPhysicsComponent*>(mode.GetComponentStore().GetPhysicsComponent(ball.GetId()));

    if (component)
    {
        b2Body_SetLinearVelocity(component->GetBodyId(), starting_velocity);
    }
}
