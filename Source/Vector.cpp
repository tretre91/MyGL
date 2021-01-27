#include <MyGL/Vector.hpp>
using namespace my;

/*
*
* Vec2 Member functions definitions
*
*/

Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(int x, int y) : x(x), y(y) {}

Vec2 Vec2::operator+(const Vec2& vec) {
    return Vec2(this->x + vec.x, this->y + vec.y);
}

Vec2& Vec2::operator+=(const Vec2& vec) {
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

Vec2 Vec2::operator-(const Vec2& vec) {
    return Vec2(this->x - vec.x, this->y - vec.y);
}

Vec2& Vec2::operator-=(const Vec2& vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

Vec2 Vec2::operator*(int scalar) {
    return Vec2(this->x * scalar, this->y * scalar);
}

Vec2& Vec2::operator*=(int scalar) {
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}

/*
*
* Vec3 Member functions definitions
*
*/

Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(int x, int y, int z) : x(x), y(y), z(z) {}

Vec3 Vec3::operator+(const Vec3& vec) {
    return Vec3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
}

Vec3& Vec3::operator+=(const Vec3& vec) {
    this->x += vec.x;
    this->y += vec.y;
    this->z += vec.z;
    return *this;
}

Vec3 Vec3::operator-(const Vec3& vec) {
    return Vec3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
}

Vec3& Vec3::operator-=(const Vec3& vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    this->z -= vec.z;
    return *this;
}

Vec3 Vec3::operator*(int scalar) {
    return Vec3(this->x * scalar, this->y * scalar, this->z * scalar);
}

Vec3& Vec3::operator*=(int scalar) {
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    return *this;
}



/*
*
* Vec4 Member functions definitions
*
*/

Vec4::Vec4() : x(0), y(0), z(0), w(0) {}

Vec4::Vec4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}

Vec4 Vec4::operator+(const Vec4& vec) {
    return Vec4(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w);
}

Vec4& Vec4::operator+=(const Vec4& vec) {
    this->x += vec.x;
    this->y += vec.y;
    this->z += vec.z;
    this->w += vec.w;
    return *this;
}

Vec4 Vec4::operator-(const Vec4& vec) {
    return Vec4(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w);
}

Vec4& Vec4::operator-=(const Vec4& vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    this->z -= vec.z;
    this->w -= vec.w;
    return *this;
}

Vec4 Vec4::operator*(int scalar) {
    return Vec4(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar);
}

Vec4& Vec4::operator*=(int scalar) {
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    this->w *= scalar;
    return *this;
}