#include"block.h"


void block::Init(Model* model, uint32_t textureHandle) {
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.Initialize();
	
}

void block::Update() {
	worldTransform_.scale_.x = 10;
	worldTransform_.scale_.y = -10;
	worldTransform_.translation_ = { 10,10,10 };
	//�s���萔�o�b�t�@�ɓ]��
	worldTransform_.TransferMatrix();
	//���[���h�g�����X�t�H�[���̍X�V
	worldTransform_.UpdateMatrix();
}

void block::Draw(ViewProjection viewProjection_) {
	worldTransform_.scale_.x += 10;

	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

}

Vector3 block::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}