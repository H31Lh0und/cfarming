#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

struct Plant {
    std::string name;
    sf::Color color;
    int growTime;
};

struct ActivePlant {
    Plant plant;
    sf::RectangleShape shape;
    sf::Text label;
    sf::Clock timer;
    bool grown = false;
    bool active = false;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Farm Game");
    window.setFramerateLimit(120);

    sf::Font font;
    if (!font.loadFromFile("assets/OpenSans.ttf")) {
        std::cerr << "Font not found!\n";
        return 1;
    }

    std::vector<Plant> plants = {
        {"Wheat", sf::Color(194, 178, 128), 3},
        {"Carrot", sf::Color(255, 119, 34), 4},
        {"Potato", sf::Color(210, 180, 140), 5},
        {"Pumpkin", sf::Color(255, 140, 0), 7}
    };

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::vector<ActivePlant> field;
    const int tilesX = 10, tilesY = 8, tileSize = 60;
    for (int y = 0; y < tilesY; ++y) {
        for (int x = 0; x < tilesX; ++x) {
            ActivePlant ap;
            ap.shape.setSize(sf::Vector2f(tileSize - 4.f, tileSize - 4.f));
            ap.shape.setPosition(static_cast<float>(x * tileSize + 2), static_cast<float>(y * tileSize + 2));
            ap.shape.setFillColor(sf::Color(120, 160, 100));
            ap.shape.setOutlineThickness(1);
            ap.shape.setOutlineColor(sf::Color::Black);

            ap.label.setFont(font);
            ap.label.setCharacterSize(10);
            ap.label.setFillColor(sf::Color::Black);
            ap.label.setPosition(ap.shape.getPosition().x, ap.shape.getPosition().y);

            field.push_back(ap);
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                for (auto& tile : field) {
                    if (tile.shape.getGlobalBounds().contains(mousePos)) {
                        if (tile.active && tile.grown) {
                            tile.active = false;
                            tile.grown = false;
                            tile.label.setString("");
                            tile.shape.setFillColor(sf::Color(120, 160, 100));
                            tile.shape.setOutlineThickness(1);
                            tile.shape.setOutlineColor(sf::Color::Black);
                        } else if (!tile.active) {
                            int idx = std::rand() % plants.size();
                            tile.plant = plants[idx];
                            tile.shape.setFillColor(tile.plant.color);
                            tile.timer.restart();
                            tile.active = true;
                            tile.grown = false;
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(150, 200, 150));

        for (auto& tile : field) {
            if (tile.active) {
                float elapsed = tile.timer.getElapsedTime().asSeconds();
                int timeLeft = static_cast<int>(tile.plant.growTime - elapsed);
                if (timeLeft < 0) timeLeft = 0;

                std::string timeText = tile.plant.name + "\n" + std::to_string(timeLeft) + " sec";
                tile.label.setString(timeText);

                if (elapsed >= tile.plant.growTime && !tile.grown) {
                    tile.grown = true;
                    tile.shape.setOutlineThickness(3);
                    tile.shape.setOutlineColor(sf::Color::Green);
                }
            }
            window.draw(tile.shape);
            if (tile.active)
                window.draw(tile.label);
        }

        window.display();
    }

    return 0;
}
