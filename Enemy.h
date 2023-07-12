#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include"EnemyBullet.h"
#include"Input.h"
#include"MathFunction.h"
#include"ImGuiManager.h"
#include <cassert>
#include<list>

class Enemy {
public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Enemy();
	/// <summary>
	///  ������
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);
	
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �t�F�[�Y
	/// </summary>
	void ApproachUpdate();
	void LeaveUpdate();

	/// <summary>
	/// �U��
	/// </summary>
	void Fire();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	enum class Phase {
		Approach,//�ڋ߂���
		Leave,//
	};

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	Vector3 velocity_ = { 0.5,0.5,0.1 };

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//�e
	std::list<EnemyBullet*> Enemybullets_;

	//�e�̘A�ˑ��x
	float RapidFire;
};