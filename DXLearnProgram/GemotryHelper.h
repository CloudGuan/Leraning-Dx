#pragma once 

#include "D3DUtils.h"
#include <DirectXMath.h>
#include <vector>
#include <windows.h>

struct Vertex
{
	Vertex();
	Vertex(const Vertex& rhs);
	Vertex(float x, float y, float z, float nx, float ny, float nz,float tx,float ty);
	DirectX::XMFLOAT3 Positon;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 Tex;
};

struct MeshDate
{
	std::vector<Vertex> PosInfos;
	std::vector<UINT> Indices;
};

class GemotryHelper
{
public:
	GemotryHelper();
	virtual~GemotryHelper();

	static GemotryHelper* GetInstance();
public:
	int GetPlatform(MeshDate& OutMeshData);
	int GetBox(MeshDate& OutMeshData);
	int GetSkull(MeshDate& OutMeshData);
private:
	static GemotryHelper* Single;
};