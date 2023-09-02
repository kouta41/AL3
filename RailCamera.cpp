#include"RailCamera.h"


void RailCamera::Init(Vector3 pos, Vector3 radius) {
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = radius;
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 10000;
	viewProjection_.Initialize();
}


void RailCamera::Update() {
	//移動（ベクトルを加算）
	
	worldTransform_.translation_.z += velocity_.z;
	
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//カメラの座標を画面表示する
	//ImGui::Begin("Camera");
	//ImGui::SliderFloat3("pos", &worldTransform_.translation_.x, -20.0f, 20.0f);
	//ImGui::SliderFloat3("rsdius", &worldTransform_.rotation_.x,-20.0f, 20.0f);

	//ImGui::End();
}