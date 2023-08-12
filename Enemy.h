#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

#include <cassert>

class Enemy {
public:

	/// <summary>
	///  ������
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);
	
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// 
	/// </summary>
	void ApproachUpdate();

	void LeaveUpdate();

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
	Vector3 velocity_ = { 0.5,0.5,1 };

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//�����o�֐��|�C���^�̃e�[�u��
	static void (Enemy::* phasePFuncTable[])();

};