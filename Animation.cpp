#include "animation.h"
#include "debug_ostream.h"

// グローバル変数
static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Logo Animation (ロゴアニメーション)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void Animation_Logo_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (!pDevice || !pContext) {
		hal::dout << "Animation_Logo_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
		return;
	}

	g_pDevice = pDevice;
	g_pContext = pContext;
}

void Animation_Logo_Update(void)
{
}

void Animation_Logo_Draw(void)
{
}

void Animation_Logo_Finalize(void)
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Op Animation (Openingアニメーション)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void Animation_Op_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (!pDevice || !pContext) {
		hal::dout << "Animation_Op_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
		return;
	}

	g_pDevice = pDevice;
	g_pContext = pContext;
}

void Animation_Op_Update(void)
{
}

void Animation_Op_Draw(void)
{
}

void Animation_Op_Finalize(void)
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Win Animation (勝ちアニメーション)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void Animation_Win_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (!pDevice || !pContext) {
		hal::dout << "Animation_Win_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
		return;
	}

	g_pDevice = pDevice;
	g_pContext = pContext;
}

void Animation_Win_Update(void)
{
}

void Animation_Win_Draw(void)
{
}

void Animation_Win_Finalize(void)
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Lose Animation (負けアニメーション)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void Animation_Lose_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (!pDevice || !pContext) {
		hal::dout << "Animation_Lose_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
		return;
	}
			
	g_pDevice = pDevice;
	g_pContext = pContext;
}

void Animation_Lose_Update(void)
{
}

void Animation_Lose_Draw(void)
{
}

void Animation_Lose_Finalize(void)
{
}
