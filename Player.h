#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

class Player {

	public:
	void Initialize(Model* model,uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection);

	private:
		//ワールド変換データ
	    WorldTransform worldTransform_;
	    //モデル
	    Model* model_ = nullptr;
		//テクスチャハンドル
	    uint32_t textureHandle_ = 0u;

};