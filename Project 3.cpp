#include "Board.h"
using namespace std;

int main()
{
    cout << "Program started." << endl;
    
    // Initialize board object, configure window size, and fill board w/ mines
    Board gameBoard = Board();
    gameBoard.Randomize();

    sf::RenderWindow window(sf::VideoMode(gameBoard.GetWidth(), gameBoard.GetHeight()), "Minesweeper");

    while (window.isOpen())
    {
        sf::Event event;
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if ((event.type == sf::Event::MouseButtonReleased) &&
                    (mousePosition.x >= 0 && mousePosition.x < gameBoard.GetWidth()) &&
                    (mousePosition.y >= 0 && mousePosition.y < gameBoard.GetHeight()))
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                    gameBoard.WhichTile(mousePosition.x, mousePosition.y, "left");
                else if (event.mouseButton.button == sf::Mouse::Right)
                    gameBoard.WhichTile(mousePosition.x, mousePosition.y, "right");
            }
        }

        // 1. Clear anything already drawn
        window.clear();

        // 2. Draw whatever
        gameBoard.DrawBoard(window);
        gameBoard.DrawButtons(window);

        // 3. Display whatever was drawn
        window.display();
    }

    cout << "Program ended." << endl;

    // Clear out any textures before program ends
    TextureManager::Clear();

    return 0;
}

