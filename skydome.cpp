#include"skydome.h"

void Skydome::Init(Model* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

}

void Skydome::Update() {
	//�s���萔�o�b�t�@�ɓ]��
	worldTransform_.TransferMatrix();
	//���[���h�g�����X�t�H�[���̍X�V
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(ViewProjection viewProjection_) {
	//3D���f��
	model_->Draw(worldTransform_, viewProjection_);
}