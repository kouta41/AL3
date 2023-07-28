#include"skydome.h"

void Skydome::Init(Model* model) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f,0.0f,0.0f, };
	worldTransform_.scale_ = { 100,100,100 };

}

void Skydome::Update() {
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(ViewProjection viewProjection_) {
	//3Dモデル
	model_->Draw(worldTransform_, viewProjection_);
}