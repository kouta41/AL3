#pragma once
#include <cassert>
#include <list>

#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include "Player.h"
#include "Input.h"
#include "MathFunction.h"
#include "ImGuiManager.h"


class Player;
class GameScene;

class Enemy {
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();
	/// <summary>
	///  初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, Vector3 translation);

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

	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を入れる変数
	Vector3 GetWorldPosition();

	/// <summary>
	///衝突を検知したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	void SetGameScene(GameScene* gameScene) { gamescene_ = gameScene; }

	//弾リストを取得
	const std::list<EnemyBullet*>& GetBullets()const { return bullets_; }
private:

	//自キャラ
	Player* player_ = nullptr;
	GameScene* gamescene_ = nullptr;

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
	std::list<EnemyBullet*> bullets_;

	//弾の発射間隔
	static const int kFireInterval = 60;

	//発射タイマー
	int32_t FireTimer = 10;
	//接近フェーズ初期化
	void Approach();

	Vector3 redius_ = { 1.0f,1.0f,1.0f };

};