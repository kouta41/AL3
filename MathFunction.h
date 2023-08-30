#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

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

//逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

Vector3 Add(const Vector3& v1, const Vector3& v2);

//ビューポート行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);


//2.正射影行列
Matrix4x4 MakOrthographicMatrix(float left, float right, float top, float bottom, float nearClip, float farClip);

//3座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);