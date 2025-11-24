#define NOMINMAX

#include "model.h"
#include <DirectXMath.h>
#include <assert.h>

using namespace DirectX;

// Assimpの行列をDirectXMath形式に変換
XMMATRIX AiMatrixToXMMatrix(const aiMatrix4x4& mat)
{
	return XMMATRIX(
		mat.a1, mat.a2, mat.a3, mat.a4,
		mat.b1, mat.b2, mat.b3, mat.b4,
		mat.c1, mat.c2, mat.c3, mat.c4,
		mat.d1, mat.d2, mat.d3, mat.d4
	);
}

// ノードを再帰的に描画する内部関数
void RenderNode(MODEL* model, aiNode* node, XMMATRIX parentTransform)
{
	// このノードのローカル変換行列を親の変換と合成
	XMMATRIX currentTransform = AiMatrixToXMMatrix(node->mTransformation) * parentTransform;

	// このノードが持つすべてのメッシュを描画
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		unsigned int meshIndex = node->mMeshes[i];
		aiMesh* mesh = model->AiScene->mMeshes[meshIndex];

		// --- ここで currentTransform を定数バッファにセットする ---
		// 例: Renderer_SetWorldMatrix(currentTransform); など
		// HLSLに渡す場合は転置が必要な場合があります
		// XMMATRIX worldT = XMMatrixTranspose(currentTransform);
		// UpdateSubresource(...);

		// テクスチャ設定
		aiString texturePath;
		aiMaterial* material = model->AiScene->mMaterials[mesh->mMaterialIndex];
		material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);

		if (texturePath.length > 0 && model->Texture.count(texturePath.data))
		{
			Direct3D_GetDeviceContext()->PSSetShaderResources(0, 1, &model->Texture[texturePath.data]);
		}

		// 頂点バッファ設定
		UINT stride = sizeof(Vertex3D);
		UINT offset = 0;
		Direct3D_GetDeviceContext()->IASetVertexBuffers(0, 1, &model->VertexBuffer[meshIndex], &stride, &offset);

		// インデックスバッファ設定
		Direct3D_GetDeviceContext()->IASetIndexBuffer(model->IndexBuffer[meshIndex], DXGI_FORMAT_R32_UINT, 0);

		// 描画
		Direct3D_GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}

	// 子ノードを再帰処理
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		RenderNode(model, node->mChildren[i], currentTransform);
	}
}

MODEL* ModelLoad(const char* FileName)
{
	MODEL* model = new MODEL;

	const std::string modelPath(FileName);

	model->AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(model->AiScene);

	model->VertexBuffer = new ID3D11Buffer * [model->AiScene->mNumMeshes];
	model->IndexBuffer = new ID3D11Buffer * [model->AiScene->mNumMeshes];

	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = model->AiScene->mMeshes[m];

		// 頂点バッファ生成
		{
			Vertex3D* vertex = new Vertex3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				// 座標変換注意: aiProcess_ConvertToLeftHandedを使う場合は素直に代入
				vertex[v].position = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].texCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				vertex[v].normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(Vertex3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			Direct3D_GetDevice()->CreateBuffer(&bd, &sd, &model->VertexBuffer[m]);

			delete[] vertex;
		}

		// インデックスバッファ生成
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];
				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Direct3D_GetDevice()->CreateBuffer(&bd, &sd, &model->IndexBuffer[m]);

			delete[] index;
		}
	}

	// テクスチャ読み込み
	for (unsigned int i = 0; i < model->AiScene->mNumTextures; i++)
	{
		aiTexture* aitexture = model->AiScene->mTextures[i];

		ID3D11ShaderResourceView* texture;
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICMemory(aitexture->pcData, aitexture->mWidth, WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Direct3D_GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);
		assert(texture);

		model->Texture[aitexture->mFilename.data] = texture;
	}

	return model;
}

void ModelRelease(MODEL* model)
{
	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		model->VertexBuffer[m]->Release();
		model->IndexBuffer[m]->Release();
	}

	delete[] model->VertexBuffer;
	delete[] model->IndexBuffer;

	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : model->Texture)
	{
		pair.second->Release();
	}

	aiReleaseImport(model->AiScene);

	delete model;
}

void ModelDraw(MODEL* model)
{
	// プリミティブトポロジ設定
	Direct3D_GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ルートノードから再帰的に描画開始（初期変換は単位行列）
	XMMATRIX identity = XMMatrixIdentity();
	RenderNode(model, model->AiScene->mRootNode, identity);
}

// オプション: モデル全体のTransformを分解して取得する関数
void ModelGetTransform(MODEL* model, XMFLOAT3& outPosition, XMFLOAT4& outRotation, XMFLOAT3& outScale)
{
	aiVector3D scaling, position;
	aiQuaternion rotation;

	model->AiScene->mRootNode->mTransformation.Decompose(scaling, rotation, position);

	outPosition = XMFLOAT3(position.x, position.y, position.z);
	outRotation = XMFLOAT4(rotation.x, rotation.y, rotation.z, rotation.w);
	outScale = XMFLOAT3(scaling.x, scaling.y, scaling.z);
}
