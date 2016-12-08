#include "GemotryHelper.h"
#include <fstream>
#include <string>

GemotryHelper* GemotryHelper::Single = nullptr;
GemotryHelper::GemotryHelper()
{

}


GemotryHelper* GemotryHelper::GetInstance()
{
	if (!Single)
		Single = new GemotryHelper();

	return Single;
}

int GemotryHelper::GetPlatform(MeshDate& OutMeshData)
{
	return 0;
}

int GemotryHelper::GetBox(MeshDate& OutMeshData)
{
	float w2 = 0.5f * 6;
	float h2 = 0.5f * 6;
	float d2 = 0.5f * 6;

	OutMeshData.PosInfos.push_back(Vertex(-w2, -h2, -d2, 0.0f, 0.2f, -1.0f));
	OutMeshData.PosInfos.push_back(Vertex(-w2, +h2, -d2, 0.0f, 0.2f, -1.0f));
	OutMeshData.PosInfos.push_back(Vertex(+w2, +h2, -d2, 0.0f, 0.2f, -1.0f));
	OutMeshData.PosInfos.push_back(Vertex(+w2, -h2, -d2, 0.0f, 0.2f, -1.0f));
	
	OutMeshData.PosInfos.push_back(Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f));
	OutMeshData.PosInfos.push_back(Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f));
	OutMeshData.PosInfos.push_back(Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f));
	OutMeshData.PosInfos.push_back(Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f));

	OutMeshData.PosInfos.push_back(Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f));
	OutMeshData.PosInfos.push_back(Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f));
	OutMeshData.PosInfos.push_back(Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f));
	OutMeshData.PosInfos.push_back(Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f));

	OutMeshData.PosInfos.push_back(Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f));
	OutMeshData.PosInfos.push_back(Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f));
	OutMeshData.PosInfos.push_back(Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f));
	OutMeshData.PosInfos.push_back(Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f));
  
	OutMeshData.PosInfos.push_back( Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f));
	OutMeshData.PosInfos.push_back( Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f));
	OutMeshData.PosInfos.push_back( Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f));
	OutMeshData.PosInfos.push_back( Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f));

	OutMeshData.PosInfos.push_back(Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f));
	OutMeshData.PosInfos.push_back(Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f));
	OutMeshData.PosInfos.push_back(Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f));
	OutMeshData.PosInfos.push_back(Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f));

	UINT i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	OutMeshData.Indices.assign(&i[0], &i[36]);

	return 36;
}

int GemotryHelper::GetSkull(MeshDate& OutMeshData)
{

	std::ifstream fin;
	fin.open("./Common/skull.txt", std::ios_base::in);

	if (!fin.is_open())
	{
		MessageBox(0, L"open skull file error", 0, 0);
		return -1;
	}
	UINT VertexCount = 0;
	UINT IndicsCount=0;
	std::string Ignore;


	fin >> Ignore >> VertexCount;
	fin >> Ignore >> IndicsCount;
	OutMeshData.PosInfos.reserve(VertexCount);
	OutMeshData.Indices.reserve(IndicsCount * 3);

	fin >> Ignore >> Ignore >> Ignore >> Ignore;
	for (UINT index = 0; index < VertexCount; index++)
	{
		Vertex Temp;
		fin >> Temp.Positon.x >> Temp.Positon.y >> Temp.Positon.z;
		/**Scape Normal info*/
		fin >> Temp.Normal.x >> Temp.Normal.y >> Temp.Normal.z;
		OutMeshData.PosInfos.push_back(Temp);
	}

	fin >> Ignore;
	fin >> Ignore;
	fin >> Ignore;

	for (UINT index = 0; index < IndicsCount; index++)
	{
		UINT sid = 0;
		fin >> sid;
		OutMeshData.Indices.push_back(sid);
		fin >> sid;
		OutMeshData.Indices.push_back(sid);
		fin >> sid;
		OutMeshData.Indices.push_back(sid);
	}
	fin.close();
	return 0;
}

GemotryHelper::~GemotryHelper()
{
	if (Single)
	{
		delete Single;
		Single = nullptr;
	}
}


Vertex::Vertex()
{

}

Vertex::Vertex(float x, float y, float z, float nx, float ny, float nz)
	:Positon(x,y,z)
	,Normal(nx,ny,nz)
{

}

Vertex::Vertex(const Vertex& rhs)
{
	this->Positon=rhs.Positon;
	this->Normal = rhs.Normal;
}
