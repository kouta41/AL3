#pragma once
#include"Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

class PlayerBullet {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};