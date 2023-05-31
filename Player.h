#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include"PlayerBullet.h"
#include"Input.h"
#include"MathFunction.h"
#include"ImGuiManager.h"
#include <cassert>
#include<list>

class Player {

	public:
		/// <summary>
	///デストラクタ 
	/// </summary>
	~Player();


	void Initialize(Model* model,uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	private:

		//キーボード入力
		Input* input_ = nullptr;
		//弾
		std::list<PlayerBullet*> bullets_;

		//ワールド変換データ
	    WorldTransform worldTransform_;
	    //モデル
	    Model* model_ = nullptr;
		//テクスチャハンドル
	    uint32_t textureHandle_ = 0u;
  
};