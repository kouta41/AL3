#include"skydome.h"

void Skydome::Init(Model* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

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