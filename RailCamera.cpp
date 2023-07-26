#include"RailCamera.h"


void RailCamera::Init(Vector3 pos, Vector3 radius) {
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = radius;
	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.Initialize();
}


void RailCamera::Update() {
	//�ړ��i�x�N�g�������Z�j
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.z -= velocity_.z;
	//
	worldTransform_.rotation_.x += velocity_.x;
	worldTransform_.rotation_.y += velocity_.y;
	worldTransform_.rotation_.z += velocity_.z;
	worldTransform_.UpdateMatrix();

	//�J�����I�u�W�F�N�g�̃��[���h�s�񂩂�r���[�s����v�Z����
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//�J�����̍��W����ʕ\������
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("worldtransform", &worldTransform_.translation_.x, -18.0f, 1.0f);

}