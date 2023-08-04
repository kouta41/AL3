#include "Player.h"



Player::~Player() {
	//bullet_の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReicle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, uint32_t textureReticle, Vector3 playerPosition) {
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = playerPosition;
	input_ = Input::GetInstance();
	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	// レティクル用テクスチャ取得
	uint32_t textureReticle_ = TextureManager::Load("point2.png");

	// スプライト生成
	sprite2DReicle_ = Sprite::Create(textureReticle_, { 640,360}, {1, 1, 1, 1}, {0.5, 0.5});

	//viewProjection.Initialize();
}

void Player::Update(const ViewProjection& viewProjection) {
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();
	


	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

	//移動ベクトル
	Vector3 move = { 0,0,0 };

	//キャラクターの移動の速さ
	const float kCaracterSpeed = 0.2f;

	//回転速さ
	const float kRotSpeed = 0.02f;

	//押した方向の移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
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


	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	{
		// 自機から3Dレティクルへの距離
		const float kDistancePlayerTo3DReticle = 50.0f;
		// 自機から3Dレティクルへのオフセット(Z+向き)
		Vector3 offset = { 0, 0, 1.0f };
		// 自機のワールド行列の回転を反映
		offset = TransformNormal(offset, worldTransform_.constMap->matWorld);
		// ベクトルの長さを変える
		offset = Normalize(offset);
		offset.x = offset.x * kDistancePlayerTo3DReticle;
		offset.y = offset.y * kDistancePlayerTo3DReticle;
		offset.z = offset.z * kDistancePlayerTo3DReticle;
		// 3Dレティクルの座標を設定
		worldTransform3DReticle_.translation_.x= GetWorldPosition().x+ offset.x;
		worldTransform3DReticle_.translation_.y = GetWorldPosition().y + offset.y;
		worldTransform3DReticle_.translation_.z = GetWorldPosition().z + offset.z;

		worldTransform3DReticle_.UpdateMatrix();
	}
	
	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		positionReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
		positionReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
		positionReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];

		// ビューポート行列
		Matrix4x4 matViewport =
			MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		// ビュー行列とプロジェクション行列、ビューポート行列を合成する
		Matrix4x4 matViewProjectionViewport = Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));

		// ワールド→スクリーン座標変換
		positionReticle = Transform(positionReticle, matViewProjectionViewport);

		// スプライトのレティクルに座標設定
		sprite2DReicle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	}
	
	Attack();

	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Player pos");
	// float3入力ボックス
	ImGui::InputFloat3("InputFloat3", &worldTransform_.translation_.x);
	// float3スライダー
	ImGui::SliderFloat3("SliderFloat3", &worldTransform_.translation_.x, -100.0f, 100.0f);

	ImGui::Text("PlayerBullet : Space");
	ImGui::Text("DedugCamera : LALT");
	ImGui::End();

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		//Vector3 kyori;

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 自機から照準オブジェクトへのベクトル
		velocity.x = GetWorldPosition3D().x - GetWorldPosition().x;
		velocity.y = GetWorldPosition3D().y - GetWorldPosition().y;
		velocity.z = GetWorldPosition3D().z - GetWorldPosition().z;

		velocity.x = Normalize(velocity).x * kBulletSpeed;
		velocity.y = Normalize(velocity).y * kBulletSpeed;
		velocity.z = Normalize(velocity).z * kBulletSpeed;


		//弾を生成し。初期化
		PlayerBullet* newBullet = new PlayerBullet();
		
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		
		//弾を登録
		bullets_.push_back(newBullet);
	}
}

void Player::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	// 弾描画
	for (PlayerBullet* bullet : bullets_){
		bullet->Draw(viewProjection_);
	}
	model_->Draw(worldTransform3DReticle_, viewProjection_);

}

void Player::DrawUI() { sprite2DReicle_->Draw(); }

void Player::OnCollision() {
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

Vector3 Player::GetWorldPosition3D() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];
	return worldPos;
}


void Player::setParent(WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

