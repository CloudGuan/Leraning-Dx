#pragma once

#include <windows.h>
#include <DirectXMath.h>

struct DirectionalLight
{
	DirectionalLight(){ ZeroMemory(this, sizeof(this)); }
	
	DirectX::XMFLOAT4 Ambient;	/**ģ�⻷������*/
	DirectX::XMFLOAT4 Diffuse;	/**ģ���������*/
	DirectX::XMFLOAT4 Specular;	/**ģ��߹�*/
	DirectX::XMFLOAT3 Direction; /**���䷽��*/
	float Pad; /**ռλ���ţ������ܹ���ʼ��Ϊ����*/
};


struct PointLigt
{
	PointLigt(){ ZeroMemory(this, sizeof(this)); }

	DirectX::XMFLOAT4 Ambinet;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;
	
	DirectX::XMFLOAT3 Position;
	float Range;
	
	DirectX::XMFLOAT3 Att; /**˥������
				  */
	float Pad;
};

struct SpotLight
{
	SpotLight(){ ZeroMemory(this, sizeof(this)); }

	DirectX::XMFLOAT4 Ambinet;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;

	DirectX::XMFLOAT3 Position;
	float Range;

	DirectX::XMFLOAT3 Direction;
	float Spot;

	DirectX::XMFLOAT3 Att;
	float Pad;

};

struct Material
{
	Material() { ZeroMemory(this, sizeof(this)); }

	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular; // w = SpecPower
	DirectX::XMFLOAT4 Reflect;
};