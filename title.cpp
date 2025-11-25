#include "title.h"
#include "shader.h"
#include "main.h"
#include "sprite.h"
#include "texture.h"
#include "keyboard.h"
#include "debug_ostream.h"

ID3D11ShaderResourceView* g_Texture = nullptr;

void Title_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	//ï`âÊópê›íË
	pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	col = { 1.0f,1.0f,1.0f,1.0f };
	texcoord = { 0.0f,0.0f };

	g_Texture = LoadTexture(L"asset\\texture\\16_9.png");
}

void Title_Finalize(void)
{
}

void Title_Update(void)
{
	if (Keyboard_IsKeyDownTrigger(KK_SPACE))
	{
		//StartFade(SCENE_GAME);
	}
}

void Title_Draw(void)
{
	Sprite_Draw(pos, size, col, BLENDSTATE_NONE, g_Texture);
}
