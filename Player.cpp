#include "Player.h"



Player::~Player() {
	//bullet_の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle, uint32_t textureHandle1) {
	assert(model);
	textureHandle_ = textureHandle;
	textureHandle1_ = textureHandle1;
	worldTransform_.scale_.x;
	model_ = model;
	worldTransform_.Initialize();
	worldTransform1_.Initialize();
	input_ = Input::GetInstance();
	worldTransform1_.translation_.x = worldTransform_.translation_.x ;
	worldTransform1_.translation_.y = worldTransform_.translation_.y + 5;
	worldTransform1_.translation_.z = worldTransform_.translation_.z - 10;
}

void Player::Update() {
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
	//行列を定数バッファに転送
	worldTransform1_.TransferMatrix();
	//ワールドトランスフォームの更新
	worldTransform1_.UpdateMatrix();

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});


	//移動ベクトル
	Vector3 move = { 0,0,0 };
	Vector3 movev2 = { 0,0,0 };
	//Vector3 movev3 = { 0,0,0 };

	int moveFlag = true;

	if (input_->PushKey(DIK_LSHIFT)) {
		moveFlag = false;
	}else{
		moveFlag = true;
	}

	if (moveFlag == false) {
		//押した方向で移動ベクトルを変更（左右）
		if (input_->PushKey(DIK_LEFT)) {
			movev2.x -= kCaracterSpeed;
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			movev2.x += kCaracterSpeed;
		}

		//押した方向で移動ベクトルを変更（上下）
		if (input_->PushKey(DIK_UP)) {
			movev2.y -= kCaracterSpeed;
		}
		else if (input_->PushKey(DIK_DOWN)) {
			movev2.y += kCaracterSpeed;
		}

		//押した方向で移動ベクトルを変更（Z軸）
		if (input_->PushKey(DIK_9)) {
			movev2.z -= kCaracterSpeed;
		}
		else if (input_->PushKey(DIK_0)) {
			movev2.z += kCaracterSpeed;
		}
	}
	
	if (moveFlag == true) {
		//押した方向の移動ベクトルを変更
		if (input_->PushKey(DIK_A)) {
			worldTransform_.rotation_.y += kRotSpeed;
		}
		else if (input_->PushKey(DIK_D)) {
			worldTransform_.rotation_.y -= kRotSpeed;
		}

		//押した方向で移動ベクトルを変更（左右）
		if (input_->PushKey(DIK_LEFT)) {
			move.x -= kCaracterSpeed;
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move.x += kCaracterSpeed;
		}

		//押した方向で移動ベクトルを変更（上下）
		if (input_->PushKey(DIK_UP)) {
			move.y -= kCaracterSpeed;
		}
		else if (input_->PushKey(DIK_DOWN)) {
			move.y += kCaracterSpeed;
		}

		//押した方向で移動ベクトルを変更（Z軸）
		if (input_->PushKey(DIK_9)) {
			move.z -= kCaracterSpeed;
		}
		else if (input_->PushKey(DIK_0)) {
			move.z += kCaracterSpeed;
		}

	}



	Attack();

	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//座標移動（ベクトルの加算）
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y -= move.y;
	worldTransform_.translation_.z -= move.z;

	// 移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	// 範囲超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//Cameraにplayerを紐づけする
	worldTransform1_.translation_.x += move.x + movev2.x + movev3.x;
	worldTransform1_.translation_.y -= move.y + movev2.y + movev3.y;
	worldTransform1_.translation_.z -= move.z - movev2.z - movev3.z;
	//Cameraにplayerを戻す
	if (input_->PushKey(DIK_SPACE)) {
		worldTransform1_.translation_.x = worldTransform_.translation_.x;
		worldTransform1_.translation_.y = worldTransform_.translation_.y + 5;
		worldTransform1_.translation_.z = worldTransform_.translation_.z - 10;
	}



	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Player pos");
	// float3入力ボックス
	ImGui::InputFloat3("InputFloat3", &worldTransform_.translation_.x);

	// float3スライダー
	ImGui::SliderFloat3("SliderFloat3", &worldTransform1_.translation_.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("SliderFloat3", &movev3.x, -20.0f, 20.0f);

	ImGui::Text("PlayerBullet : Space");
	ImGui::Text("DedugCamera : LALT");
	ImGui::End();

	
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//弾を生成し。初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//弾を登録
		bullets_.push_back(newBullet);
	}
}

void Player::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	model_->Draw(worldTransform1_, viewProjection_, textureHandle1_);


	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Player::OnCollision() {
	movev3.y += 0.01;
	movev3.z += 0.01;
}

void Player::NotOnCollision() {
	movev3 = { 0,0,0 };
}
Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

Vector3 Player::GetWorldPosition1() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform1_.matWorld_.m[3][0];
	worldPos.y = worldTransform1_.matWorld_.m[3][1];
	worldPos.z = worldTransform1_.matWorld_.m[3][2];
	return worldPos;
}