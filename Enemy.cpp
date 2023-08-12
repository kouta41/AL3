#include"Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle ) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
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
	//���[���h�g�����X�t�H�[���̍X�V
	worldTransform_.UpdateMatrix();

	
	ImGui::Begin("Enemy pos");
	// float3���̓{�b�N�X
	ImGui::InputFloat3("InputFloat3", &worldTransform_.translation_.x);
	// float3�X���C�_�[
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
	//�ړ�
	pEnemy->Move();
	//����̈ʒu�ɒB�����痣�E
	if (pEnemy->GetWorldTransform().z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave());
	}
}


void EnemyStateLeave::Update(Enemy* pEnemy) {
	pEnemy->SetVelocity(0.5f, 0.5f, 0.0f);
	//�ړ�
	pEnemy->Move();
}