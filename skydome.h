#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>


class Skydome {
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

public:
	/// <summary>
	/// ������
	/// </summary>
	void Init(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection);
};