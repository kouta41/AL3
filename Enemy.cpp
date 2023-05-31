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

void Enemy::Update() {
	//�ړ�����
	worldTransform_.translation_.z -= velocity_.z;

	//���[���h�g�����X�t�H�[���̍X�V
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}