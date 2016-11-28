#pragma once

#include <windows.h>
#include <DirectXMath.h>

struct DirectionalLight
{
	DirectionalLight(){ ZeroMemory(this, sizeof(this)); }
	
	DirectX::XMFLOAT4 Ambient;	/**模拟环境光照*/
	DirectX::XMFLOAT4 Diffuse;	/**模拟漫反射光*/
	DirectX::XMFLOAT4 Specular;	/**模拟高光*/
	DirectX::XMFLOAT3 Direction; /**照射方向*/
	float Pad; /**占位符号，用于能够初始化为数组*/
};


struct PointLigt
{
	PointLigt(){ ZeroMemory(this, sizeof(this)); }

	DirectX::XMFLOAT4 Ambinet;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;
	
	DirectX::XMFLOAT3 Position;
	float Range;
	
	DirectX::XMFLOAT3 Att; /**衰减方程
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