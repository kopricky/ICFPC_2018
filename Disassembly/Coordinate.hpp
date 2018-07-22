#ifndef COORDINATE_HPP
#define COORDINATE_HPP

class Coordinate {
public:

    int x, y, z;
    Coordinate(){};
    Coordinate(int x_, int y_, int z_) : x(x_), y(y_), z(z_){};

    Coordinate operator+ (const Coordinate& another) const {
        return Coordinate(this->x+another.x, this->y+another.y, this->z+another.z);
    }

    Coordinate operator- (const Coordinate& another) const {
        return Coordinate(this->x-another.x, this->y-another.y, this->z-another.z);
    }

    Coordinate operator* (const Coordinate& another) const {
        return Coordinate(this->x*another.x, this->y*another.y, this->z*another.z);
    }

    Coordinate operator+() const {
        return Coordinate(+this->x, +this->y, +this->z);
    }

    Coordinate operator-() const {
        return Coordinate(-this->x, -this->y, -this->z);
    }

    Coordinate& operator+= (const Coordinate& another) {
        this->x += another.x, this->y += another.y, this->z += another.z;
        return *this;
    }

    Coordinate& operator-= (const Coordinate& another) {
        this->x -= another.x, this->y -= another.y, this->z -= another.z;
        return *this;
    }

    bool operator< (const Coordinate& another) const {
        return (x == another.x) ? ((y == another.y) ? (z < another.z) : (y < another.y)) : (x < another.x);
    }

    bool operator== (const Coordinate& another) const {
        return (x == another.x) && (y == another.y) && (z == another.z);
    }

    static int tritoi(int x_, int y_, int z_, int R) {
        return x_*R*R + y_*R + z_;
    }

    static int ctoi(Coordinate original, int R) {
        return original.x*R*R + original.y*R + original.z;
    }

    static Coordinate itoc(int original, int R) {
        return Coordinate(original/R/R, (original%R)/R, original%R);
    }
};

#endif
