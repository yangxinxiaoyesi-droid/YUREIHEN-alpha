#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

void Animation_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Animation_Update(void);
void Animation_DrawAll(void);
void Animation_Finalize(void);

// 各アニメーションシーン向けのライフサイクル関数
void Animation_Logo_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Animation_Logo_Update(void);
void Animation_Logo_Draw(void);
void Animation_Logo_Finalize(void);

void Animation_Op_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Animation_Op_Update(void);
void Animation_Op_Draw(void);
void Animation_Op_Finalize(void);

void Animation_Win_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Animation_Win_Update(void);
void Animation_Win_Draw(void);
void Animation_Win_Finalize(void);

void Animation_Lose_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Animation_Lose_Update(void);
void Animation_Lose_Draw(void);
void Animation_Lose_Finalize(void);