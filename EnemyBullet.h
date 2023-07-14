#pragma once
#include"Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include <cassert>

class EnemyBullet {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	///衝突を検知したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	bool IsDead() const { return isDead_; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;

	// デスフラグ
	bool isDead_ = false;

	Vector3 redius_ = { 1.0f,1.0f,1.0f };

};