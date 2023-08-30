#include <fstream>
#include <cassert>

#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"

GameScene::GameScene() {
	playerTh_ = NULL;
	enemyTh_ = NULL;
}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete player_;
	delete skydome_;
	delete modelSkydome_;
	delete debugCamera_;
	// bullet_の解放
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}

	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
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
	textureReticle_ = TextureManager::Load("point.png");

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
	player_->Initialize(model_,playerTh_, textureReticle_,playerPosition);
	//自キャラとレールカメラの親子関係を結ぶ
	player_->setParent(&railCamera_->GetworldTransform_());


	LoadEnemyPopData();

	

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
	
	UpdateEnemyPopCommands();
	//当たり判定
	CheckAllCollisions();

	//敵キャラの更新
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	// 弾更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	// デスフラグんお立った弾を排除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});


	
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
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	// 弾描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

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
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;

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
	}
#pragma endregion

	///自弾と敵キャラの当たり判定///
#pragma region
	


	//自弾と敵キャラの当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		for (Enemy* enemy : enemys_) {
			//敵キャラの座標
			posA = enemy->GetWorldPosition();
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
				enemy->OnCollision();
			}
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
			ImGui::Begin("OnCollision");
			ImGui::Text("posA:(%+.2f,%+.2f,%+.2f)", posA.x, posA.y, posA.z);
			ImGui::Text("posB:(%+.2f,%+.2f,%+.2f)", posB.x, posB.y, posB.z);
			ImGui::End();
		}
	}
#pragma endregion
}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルも内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {

	// 待機処理
	if (isWait) {
		waitTimer--;
		if (waitTimer <= 0) {
			// 待機完了
			isWait = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// Y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// Z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			EnemyInit(Vector3(x, y, z));
		}

		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWait = true;
			waitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::EnemyInit(Vector3 translation) {
	// 敵キャラの生成
	Enemy* enemy_ = new Enemy();
	// 敵キャラの初期化
	enemy_->Initialize(model_, enemyTh_, translation);
	// 敵キャラにゲームシーンを渡す
	enemy_->SetGameScene(this);
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	AddEnemy(enemy_);
}

void GameScene::AddEnemy(Enemy* enemy) {
	// リストに登録する
	enemys_.push_back(enemy);
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}