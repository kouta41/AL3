#include"Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle ) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	//因数で受け取った初期座標をセット
	worldTransform_.translation_ = { 0,3,100 };
}

void Enemy::ApproachUpdate() {
	//移動（ベクトルを加算）
	worldTransform_.translation_.z -= velocity_.z;
	//規定の位置まで到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate() {
	//移動（ベクトルを加算）
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.x -= velocity_.x;
}

//staticで宣言したメンバ関数ポインタテーブルの実態
void(Enemy::*Enemy::phasePFuncTable[])() = { &Enemy::ApproachUpdate,&Enemy::LeaveUpdate };

void Enemy::Update() {
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//メンバ関数ポインタに入ってる関数を呼び出す
	(this->*phasePFuncTable[static_cast<size_t>(phase_)])();

	ImGui::Begin("Enemy pos");
	// float3入力ボックス
	ImGui::InputFloat3("InputFloat3", &worldTransform_.translation_.x);
	// float3スライダー
	ImGui::SliderFloat3("SliderFloat3", &worldTransform_.translation_.x, -18.0f, 1.0f);
	ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}