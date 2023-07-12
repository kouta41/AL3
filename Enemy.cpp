#include"Enemy.h"

Enemy::~Enemy() {
	//Enemyの解放
	for (EnemyBullet* EnemyBullet : Enemybullets_) {
		delete EnemyBullet;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle ) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	//因数で受け取った初期座標をセット
	worldTransform_.translation_ = { 25,3,100 };

	RapidFire = 0;
	Fire();
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

void Enemy::Update() {
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	switch (phase_) {
	case Phase::Approach:
	default:
		ApproachUpdate();
		break;

	case Phase::Leave:
		LeaveUpdate();
		break;
	}
	Fire();
	//弾の更新
	for (EnemyBullet* bullet : Enemybullets_) {
		bullet->Update();
	}
}

void Enemy::Fire() {
	RapidFire++;
	if (RapidFire > 60) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		assert(player_);

		player_->GetWorldPosition();
		GetWorldPosition();
		Enemy::GetWorldPosition() = player_->GetWorldPosition() - worldTransform_.translation_;

		EnemyBullet* newEnemyBullet = new EnemyBullet();
		newEnemyBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//弾を登録
		Enemybullets_.push_back(newEnemyBullet);
		RapidFire = 0;
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (EnemyBullet* bullet : Enemybullets_) {
		bullet->Draw(viewProjection);
	}
}

Vector3 Enemy::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[1][0];
	worldPos.y = worldTransform_.matWorld_.m[1][1];
	worldPos.z = worldTransform_.matWorld_.m[1][2];

	return worldPos;
}