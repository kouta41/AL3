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
	
	//ゲームパットの状態を得る変数

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

	
	XINPUT_STATE joyState;

	//ゲームパッド状態を取得する
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCaracterSpeed;
		move.y -= (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCaracterSpeed;

	}

	//スプライトの現在座標を取得
	Vector2 spritePosition = sprite2DReicle_->GetPosition();

	// joyState;

	

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
		worldTransform3DReticle_.translation_.x = GetWorldPosition().x + offset.x;
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

	
	//ジョイスティック状態を取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		//スプライトの座標変更を反映
		sprite2DReicle_->SetPosition(Vector2(spritePosition.x, spritePosition.y));
	}


	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	{
		POINT mousePosition;
		//マウス座標（スクリーン座標）を取得する
		GetCursorPos(&mousePosition);

		//クライアントエリア座標に変換する
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);

		//マウス座標を2Dレティクルのスプライトに代入する
		//sprite2DReicle_->SetPosition(Vector2(float(mousePosition.x),float( mousePosition.y)));

		 //ビューポート行列
		Matrix4x4 mousematViewport =
			MakeViewportMatrix(0, 0, WinApp::kWindowWidth ,WinApp::kWindowHeight, 0, 1);
		 //ビュー行列とプロジェクション行列、ビューポート行列を合成する
		Matrix4x4 matVPV = Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, mousematViewport));
		//合成行列の逆行列を計算
		Matrix4x4 matInverseVPV = Inverse(matVPV);

		Matrix4x4 test = Multiply(matInverseVPV, matVPV);

		//スクリーン座標
		posNear = Vector3(float(sprite2DReicle_->GetPosition().x),float(sprite2DReicle_->GetPosition().y), 0.0f);
		posFor = Vector3(float(sprite2DReicle_->GetPosition().x), float(sprite2DReicle_->GetPosition().y), 1.000001f);


		//スクリーン座標系空ワールド座標系へ
		posNear = Transform(posNear, matInverseVPV);
		posFor = Transform(posFor, matInverseVPV);

		//マウスレイの方向
		Vector3 mouseDirection;
		mouseDirection.x = posFor.x - posNear.x;
		mouseDirection.y = posFor.y - posNear.y;
		mouseDirection.z = posFor.z - posNear.z;
		mouseDirection = Normalize(mouseDirection);
		//カメラの照準オブジェクトの距離
		const float kDistanceTestObject = 100;


		worldTransform3DReticle_.translation_.x = kDistanceTestObject * mouseDirection.x  +posNear.x;
		worldTransform3DReticle_.translation_.y = kDistanceTestObject * mouseDirection.y  +posNear.y;
		worldTransform3DReticle_.translation_.z = posNear.z + mouseDirection.z *kDistanceTestObject;

		
		

		worldTransform3DReticle_.UpdateMatrix();
		worldTransform3DReticle_.TransferMatrix();

		ImGui::Begin("Player");
		ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
			worldTransform3DReticle_.translation_.y,worldTransform3DReticle_.translation_.z);
		ImGui::End();
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
		velocity.x = worldTransform3DReticle_.translation_.x - GetWorldPosition().x;
		velocity.y = worldTransform3DReticle_.translation_.y - GetWorldPosition().y;
		velocity.z = worldTransform3DReticle_.translation_.z - GetWorldPosition().z;

		velocity.x = Normalize(velocity).x * kBulletSpeed;
		velocity.y = Normalize(velocity).y * kBulletSpeed;
		velocity.z = Normalize(velocity).z * kBulletSpeed;


		//弾を生成し。初期化
		PlayerBullet* newBullet = new PlayerBullet();
		
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		
		//弾を登録
		bullets_.push_back(newBullet);
	}

	XINPUT_STATE joyState;

	//ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		//Vector3 kyori;

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 自機から照準オブジェクトへのベクトル
		velocity.x = worldTransform3DReticle_.translation_.x - GetWorldPosition().x;
		velocity.y = worldTransform3DReticle_.translation_.y - GetWorldPosition().y;
		velocity.z = worldTransform3DReticle_.translation_.z - GetWorldPosition().z;

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
	//model_->Draw(worldTransform3DReticle_, viewProjection_);

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

