#include"RailCamera.h"


void RailCamera::Init(Vector3 pos, Vector3 radius) {
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = radius;
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}


void RailCamera::Update() {
	//移動（ベクトルを加算）
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.z -= velocity_.z;
	//
	worldTransform_.rotation_.x += velocity_.x;
	worldTransform_.rotation_.y += velocity_.y;
	worldTransform_.rotation_.z += velocity_.z;
	worldTransform_.UpdateMatrix();

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//カメラの座標を画面表示する
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("worldtransform", &worldTransform_.translation_.x, -18.0f, 1.0f);

}