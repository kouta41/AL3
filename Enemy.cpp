#include"Enemy.h"

Enemy::~Enemy() {
	//Enemy�̉��
	for (EnemyBullet* EnemyBullet : Enemybullets_) {
		delete EnemyBullet;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle ) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = { 25,3,100 };

	RapidFire = 0;
	Fire();
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
	Fire();
	//�e�̍X�V
	for (EnemyBullet* bullet : Enemybullets_) {
		bullet->Update();
	}
}

void Enemy::Fire() {
	RapidFire++;
	if (RapidFire > 60) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		EnemyBullet* newEnemyBullet = new EnemyBullet();
		newEnemyBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//�e��o�^
		Enemybullets_.push_back(newEnemyBullet);
		RapidFire = 0;
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// �e�`��
	for (EnemyBullet* bullet : Enemybullets_) {
		bullet->Draw(viewProjection);
	}
}