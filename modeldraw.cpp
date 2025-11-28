#include "modeldraw.h"
#include "Camera.h"
#include "shader.h"
#include "keyboard.h"
#include "sprite3d.h"

// グローバル変数
static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;

// モデル
Sprite3D* g_Ghost = NULL;
Sprite3D* g_Box = NULL;

void ModelDraw_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	g_Ghost = new Sprite3D(
		{ 0.0f, 2.0f, 0.0f },			//位置
		{ 1.0f, 1.0f, 1.0f },			//スケール
		{ 0.0f, 0.0f, 0.0f },			//回転（度）
		"asset\\model\\ghost.fbx"		//モデルパス
	);

	g_Box = new Sprite3D(
		{ 0.0f, 2.0f, 0.0f },			//位置
		{ 0.2f, 0.2f, 0.2f },			//スケール
		{ 0.0f, 0.0f, 0.0f },			//回転（度）
		"asset\\model\\kirby2.fbx"		//モデルパス
	);
}

void ModelDraw_Update(void)
{
	// スペースキーが押されたらy軸回転
	if (Keyboard_IsKeyDown(KK_I))
	{
		g_Ghost->AddRotY(1.0f);
		if (g_Ghost->GetRotY() >= 360.0f)
		{
			g_Ghost->SetRotY(0.0f);
		}

		//サイズをデバッグ出力
		hal::dout << "Model Size: (" << g_Box->GetDisplaySize().x << ", " << g_Box->GetDisplaySize().y << ", " << g_Box->GetDisplaySize().z << ")" << std::endl;
	}
}

void ModelDraw_Draw(void)
{
	// モデル描画
	g_Ghost->Draw();
	g_Box->Draw();
}

void ModelDraw_Finalize(void)
{
	delete g_Ghost;
	delete g_Box;
}

