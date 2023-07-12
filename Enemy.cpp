#include"Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle ) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = { 0,3,100 };
}

void Enemy::ApproachUpdate() {
	//�ړ��i�x�N�g�������Z�j
	worldTransform_.translation_.z -= velocity_.z;
	//�K��̈ʒu�܂œ��B�����痣�E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate() {
	//�ړ��i�x�N�g�������Z�j
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.x -= velocity_.x;
}

void Enemy::Update() {
	//���[���h�g�����X�t�H�[���̍X�V
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
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}