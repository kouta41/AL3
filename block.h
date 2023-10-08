#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include"EnemyBullet.h"
#include"Player.h"
#include"Input.h"
#include"MathFunction.h"
#include"ImGuiManager.h"
#include <cassert>
#include<list>

class block {
public:
	void Init(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection);

	Vector3 GetWorldPosition();
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};