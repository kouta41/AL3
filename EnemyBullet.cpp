#include"EnemyBullet.h"


void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("suro.png");

	worldTransform_.Initialize();

	//因数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	//因数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	//Z方向に伸びた形状
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	//Y軸周り角度（0y）
	worldTransform_.rotation_.y = std::atan2(velocity_.z, velocity_.y);
	MakeRotateYMatrix(velocity_.y);
	MakeRotateYMatrix(velocity_.z);
	worldTransform_.rotation_.x = std::atan2(velocity_.z,velocity_.y );
}

void EnemyBullet::Update() {
	//座標を移動させる。
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z -= velocity_.z;

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}