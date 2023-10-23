#include "Player.h"

float Player::easeInQuart(float x) {
	return x * x * x * x ;
}

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
	worldTransform_.translation_ = { 0,0,-110 };
	worldTransform1_.translation_.x = worldTransform_.translation_.x +2;
	worldTransform1_.translation_.y = worldTransform_.translation_.y + 2;
	worldTransform1_.translation_.z = worldTransform_.translation_.z -2;
	worldTransform1_.scale_ = { 0.5f,1.0f,0.5f };
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


	worldTransform1_.translation_.x = worldTransform_.translation_.x + 2;
	worldTransform1_.translation_.y = worldTransform_.translation_.y + 2;
	worldTransform1_.translation_.z = worldTransform_.translation_.z - 2;

	if (worldTransform1_.scale_.y >= 3.0f) {
		powerSpeed = powerSpeed * (-1);
	}
	else if (worldTransform1_.scale_.y <= 0.1f) {
		powerSpeed = powerSpeed * (-1);
	}

	if (moveFlag1 == true) {
		worldTransform1_.scale_.y += powerSpeed;
	}
	if (input_->PushKey(DIK_SPACE)) {
		Speed = worldTransform1_.scale_.y - (worldTransform1_.scale_.y / 20);
		//Speed = 0.5;
		moveFlag1 = false;
	}

	move = { 0,0,0 };
	Vector3 movev2 = { 0,0,0 };
	Vector3 movev3 = { 0, 0, Speed };

	movev3 = Normalize(movev3);
	movev3.x *= Speed;
	movev3.y *= Speed;
	movev3.z *= Speed;
	movev3 = TransformNormal(movev3, worldTransform_.matWorld_);




	


	

	if (input_->PushKey(DIK_1)) {
		worldTransform_.translation_ = { 0,0,-110 };
		worldTransform_.rotation_ = { 0,0,0 };

	}
	

	if (input_->PushKey(DIK_LSHIFT)) {
		moveFlag = false;
	}else{
		moveFlag = true;
	}
	
	if (moveFlag == true) {
		//押した方向の移動ベクトルを変更
		if (input_->PushKey(DIK_A)) {
			worldTransform_.rotation_.y -= kRotSpeed;
		}
		else if (input_->PushKey(DIK_D)) {
			worldTransform_.rotation_.y += kRotSpeed;

		}
	}
	if (moveFlag1 == false) {
		Speed -= 0.025;
		move = Transform_Move(movev3, move);
		if (Speed <= 0.00) {
			Speed = 0.0f;
			moveFlag1 = true;
		}
	}

	if (worldTransform_.translation_.z >= 0) {
		//worldTransform_.rotation_.y = 1.575*2;
		worldTransform_.rotation_.y *= -1;
		//worldTransform_.rotation_.z *= -1;
	}
	
		
		

		//座標移動（ベクトルの加算）
		worldTransform_.translation_ = Transform_Move(worldTransform_.translation_, move);
		

		
		


		// キャラクターの座標を画面表示する処理
		ImGui::Begin("Player pos");
		// float3入力ボックス
		ImGui::InputFloat3("InputFloat3", &worldTransform_.translation_.x);
		ImGui::InputFloat3("rotation_", &worldTransform_.rotation_.x);

		// float3スライダー
		ImGui::InputFloat3("worldTransform1_.scale_.y", &worldTransform1_.scale_.x);
		ImGui::InputFloat3("powerSpeed", &powerSpeed);

		ImGui::SliderFloat3("SliderFloat3", &movev3.x, -20.0f, 20.0f);

		ImGui::Text("PlayerBullet : Space");
		ImGui::Text("DedugCamera : LALT");
		ImGui::End();
		if (worldTransform_.translation_.x > 10) {
			movev3.x *= -1;
			movev3.z *= -1;
		}
	
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
		//	bullets_.push_back(newBullet);
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
	movev3.x *= -1;
	movev3.z *= -1;
	//worldTransform_.rotation_.y *= -1;
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