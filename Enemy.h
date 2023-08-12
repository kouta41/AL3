#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

#include <cassert>

class Enemy {
public:

	/// <summary>
	///  初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 
	/// </summary>
	void ApproachUpdate();

	void LeaveUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	enum class Phase {
		Approach,//接近する
		Leave,//
	};

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_ = { 0.5,0.5,1 };

	//フェーズ
	Phase phase_ = Phase::Approach;

	//メンバ関数ポインタのテーブル
	static void (Enemy::* phasePFuncTable[])();

};