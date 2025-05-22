#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

const int TILE_SIZE = 64;
const int FIELD_WIDTH = 10;
const int FIELD_HEIGHT = 6;

class Tile {
public:
    sf::RectangleShape shape;
    bool hasCrop = false;
    int growth = 0;

    Tile(float x, float y) {
        shape.setSize(sf::Vector2f(TILE_SIZE - 2, TILE_SIZE - 2));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color(139, 69, 19)); // земля
    }

    void plantCrop() {
        if (!hasCrop) {
            hasCrop = true;
            growth = 0;
        }
    }

    void grow() {
        if (hasCrop && growth < 5) {
            growth++;
            shape.setFillColor(sf::Color(34 + growth * 40, 139, 34)); // чим більший ріст, тим зеленіше
        }
    }

    void harvest() {
        if (hasCrop && growth >= 5) {
            hasCrop = false;
            growth = 0;
            shape.setFillColor(sf::Color(139, 69, 19));
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(TILE_SIZE * FIELD_WIDTH, TILE_SIZE * FIELD_HEIGHT), "Farm Game (SFML)");
    std::vector<Tile> tiles;

    for (int y = 0; y < FIELD_HEIGHT; ++y) {
        for (int x = 0; x < FIELD_WIDTH; ++x) {
            tiles.emplace_back(x * TILE_SIZE, y * TILE_SIZE);
        }
    }

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                auto pos = sf::Mouse::getPosition(window);
                int index = (pos.y / TILE_SIZE) * FIELD_WIDTH + (pos.x / TILE_SIZE);
                if (index >= 0 && index < tiles.size()) {
                    if (event.mouseButton.button == sf::Mouse::Left)
                        tiles[index].plantCrop();
                    else if (event.mouseButton.button == sf::Mouse::Right)
                        tiles[index].harvest();
                }
            }
        }

        if (clock.getElapsedTime().asSeconds() > 2.0f) {
            for (auto& tile : tiles) tile.grow();
            clock.restart();
        }

        window.clear();
        for (auto& tile : tiles) window.draw(tile.shape);
        window.display();
    }

    return 0;
}
