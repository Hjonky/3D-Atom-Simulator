#pragma once
#include <vector>
#include <cstdint>
#include <cmath>

// Raw screen
extern std::vector<uint32_t> framebuffer;

constexpr int WIDTH = 1600;
constexpr int HEIGHT = 900;
constexpr float FOV = 128.0f;

struct Vec2 
{
    int x, y;
    Vec2 operator+(const Vec2& v) const { return {x + v.x, y + v.y}; }
    Vec2& operator+=(const Vec2& v) { x += v.x; y += v.y; return *this; }
};

struct Vec3 
{
    float x, y, z;
    Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vec3& operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
    Vec3 operator/(float s) const { return {x / s, y / s, z / s}; }
    Vec3 Normalize() const { float m = Magnitude(); return m == 0.0f ? Vec3{0,0,0} : Vec3{x/m, y/m, z/m}; }
    float Magnitude() const { return std::sqrt(x*x + y*y + z*z); } 
    float Dot(const Vec3& v) const { return x*v.x + y*v.y + z*v.z; }
};


void SetPixel(Vec2 p, uint32_t color);
void DrawLine(Vec2 p0, Vec2 p1, uint32_t color);
void DrawCircle(Vec2 center, int radius, uint32_t color);
void DrawFilledTriangle(Vec2 p0, Vec2 p1, Vec2 p2, uint32_t color);
void DrawWireframeTriangle(Vec2 p0, Vec2 p1, Vec2 p2, uint32_t color);
Vec2 Project(Vec3 p);
Vec3 RotateY(Vec3 p, float theta);
Vec3 RotateX(Vec3 p, float theta);
Vec3 RotateZ(Vec3 p, float theta);