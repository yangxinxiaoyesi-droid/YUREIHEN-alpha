#include "busters.h"
#include "Camera.h"
#include "shader.h"
#include "keyboard.h"
#include "sprite3d.h"
#include "debug_ostream.h"
#include "define.h"

Busters* g_Busters = NULL;

// パトロール位置を計算する関数
XMFLOAT3 Busters::CalculatePatrolPosition(float pathPosition)
{
	// パスの周長: (14 + 20) * 2 = 68ユニット
	// 移動: Z=-10からの右辺 -> Z=10からの右辺 -> Z=10からの左辺 -> Z=-10からの左辺 -> 戻る

	float perimeter = (MAP_MAX_X - MAP_MIN_X) * 2.0f + (MAP_MAX_Z - MAP_MIN_Z) * 2.0f;
	float distance = pathPosition * perimeter;

	// 各辺の長さ
	float rightEdgeLength = MAP_MAX_Z - MAP_MIN_Z;	// 20 (Z: -10 to 10, X=7)
	float topEdgeLength = MAP_MAX_X - MAP_MIN_X;	// 14 (X: -7 to 7, Z=10)
	float leftEdgeLength = MAP_MAX_Z - MAP_MIN_Z;	// 20 (Z: 10 to -10, X=-7)
	float bottomEdgeLength = MAP_MAX_X - MAP_MIN_X;	// 14 (X: 7 to -7, Z=-10)

	XMFLOAT3 pos;
	pos.y = PATROL_HEIGHT;

	if (distance < rightEdgeLength)
	{
		// 右辺: Z=-10からZ=10へ移動
		pos.x = MAP_MAX_X;
		pos.z = MAP_MIN_Z + distance;
	}
	else if (distance < rightEdgeLength + topEdgeLength)
	{
		// 上辺: X=7からX=-7へ移動
		pos.x = MAP_MAX_X - (distance - rightEdgeLength);
		pos.z = MAP_MAX_Z;
	}
	else if (distance < rightEdgeLength + topEdgeLength + leftEdgeLength)
	{
		// 左辺: Z=10からZ=-10へ移動
		pos.x = MAP_MIN_X;
		pos.z = MAP_MAX_Z - (distance - rightEdgeLength - topEdgeLength);
	}
	else
	{
		// 下辺: X=-7からX=7へ移動
		pos.x = MAP_MIN_X + (distance - rightEdgeLength - topEdgeLength - leftEdgeLength);
		pos.z = MAP_MIN_Z;
	}

	return pos;
}

// 回転を計算する関数
XMFLOAT3 Busters::CalculatePatrolRotation(float pathPosition)
{
	float perimeter = (MAP_MAX_X - MAP_MIN_X) * 2.0f + (MAP_MAX_Z - MAP_MIN_Z) * 2.0f;
	float distance = pathPosition * perimeter;

	float rightEdgeLength = MAP_MAX_Z - MAP_MIN_Z;
	float topEdgeLength = MAP_MAX_X - MAP_MIN_X;
	float leftEdgeLength = MAP_MAX_Z - MAP_MIN_Z;

	XMFLOAT3 rot(0.0f, 0.0f, 0.0f);

	// Y軸回転で向き設定
	if (distance < rightEdgeLength)
	{
		// 右辺: +Z向き
		rot.y = 180.0f;
	}
	else if (distance < rightEdgeLength + topEdgeLength)
	{
		// 上辺: -X向き
		rot.y = 90.0f;
	}
	else if (distance < rightEdgeLength + topEdgeLength + leftEdgeLength)
	{
		// 左辺: -Z向き
		rot.y = 0.0f;
	}
	else
	{
		// 下辺: +X向き
		rot.y = 270.0f;
	}

	return rot;
}

void Busters_Initialize(void)
{
	g_Busters = new Busters(
		{ 0.0f, PATROL_HEIGHT, -10.0f },	// 初期位置: マップの右下隅
		{ 1.0f, 1.0f, 1.0f },				// スケール
		{ 0.0f, 0.0f, 0.0f },				// 回転（度）
		"asset\\model\\buster.fbx"			// モデルパス
	);
}

void Busters_Update(void)
{
	if (g_Busters)
	{
		g_Busters->Update();
	}
}

void Busters_Draw(void)
{
	if (g_Busters)
	{
		g_Busters->Draw();
	}
}

void Busters_Finalize(void)
{
	if (g_Busters)
	{
		delete g_Busters;
		g_Busters = NULL;
	}
}

// ゲッター関数
Busters* GetBusters(void)
{
	return g_Busters;
}

// Bustersをジャンプさせる関数
void BustersScare(void)
{
	if (g_Busters)
	{
		g_Busters->JumpStart();
		hal::dout << "Busters jumped!" << std::endl;
	}
	else
	{
		hal::dout << "Busters is not initialized" << std::endl;
	}
}

