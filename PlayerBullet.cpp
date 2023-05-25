#include"PlayerBullet.h"
#include"Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include <cassert>
void PlayerBullet::Initiaize(Model* model, const Vector3& position) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("block,png");

	worldTransform_.Initialize();

	//因数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

}

void PlayerBullet::Update() {
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}