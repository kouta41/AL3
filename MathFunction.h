#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>

struct Block {
	Vector3 pos;
	float radius;
	int color;
};

struct Circle {
	Vector3 center;     // 中心
	float radius;       // 半径
	float speed;        // 移動速度
	unsigned int color; // 色
};
struct Capsule {
	Vector3 start;
	Vector3 end;
	float radius;
	unsigned int color; // 色
};
struct Line {
	Vector3 start;
	Vector3 end;
	unsigned int color; // 色
};

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Vector3 Transform_Move(const Vector3& translate, const Vector3& move);

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// X軸周りの回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸周りの回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸周りの回転行列
Matrix4x4 MakeRotateZMatrix(float radian);
// 回転行列
Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);
// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
//  アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
//ワールド座標を取得
Vector3 Normalize(const Vector3& v);

Vector3 ToScreen(const Vector3* world);

//float Dot(const Vector3* lhs, const Vector3* rhs) { return lhs->x * rhs->x + lhs->y * rhs->y; }

void DrawCircle(const Circle* circle);

void DrawLine(const Line* line);

//Vector3 Perpendicular(const Vector3* vector) { return { -vector->y, vector->x }; }

Vector3 Normalize(const Vector3* original);

void DrawCapsule(const Capsule* capsule);

Vector3 ClosestPoint(const Line* line, const Vector3* point);