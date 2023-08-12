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

//static�Ő錾���������o�֐��|�C���^�e�[�u���̎���
void(Enemy::*Enemy::phasePFuncTable[])() = { &Enemy::ApproachUpdate,&Enemy::LeaveUpdate };

void Enemy::Update() {
	//���[���h�g�����X�t�H�[���̍X�V
	worldTransform_.UpdateMatrix();

	//�����o�֐��|�C���^�ɓ����Ă�֐����Ăяo��
	(this->*phasePFuncTable[static_cast<size_t>(phase_)])();

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