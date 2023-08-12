#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ImGuiManager.h"
#include "input/Input.h"

#include <cassert>

class Enemy;

class BaseEnemyState {
public:
	virtual void Update(Enemy* pEnemy) = 0;
};

class EnemyStateApproach :public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);
};

class EnemyStateLeave :public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);
};



class Enemy {
public:

	/// <summary>
	///  初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void Move();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);



	void ChangeState(BaseEnemyState* newState);

	void SetVelocity(float x, float y, float z);
	/// <summary>
	/// 
	/// </summary>
	//void ApproachUpdate();
	//void LeaveUpdate();

	Vector3 GetWorldTransform() { return worldTransform_.translation_; }
	Vector3 GetVelocity() { return velocity_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_ = { 0.5,0.5,1 };


	//ステート
	BaseEnemyState* state;

};