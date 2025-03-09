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

};



