#include "Player.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "ImGuiManager.h"
#include <cmath>
#include <cassert>



void Player::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	}

void Player::Update() {
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	//移動ベクトル
	Vector3 move = { 0,0,0 };

	//キャラクターの移動の速さ
	const float kCaracterSpeed = 0.2f;

	//回転速さ
	const float kRotSpeed = 0.02f;

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

	Attack();

	//弾更新
	if (bullet_) {
		bullet_->Update();
	}

	//座標移動（ベクトルの加算）
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y -= move.y;

	

	// 移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	// 範囲超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Player pos");
	// float3入力ボックス
	ImGui::InputFloat3("InputFloat3", &worldTransform_.translation_.x);
	// float3スライダー
	ImGui::SliderFloat3("SliderFloat3", &worldTransform_.translation_.x, -18.0f, 1.0f);
	ImGui::Text("PlayerBullet : Space");
	ImGui::End();
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾を生成し。初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initiaize(model_, worldTransform_.translation_);

		//弾を登録する
		bullet_ = newBullet;
	}
}

void Player::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//弾描画
	if (bullet_) {
		bullet_->Draw(viewProjection_);
	}
}

