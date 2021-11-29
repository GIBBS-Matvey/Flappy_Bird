#include <iostream>
#include <vector>
#include <cmath>
#include <random>

const double Pi = 3.141596;

size_t pointSize = 1;
// todo initialized

size_t lenScreen = 100;
//todo initialized

size_t hScreen = 70;
//todo initialized

size_t deltaVelocityForBird = 2;
//todo (for bird) initialized

size_t pipeVelocity = 2;
//todo (for pipe) initialized

int timeBetweenPipes = 4;
//todo (for pipe) initialized

double birdRad = 8;
//todo (for bird) initialized

int timeBetFrames = 3;
//todo (for Game) initialized

int maxPipesNumber = 10;
//todo (for Game) initialized

double pipeLen = 3;
//todo (for Pipe) initialized



class Pipe;

class Point {
private:
    double x;
    double y;
public:
    Point() = default;

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

    void setUpLeftVertex(int newX) {
        upLeftVertex.setX(newX);
    }

    void buildRectangle() {

        for (int i = 0; i < strLen; ++i) {
            for (int j = 0; j < rowLen; ++j) {
                rectangle[i + j] = upLeftVertex + std::pair<int, int>(i, j);    /// rectangle обновляется (перезапись) todo
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

    void buildCircle(const Point& newCentre) {
        centre = newCentre;
        Rectangle square(Point(centre.getX() - rad, centre.getY() + rad), 2 * rad, 2 * rad);
        std::vector<Point> newCircle;
        newCircle.reserve(size_t(Pi * rad * rad) / pointSize * pointSize);
        for (const Point& point : square.getAllPoints()) {
            if (point.getX() * point.getX() + point.getY() * point.getY() <= rad * rad) {
                newCircle.push_back(point);
            }
        }

        circle = std::move(newCircle);
    }

    const std::vector<Point>& getAllPoints() const {
        return circle;
    }

    bool isCrossedBySomething(const std::vector<Point>& points) const {
        for (const Point& recPoint : points) {
            for (const Point& cirPoint : circle) {
                if (recPoint == cirPoint) {
                    return true;
                }
            }
        }
        return false;
    }

    ~Circle() = default;

};

class Pipe {
    Rectangle pipeBody;
    int birthTime;
    size_t pipeVel;
public:
    Pipe(const Point& upLeftPoint, double h, int birthTime) : pipeVel(pipeVelocity),
    pipeBody(upLeftPoint, pipeLen, h), birthTime(birthTime) {}

    void setPosition(int tmpTime) {
        pipeBody.setUpLeftVertex(lenScreen - pipeVel * (tmpTime - birthTime));
        pipeBody.buildRectangle();
    }

    const std::vector<Point>& getAllPoints() const {
        return pipeBody.getAllPoints();
    }

    ~Pipe() = default;

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
        body.buildCircle(centrePosition);
    }

    const std::vector<Point>& getAllPoints() {
        return body.getAllPoints();
    }

    Point getCentrePosition() const {
        return centrePosition;
    }

    bool isCrossedByPipe(const Pipe& pipe) const {
        return body.isCrossedBySomething(pipe.getAllPoints());
    }

    bool isCrossedVectorPipes(const std::vector<std::pair<Pipe, Pipe>>& pipes) const {
        for (const auto& pipePair : pipes) {
            if (body.isCrossedBySomething(pipePair.first.getAllPoints()) ||
            body.isCrossedBySomething(pipePair.second.getAllPoints())) {
                return true;
            }
        }
        return false;
    }

    void setLastPressPosition() {
        lastPressPosition = centrePosition;
    }

    ~Bird() = default;
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

        while (!bird.isCrossedVectorPipes(pipes)) {
            time += ::timeBetFrames;    //todo abstract time increment

            // todo if (clicked) -> bird.setLastPressPosition(bird.getCentrePosition);

            bird.setPosition(time, lastPressTime);
            for (auto& pipePair : pipes) {
                pipePair.first.setPosition(time);
                pipePair.second.setPosition(time);
            }

            if (time % timeBetweenPipes == 0) {
                pipes.push_back(generatePipesPair());
            }
        }
    }

    void clicked() {
        lastPressTime = time;
        //todo
    }

    /// Pipe(const Point& upLeftPoint, double h, int birthTime)
    std::pair<Pipe, Pipe> generatePipesPair() const {
        double upPipeH;
        double downPipeH;

        do {
            upPipeH = std::rand() * hScreen;
            downPipeH = std::rand() * hScreen;
        } while (upPipeH + downPipeH >= hScreen - 3 * birdRad);

        Point upPipeUpLeftVertex(lenScreen, hScreen);
        Point downPipeUpLeftVertex(lenScreen, downPipeH);

        std::pair<Pipe, Pipe> pipePair({upPipeUpLeftVertex, upPipeH, time}, {downPipeUpLeftVertex, downPipeH, time});

        return pipePair;
    }

};

int main() {
    return 0;
}
