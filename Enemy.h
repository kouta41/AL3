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
	/// デストラクタ
	/// </summary>
	~Enemy();
	/// <summary>
	///  初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// フェーズ
	/// </summary>
	void ApproachUpdate();
	void LeaveUpdate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Fire();

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
	Vector3 velocity_ = { 0.5,0.5,0.1 };

	//フェーズ
	Phase phase_ = Phase::Approach;

	//弾
	std::list<EnemyBullet*> Enemybullets_;

	//弾の連射速度
	float RapidFire;
};