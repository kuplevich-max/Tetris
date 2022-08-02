#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

const int M = 20;
const int N = 10;

int field[M][N] = { 0 };
int w = 36;

struct Point
{
    int x, y;
}a[4], b[4];

int figures[7][4] = {
    1,3,5,7,
    2,4,5,7,
    3,5,4,6,
    3,5,4,7,
    2,3,5,7,
    3,5,7,6,
    2,3,4,5,
};

bool check() {
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
            return 0;
        else if (field[a[i].y][a[i].x])
            return 0;

    return 1;
}

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(N * w, M * w), "Tetris!");

    int score = 0;
    float g_time = 0;
    Texture t, texture_background;
    t.loadFromFile("tiles.png");
    texture_background.loadFromFile("background.png");
    Sprite tiles(t), sprite_background(texture_background);

    int dx = 0, colorNum = 1;
    bool rotate = false;
    float timer = 0, delay = 0.3;
    Clock clock;
    bool ad = true;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::Up)
                    rotate = true;
                else if (event.key.code == Keyboard::Right)
                    dx = 1;
                else if (event.key.code == Keyboard::Left)
                    dx = -1;
        }


        if (Keyboard::isKeyPressed(Keyboard::Down))
            delay = 0.05;

        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
            a[i].x += dx;
        }

        if (!check()) {
            for (int i = 0; i < 4; i++)
                a[i] = b[i];
        }

        if (rotate) {
            Point p = a[1];
            for (int i = 0; i < 4; i++) {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;

                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }

            if (!check()) {
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
            }
        }

        if (timer > delay) {
            for (int i = 0; i < 4; i++) {
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!check()) {
                for (int i = 0; i < 4; i++)
                    field[b[i].y][b[i].x] = colorNum;
                colorNum = 1 + rand() % 7;
                int n = rand() % 7;
                for (int i = 0; i < 4; i++) {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }

                if (!check()) // если новая фигура не проходит проверку, заканчиваем игру
                    break;
            }
            g_time += timer; // 
            score += 1; //
            timer = 0;
        }

        if (ad) {
            int n = rand() % 7;
            if (a[0].x == 0)
                for (int i = 0; i < 4; i++) {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            ad = false;
        }

        int k = M - 1;
        for (int i = M - 1; i > 0; i--) {
            int count = 0;
            for (int j = 0; j < N; j++) {
                if (field[i][j])
                    count++;
                field[k][j] = field[i][j];
            }
            if (count < N)
                k--;
        }

        dx = 0;
        rotate = false;
        delay = 0.3;

        window.clear(Color::White);

        window.draw(sprite_background); // отрисовываем фон
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++) {
                if (field[i][j] == 0)
                    continue;
                tiles.setTextureRect(IntRect(field[i][j] * w, 0, w, w));
                tiles.setPosition(j * w, i * w);
                window.draw(tiles);
            }

        for (int i = 0; i < 4; i++) {
            tiles.setTextureRect(IntRect(colorNum * w, 0, w, w));
            tiles.setPosition(a[i].x * w, a[i].y * w);
            window.draw(tiles);
        }

        window.display();
    }

    // Создаём окно с сообщение о конце игры
    RenderWindow message(VideoMode(400, 300), "Game Over", sf::Style::Close);
    sf::Font font;
    font.loadFromFile("CyrilicOld.ttf");   
  

    int j = 0;
    while (message.isOpen())
    {
        message.clear(Color::Blue);
        Text text("Cчёт: " + std::to_string(score), font, 20U);
        text.setPosition(message.getSize().x / 2 - 70, message.getSize().y / 2 - 50);
        message.draw(text);
        text.setString("Время: " + std::to_string(g_time) + " с");
        text.setPosition(message.getSize().x / 2 - 70, message.getSize().y / 2);
        message.draw(text);

        Event event;
        while (message.pollEvent(event))
        {
            if (event.type == Event::Closed)
                message.close();
        }  

        
        message.display();
    }
    
    

    return 0;
}
