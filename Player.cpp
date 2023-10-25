﻿#include "Player.h"

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
//	worldTransform_.rotation_.y = 1;
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
		powerSpeed *=-1;
	}
	else if (worldTransform1_.scale_.y <= 0.1f) {
		powerSpeed *=-1;
	}
	if (moveFlag1 == true) {
		worldTransform1_.scale_.y += powerSpeed;
	}


	if (input_->PushKey(DIK_2)) {
		moveFlag2 = true;
	}
	if (input_->PushKey(DIK_1)) {
		worldTransform_.translation_ = { 0,0,-110 };
		worldTransform_.rotation_ = { 0,0,0 };
		Speed = 0;
		movev3 = { 0.0f,0.0f, Speed };
		move = { 0,0,0 };
		movev2 = { 1,0,1 };
		moveFlag1 = true;  
		moveFlag = true;
		moveFlag2 = true;
	}

	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	if (input_->TriggerKey(DIK_SPACE)){
		Speed = worldTransform1_.scale_.y ;
		move = { 0,0,0 };
		movev2 = { 1,0,1 };
		movev3 = { 0.0f,0.0f, Speed };
		movev3 = TransformNormal(movev3, worldTransform_.matWorld_);
		move = movev3;

		moveFlag1 = false;
	}

	if (moveFlag1 == false) {

		if (moveFlag == true) {
			moveFlag = false;
		}

		if (moveFlag2 == true) {
			move.x -= (move.x / 100) * 2;
			move.z -= (move.z / 100) * 2;
			if (move.x < 0 || move.z < 0) {
				//moveFlag1 = true;
			}
		}


		if (worldTransform_.translation_.x > 10) {
			movev2.x *= -1;
		}
		
		if (worldTransform_.translation_.x < -10) {
			movev2.x *= -1;
		}
		if (worldTransform_.translation_.z < -120) {
			movev2.z *= -1;
		}
		if (worldTransform_.translation_.z > -90) {
			movev2.z *= -1;
		}

		worldTransform_.translation_.x += move.x * movev2.x;
		worldTransform_.translation_.z += move.z * movev2.z;
	}

	
	

		// キャラクターの座標を画面表示する処理
		ImGui::Begin("Player pos");
		// float3入力ボックス
		ImGui::InputFloat3("worldTransform", &worldTransform_.translation_.x);
		ImGui::InputFloat3("rotation_", &worldTransform_.rotation_.x);

		// float3スライダー
		ImGui::InputFloat3("worldTransform1_.scale_.y", &worldTransform1_.scale_.x);
		ImGui::InputFloat3("powerSpeed", &powerSpeed);

		ImGui::SliderFloat3("movev3", &movev3.x, -20.0f, 20.0f);
		ImGui::SliderFloat3("move", &move.x, -20.0f, 20.0f);

		ImGui::SliderFloat3("worldTransform_.matWorld_0", &worldTransform_.matWorld_.m[0][1], -20.0f, 20.0f);
		ImGui::SliderFloat3("worldTransform_.matWorld_1", &worldTransform_.matWorld_.m[1][1], -20.0f, 20.0f);
		ImGui::SliderFloat3("worldTransform_.matWorld_2", &worldTransform_.matWorld_.m[2][1], -20.0f, 20.0f);
		ImGui::SliderFloat3("worldTransform_.matWorld_3", &worldTransform_.matWorld_.m[3][1], -20.0f, 20.0f);

		ImGui::Text("PlayerBullet : Space");
		ImGui::Text("DedugCamera : LALT");
		ImGui::End();
	
}

	void Player::Attack() {
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
	
}

void Player::NotOnCollision() {
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