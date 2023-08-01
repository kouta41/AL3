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


	void Initialize(Model* model,uint32_t textureHandle, uint32_t textureHandle1);

	void Update();

	void Draw(ViewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();
	//ワールド座標を取得
	Vector3 GetWorldPosition();

	Vector3 GetWorldPosition1();

	/// <summary>
	///衝突を検知したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	void NotOnCollision();

	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets()const { return bullets_; }

	private:

		//キーボード入力
		Input* input_ = nullptr;
		//弾
		std::list<PlayerBullet*> bullets_;

		//ワールド変換データ
	    WorldTransform worldTransform_;

		WorldTransform worldTransform1_;
	    //モデル
	    Model* model_ = nullptr;
		//テクスチャハンドル
	    uint32_t textureHandle_ = 0u;
		uint32_t textureHandle1_ = 0u;

  
		Vector3 redius_ = { 1.0f,1.0f,1.0f };

		//移動ベクトル
		Vector3 move = { 0,0,0 };
		Vector3 movev2 = { 0,0,0 };
		Vector3 movev3 = { 0,0,0 };


		//キャラクターの移動の速さ
		const float kCaracterSpeed = 0.2f;

		//回転速さ
		const float kRotSpeed = 0.02f;
};