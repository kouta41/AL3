#pragma once
#include <Sprite.h>
#include <list>
#include <cassert>

#include "Model.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"
#include "Input.h"
#include "MathFunction.h"
#include "ImGuiManager.h"

class Player {
	public:
	/// <summary>
	///デストラクタ 
	/// </summary>
	~Player();


	void Initialize(Model* model,uint32_t textureHandle,Vector3 playerPosition);

	void Update(const ViewProjection& viewProjection);

	void Draw(ViewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();
	//ワールド座標を取得
	Vector3 GetWorldPosition();

	Vector3 GetWorldPosition3D();


	//
	Vector3 GetRadius()const { return worldTransform_.rotation_; }

	/// <summary>
	///衝突を検知したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets()const { return bullets_; }

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent"></param>
	void setParent(WorldTransform* parent);


	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	private:

		//キーボード入力
		Input* input_ = nullptr;
		//弾
		std::list<PlayerBullet*> bullets_;

		//ワールド変換データ
		WorldTransform worldTransform_;
		//3Dレティクル用ワールドトランスフォーム
		WorldTransform worldTransform3DReticle_;
		//モデル
		Model* model_ = nullptr;
		//テクスチャハンドル
		uint32_t textureHandle_ = 0u;

		Vector3 redius_ = { 1.0f,1.0f,1.0f };

		Vector3 positionReticle;
		// 2Dレティクル用スプライト
		Sprite* sprite2DReicle_ = nullptr;

};