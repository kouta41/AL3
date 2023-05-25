#include"PlayerBullet.h"
#include"Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include <cassert>
void PlayerBullet::Initiaize(Model* model, const Vector3& position) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("block,png");

	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

}

void PlayerBullet::Update() {
	//���[���h�g�����X�t�H�[���̍X�V
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}