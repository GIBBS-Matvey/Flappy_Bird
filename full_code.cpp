#include <iostream>
#include <vector>
#include <cmath>

const double Pi = 3.141596;

size_t pointSize;
// todo initialized

size_t lenScreen;
//todo initialized

size_t hScreen;
//todo initialized

size_t deltaVelocityForBird;
//todo (for bird) initialized

size_t pipeVelocity;
//todo (for pipe) initialized

int timeBetweenPipes;
//todo (for pipe) initialized

double birdRad;
//todo (for bird) initialized

int timeBetFrames;
//todo (for Game) initialized

int maxPipesNumber;
//todo (for Game) initialized



class Point {
private:
    double x;
    double y;
public:
    Point(double x, double y) : x(x), y(y) {}

    double getX() const {
        return x;
    }

    double getY() const {
        return y;
    }

    void setY(double newY) {
        y = newY;
    }

    void setX(double newX) {
        x = newX;
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    Point operator+(const std::pair<int, int>& pair) const {
        return {x + pair.first * pointSize, y - pair.second * pointSize};
    }

    ~Point() = default;
};



class Rectangle {
private:
    Point upLeftVertex;
    double l;
    double h;
    const size_t strLen;
    const size_t rowLen;
    std::vector<Point> rectangle;

public:
    Rectangle(const Point& upLeftVertex, double l, double h) : upLeftVertex(upLeftVertex), l(l), h(h),
    strLen(size_t(l) / pointSize), rowLen(size_t(h) / pointSize) {

        rectangle.resize(strLen * rowLen);
        buildRectangle();
    }

    void buildRectangle() {
        for (int i = 0; i < strLen; ++i) {
            for (int j = 0; j < rowLen; ++j) {
                rectangle[i + j] = upLeftVertex + std::pair<int, int>(i, j);
            }
        }
    }

    const std::vector<Point>& getAllPoints() const {
        return rectangle;
    }

    ~Rectangle() = default;
};


class Circle {
private:
    Point centre;
    double rad;
    std::vector<Point> circle;

public:
    explicit Circle(const Point& centre, double rad) : centre(centre), rad(rad) {
        circle.reserve(size_t(Pi * rad * rad) / pointSize * pointSize);
        Rectangle square(Point(centre.getX() - rad, centre.getY() + rad), 2 * rad, 2 * rad);
        for (const Point& point : square.getAllPoints()) {
            if (point.getX() * point.getX() + point.getY() * point.getY() <= rad * rad) {
                circle.push_back(point);
            }
        }
    }

    void buildCircle() {

    }

    const std::vector<Point>& getAllPoints() const {
        return circle;
    }

    bool isCrossedBy(const Rectangle& rectangle) const {
        for (const Point& recPoint : rectangle.getAllPoints()) {
            for (const Point& cirPoint : getAllPoints()) {
                if (recPoint == cirPoint) {
                    return true;
                }
            }
        }
        return false;
    }

    ~Circle() = default;

};


class Bird {
private:
    Circle body;
    Point centrePosition;
    Point lastPressPosition;
    double deltaV;
public:
    Bird() : body(Point(lenScreen / 2, hScreen / 2), birdRad),
    centrePosition(lenScreen / 2, hScreen / 2), deltaV(deltaVelocityForBird), lastPressPosition(centrePosition) {}

    void setPosition(int tmpTime, int lastPressTime) {
        /// y_tmp += deltaV * (tmpTime - las..) - 10 * (tmpTime - las..) ^ 2 / 2;
        centrePosition.setY(lastPressPosition.getY() + deltaV * (tmpTime - lastPressTime) - 4.9 * pow(tmpTime - lastPressTime, 2));
        body.buildCircle();
    }

    void setLastPressPosition() {
        lastPressPosition = centrePosition;
    }

    ~Bird() = default;
};

class Pipe {
    Rectangle pipeBody;
    int birthTime;
    size_t vel;
public:
    Pipe(const Point& upLeftPoint, double l, double h, int birthTime) : vel(pipeVelocity),
    pipeBody(upLeftPoint, l, h), birthTime(birthTime) {}

    void setX(int tmpTime) {
        for (auto& point : pipeBody.getAllPoints()) {
            /*point.setX();*/
            // todo (depending from getX())
        }
    }

    ~Pipe() = default;

};

class Game {
    int time;
    int lastPressTime;
    const int timeBetweenPipes;

public:
    Game() : time(0), lastPressTime(0), timeBetweenPipes(::timeBetweenPipes) {}

    void game() {
        Bird bird;
        std::vector<std::pair<Pipe, Pipe>> pipes;
        pipes.reserve(maxPipesNumber);

        while (!bird.isCrossedPipe(pipes)) {
            time += ::timeBetFrames;
            bird.setPosition(time, lastPressTime);

        }
        //todo
    }

    void clicked() {
        lastPressTime = time;
        //todo
    }

    void generate() {
        //todo
    }




};
// todo
