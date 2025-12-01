#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "model.h"
#include "sprite3d.h"
#include "component.h"

using namespace DirectX;

// パトロール関連の定数
#define BUSTERS_PATROL_SPEED (0.001f)    // パトロール速度
#define MAP_MIN_X (-7.0f)
#define MAP_MAX_X (7.0f)
#define MAP_MIN_Z (-10.0f)
#define MAP_MAX_Z (10.0f)
#define PATROL_HEIGHT (1.0f)

// Bustersクラス - パトロール機能とジャンプ機能を持つ
class Busters : public Sprite3D, public Jump
{
private:
	float m_CurrentPathPosition;  // パトロール経路上の位置（0.0～1.0）
	float m_PatrolSpeed;          // パトロール速度

public:
	Busters(const XMFLOAT3& pos, const XMFLOAT3& scale, const XMFLOAT3& rot, const char* pass)
		: Sprite3D(pos, scale, rot, pass),
		  Jump(0.01f, 0.2f, PATROL_HEIGHT),  // gravity, jumpPower, groundLevel
		  m_CurrentPathPosition(0.0f),
		  m_PatrolSpeed(BUSTERS_PATROL_SPEED)
	{
	}

	~Busters() = default;

	// パトロール位置を計算する関数
	XMFLOAT3 CalculatePatrolPosition(float pathPosition);

	// パトロール時の回転を計算する関数
	XMFLOAT3 CalculatePatrolRotation(float pathPosition);

	// 更新メソッド（パトロールとジャンプ処理を含む）
	void Update(void)
	{
		//ジャンプ中なら色を変える
		if (m_IsJumping)
		{
			this->SetColor(0.0f, 0.0f, 1.0f, 1.0f);//青
		}

		// ジャンプ状態の更新（まず最初に実行してY座標を計算）
		JumpUpdate(*(Transform3D*)this);

		// パトロール位置を進行
		m_CurrentPathPosition += m_PatrolSpeed;

		// ループのためにリセット
		if (m_CurrentPathPosition >= 1.0f)
		{
			m_CurrentPathPosition -= 1.0f;
		}

		// 新しい位置を計算
		XMFLOAT3 newPos = CalculatePatrolPosition(m_CurrentPathPosition);
		
		// ジャンプ中は現在のY座標を保持、そうでなければパトロール高さを使用
		if (!GetIsJumping())
		{
			newPos.y = PATROL_HEIGHT;
		}
		else
		{
			// ジャンプ中は JumpUpdate() で設定された Y座標を保持
			newPos.y = GetPos().y;
		}
		
		SetPos(newPos);

		// 新しい回転を計算
		XMFLOAT3 newRot = CalculatePatrolRotation(m_CurrentPathPosition);
		SetRot(newRot);
	}

	// パトロール速度の設定
	void SetPatrolSpeed(float speed)
	{
		m_PatrolSpeed = speed;
	}

	// パトロール速度の取得
	float GetPatrolSpeed(void) const
	{
		return m_PatrolSpeed;
	}

	// 現在のパトロール位置を取得
	float GetCurrentPathPosition(void) const
	{
		return m_CurrentPathPosition;
	}
};

void Busters_Initialize(void);
void Busters_Update(void);
void Busters_Draw(void);
void Busters_Finalize(void);

// ゲッター関数
Busters* GetBusters(void);

// Bustersをジャンプさせる関数
void BustersScare(void);