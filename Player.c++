#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Player {

	public:
	void Initialize(Model* model,uint32_t texrureHandele);

	void Update();

	void Draw(ViewProjection);

	private:
		//ワールド変換データ
	    WorldTransform worldTransfrom_;
	    //モデル
	    Model* model_ = nullptr;
		//テクスチャハンドル
	    uint32_t textureHandle_ = 0u;

};