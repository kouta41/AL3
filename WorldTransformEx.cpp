#include "Vector3.h"
#include "Matrix4x4.h"
#include"WorldTransform.h"
#include "MathFunction.h"
#include <cmath>




void WorldTransform::UpdateMatrix() {
	//スケーリング行列
	Matrix4x4 scale = MakeScaleMatrix(scale_);

	//X軸周りの回転行列
	Matrix4x4 rotateX = MakeRotateXMatrix(rotation_.x);
	//Y軸周りの回転行列
	Matrix4x4 rotateY = MakeRotateYMatrix(rotation_.y);
	//Z軸方向の回転行列
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotation_.z);
	//回転行列
	Matrix4x4 rotateXYZ = Multiply(MakeRotateXMatrix(rotation_.x), MakeRotateYMatrix(rotation_.y));

	//平行移動行列
	Matrix4x4 translate = MakeTranslateMatrix(translation_);

	//アフィン変換行列
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	//親があれば親のワールド行列をかける
	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}

	//行列を定数バッファに転送
	TransferMatrix();
}