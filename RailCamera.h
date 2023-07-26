#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"MathFunction.h"
#include"ImGuiManager.h"

class RailCamera {
public:
	/// <summary>
	/// ������
	/// </summary>
	void Init(Vector3 pos,Vector3 radius);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection 	viewProjection_;

	//���x
	Vector3 velocity_ = { 0.0,0.0,0.1 };
};