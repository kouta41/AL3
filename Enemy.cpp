#include"Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle ) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	//因数で受け取った初期座標をセット
	worldTransform_.translation_ = { 0,3,100 };

	state = new EnemyStateApproach();
}

void Enemy::Move() {
	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z -= velocity_.z;

}


void Enemy::Update() {

	state->Update(this);
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	
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


void Enemy::SetVelocity(float x, float y, float z) {
	velocity_.x = x;
	velocity_.y = y;
	velocity_.z = z;
}

void Enemy::ChangeState(BaseEnemyState* newState) { state = newState; }


void EnemyStateApproach::Update(Enemy* pEnemy) {
	pEnemy->SetVelocity(0, 0, 1);
	//移動
	pEnemy->Move();
	//既定の位置に達したら離脱
	if (pEnemy->GetWorldTransform().z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave());
	}
}


void EnemyStateLeave::Update(Enemy* pEnemy) {
	pEnemy->SetVelocity(0.5f, 0.5f, 0.0f);
	//移動
	pEnemy->Move();
}