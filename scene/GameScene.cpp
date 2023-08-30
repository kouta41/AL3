#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {
	playerTh_ = NULL;
	enemyTh_ = NULL;
}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete player_;
	delete enemy_;
	delete skydome_;
	delete modelSkydome_;
	delete debugCamera_;
	
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1260, 700);

	//ファイル名を指定してテクスチャを読み込む
	playerTh_ = TextureManager::Load("FOX.png");
	//テクスチャ読み込み
	enemyTh_ = TextureManager::Load("block.png");
	sprite_ = Sprite::Create(playerTh_, {100, 50});
	// 3Dモデルの生成
	model_ = Model::Create();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//レールカメラの生成
	railCamera_ = new RailCamera();
	//レールカメラの初期化
	railCamera_->Init({0,5,-15},{0,0,0});

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	Vector3 playerPosition(0, -5, 30);
	player_->Initialize(model_,playerTh_,playerPosition);
	//自キャラとレールカメラの親子関係を結ぶ
	player_->setParent(&railCamera_->GetworldTransform_());

	//敵の生成
	enemy_ = new Enemy();
	//敵の初期化
	enemy_->Initialize(model_,enemyTh_);
	
	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	//天球の生成
	skydome_ = new Skydome();
	//天球の初期化
	skydome_->Init(modelSkydome_);

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1260, 700);

	
	

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() { 
	//レールカメラの更新
	railCamera_->Update();
	//自キャラの更新
	player_->Update();
	//敵の更新
	enemy_->Update();
	//当たり判定
	CheckAllCollisions();
	//天球の更新
	skydome_->Update();
	//デバイスを更新
	debugCamera_->Update();
	
	

#ifdef _DEBUG
	if (input_->PushKey(DIK_LALT)) {
		isDebugCameraActive_ = true;
	}
	else {
		isDebugCameraActive_ = false;

	}


#endif

	// カメラの処理
	if (isDebugCameraActive_) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
	else {

		viewProjection_.matView = railCamera_->GetViewProjection_().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection_().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}

	//当たり判定
	CheckAllCollisions();

}

void GameScene::Draw() {
	
	
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	//自キャラの描画
	player_->Draw(viewProjection_); 
	//敵キャラの描画
	enemy_->Draw(viewProjection_);
	//天球の描画
	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}


void GameScene::CheckAllCollisions() {
	//判定対象AとBの座標
	Vector3 posA, posB, posC;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	///自キャラと敵弾の当たり判定///
#pragma region
	//自キャラの座標
	posA =player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		posC.x = (posA.x - posB.x) * (posA.x - posB.x);
		posC.y = (posA.y - posB.y) * (posA.y - posB.y);
		posC.z = (posA.z - posB.z) * (posA.z - posB.z);
		
		//球と球の当たり判定
		if (posC.x + posC.y + posC.z <= (1 + 1) * (1 + 1)) {
			//自キャラがの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
		ImGui::Begin("OnCollision");
		ImGui::Text("posA:(%+.2f,%+.2f,%+.2f)", posA.x, posA.y, posA.z);
		ImGui::Text("posB:(%+.2f,%+.2f,%+.2f)", posB.x, posB.y, posB.z);
		ImGui::End();
	}
#pragma endregion

	///自弾と敵キャラの当たり判定///
#pragma region
	//敵キャラの座標
	posA = enemy_->GetWorldPosition();


	//自弾と敵キャラの当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		//自弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		posC.x = (posA.x - posB.x) * (posA.x - posB.x);
		posC.y = (posA.y - posB.y) * (posA.y - posB.y);
		posC.z = (posA.z - posB.z) * (posA.z - posB.z);

		//球と球の当たり判定
		if (posC.x + posC.y + posC.z <= (1 + 1) * (1 + 1)) {
			//自弾がの衝突時コールバックを呼び出す
			bullet->OnCollision();
			//敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
		}
	}
#pragma endregion

	///自弾と敵弾の当たり判定///
#pragma region
	//自弾と敵弾の当たり判定
	for (PlayerBullet* playerbullet : playerBullets) {
		for (EnemyBullet* enemybullet : enemyBullets) {
			//自弾の座標
			posA = playerbullet->GetWorldPosition();
			//敵弾の座標
			posB = enemybullet->GetWorldPosition();

			//座標AとBの距離を求める
			posC.x = (posA.x - posB.x) * (posA.x - posB.x);
			posC.y = (posA.y - posB.y) * (posA.y - posB.y);
			posC.z = (posA.z - posB.z) * (posA.z - posB.z);

			//球と球の当たり判定
			if (posC.x + posC.y + posC.z <= (100 + 100) * (100 + 100)) {
				//自弾がの衝突時コールバックを呼び出す
				playerbullet->OnCollision();
				//敵弾の衝突時コールバックを呼び出す
				enemybullet->OnCollision();
			}
		}
	}
#pragma endregion
}

