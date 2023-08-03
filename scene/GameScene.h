#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"DebugCamera.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "Player.h"
#include"Enemy.h"
#include"skydome.h"
#include"block.h"

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


	void CheckWallCollisions();
private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t playerTh_;
	uint32_t enemyTh_;
	uint32_t cameraTh_;
	uint32_t blockTh_;
	//スプライト
	Sprite* sprite_ = nullptr;
	// 3Dモデル
	Model* model_ = nullptr;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 自キャラ
	Player* player_ = nullptr;
	//敵キャラ
	Enemy* enemy_ = nullptr;
	//block
	block* block_ =  nullptr ;
	//天球
	Skydome* skydome_ = nullptr;
	//3Dモデル
	Model* modelSkydome_ = nullptr;

	//デバックカメラ有効
	bool isDebugCameraActive_ = false;
	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 変数と定数
	const float kRotateAngle = 1.0f / 256.0f * float(M_PI);
	Vector3 kPointA{ 600, 400 };
	const Vector3 kBaseVector{ -190, -60 };
	const float kMaxScale = 2.0f;
	const float kMinScale = 0.5f;
	const float kScaleIncrement = 0.01f;
	float scale = 0.4f;
	float theta = 0.4f;
	float addScaleValue = 0.01f;

	struct Capsule {
		Vector3 start;
		Vector3 end;
		float radius;
	};

	Capsule capsule = {
	  {100.0f,  100.0f},
	  {1000.0f, 400.0f},
	  1.0f
	};
	float dot(const Vector3* lhs, const Vector3* rhs) { return lhs->x * rhs->x + lhs->y * rhs->y; }

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
