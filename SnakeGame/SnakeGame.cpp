#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <list>

struct SnakeCor1
{
    int x;
    int y;
};

struct SnakeCor2
{
    int x2;
    int y2;
};

// Setup for the game
const int ScreenWidth = 1000;
const int ScreenHeight = 1000;
int loop_pause = 100;
const int loop_pause_power = 50;

const int snake_size = 25;
const sf::Color snake_color = sf::Color(70, 115, 232);
const sf::Color BG_color = sf::Color(170, 215, 81);
std::list<SnakeCor1> snake = { {500, 500}, {525, 500}, {550, 500}, {575, 500}, {600, 500} };
std::list<SnakeCor2> snake2 = { {300, 200}, {275, 200}, {250, 200}, {225, 200}, {200, 200} };
int snake_direction = 3;
int snake_direction2 = 1;
bool isDead = false;
int score = 0;
int score2 = 0;

const sf::Color food_color = sf::Color(231, 71, 29);
int food_X = 100;
int food_Y = 700;
int Badfood_X = 300;
int Badfood_Y = 600;

void OnePlayer() {
    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Snake Game");
    sf::Texture BG_texture;
    BG_texture.loadFromFile("Background.png");

    sf::Sprite sprite;
    sprite.setTexture(BG_texture);

    sf::SoundBuffer Buffer;
    Buffer.loadFromFile("collision.wav");
    sf::Sound ColSound;
    ColSound.setBuffer(Buffer);

    sf::SoundBuffer AppleBuffer;
    AppleBuffer.loadFromFile("AppleBite.wav");
    sf::Sound AppleSound;
    AppleSound.setBuffer(AppleBuffer);

    while (window.isOpen())
    {
        // events and delay

        sf::sleep(sf::Time(sf::milliseconds(loop_pause)));

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                //when the user wants to restart the game

                //directional values
                switch (event.key.code)
                {
                case sf::Keyboard::Up:
                    snake_direction = 0;
                    break;
                case sf::Keyboard::Left:
                    snake_direction = 3;
                    break;
                case sf::Keyboard::Down:
                    snake_direction = 2;
                    break;
                case sf::Keyboard::Right:
                    snake_direction = 1;
                    break;

                }

            }
        }

        window.clear();

        window.draw(sprite);


        // Game logic

        // snake position update

        switch (snake_direction)
        {
            // Front will give the first element in a list
            // Back will give the last element
        case 0: // Up
            snake.push_front({ snake.front().x, snake.front().y - snake_size });
            break;
        case 1: // Right
            snake.push_front({ snake.front().x + snake_size, snake.front().y });
            break;
        case 2: // Down
            snake.push_front({ snake.front().x, snake.front().y + snake_size });
            break;
        case 3: // Left
            snake.push_front({ snake.front().x - snake_size, snake.front().y });
            break;
        }

        // Destroy tail
        snake.pop_back();

        if (!isDead)
        {
            //collision walls

            if (snake.front().x < 0 || snake.front().x + snake_size > ScreenWidth)
            {
                isDead = true;
                ColSound.play();
            }
            if (snake.front().y < 0 || snake.front().y + snake_size > ScreenHeight)
            {
                isDead = true;
                ColSound.play();
            }

            //collision to itself
            // Begin return the iterator to the first element
            // End return one position after the last element
            for (std::list<SnakeCor1>::iterator i = snake.begin(); i != snake.end(); i++)
            {
                if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y)
                {
                    isDead = true;
                    ColSound.play();
                }

            }

            for (std::list<SnakeCor2>::iterator j = snake2.begin(); j != snake2.end(); j++)
            {
                if (j != snake2.begin() && j->x2 == snake2.front().x2 && j->y2 == snake2.front().y2)
                {
                    isDead = true;
                    ColSound.play();
                }

            }

            // He ate the Food
            bool newBadFoodPos = false;
            bool newFoodPos = false;
            // player 1 ate the food
            if (snake.front().x == food_X && snake.front().y == food_Y)
            {
                score++;
                AppleSound.play();

                while (!newFoodPos)
                {
                    food_X = (rand() % (ScreenWidth / 50) * 50);
                    food_Y = (rand() % (ScreenHeight / 50) * 50);

                    if (snake.front().x != food_X && snake.front().y != food_Y)
                    {
                        newFoodPos = true;
                    }

                    // To ensure that the food does not magically appear in the snake
                    for (auto s : snake)
                    {
                        if (s.x != food_X && s.y != food_Y && newFoodPos)
                        {
                            newFoodPos = true;
                        }
                        else
                        {
                            newFoodPos = false;
                        }


                    }


                }
                // add to the snake
                snake.push_back({ snake.back().x, snake.back().y });

            }
            
            // Eating and finding the bad food
            if ((score % 5 == 0) && (score != 0))
            {

                if (snake.front().x == Badfood_X && snake.front().y == Badfood_Y)
                {
                    score = score - 3;
                    AppleSound.play();

                    while (!newBadFoodPos)
                    {
                        Badfood_X = ((rand() + 1) % (ScreenWidth / 50) * 50);
                        Badfood_Y = ((rand() + 1) % (ScreenHeight / 50) * 50);

                        if (snake.front().x != Badfood_X && snake.front().y != Badfood_Y)
                        {
                            newBadFoodPos = true;
                        }

                        // To ensure that the food does not magically appear in the snake
                        for (auto s : snake)
                        {
                            if (s.x != Badfood_X && s.y != Badfood_Y && newBadFoodPos)
                            {
                                newBadFoodPos = true;
                            }
                            else
                            {
                                newBadFoodPos = false;
                            }


                        }


                    }
                    // add to the snake
                    snake.pop_back();
                    snake.pop_back();
                    snake.pop_back();
                }
            }



            // Render and display

            //draw snake
            for (auto s : snake)
            {
                sf::Texture texture;
                texture.loadFromFile("snake1Texture.png");
                sf::RectangleShape part;
                part.setPosition(s.x, s.y);
                part.setSize(sf::Vector2f{ snake_size, snake_size });
                // part.setFillColor(snake_color);
                part.setTexture(&texture);
                window.draw(part);
            }


            //draw food
            sf::Texture texture;
            texture.loadFromFile("apple2.png");
            sf::RectangleShape food_shape;
            food_shape.setPosition(food_X, food_Y);
            // food_shape.setFillColor(food_color);
            food_shape.setTexture(&texture);
            food_shape.setSize(sf::Vector2f{ snake_size, snake_size });
            window.draw(food_shape);

            //Draw bad food
            if ((score % 5 == 0) && (score != 0))
            {
                sf::Texture Badtexture;
                Badtexture.loadFromFile("BadApple.png");
                sf::RectangleShape Badfood_shape;
                Badfood_shape.setPosition(Badfood_X, Badfood_Y);
                Badfood_shape.setTexture(&Badtexture);
                Badfood_shape.setSize(sf::Vector2f{ snake_size, snake_size });
                window.draw(Badfood_shape);
            }

            //Player scores while in game
            sf::Font fontP1;
            fontP1.loadFromFile("absci___.ttf");
            sf::Text Pscore1;
            Pscore1.setFont(fontP1);
            Pscore1.setString("Player 1 score: " + std::to_string(score));
            Pscore1.setCharacterSize(20);
            Pscore1.setFillColor(sf::Color::Black);
            Pscore1.setOrigin(Pscore1.getGlobalBounds().width / 2, Pscore1.getGlobalBounds().height / 2);
            Pscore1.setPosition(80, 30);
            window.draw(Pscore1);


        }
        else
        {
            sf::Font font;
            font.loadFromFile("absci___.ttf");
            sf::Text text;
            text.setFont(font);
            text.setString("player 1 Score: " + std::to_string(score) + +"\nclose this window\nand resume the first one");
            text.setCharacterSize(80);
            text.setFillColor(sf::Color::Black);
            text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
            text.setPosition(ScreenWidth / 2, ScreenHeight / 2);
            window.draw(text);
        }





        window.display();

    }

}

int main()
{
    //making the window screen
    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Snake Game");
    
    // Background texture
    sf::Texture BG_texture;
    BG_texture.loadFromFile("Background.png");
    sf::Sprite sprite;
    sprite.setTexture(BG_texture);

    // Music
    sf::Music music;
    music.openFromFile("SnakeMusic2.ogg");
    music.setLoop(true);
    music.setVolume(15);
    music.play();

    // Sound effects
    sf::SoundBuffer Buffer;
    Buffer.loadFromFile("collision.wav");
    sf::Sound ColSound;
    ColSound.setBuffer(Buffer);

    sf::SoundBuffer AppleBuffer;
    AppleBuffer.loadFromFile("AppleBite.wav");
    sf::Sound AppleSound;
    AppleSound.setBuffer(AppleBuffer);
    

    // game loop
    while (window.isOpen())
    {
        // events and delay

        sf::sleep(sf::Time(sf::milliseconds(loop_pause)));

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                //when the user wants to restart the game
                if (isDead && event.key.code == sf::Keyboard::Num2)
                {
                    score = 0;
                    score2 = 0;
                    food_X = 100;
                    food_Y = 700;
                    snake_direction = 3;
                    snake = { {500, 500}, {525, 500}, {550, 500}, {575, 500}, {600, 500} };
                    snake_direction2 = 1;
                    snake2 = { {300, 200}, {275, 200}, {250, 200}, {225, 200}, {200, 200} };
                    isDead = false;
                }
                if (isDead && event.key.code == sf::Keyboard::Num1)
                {
                    score = 0;
                    food_X = 100;
                    food_Y = 700;
                    snake_direction = 3;
                    snake = { {500, 500}, {525, 500}, {550, 500}, {575, 500}, {600, 500} };
                    isDead = false;
                    OnePlayer();

                }

                //directional values
                switch (event.key.code)
                {
                case sf::Keyboard::Up:
                    snake_direction = 0;
                    break;
                case sf::Keyboard::Left:
                    snake_direction = 3;
                    break;
                case sf::Keyboard::Down:
                    snake_direction = 2;
                    break;
                case sf::Keyboard::Right:
                    snake_direction = 1;
                    break;

                case sf::Keyboard::W:
                    snake_direction2 = 0;
                    break;
                case sf::Keyboard::A:
                    snake_direction2 = 3;
                    break;
                case sf::Keyboard::S:
                    snake_direction2 = 2;
                    break;
                case sf::Keyboard::D:
                    snake_direction2 = 1;
                    break;
                }

            }

        }

        window.clear();

        window.draw(sprite);


        // Game logic

        // snake position update

        switch (snake_direction)
        {
            // Front will give the first element in a list
            // Back will give the last element
        case 0: // Up
            snake.push_front({ snake.front().x, snake.front().y - snake_size });
            break;
        case 1: // Right
            snake.push_front({ snake.front().x + snake_size, snake.front().y });
            break;
        case 2: // Down
            snake.push_front({ snake.front().x, snake.front().y + snake_size });
            break;
        case 3: // Left
            snake.push_front({ snake.front().x - snake_size, snake.front().y });
            break;
        }

        switch (snake_direction2)
        {
            // Front will give the first element in a list
            // Back will give the last element
        case 0: // Up
            snake2.push_front({ snake2.front().x2, snake2.front().y2 - snake_size });
            break;
        case 1: // Right
            snake2.push_front({ snake2.front().x2 + snake_size, snake2.front().y2 });
            break;
        case 2: // Down
            snake2.push_front({ snake2.front().x2, snake2.front().y2 + snake_size });
            break;
        case 3: // Left
            snake2.push_front({ snake2.front().x2 - snake_size, snake2.front().y2 });
            break;
        }

        // Destroy tail
        snake.pop_back();
        snake2.pop_back();

        if (!isDead)
        {
            //collision walls

            if (snake.front().x < 0 || snake.front().x + snake_size > ScreenWidth)
            {
                isDead = true;
                ColSound.play();
            }
            if (snake.front().y < 0 || snake.front().y + snake_size > ScreenHeight)
            {
                isDead = true;
                ColSound.play();
            }
            if (snake2.front().x2 < 0 || snake2.front().x2 + snake_size > ScreenWidth)
            {
                isDead = true;
                ColSound.play();
            }
            if (snake2.front().y2 < 0 || snake2.front().y2 + snake_size > ScreenHeight)
            {
                isDead = true;
                ColSound.play();
            }

            //collision to itself
            // Begin return the iterator to the first element
            // End return one position after the last element
            for (std::list<SnakeCor1>::iterator i = snake.begin(); i != snake.end(); i++)
            {
                if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y)
                {
                    isDead = true;
                    ColSound.play();
                }
                // collision to the other snake
                if (i != snake.begin() && i->x == snake2.front().x2 && i->y == snake2.front().y2)
                {
                    isDead = true;
                    ColSound.play();
                }


            }

            for (std::list<SnakeCor2>::iterator j = snake2.begin(); j != snake2.end(); j++)
            {
                if (j != snake2.begin() && j->x2 == snake2.front().x2 && j->y2 == snake2.front().y2)
                {
                    isDead = true;
                    ColSound.play();
                }
                //collision to the other snake
                if (j != snake2.begin() && j->x2 == snake.front().x && j->y2 == snake.front().y)
                {
                    isDead = true;
                    ColSound.play();
                }

            }





            // He ate the Food
            bool newBadFoodPos = false;
            bool newFoodPos = false;
            // player 1 ate the food
            if (snake.front().x == food_X && snake.front().y == food_Y)
            {
                score++;
                AppleSound.play();

                while (!newFoodPos)
                {
                    food_X = (rand() % (ScreenWidth / 50) * 50);
                    food_Y = (rand() % (ScreenHeight / 50) * 50);

                    if (snake.front().x != food_X && snake.front().y != food_Y)
                    {
                        newFoodPos = true;
                    }

                    // To ensure that the food does not magically appear in the snake
                    for (auto s : snake)
                    {
                        if (s.x != food_X && s.y != food_Y && newFoodPos)
                        {
                            newFoodPos = true;
                        }
                        else
                        {
                            newFoodPos = false;
                        }


                    }


                }
                // add to the snake
                snake.push_back({ snake.back().x, snake.back().y });

            }

            //player 2 ate the food
            if (snake2.front().x2 == food_X && snake2.front().y2 == food_Y)
            {
                score2++;
                AppleSound.play();

                while (!newFoodPos)
                {
                    food_X = (rand() % (ScreenWidth / 50) * 50);
                    food_Y = (rand() % (ScreenHeight / 50) * 50);

                    if (snake2.front().x2 != food_X && snake2.front().y2 != food_Y)
                    {
                        newFoodPos = true;
                    }

                    // To ensure that the food does not magically appear in the snake
                    for (auto s2 : snake2)
                    {
                        if (s2.x2 != food_X && s2.y2 != food_Y && newFoodPos)
                        {
                            newFoodPos = true;
                        }
                        else
                        {
                            newFoodPos = false;
                        }


                    }


                }
                // add to the snake
                snake2.push_back({ snake2.back().x2, snake2.back().y2 });

            }

            //Bad food finding and eating
            if ((score % 5 == 0) && (score != 0))
            {

                if (snake.front().x == Badfood_X && snake.front().y == Badfood_Y)
                {
                    score = score - 3;
                    AppleSound.play();

                    while (!newBadFoodPos)
                    {
                        Badfood_X = ((rand() + 1) % (ScreenWidth / 50) * 50);
                        Badfood_Y = ((rand() + 1) % (ScreenHeight / 50) * 50);

                        if (snake.front().x != Badfood_X && snake.front().y != Badfood_Y)
                        {
                            newBadFoodPos = true;
                        }

                        // To ensure that the food does not magically appear in the snake
                        for (auto s : snake)
                        {
                            if (s.x != Badfood_X && s.y != Badfood_Y && newBadFoodPos)
                            {
                                newBadFoodPos = true;
                            }
                            else
                            {
                                newBadFoodPos = false;
                            }


                        }


                    }
                    // add to the snake
                    snake.pop_back();
                    snake.pop_back();
                    snake.pop_back();
                }
            }

            //Bad food finding and eating
            if ((score2 % 5 == 0) && (score2 != 0))
            {

                if (snake2.front().x2 == Badfood_X && snake2.front().y2 == Badfood_Y)
                {
                    score2 = score2 - 3;
                    AppleSound.play();

                    while (!newBadFoodPos)
                    {
                        Badfood_X = ((rand() + 1) % (ScreenWidth / 50) * 50);
                        Badfood_Y = ((rand() + 1) % (ScreenHeight / 50) * 50);

                        if (snake2.front().x2 != Badfood_X && snake2.front().y2 != Badfood_Y)
                        {
                            newBadFoodPos = true;
                        }

                        // To ensure that the food does not magically appear in the snake
                        for (auto s2 : snake2)
                        {
                            if (s2.x2 != Badfood_X && s2.y2 != Badfood_Y && newBadFoodPos)
                            {
                                newBadFoodPos = true;
                            }
                            else
                            {
                                newBadFoodPos = false;
                            }


                        }


                    }
                    // add to the snake
                    snake2.pop_back();
                    snake2.pop_back();
                    snake2.pop_back();
                }
            }


            // Render and display

            //draw snake
            for (auto s : snake)
            {
                sf::Texture texture;
                texture.loadFromFile("snake1Texture.png");
                sf::RectangleShape part;
                part.setPosition(s.x, s.y);
                part.setSize(sf::Vector2f{ snake_size, snake_size });
                // part.setFillColor(snake_color);
                part.setTexture(&texture);
                window.draw(part);
            }

            for (auto s2 : snake2)
            {
                sf::Texture texture;
                texture.loadFromFile("snake2Texture.png");
                sf::RectangleShape part;
                part.setPosition(s2.x2, s2.y2);
                part.setSize(sf::Vector2f{ snake_size, snake_size });
                // part.setFillColor(snake_color);
                part.setTexture(&texture);
                window.draw(part);
            }

            //draw food
            sf::Texture texture;
            texture.loadFromFile("apple2.png");
            sf::RectangleShape food_shape;
            food_shape.setPosition(food_X, food_Y);
            // food_shape.setFillColor(food_color);
            food_shape.setTexture(&texture);
            food_shape.setSize(sf::Vector2f{ snake_size, snake_size });
            window.draw(food_shape);

            //Draw bad food
            if (((score % 5 == 0) || (score2 % 5 == 0)) && (score != 0) && (score2 != 0))
            {
                sf::Texture Badtexture;
                Badtexture.loadFromFile("BadApple.png");
                sf::RectangleShape Badfood_shape;
                Badfood_shape.setPosition(Badfood_X, Badfood_Y);
                Badfood_shape.setTexture(&Badtexture);
                Badfood_shape.setSize(sf::Vector2f{ snake_size, snake_size });
                window.draw(Badfood_shape);
            }

            //player scores while in game
            sf::Font fontP1;
            fontP1.loadFromFile("absci___.ttf");
            sf::Text Pscore1;
            sf::Text Pscore2;
            Pscore1.setFont(fontP1);
            Pscore2.setFont(fontP1);
            Pscore1.setString("Player 1 score: " + std::to_string(score));
            Pscore2.setString("Player 2 score: " + std::to_string(score2));
            Pscore1.setCharacterSize(20);
            Pscore2.setCharacterSize(20);
            Pscore1.setFillColor(sf::Color::Black);
            Pscore2.setFillColor(sf::Color::Black);
            Pscore1.setOrigin(Pscore1.getGlobalBounds().width / 2, Pscore1.getGlobalBounds().height / 2);
            Pscore2.setOrigin(Pscore2.getGlobalBounds().width / 2, Pscore2.getGlobalBounds().height / 2);
            Pscore1.setPosition(80, 30);
            Pscore2.setPosition(920, 30);
            window.draw(Pscore1);
            window.draw(Pscore2);

        }
        // When the snake dies, show the score 
        else
        {
            sf::Font font;
            font.loadFromFile("absci___.ttf");
            sf::Text text;
            text.setFont(font);
            text.setString("player 1 Score: " + std::to_string(score) + "\nPlayer 2 Score: " + std::to_string(score2) + "\npress 1 to play single player\npress 2 to play two player");
            text.setCharacterSize(80);
            text.setFillColor(sf::Color::Black);
            text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
            text.setPosition(ScreenWidth / 2, ScreenHeight / 2);
            window.draw(text);
        }





        window.display();

    }

    return 0;
}