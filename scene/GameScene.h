#pragma once
#include <sstream>

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"DebugCamera.h"

#include "Player.h"
#include "Enemy.h"
#include "skydome.h"
#include "RailCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {


public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	void EnemyInit(Vector3);


	/// <summary>
	///	敵:敵弾を追加する
	/// </summary>
	void AddEnemyBullet(EnemyBullet* enemyBullet);
	void AddEnemy(Enemy* enemy);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t playerTh_;
	uint32_t enemyTh_;
	uint32_t textureReticle_;
	//スプライト
	Sprite* sprite_ = nullptr;
	// 3Dモデル
	Model* model_ = nullptr;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 自キャラ
	Player* player_ = nullptr;

	//敵キャラ
	std::list<Enemy*> enemys_;

	// 敵弾
	std::list<EnemyBullet*> enemyBullets_;
	
	//天球
	Skydome* skydome_ = nullptr;
	//3Dモデル
	Model* modelSkydome_ = nullptr;

	//デバックカメラ有効
	bool isDebugCameraActive_ = false;
	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	// 待機タイマー
	int32_t waitTimer;
	// 待機フラグ
	bool isWait = false;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
