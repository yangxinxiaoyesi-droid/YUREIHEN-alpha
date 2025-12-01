#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "model.h"
#include "sprite3d.h"
#include "component.h"

using namespace DirectX;

// Furniture定数
#define FURNITURE_NUM (3)
#define FURNITURE_DETECTION_RANGE (5.0f) // Ghost検出範囲

// Furniture クラス - 色を変更でき、ジャンプ機能を持つ
class Furniture : public Sprite3D, public Jump
{
public:
	Furniture(const XMFLOAT3& pos, const XMFLOAT3& scale, const XMFLOAT3& rot, const char* pass)
		: Sprite3D(pos, scale, rot, pass),
		  Jump(0.01f, 0.2f, 1.0f)  // gravity, jumpPower, groundLevel
	{
	}

	~Furniture() = default;

	// 更新メソッド（ジャンプ処理を含む）
	void Update(void)
	{
		// ジャンプ状態の更新（基クラスJumpのJumpUpdateを呼ぶ）
		// Furniture は Sprite3D でもあり Jump でもあるため、
		// Sprite3D（Transform3D）への参照として渡す
		JumpUpdate(*(Transform3D*)this);
	}
};

void Furniture_Initialize(void);
void Furniture_Update(void);
void Furniture_Draw(void);
void Furniture_Finalize(void);

// ゲッター関数
Furniture* GetFurniture(int index);

// ジャンプ関数
void FurnitureScare(int index);
