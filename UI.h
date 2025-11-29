#pragma once

#include <d3d11.h>
#include "direct3d.h"
#include "sprite.h"
#include <DirectXMath.h>
#include <windows.h>
using namespace DirectX;

#define CLOCK_POS_X (120.0f)
#define CLOCK_POS_Y (120.0f)
#define CLOCK_SIZE (300.0f)
#define CLOCK_MIN (0.0f)
#define CLOCK_MAX (60.0f)


class Timer : public SplitSprite
{
protected:
	float m_HandRot;
	float m_Time;	//経過時間
	float m_MinTime;
	float m_MaxTime;
	DWORD m_LastUpdateTime;  // 前回の更新時刻
	int m_UpdateInterval;    // 更新間隔（ミリ秒）
public:
	Timer(const XMFLOAT2& pos, const XMFLOAT2& size, BLENDSTATE bstate, const wchar_t* texturePath, int divideX, int divideY, int minT, int maxT, int updateIntervalMs = 100)
		: SplitSprite(pos, size, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, bstate, texturePath, divideX, divideY), 
		m_HandRot(0.0f), m_Time(0.0f), m_MinTime(minT), m_MaxTime(maxT), m_LastUpdateTime(timeGetTime()), m_UpdateInterval(updateIntervalMs)
	{
	}

	bool Update()
	{
		bool isEnded = false;

		// 現在時刻を取得
		DWORD currentTime = timeGetTime();
		DWORD elapsedTime = currentTime - m_LastUpdateTime;

		// 指定されたミリ秒以上経過したら時間を加算
		if (elapsedTime >= m_UpdateInterval)
		{
			m_Time += (float)elapsedTime / 1000.0f;  // ミリ秒から秒に変換
			m_LastUpdateTime = currentTime;
		}

		// 最大時間を超えないようにキャップ
		if (m_Time > m_MaxTime)
		{
			m_Time = m_MaxTime;
			isEnded = true;
		}

		// 0から1の正規化された値を計算
		float t = (m_Time - m_MinTime) / (m_MaxTime - m_MinTime);
		m_HandRot = t * 360.0f; // 360度回転

		return isEnded;
	}

	void Draw()
	{
		m_Rotation = 0.0f;
		SplitSprite::Draw(0); // 時計本体
		m_Rotation = m_HandRot;
		SplitSprite::Draw(1); // 針
	}

	// ゲッター・セッター
	float GetTime() const { return m_Time; }
	void SetTime(float time) { m_Time = time; }
	void Reset() { m_Time = m_MinTime; m_LastUpdateTime = timeGetTime(); }
};

// UI初期化
void UI_Initialize(void);

// UI終了
void UI_Finalize(void);

// UI更新
void UI_Update(void);

// UI描画
void UI_Draw(void);
