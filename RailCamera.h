#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"MathFunction.h"
#include"ImGuiManager.h"

class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init(Vector3 pos,Vector3 radius);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection 	viewProjection_;

	//速度
	Vector3 velocity_ = { 0.0,0.0,0.1 };
};