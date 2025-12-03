#include <iostream>
#include <cmath>
#include <algorithm> 
#include <windows.h> 

struct Point {
    double x;
    double y;
};

class Triangle {
protected:
    double a, b, c;

    bool isTriangleValid() const {
        return (a + b > c) && (a + c > b) && (b + c > a);
    }

public:

    Triangle() : a(0), b(0), c(0) {}
    virtual ~Triangle() = default;

    virtual void calculateSides() = 0;
    virtual void printInfo() const = 0;

    double calculatePerimeter() const {
        if (!isTriangleValid()) return 0;
        return a + b + c;
    }

    double calculateArea() const {
        if (!isTriangleValid()) return 0;
        double s = calculatePerimeter() / 2.0;
        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }

    double calculateMedian(char side) const {
        if (!isTriangleValid()) return 0;
        if (side == 'a' || side == 'A') {
            return 0.5 * std::sqrt(2 * b * b + 2 * c * c - a * a);
        }
        else if (side == 'b' || side == 'B') {
            return 0.5 * std::sqrt(2 * a * a + 2 * c * c - b * b);
        }
        else if (side == 'c' || side == 'C') {
            return 0.5 * std::sqrt(2 * a * a + 2 * b * b - c * c);
        }
        return 0.0;
    }

    double calculateMidline(char side) const {
        if (!isTriangleValid()) return 0;
        if (side == 'a' || side == 'A') return a / 2.0;
        if (side == 'b' || side == 'B') return b / 2.0;
        if (side == 'c' || side == 'C') return c / 2.0;
        return 0.0;
    }
};

class TriangleByVertices : public Triangle {
private:
    Point v1, v2, v3;

    double distance(Point p1, Point p2) const {
        return std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
    }

public:
    TriangleByVertices(Point p1, Point p2, Point p3) : v1(p1), v2(p2), v3(p3) {
        calculateSides();
    }

    void calculateSides() override {
        a = distance(v2, v3);
        b = distance(v1, v3);
        c = distance(v1, v2);

        if (!isTriangleValid()) {
            std::cout << "Помилка: Задані вершини не утворюють трикутник." << std::endl;
        }
    }

    void printInfo() const override {
        std::cout << "  Тип: Трикутник за вершинами" << std::endl;
        std::cout << "  Вершини: V1(" << v1.x << ", " << v1.y << "), V2(" << v2.x << ", " << v2.y << "), V3(" << v3.x << ", " << v3.y << ")" << std::endl;
        std::cout << "  Сторони: a=" << a << ", b=" << b << ", c=" << c << std::endl;
    }
};

class TriangleBySides : public Triangle {
public:
    TriangleBySides(double sideA, double sideB, double sideC) {
        a = sideA;
        b = sideB;
        c = sideC;
        calculateSides();
    }

    void calculateSides() override {
        if (!isTriangleValid()) {
            std::cout << "Помилка: Задані довжини сторін не утворюють трикутник." << std::endl;
        }
    }

    void printInfo() const override {
        std::cout << "  Тип: Трикутник за сторонами" << std::endl;
        std::cout << "  Сторони: a=" << a << ", b=" << b << ", c=" << c << std::endl;
    }
};

class TriangleByMedians : public Triangle {
private:
    double ma, mb, mc;

    bool isMediansValid() const {
        return (ma + mb > mc) && (ma + mc > mb) && (mb + mc > ma);
    }

public:
    TriangleByMedians(double medianA, double medianB, double medianC)
        : ma(medianA), mb(medianB), mc(medianC) {
        calculateSides();
    }

    void calculateSides() override {
        if (!isMediansValid()) {
            std::cout << "Помилка: Задані медіани не утворюють трикутник." << std::endl;
            a = b = c = 0;
            return;
        }

        a = (2.0 / 3.0) * std::sqrt(2 * mb * mb + 2 * mc * mc - ma * ma);
        b = (2.0 / 3.0) * std::sqrt(2 * ma * ma + 2 * mc * mc - mb * mb);
        c = (2.0 / 3.0) * std::sqrt(2 * ma * ma + 2 * mb * mb - mc * mc);

        if (!isTriangleValid()) {
            std::cout << "Помилка: Розраховані сторони не утворюють трикутник (похибка?)." << std::endl;
        }
    }

    void printInfo() const override {
        std::cout << "  Тип: Трикутник за медіанами" << std::endl;
        std::cout << "  Медіани: ma=" << ma << ", mb=" << mb << ", mc=" << mc << std::endl;
        std::cout << "  Сторони (розрахункові): a=" << a << ", b=" << b << ", c=" << c << std::endl;
    }
};

class RightTriangle : public TriangleBySides {
private:
    double hypotenuse;
    double cathetus1, cathetus2;


    bool isRight() {

        double sides[3] = { a, b, c };
        std::sort(sides, sides + 3);

        bool result = std::abs(sides[0] * sides[0] + sides[1] * sides[1] - sides[2] * sides[2]) < 1e-9;

        if (result) {
            cathetus1 = sides[0];
            cathetus2 = sides[1];
            hypotenuse = sides[2];
        }
        return result;
    }

public:
    RightTriangle(double sideA, double sideB, double sideC)
        : TriangleBySides(sideA, sideB, sideC) {
        if (!isRight()) {
            std::cout << "Помилка: Заданий трикутник не є прямокутним!" << std::endl;
        }
    }

    double calculateInscribedRadius() const {
        if (!const_cast<RightTriangle*>(this)->isRight()) return 0;
        return (cathetus1 + cathetus2 - hypotenuse) / 2.0;
    }

    double calculateCircumscribedRadius() const {
        if (!const_cast<RightTriangle*>(this)->isRight()) return 0;
        return hypotenuse / 2.0;
    }

    void printInfo() const override {
        if (!const_cast<RightTriangle*>(this)->isRight()) {
            TriangleBySides::printInfo();
            std::cout << "  (Не є прямокутним, R і r не розраховані)." << std::endl;
        }
        else {
            std::cout << "  Тип: ПРЯМОКУТНИЙ ТРИКУТНИК" << std::endl;
            std::cout << "  Сторони: Катети " << cathetus1 << ", " << cathetus2 << ", Гіпотенуза " << hypotenuse << std::endl;
        }
    }
};

int main() {
    SetConsoleOutputCP(1251);

    Triangle* triangles[4];

    triangles[0] = new TriangleByVertices({ 0, 0 }, { 4, 0 }, { 2, 3 });

    triangles[1] = new TriangleBySides(5.0, 6.0, 7.0);

    triangles[2] = new TriangleByMedians(7.5, 6.0, 4.5);

    triangles[3] = new RightTriangle(3.0, 4.0, 5.0);

    for (int i = 0; i < 4; ++i) {
        std::cout << "\n--- ТРИКУТНИК #" << i + 1 << " ---" << std::endl;
        triangles[i]->printInfo();

        if (triangles[i]->calculatePerimeter() > 0) {
            std::cout << "  Медіана ma: " << triangles[i]->calculateMedian('a') << std::endl;
            std::cout << "  Середня лінія || b: " << triangles[i]->calculateMidline('b') << std::endl;
            std::cout << "  Периметр: " << triangles[i]->calculatePerimeter() << std::endl;
            std::cout << "  Площа: " << triangles[i]->calculateArea() << std::endl;
        }

        if (RightTriangle* rt = dynamic_cast<RightTriangle*>(triangles[i])) {
            std::cout << "  Радіус вписаного кола (r): " << rt->calculateInscribedRadius() << std::endl;
            std::cout << "  Радіус описаного кола (R): " << rt->calculateCircumscribedRadius() << std::endl;
        }
    }

    for (int i = 0; i < 4; ++i) {
        delete triangles[i];
    }

    return 0;
}