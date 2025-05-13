import std.stdio;
import std.conv;
import std.random;
import std.datetime;
import core.thread;
import core.sys.posix.unistd;
import core.sys.posix.termios;
import core.stdc.stdlib;

enum Direction { UP, DOWN, LEFT, RIGHT }

struct Point {
    int x, y;
    
    bool opEquals(ref const Point other) const {
        return x == other.x && y == other.y;
    }
}

class SnakeGame {
private:
    Point[] snake;
    Direction dir;
    Point food;
    Point bonus;
    int score;
    int highScore;
    int level;
    bool bonusActive;
    SysTime bonusEndTime;
    bool gameOver;
    bool paused;
    int speed;
    int width = 20;
    int height = 15;
    int[][][] levelMaps;
    bool wallsEnabled;
    termios originalTermios;
    termios gameTermios;
    
    void initLevels() {
        levelMaps = [
            [
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
            ],
            [
                [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],
                [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
                [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
                [1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1],
                [1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1],
                [1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1],
                [1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1],
                [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
                [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
                [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
                [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
                [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
                [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
                [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
                [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
            ]
        ];
    }
    
    void setupTerminal() {
        tcgetattr(0, &originalTermios);
        gameTermios = originalTermios;
        gameTermios.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(0, TCSANOW, &gameTermios);
    }
    
    void restoreTerminal() {
        tcsetattr(0, TCSANOW, &originalTermios);
    }
    
    void placeFood() {
        while (true) {
            food.x = uniform(1, width-1);
            food.y = uniform(1, height-1);
            
            bool onSnake = false;
            foreach (segment; snake) {
                if (segment == food) {
                    onSnake = true;
                    break;
                }
            }
            
            bool onWall = false;
            if (level < levelMaps.length && 
                food.y < levelMaps[level].length && 
                food.x < levelMaps[level][food.y].length) {
                onWall = levelMaps[level][food.y][food.x] == 1;
            }
            
            if (!onSnake && !onWall) break;
        }
    }
    
    void placeBonus() {
        if (bonusActive) return;
        
        if (uniform(0, 100) < 20) {
            while (true) {
                bonus.x = uniform(1, width-1);
                bonus.y = uniform(1, height-1);
                
                bool onSnakeOrFood = false;
                foreach (segment; snake) {
                    if (segment == bonus) {
                        onSnakeOrFood = true;
                        break;
                    }
                }
                if (bonus == food) onSnakeOrFood = true;
                
                bool onWall = false;
                if (level < levelMaps.length && 
                    bonus.y < levelMaps[level].length && 
                    bonus.x < levelMaps[level][bonus.y].length) {
                    onWall = levelMaps[level][bonus.y][bonus.x] == 1;
                }
                
                if (!onSnakeOrFood && !onWall) {
                    bonusActive = true;
                    bonusEndTime = Clock.currTime() + dur!"seconds"(10);
                    break;
                }
            }
        }
    }
    
    void checkBonus() {
        if (bonusActive && Clock.currTime() > bonusEndTime) {
            bonusActive = false;
        }
    }
    
    void clearScreen() {
        write("\033[2J\033[H");
    }
    
    void drawBorder() {
        for (int i = 0; i < width; i++) {
            writef("\033[%d;%dH#", 0, i);
            writef("\033[%d;%dH#", height-1, i);
        }
        for (int i = 0; i < height; i++) {
            writef("\033[%d;0H#", i);
            writef("\033[%d;%dH#", i, width-1);
        }
    }
    
    void drawWalls() {
        if (level >= levelMaps.length) return;
        
        for (int y = 0; y < levelMaps[level].length; y++) {
            for (int x = 0; x < levelMaps[level][y].length; x++) {
                if (levelMaps[level][y][x] == 1) {
                    writef("\033[%d;%dH#", y, x);
                }
            }
        }
    }
    
    void drawSnake() {
        foreach (i, segment; snake) {
            if (i == 0) {
                char headChar;
                switch (dir) {
                    case Direction.UP: headChar = '^'; break;
                    case Direction.DOWN: headChar = 'v'; break;
                    case Direction.LEFT: headChar = '<'; break;
                    case Direction.RIGHT: headChar = '>'; break;
                    default: headChar = 'O';
                }
                writef("\033[%d;%dH%c", segment.y, segment.x, headChar);
            } else {
                writef("\033[%d;%dHo", segment.y, segment.x);
            }
        }
    }
    
    void drawFood() {
        writef("\033[%d;%dH@", food.y, food.x);
    }
    
    void drawBonus() {
        if (bonusActive) {
            writef("\033[%d;%dH*", bonus.y, bonus.x);
        }
    }
    
    void drawUI() {
        writef("\033[%d;0HScore: %d | High Score: %d | Level: %d | Speed: %d", 
              height, score, highScore, level + 1, speed);
        
        if (paused) {
            writef("\033[%d;0HPAUSED - Press SPACE to continue", height + 1);
        }
        
        if (bonusActive) {
            auto remaining = bonusEndTime - Clock.currTime();
            writef("\033[%d;0HBonus: %d seconds remaining", height + 2, remaining.total!"seconds"());
        }
    }
    
    void drawGameOver() {
        int centerY = height / 2;
        int centerX = width / 2 - 5;
        
        writef("\033[%d;%dH==========================", centerY - 1, centerX - 10);
        writef("\033[%d;%dH      GAME OVER!      ", centerY, centerX - 10);
        writef("\033[%d;%dH  Final Score: %d  ", centerY + 1, centerX - 10, score);
        writef("\033[%d;%dHPress R to restart or Q to quit", centerY + 2, centerX - 10);
        writef("\033[%d;%dH==========================", centerY + 3, centerX - 10);
    }
    
    void moveSnake() {
        for (int i = cast(int)snake.length - 1; i > 0; i--) {
            snake[i] = snake[i-1];
        }
        
        switch (dir) {
            case Direction.UP:
                snake[0].y--;
                break;
            case Direction.DOWN:
                snake[0].y++;
                break;
            case Direction.LEFT:
                snake[0].x--;
                break;
            case Direction.RIGHT:
                snake[0].x++;
                break;
            default:
                break;
        }
    }
    
    bool checkCollision() {
        if (wallsEnabled) {
            if (snake[0].x <= 0 || snake[0].x >= width-1 || 
                snake[0].y <= 0 || snake[0].y >= height-1) {
                return true;
            }
        } else {
            if (snake[0].x <= 0) snake[0].x = width-2;
            if (snake[0].x >= width-1) snake[0].x = 1;
            if (snake[0].y <= 0) snake[0].y = height-2;
            if (snake[0].y >= height-1) snake[0].y = 1;
        }
        
        if (level < levelMaps.length && 
            snake[0].y < levelMaps[level].length && 
            snake[0].x < levelMaps[level][snake[0].y].length) {
            if (levelMaps[level][snake[0].y][snake[0].x] == 1) {
                return true;
            }
        }
        
        for (int i = 1; i < snake.length; i++) {
            if (snake[0] == snake[i]) {
                return true;
            }
        }
        
        return false;
    }
    
    void checkFood() {
        if (snake[0] == food) {
            snake ~= snake[$-1];
            
            score += 10 * (level + 1);
            if (score > highScore) {
                highScore = score;
            }
            
            if (score % 50 == 0 && speed > 50) {
                speed -= 10;
            }
            
            if (score / 100 > level && level < levelMaps.length - 1) {
                level++;
            }
            
            placeFood();
            placeBonus();
        }
        
        if (bonusActive && snake[0] == bonus) {
            score += 50 * (level + 1);
            if (score > highScore) {
                highScore = score;
            }
            bonusActive = false;
            placeBonus();
        }
    }
    
    void handleInput() {
        ubyte[1] ch;
        if (read(0, ch.ptr, 1) > 0) {
            switch (ch[0]) {
                case 'w':
                    if (dir != Direction.DOWN) dir = Direction.UP;
                    break;
                case 's':
                    if (dir != Direction.UP) dir = Direction.DOWN;
                    break;
                case 'a':
                    if (dir != Direction.RIGHT) dir = Direction.LEFT;
                    break;
                case 'd':
                    if (dir != Direction.LEFT) dir = Direction.RIGHT;
                    break;
                case ' ':
                    paused = !paused;
                    break;
                case 'q':
                    if (gameOver) {
                        restoreTerminal();
                        exit(0);
                    }
                    break;
                case 'r':
                    if (gameOver) {
                        resetGame();
                    }
                    break;
                case '1':
                    wallsEnabled = !wallsEnabled;
                    break;
                default:
                    break;
            }
        }
    }
    
    void resetGame() {
        snake = [Point(width/2, height/2), Point(width/2+1, height/2), Point(width/2+2, height/2)];
        dir = Direction.LEFT;
        score = 0;
        level = 0;
        speed = 150;
        gameOver = false;
        paused = false;
        bonusActive = false;
        wallsEnabled = true;
        placeFood();
    }

public:
    this() {
        setupTerminal();
        initLevels();
        resetGame();
    }
    
    ~this() {
        restoreTerminal();
    }
    
    void run() {
        while (true) {
            clearScreen();
            
            if (!gameOver && !paused) {
                handleInput();
                moveSnake();
                
                if (checkCollision()) {
                    gameOver = true;
                }
                
                checkFood();
                checkBonus();
                placeBonus();
            } else {
                handleInput();
            }
            
            drawBorder();
            drawWalls();
            drawSnake();
            drawFood();
            drawBonus();
            drawUI();
            
            if (gameOver) {
                drawGameOver();
            }
            
            Thread.sleep(dur!"msecs"(speed));
        }
    }
}

void main() {
    auto game = new SnakeGame();
    game.run();
}
