﻿#include"PlayerBullet.h"


void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("block.png");

	worldTransform_.Initialize();

	//因数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	//因数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

}

void PlayerBullet::Update() {
	//座標を移動させる。
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void PlayerBullet::OnCollision() {

}