#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include"PlayerBullet.h"
#include"Input.h"
#include <cassert>

class Player {

	public:
	void Initialize(Model* model,uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	//キーボード入力
	Input* input_ = nullptr;
	//弾
	PlayerBullet* bullet_ = nullptr;

	private:
		//ワールド変換データ
	    WorldTransform worldTransform_;
	    //モデル
	    Model* model_ = nullptr;
		//テクスチャハンドル
	    uint32_t textureHandle_ = 0u;
  

};