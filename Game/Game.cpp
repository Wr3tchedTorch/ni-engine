#include "Engine.h";

#include <SFML/Window/WindowEnums.hpp>

int main()
{
    ni::Engine engine("Shooter by Eric", sf::State::Windowed);

    engine.Run();
}