#pragma once
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <print>
#include <ranges>	
#include <string>

class Utils
{
public:
	static void PrintXMVector(DirectX::FXMVECTOR v, std::string_view sv)
	{
		std::print("{} : ", sv);
		std::println("{{ {}, {}, {}, {} }}", DirectX::XMVectorGetX(v), DirectX::XMVectorGetY(v), DirectX::XMVectorGetZ(v), DirectX::XMVectorGetW(v));
	}
	
	static void PrintXMMatrix(DirectX::FXMMATRIX m, std::string_view sv)
	{
		std::println("{} : ", sv);
		for (int i : std::ranges::iota_view{ 0, 4 })
		{
			std::println("{{ {}, {}, {}, {} }}", DirectX::XMVectorGetX(m.r[i]), DirectX::XMVectorGetY(m.r[i]), DirectX::XMVectorGetZ(m.r[i]), DirectX::XMVectorGetW(m.r[i]));
		}
	}

	static void PrintBoundingBox(const DirectX::BoundingBox& b, std::string_view sv)
	{
		std::println("BoundingBox <{}> Infos : ", sv);
		std::println("Center : {{ {}, {}, {} }}", b.Center.x, b.Center.y, b.Center.z);
		std::println("Extents : {{ {}, {}, {} }}", b.Extents.x, b.Extents.y, b.Extents.z);
		std::println("CORNER_COUNT : {}", b.CORNER_COUNT);
	}
	
	static void PrintBoundingOrientedBox(const DirectX::BoundingOrientedBox& b, std::string_view sv)
	{
		std::println("BoundingOrientedBox <{}> Infos : ", sv);
		std::println("Center : {{ {}, {}, {} }}", b.Center.x, b.Center.y, b.Center.z);
		std::println("Extents : {{ {}, {}, {} }}", b.Extents.x, b.Extents.y, b.Extents.z);
		std::println("Orientation : {{ {}, {}, {}, {} }}", b.Orientation.x, b.Orientation.y, b.Orientation.z, b.Orientation.w);
		std::println("CORNER_COUNT : {}", b.CORNER_COUNT);
	}
	
	static void PrintBoundingSphere(const DirectX::BoundingSphere& b, std::string_view sv)
	{
		std::println("BoundingSphere <{}> Infos : ", sv);
		std::println("Center : {{ {}, {}, {} }}", b.Center.x, b.Center.y, b.Center.z);
		std::println("Radius : {}", b.Radius);
	}
	
	static void PrintBoundingFrustum(const DirectX::BoundingFrustum& b, std::string_view sv)
	{
		std::println("BoundingFrustum <{}> Infos : ", sv);
		std::println("Origin : {{ {}, {}, {} }}", b.Origin.x, b.Origin.y, b.Origin.z);
		std::println("Orientation : {{ {}, {}, {}, {} }}", b.Orientation.x, b.Orientation.y, b.Orientation.z, b.Orientation.w);
		std::println("Near : {}", b.Near);
		std::println("Far : {}", b.Far);
		std::println("LeftSlope : {}", b.LeftSlope);
		std::println("RightSlope : {}", b.RightSlope);
		std::println("TopSlope : {}", b.TopSlope);
		std::println("BottomSlope : {}", b.BottomSlope);
		std::println("CORNER_COUNT : {}", b.CORNER_COUNT);
	}

};



