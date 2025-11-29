//UI.cpp

#include "UI.h"
#include "sprite.h"
#include "debug_ostream.h"
#include "keyboard.h"

// グローバル変数
static Timer* g_Clock = nullptr;
static SplitSprite* g_ScareGauge = nullptr;


//----------------------------
//UI初期化
//----------------------------
void UI_Initialize(void)
{
	// 時計の作成
	g_Clock = new Timer(
		{ CLOCK_POS_X, CLOCK_POS_Y },	// 位置
		{ CLOCK_SIZE, CLOCK_SIZE },		// サイズ
		BLENDSTATE_ALFA,				// BlendState
		L"asset\\texture\\clock.png",	// テクスチャパス
		2, 1,							// 分割数X, Y
		CLOCK_MIN, CLOCK_MAX			// 最小時間、最大時間
	);

	// 恐怖ゲージの作成
	g_ScareGauge = new SplitSprite(
		{ SCREEN_WIDTH - 270.0f, 70.0f },				// 位置
		{ 500.0f, 500.0f },				// サイズ
		0.0f,							// 回転（度）
		{ 1.0f, 1.0f, 1.0f, 1.0f },		// RGBA
		BLENDSTATE_ALFA,				// BlendState
		L"asset\\texture\\gauge.png",	// テクスチャパス
		3, 1							// 分割数X, Y
	);
}

//----------------------------
//UI更新
//----------------------------
void UI_Update(void)
{
	// 時計の時間を更新
	if (g_Clock->Update())
	{
		hal::dout << "タイマーが終了しました" << std::endl;
	}

	//qキーでリセット
	if (Keyboard_IsKeyDownTrigger(KK_Q))
	{
		g_Clock->Reset();
		hal::dout << "Qキーでタイマーをリセットしました" << std::endl;
	}
}

//----------------------------
//UI描画
//----------------------------
void UI_Draw(void)
{
	g_Clock->Draw(); // 時計を描画

	//強引にゲージを半分にする
	g_ScareGauge->SetSize({ 250.0f, 500.0f });
	g_ScareGauge->SetPos({ SCREEN_WIDTH - (270.0f + 100.0f), 70.0f });
	g_ScareGauge->Draw(2); // テクスチャ番号1（紫ゲージ）を描画

	g_ScareGauge->SetSize({ 500.0f, 500.0f });
	g_ScareGauge->SetPos({ SCREEN_WIDTH - 270.0f, 70.0f });
	g_ScareGauge->Draw(0); // テクスチャ番号0（ゲージ背景）を描画

}

//----------------------------
//UI終了
//----------------------------
void UI_Finalize(void)
{
	delete g_Clock;
	delete g_ScareGauge;
}