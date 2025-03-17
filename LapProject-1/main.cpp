// ------------------------------------------------------------------
// Tutorial for DirectXMath
// ------------------------------------------------------------------

// DirectXMath Headers
#include <DirectXMath.h>			// Provides struct/class/function for vector and matrix
#include <DirectXPackedVector.h>	// Provides packed vectors
#include <DirectXColors.h>			// Provides definition of colors
#include <DirectXCollision.h>		// Provides struct/class/function for collision detection

using namespace DirectX;
using namespace DirectX::PackedVector;

#include <iostream>
#include <print>
#include <typeinfo>
#include <string>
#include <vector>

#include "Utils.h"
using namespace std::literals;

// 1. Basics of DirectXMath
void XMBasics()
{
	// 1. Constants(radian)
	// DirectXMath provides some constants like radian : more in DirectXMath.h 
	std::println("1. Constants(radian)");
	std::println("XM_PI (π) : {}", XM_PI);
	std::println("XM_2PI (2π) : {}", XM_2PI);
	std::println("XM_1DIVPI (1/π) : {}", XM_1DIVPI);
	std::println("XM_1DIV2PI (1/2π) : {}", XM_1DIVPI);
	std::println("XM_PIDIV2 (π/2) : {}", XM_PIDIV2);
	std::println("XM_PIDIV4 (π/4) : {}", XM_PIDIV2);

	std::println();

	// 2. Easy conversion between radian and degree
	std::println("2. Easy conversion between radian and degree");
	float deg = 45;
	float rad = XMConvertToRadians(deg);
	float radToDeg = XMConvertToDegrees(rad);
	std::println("45 Deg to rad : {}", rad);
	std::println("Re-convert to deg : {}", radToDeg);

	// 3. XMVECTOR & XMMATRIX
	// This two types are must be allign as 16 byte.
	// But DirectX do not guarantees to Compile as 16 byte allign.
	// Only Compile this in x64 system so that compiler can compile this as 16 byte allign.
	// Also, DO NOT USE XMVECTOR AND XMMATRIX AS MEMBER OF CLASS OR STRUCT (bc it's not guaranteed as 16 byte allign)

	std::println();

	// 3-1. XMVECTOR
	{
		// Keep it mind that XMVECTOR is not struct. It's just a 128bit type.
		// Real implementation of XMVECTOR is differ by system.
		std::println("3-1. XMVECTOR");

		std::println("typeid of XMVECTOR : {}", typeid(XMVECTOR).name());	//union __m128

		// We cannot access elements of XMVECTOR directly.
		// Only way to access the element is using XMVectorGet...() and XMVectorSet...() functions (this functions called Vector Accessor)

		XMVECTOR v = XMVectorSet(1.f, 2.f, 3.f, 4.f);
		std::println("XMVECTOR v : {{ {}, {}, {}, {} }}", XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v));

		// DirectXMath provides operator overloadings for XMVECTOR.
		// extended vector operations are basically opration of eash elements.
		// example) operator* is not dot or cross product. It just multiply each elements,
		// For performance, Avoid using overloaded operators as much as possible

		XMVECTOR v2 = XMVectorSet(2.f, 3.f, 4.f, 5.f);
		XMVECTOR res = v * v2;
		std::println("{{ 1.f, 2.f, 3.f, 4.f }} * {{ 2.f, 3.f, 4.f, 5.f }} = {{ {}, {}, {}, {} }}", XMVectorGetX(res), XMVectorGetY(res), XMVectorGetZ(res), XMVectorGetW(res));

		// DirectXMath provides struct called XMVECTORF32.
		// XMVECTORF32 can convert type to plane XMVECTOR. (reverse is not easy)

		XMVECTORF32 f32Vector = { 1.f, 2.f, 3.f, 4.f };
		XMVECTOR f32toXMVec = f32Vector;
		std::println("{{ {}, {}, {}, {} }}", XMVectorGetX(f32toXMVec), XMVectorGetY(f32toXMVec), XMVectorGetZ(f32toXMVec), XMVectorGetW(f32toXMVec));
	}

	std::println();

	// 3-2. XMMATRIX
	{
		// Unlike XMVECTOR, XMMATRIX is struct of 4-XMVECTOR array (XMVECTOR r[4]).
		// Each XMVECTOR means row by order. This also means XMMATRIX is row-major matrix. (Unlike OpenGL's glm::mat4)
		// We can't access each elements of XMMATRIX. But we can access using vector accessor line XMVECTOR.
		std::println("3-2 XMMATRIX");

		XMMATRIX mat;
		mat.r[0] = XMVectorSet(1.f, 2.f, 3.f, 4.f);
		mat.r[1] = XMVectorSet(5.f, 6.f, 7.f, 8.f);
		mat.r[2] = XMVectorSet(9.f, 10.f, 11.f, 12.f);
		mat.r[3] = XMVectorSet(13.f, 14.f, 15.f, 16.f);

		std::println("XMMATRIX Created : ");
		for (int i = 0; i < 4; i++)
		{
			std::println("{{ {}, {}, {}, {} }}", XMVectorGetX(mat.r[i]), XMVectorGetY(mat.r[i]), XMVectorGetZ(mat.r[i]), XMVectorGetW(mat.r[i]));
		}

		// XMMATRIX also provides operator overloading.
		// For performance, Avoid using overloaded operators as much as possible
		XMMATRIX matAdd = mat + mat;
		std::println("mat + mat");
		for (int i = 0; i < 4; i++)
		{
			std::println("{{ {}, {}, {}, {} }}", XMVectorGetX(matAdd.r[i]), XMVectorGetY(matAdd.r[i]), XMVectorGetZ(matAdd.r[i]), XMVectorGetW(matAdd.r[i]));
		}

		XMMATRIX matMul = mat * mat;
		std::println("mat * mat");
		for (int i = 0; i < 4; i++)
		{
			std::println("{{ {}, {}, {}, {} }}", XMVectorGetX(matMul.r[i]), XMVectorGetY(matMul.r[i]), XMVectorGetZ(matMul.r[i]), XMVectorGetW(matMul.r[i]));
		}

	}

	std::println();

	// 4. Use of DirectXMath
	{
		// For using XMVECTOR and XMMATRIX as function parameters, We need to use proper calling convention for optimize data (which is 16 byte alligned).
		// Proper 2 calling conventions are : 1. __fastcall / 2. __vetorcall
		// How this works?
		//		In 32-bit Windows (x86 machine):
		//			__fastcall : Pass first 3 XMVECTOR parameters to SSE/SSE2 register and rest is pass to stack. -> default 32-bit Windows calling convention.
		//			__vectorcall : Pass first 6 XMVECTOR parameters to SSE/SSE2 register and rest is pass to stack.
		//		
		//		In 64-bit Windows (x64 machine):
		//			__fastcall : Pass every XMVECTOR parameters to SSE/SSE2 register. -> default 64-bit Windows calling convention.
		//			__vectorcall : Pass first 6 XMVECTOR parameters in SSE/SSE2 register and rest is pass to stack. + This one can pass XMMATRIX to SSE/SSE2 register.
		// 
		// In conclusion : Use __vectorcall in 64-bit (for XMMATRIX to SIMD register)
		// Easy method : just set default calling convention as __vectorcall in visual studio's project options

		// DirectXMath provides special types for support this calling conventions : 
		//		typedef const XMVECTOR FXMVECTOR;
		//		typedef const XMVECTOR GXMVECTOR;
		//		typedef const XMVECTOR HXMVECTOR;
		//		typedef const XMVECTOR& CXMVECTOR;
		//		typedef const XMMATRIX FXMMATRIX;
		//		typedef const XMMATRIX& CXMMATRIX
		// For performance, Recommand using this special types and calling conventions rules below.
		//		1. For proper use of calling conventions : use XM_CALLCONV like example below. (but not in C++'s constructors. because of __vectorcall's limitation)
		//			Example from DirectXMathVector.inl) XMVECTOR XM_CALLCONV XMVector3Transform(FXMVECTOR V, FXMMATRIX M);
		//		2. To pass XMVECTOR as parameter, Use FXMVECTOR for first 3 XMVECTOR parameters
		//		3. Use GXMVECTOR for 4th XMVECTOR parameter.
		//		4. Use HXMVECTOR for 5, 6th XMVECTOR parameters.
		//		5. Use CXMVECTOR for over 7th XMVECTOR parameter.
		//		6. Because of limitation of __vectorcall, Use FXMVECTOR and CXMVECTOR instead of GXMVECTOR and HXMVECTOR. -> I cannot understand this : conflict with 4,5 rules
		//		7. Use FXMVECTOR for first 3 XMVECTOR parameters and use CXMVECTOR for rest of XMVECTOR parameters. -> I cannot understand this : conflict with 4,5 rules
		//		8. Always use XMVECTOR* or XMVECCTOR& as output parameters.
		//		9. Use FXMMATRIX for pass XMMATRIX as first parameter and use CXMMATRIX for rest of XMMATRIX parameters.
		//		10. Because if limitation of __vectorcall, use CXMMATRIX in constructors.

		// XM_CALLCONV is macro of __vectorcall.

		// Most of DirectXMath's functions are declared as inline function. -> this calling convention rules will not applied.

	}

	std::println();

	// 5. Load & Store of XMVECTOR / XMMATRIX + XMCOLORS
	{
		std::println("5. Load & Store of XMVECTOR / XMMATRIX + XMCOLORS");
		// Because It's not guaranteed as 16-byte allign, You must not use XMVECTOR and XMMATRIX as member of class or struct.
		// DirectXMath provides 32-bit real number struct for when use of XMVECTOR and XMMATRIX as member variables. such as : XMFLOAT3, XMFLOAT4, XMFLOAT4X4, ...
		// DirectXMath also provides XMINT4, XMUINT4, ... -> uses very little. Not covered here.

		// 5-1. XMFLOAT#
		// XMFLOAT2 : struct of 2-floating numbers
		// XMFLOAT3 : struct of 3-floating numbers
		// XMFLOAT4 : struct of 4-floating numbers
		// 
		// Types end with A : guarantees 16 byte allign -> XMFLOAT2A, XMFLOAT3A, XMFLOAT4A

		// 5-2. XMFLOAT#X#
		// XMFLOAT(N)X(M) represent row-major matrix -> XMMATRIX4X4, XMMATRIX4X3, XMMATRIX3X3, ...
		// member m is 2D array of elements. : m[2][3] means row 3, colmn 4 element. (row-major)
		// X is uppercase. Do not confuse.

		// 5-3. Operations of vectors and matrices
		// XMFLOAT# types are not support SIMD extended operations directly..
		// To use advantage of SIMD operations, We need to follw this steps :
		//		1. Load XMFLOAT members to local XMVECTOR or XMMATRIX using XMLoadFloat...() functions. (Vector Load Function)
		//		2. Perform SIMD operations using local XMVECTOR or XMMATRIX.
		//		3. Store local XMVECTOR and XMMATRIX to member of XMFLOAT# using XMStoreFloat...() functions. (Vector Store Function)

		// 5-3-1. Vector Load Functions
		// XMLoadFloat...() series are fuctions for copy XMFLOAT# values to XMVECTOR. Such as :
		//		XMVECTOR XMLoadFloat(const float* pSource);
		//		XMVECTOR XMLoadFloat3(const XMFLOAT3* pSource);
		//		XMVECTOR XMLoadFloat3A(const XMFLOAT3A* pSource);
		//		XMVECTOR XMLoadFloat4(const float* pSource);
		// if number of real numbers in parameter are less then 4, blank elements will filled with 0.
		// Like this below

		float source = 3;
		XMVECTOR a = XMLoadFloat(&source);
		std::println("element of a : {{ {}, {}, {}, {} }}", XMVectorGetX(a), XMVectorGetY(a), XMVectorGetZ(a), XMVectorGetW(a));	// element of a : { 3, 0, 0, 0 }

		// We can also load matrix using XMLoadFloat#x#()
		// Like this below
		XMFLOAT4X4 m = {};
		XMMATRIX xmm = XMLoadFloat4x4(&m);
		for (int i = 0; i < 4; i++)
		{
			std::println("{{ {}, {}, {}, {} }}", XMVectorGetX(xmm.r[i]), XMVectorGetY(xmm.r[i]), XMVectorGetZ(xmm.r[i]), XMVectorGetW(xmm.r[i]));	
		}
		// { 0, 0, 0, 0 }
		// { 0, 0, 0, 0 }
		// { 0, 0, 0, 0 }
		// { 0, 0, 0, 0 }

		// 5-3-2. Vector Store Functions
		// XMStoreFloat...() series are functions for copy XMVECTOR to XMFLOAT#. such as
		//		void XMStoreFloat(float* pDestination, XMVECTOR v);
		//		void XMStoreFloat3(XMFLOAT3* pDestination, XMVECTOR v);
		//		void XMStoreFloat4(XMFLOAT4* pDestination, XMVECTOR v);
		//		void XMStoreFloat4A(XMFLOAT4A* pDestination, XMVECTOR v);
		// Store functions are copy varibles count of *pDestinations's real numbers by order of x, y, z, w.
		// Like this below

		source = 0;
		XMStoreFloat(&source, a/*XMVECTOR defined above : value is { 3, 0, 0, 0 }*/);
		std::println("source = {}", source);	// source = 3

		// We can also store matrix using XMStoreFloat#x#()

		// 5-4. Color load/store functions
		// Colors can be represented using XMVECTOR. ({ r, g, b, a })
		// DirectXMath provides XMCOLOR type. 
		// XMCOLOR is A8R8G8B8 32-bit packed normalize integer vector. Store elements as :
		// [32] aaaaaaaa rrrrrrrr gggggggg bbbbbbbb [0]
		// Note that alpha value is stored in most significant bits.

		// To copy between XMCOLOR and XMVECTOR, We can use XMLoadColor() function.
		// XMVECTOR XMLoadColor(XMCOLOR* pSource);
		// void XMStoreColor(XMCOLOR* pDestination, XMVECTOR v)

		// DirectXColors.h provides standard colors in DirectX::Colors namespaces as constants
		// Note that constants from DirectX::Colors are XMFLOATF32 not XMCOLOR. -> This makes us to easy convert to XMVECTOR of colors
		std::println("type from DirectX::Colors : {}", typeid(decltype(DirectX::Colors::AliceBlue)).name());	// type from DirectX::Colors : struct DirectX::XMVECTORF32
		XMVECTOR aliceBlue = DirectX::Colors::AliceBlue;
		std::println("element of aliceBlue : {{ {}, {}, {}, {} }}", XMVectorGetX(aliceBlue), XMVectorGetY(aliceBlue), XMVectorGetZ(aliceBlue), XMVectorGetW(aliceBlue));	// element of aliceBlue : { 0.94117653, 0.9725491, 1, 1 }
	}

	std::println();

	// 6. Vector Initialize Functions
	{
		std::println("6. Vector Initialize Functions");
		// We an initialize XMVECTOR easily using XMVECTORF32.
		XMVECTORF32 v1 = { 1.f, 2.f, 3.f, 4.f };
		XMVECTOR v2 = v1;

		// DirectXMath provides pre-defined global constants such as:
		std::println("g_XMOne : {{ {}, {}, {}, {} }}", XMVectorGetX(g_XMOne), XMVectorGetY(g_XMOne), XMVectorGetZ(g_XMOne), XMVectorGetW(g_XMOne));			// g_XMOne: { 1, 1, 1, 1 }
		std::println("g_XMOne3 : {{ {}, {}, {}, {} }}", XMVectorGetX(g_XMOne3), XMVectorGetY(g_XMOne3), XMVectorGetZ(g_XMOne3), XMVectorGetW(g_XMOne3));	// g_XMOne3 : { 1, 1, 1, 0 }
		std::println("g_XMZero : {{ {}, {}, {}, {} }}", XMVectorGetX(g_XMZero), XMVectorGetY(g_XMZero), XMVectorGetZ(g_XMZero), XMVectorGetW(g_XMZero));	// g_XMZero : { 0, 0, 0, 0 }
		std::println("g_XMPi : {{ {}, {}, {}, {} }}", XMVectorGetX(g_XMPi), XMVectorGetY(g_XMPi), XMVectorGetZ(g_XMPi), XMVectorGetW(g_XMPi));				// g_XMPi : { 3.1415927, 3.1415927, 3.1415927, 3.1415927 }
		// more in DirectXMath.h

		// Functions below is examples of initialize XMVECTOR type variables.
		std::println("Initializing vectors");

		// XMVectorZero()
		XMVECTOR xmTest = {};
		xmTest = XMVectorZero();
		std::println("XMVectorZero() : {{ {}, {}, {}, {} }}", XMVectorGetX(xmTest), XMVectorGetY(xmTest), XMVectorGetZ(xmTest), XMVectorGetW(xmTest));	// XMVectorZero() : { 0, 0, 0, 0 }

		// XMVectorSet(float x, float y, float z, float w)
		xmTest = XMVectorSet(1.f, 2.f, 3.f, 4.f);
		std::println("XMVectorSet(1.f, 2.f, 3.f, 4.f) : {{ {}, {}, {}, {} }}", XMVectorGetX(xmTest), XMVectorGetY(xmTest), XMVectorGetZ(xmTest), XMVectorGetW(xmTest));	// XMVectorSet(1.f, 2.f, 3.f, 4.f) : { 1, 2, 3, 4 }

		// XMVectorReplicate(float value)
		xmTest = XMVectorReplicate(1.f);
		std::println("XMVectorReplicate(1.f) : {{ {}, {}, {}, {} }}", XMVectorGetX(xmTest), XMVectorGetY(xmTest), XMVectorGetZ(xmTest), XMVectorGetW(xmTest));	// XMVectorReplicate(1.f) : { 1, 1, 1, 1 }

		// XMVectorReplicatePtr(const float* pValue)
		float value = 3.f;
		xmTest = XMVectorReplicatePtr(&value);
		std::println("XMVectorReplicatePtr(&value) (value = 3.f) : {{ {}, {}, {}, {} }}", XMVectorGetX(xmTest), XMVectorGetY(xmTest), XMVectorGetZ(xmTest), XMVectorGetW(xmTest));	// XMVectorReplicatePtr(&value) (value = 3.f) : { 3, 3, 3, 3 }

		// XMVectorSplatOne()
		xmTest = XMVectorSplatOne();
		std::println("XMVectorSplatOne() : {{ {}, {}, {}, {} }}", XMVectorGetX(xmTest), XMVectorGetY(xmTest), XMVectorGetZ(xmTest), XMVectorGetW(xmTest));	// XMVectorSplatOne() : { 1, 1, 1, 1 }

		// 6-1. Vector Component-Wise Function
		// We an create new XMVECTOR from existing XMVECTOR

		XMVECTOR exist = XMVectorSet(1.f, 2.f, 3.f, 4.f);
		std::println("XMVECTOR exist : {{ {}, {}, {}, {} }}", XMVectorGetX(exist), XMVectorGetY(exist), XMVectorGetZ(exist), XMVectorGetW(exist));


		// XMVectorSplat#(XMVECTOR v);

		xmTest = XMVectorSplatX(exist);
		std::println("XMVectorSplatX(exist) : {{ {}, {}, {}, {} }}", XMVectorGetX(xmTest), XMVectorGetY(xmTest), XMVectorGetZ(xmTest), XMVectorGetW(xmTest));	// XMVectorSplatX(exist) : { 1, 1, 1, 1 }
		
		xmTest = XMVectorSplatY(exist);
		std::println("XMVectorSplatY(exist) : {{ {}, {}, {}, {} }}", XMVectorGetX(xmTest), XMVectorGetY(xmTest), XMVectorGetZ(xmTest), XMVectorGetW(xmTest));	// XMVectorSplatY(exist) : { 2, 2, 2, 2 }
		
		xmTest = XMVectorSplatZ(exist);
		std::println("XMVectorSplatZ(exist) : {{ {}, {}, {}, {} }}", XMVectorGetX(xmTest), XMVectorGetY(xmTest), XMVectorGetZ(xmTest), XMVectorGetW(xmTest));	// XMVectorSplatZ(exist) : { 3, 3, 3, 3 }
		
		xmTest = XMVectorSplatW(exist);
		std::println("XMVectorSplatW(exist) : {{ {}, {}, {}, {} }}", XMVectorGetX(xmTest), XMVectorGetY(xmTest), XMVectorGetZ(xmTest), XMVectorGetW(xmTest));	// XMVectorSplatW(exist) : { 4, 4, 4, 4 }

		// XMVectorPermute() can create new XMVECTOR from 2 existing XMVECTORS
		// new XMVECTOR's { x, y, z, w } can be selected in 0 ~ 7 or XM_PERMUTE_0X ~ XM_PERMUTE_1W.
		// XM_PERMUTE is constant uint32_t type integer in DirectXMath.h.
		// XM_PERMUTE_0X = 0 / XM_PERMUTE_0Y = 1 / ... / XM_PERMUTE_0W = 3 / XM_PERMUTE_1X = 4 / ... / XM_PERMUTE_1Z = 6 / XM_PERMUTE_1W = 7
		// XMVectorPermute is template functions. Use like below

		XMVECTOR exist2 = XMVectorSet(5.f, 6.f, 7.f, 8.f);
		std::println("XMVECTOR exist2 : {{ {}, {}, {}, {} }}", XMVectorGetX(exist2), XMVectorGetY(exist2), XMVectorGetZ(exist2), XMVectorGetW(exist2));

		xmTest = XMVectorPermute<0, 1, 4, 5>(exist, exist2);
		std::println("XMVectorPermute<0, 1, 4, 5>(exist, exist2) : {{ {}, {}, {}, {} }}", XMVectorGetX(xmTest), XMVectorGetY(xmTest), XMVectorGetZ(xmTest), XMVectorGetW(xmTest));	// XMVectorPermute<0, 1, 4, 5>(exist, exist2) : { 1, 2, 5, 6 }

		xmTest = XMVectorPermute<XM_PERMUTE_0X, XM_PERMUTE_0Y, XM_PERMUTE_1X, XM_PERMUTE_1Y>(exist, exist2);
		std::println("XMVectorPermute<XM_PERMUTE_0X, XM_PERMUTE_0Y, XM_PERMUTE_1X, XM_PERMUTE_1Y>(exist, exist2) : {{ {}, {}, {}, {} }}", XMVectorGetX(xmTest), XMVectorGetY(xmTest), XMVectorGetZ(xmTest), XMVectorGetW(xmTest));	
		// XMVectorPermute<XM_PERMUTE_0X, XM_PERMUTE_0Y, XM_PERMUTE_1X, XM_PERMUTE_1Y>(exist, exist2) : { 1, 2, 5, 6 }

		// XMVectorSwizzle() can create new XMVECTOR from exist XMVECTOR in diffrent ordering
		// XM_SWIZZLE constants are provided : XM_SWIZZLE_X = 0 / ... / XM_SWIZZLE_W = 3
		// XMVectorSwizzle is also template functions. Use like below

		xmTest = XMVectorSwizzle</*3, 3, 0, 2*/XM_SWIZZLE_Y, XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_Y>(exist);
		std::println("XMVectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_Y>(exist) : {{ {}, {}, {}, {} }}", XMVectorGetX(exist2), XMVectorGetY(exist2), XMVectorGetZ(exist2), XMVectorGetW(exist2));
		// XMVectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_Y>(exist) : { 5, 6, 7, 8 }
	}

	std::println();

	// 7. Vector Accessor Functions
	{
		std::println("7. Vector Accessor Functions");
		// We cannot access XMVECTOR's elements. So, DirectXMath provides Functions that we can access elements indirectly. -> We call this Vector Accessor Funtion
		// Here is some examples below

		XMVECTOR xmTest = XMVectorSet(1.f, 2.f, 3.f, 4.f);
		std::println("XMVECTOR xmTest : {{ {}, {}, {}, {} }}", XMVectorGetX(xmTest), XMVectorGetY(xmTest), XMVectorGetZ(xmTest), XMVectorGetW(xmTest));

		// XMVectorGetByIndex(XMVECTOR v, size_t index)
		std::println("XMVectorGetByIndex(xmTest, 0) : {}", XMVectorGetByIndex(xmTest, 0));	// XMVectorGetByIndex(xmTest, 0) : 1
		std::println("XMVectorGetByIndex(xmTest, 1) : {}", XMVectorGetByIndex(xmTest, 1));	// XMVectorGetByIndex(xmTest, 1) : 2
		std::println("XMVectorGetByIndex(xmTest, 2) : {}", XMVectorGetByIndex(xmTest, 2));	// XMVectorGetByIndex(xmTest, 2) : 3
		std::println("XMVectorGetByIndex(xmTest, 3) : {}", XMVectorGetByIndex(xmTest, 3));	// XMVectorGetByIndex(xmTest, 3) : 4

		// XMVectorGet#(XMVECTOR v)
		std::println("XMVectorGetX(xmTest) : {}", XMVectorGetX(xmTest));	// XMVectorGetX(xmTest) : 1
		std::println("XMVectorGetY(xmTest) : {}", XMVectorGetY(xmTest));	// XMVectorGetY(xmTest) : 2
		std::println("XMVectorGetZ(xmTest) : {}", XMVectorGetZ(xmTest));	// XMVectorGetZ(xmTest) : 3
		std::println("XMVectorGetW(xmTest) : {}", XMVectorGetW(xmTest));	// XMVectorGetW(xmTest) : 4

		// XMVectorGet#Ptr(float* px, XMVECTOR v)
		float p = 0;
		XMVectorGetXPtr(&p, xmTest);
		std::println("XMVectorGetXPtr(p, xmTest) : {}", p);	// XMVectorGetXPtr(p, xmTest) : 1

		// XMVectorSet#(XMVECTOR v, float x) creates new XMVECTOR : NOT MODIFY ORIGINAL v
		xmTest = XMVectorSetY(xmTest, 100.f);
		std::println("after xmTest = XMVectorSetY(xmTest, 100.f); : {{ {}, {}, {}, {} }}", XMVectorGetX(xmTest), XMVectorGetY(xmTest), XMVectorGetZ(xmTest), XMVectorGetW(xmTest)); // after xmTest = XMVectorSetY(xmTest, 100.f); : { 1, 100, 3, 4 }

		// XMVectorSet#Ptr(XMVECTOR v, float* px) creates new XMVECTOR : NOT MODIFY ORIGINAL v
		xmTest = XMVectorSetYPtr(xmTest, &p);
		std::println("after xmTest = XMVectorSetYPtr(xmTest, &p); : {{ {}, {}, {}, {} }}", XMVectorGetX(xmTest), XMVectorGetY(xmTest), XMVectorGetZ(xmTest), XMVectorGetW(xmTest)); // after xmTest = XMVectorSetYPtr(xmTest, &p); : { 1, 1, 3, 4 }

	}

	std::println();

	// 8. Color Functions
	{
		std::println("8. Color Functions");
		// When XMVECTOR represent colors, elements are clamped to [0.0f, 1.0f]
		// each x, y, z, w elements represents r, g, b, a

		// Color Comparison Functions
		XMVECTOR red = XMVectorSet(1.f, 0.f, 0.f, 1.f);		// RED
		XMVECTOR green = XMVectorSet(0.f, 1.f, 0.f, 1.f);	// GREEN
		XMVECTOR blue = XMVectorSet(0.f, 0.f, 1.f, 1.f);	// BLUE

		std::cout << std::boolalpha;
		std::cout << "XMColorLess(red, green) : "			<< XMColorLess(red, green)				<< '\n';	// XMColorLess(red, green) : false
		std::cout << "XMColorLessOrEqual(red, blue) : "		<< XMColorLessOrEqual(red, blue)		<< '\n';	// XMColorLessOrEqual(red, blue) : false
		std::cout << "XMColorEqual(green, green) : "		<< XMColorEqual(green, green)			<< '\n';	// XMColorEqual(green, green) : true
		std::cout << "XMColorNotEqual(red, green) : "		<< XMColorNotEqual(red, green)			<< '\n';	// XMColorNotEqual(red, green) : true
		std::cout << "XMColorGreater(red, blue) : "			<< XMColorGreater(red, blue)			<< '\n';	// XMColorGreater(red, blue) : false
		std::cout << "XMColorGreaterOrEqual(red, green) : " << XMColorGreaterOrEqual(red, green)	<< '\n';	// XMColorGreaterOrEqual(red, green) : false

		// XMColorModulate(XMVECTOR c1, XMVECTOR c2) computes element-wise Product.
		XMVECTOR redMulGreen = XMColorModulate(red, green);
		std::println("XMColorModulate(red, green) : {{ {}, {}, {}, {} }}", XMVectorGetX(redMulGreen), XMVectorGetY(redMulGreen), XMVectorGetZ(redMulGreen), XMVectorGetW(redMulGreen));	// XMColorModulate(red, green) : { 0, 0, 0, 1 }

		// XMColorNegative(XMVECTOR c) returns complementary color. (1.0 - c.x, 1.0 - c.y, 1.0 - c.z, c.w)
		XMVECTOR negRed = XMColorNegative(red);
		std::println("XMColorNegative(red) : {{ {}, {}, {}, {} }}", XMVectorGetX(negRed), XMVectorGetY(negRed), XMVectorGetZ(negRed), XMVectorGetW(negRed));	// XMColorNegative(red) : { 0, 1, 1, 1 }

		// XMColorAdjustContrast(XMVECTOR cc, float contrast) returns after adjust contrast of c using contrast.
		// Result will be :
		// result.x = (c.x - 0.5f) * constrast + 0.5f;
		// result.y = (c.y - 0.5f) * constrast + 0.5f;
		// result.z = (c.z - 0.5f) * constrast + 0.5f;
		// result.x = c.w;
		XMVECTOR contRed = XMColorAdjustContrast(red, 0.2f);
		std::println("XMColorAdjustContrast(red, 0.2f) : {{ {}, {}, {}, {} }}", XMVectorGetX(contRed), XMVectorGetY(contRed), XMVectorGetZ(contRed), XMVectorGetW(contRed));	// XMColorAdjustContrast(red, 0.2f) : { 0.6, 0.4, 0.4, 1 }

		// XMVColorAdjustSaturation(XMVECTOR c, float saturation) returns after adjust saturation of c using saturation.
		// Result will be:
		// float fLuminance = 0.2125f * c.x + 0.7154f * c.y + 0.0721f * c.x;
		// result.x = (c.x - fLuminance) * saturation + fLuminance;
		// result.y = (c.y - fLuminance) * saturation + fLuminance;
		// result.z = (c.z - fLuminance) * saturation + fLuminance;
		// result.w = c.w;
		XMVECTOR satRed = XMColorAdjustSaturation(red, 0.2f);
		std::println("XMColorAdjustSaturation(red, 0.2f) : {{ {}, {}, {}, {} }}", XMVectorGetX(satRed), XMVectorGetY(satRed), XMVectorGetZ(satRed), XMVectorGetW(satRed));	// XMColorAdjustSaturation(red, 0.2f) : { 0.37, 0.17, 0.17, 1 }

		// XMColorRGBToSRGB(XMVECTOR rgb) <-> XMColorSRGBToRGB(XMVECTOR srgb)
		// Convert between RGB and sRGB
		XMVECTOR sRGBRed = XMColorRGBToSRGB(red);
		XMVECTOR RGBRed = XMColorSRGBToRGB(sRGBRed);
		std::println("XMVECTOR sRGBRed = XMColorRGBToSRGB(red); : {{ {}, {}, {}, {} }}", XMVectorGetX(sRGBRed), XMVectorGetY(sRGBRed), XMVectorGetZ(sRGBRed), XMVectorGetW(sRGBRed));	// XMVECTOR sRGBRed = XMColorRGBToSRGB(red); : { 0.99999994, 0, 0, 1 }
		std::println("XMColorSRGBToRGB(sRGBRed) : {{ {}, {}, {}, {} }}", XMVectorGetX(RGBRed), XMVectorGetY(RGBRed), XMVectorGetZ(RGBRed), XMVectorGetW(RGBRed));	// XMColorSRGBToRGB(sRGBRed) : { 1, 0, 0, 1 }
		
		// XMColorRGBToHSL(XMVECTOR rgb) <-> XMColorHSLToRGB(XMVECTOR hsl)
		// Convert between RGB and HSL (Hue Saturation Luminance)
		XMVECTOR hslRed = XMColorRGBToHSL(red);
		RGBRed = XMColorHSLToRGB(hslRed);
		std::println("XMVECTOR hslRed = XMColorRGBToHSL(red); : {{ {}, {}, {}, {} }}", XMVectorGetX(hslRed), XMVectorGetY(hslRed), XMVectorGetZ(hslRed), XMVectorGetW(hslRed));	// XMVECTOR hslRed = XMColorRGBToHSL(red); : { 0, 1, 0.5, 1 }
		std::println("XMColorHSLToRGB(HSLRed) : {{ {}, {}, {}, {} }}", XMVectorGetX(RGBRed), XMVectorGetY(RGBRed), XMVectorGetZ(RGBRed), XMVectorGetW(RGBRed));	// XMColorHSLToRGB(HSLRed) : { 1, 0, 3.5762787e-07, 1 } <- ????????
		
		// XMColorRGBToHSV(XMVECTOR rgb) <-> XMColorHSVToRGB(XMVECTOR hsv)
		// Convert between RGB and HSV (Hue Saturation Value)
		XMVECTOR hsvRed = XMColorRGBToHSV(red);
		RGBRed = XMColorHSVToRGB(hsvRed);
		std::println("XMVECTOR hsvRed = XMColorRGBToHSV(red); : {{ {}, {}, {}, {} }}", XMVectorGetX(hsvRed), XMVectorGetY(hsvRed), XMVectorGetZ(hsvRed), XMVectorGetW(hsvRed));	// XMVECTOR hsvRed = XMColorRGBToHSV(red); : { 0, 1, 1, 1 }
		std::println("XMColorHSVToRGB(HSVRed) : {{ {}, {}, {}, {} }}", XMVectorGetX(RGBRed), XMVectorGetY(RGBRed), XMVectorGetZ(RGBRed), XMVectorGetW(RGBRed));	// XMColorHSVToRGB(HSVRed) : { 1, 0, 0, 1 }
		
		// XMColorRGBToXYZ(XMVECTOR rgb) <-> XMColorXYZToRGB(XMVECTOR xyz)
		// Convert between RGB and XYZ
		XMVECTOR xyzRed = XMColorRGBToXYZ(red);
		RGBRed = XMColorXYZToRGB(xyzRed);
		std::println("XMVECTOR xyzRed = XMColorRGBToXYZ(red); : {{ {}, {}, {}, {} }}", XMVectorGetX(xyzRed), XMVectorGetY(xyzRed), XMVectorGetZ(xyzRed), XMVectorGetW(xyzRed));	// XMVECTOR xyzRed = XMColorRGBToXYZ(red); : { 2.7615867, 0.99567384, 0, 1 }
		std::println("XMColorXYZToRGB(XYZRed) : {{ {}, {}, {}, {} }}", XMVectorGetX(RGBRed), XMVectorGetY(RGBRed), XMVectorGetZ(RGBRed), XMVectorGetW(RGBRed));	// XMColorXYZToRGB(XYZRed) : { 1.0000001, -9.4547445e-08, 1.9659051e-08, 1 } <- ??????????????
	}


}

void XM3DGraphicsFundametals()
{
	// 1. trigonometric functions
	{
		std::println("1. trigonometric functions");
		// DirectXMath provides trigonometric functions using radian values. <- CAUTION!!! NOT DEGREE
		// Postfix ...Est functions like XMScalarCosEst() will returns an approximate value. This will be more faster in case of value is OK with less pricision.
		
		float pi = XM_PIDIV2;
		std::println("XMScalarSin(XM_PIDIV2) : {}", XMScalarSin(XM_PIDIV2));		// XMScalarSin(XM_PIDIV2) : 0.9999999
		std::println("XMScalarCos(XM_PIDIV2) : {}", XMScalarCos(XM_PIDIV2));		// XMScalarCos(XM_PIDIV2) : -1.1920929e-07
		std::println("XMScalarSinEst(XM_PIDIV2) : {}", XMScalarSinEst(XM_PIDIV2));	// XMScalarSinEst(XM_PIDIV2) : 0.9999999
		std::println("XMScalarCosEst(XM_PIDIV2) : {}", XMScalarCosEst(XM_PIDIV2));	// XMScalarCosEst(XM_PIDIV2) : -1.1920929e-07

		// float XMScalarModAngle(float value) will return value % 2PI -> this will return value between [-PI, PI].
		std::println("XMScalarModAngle(XM_PIDIV2 * 10) : {}", XMScalarModAngle(XM_PIDIV2 * 10));	// XMScalarModAngle(XM_PIDIV2 * 10) : -3.1415927

		// bool XMScalarNearEqual(float v1, float v2, float epsilon) will return true if v1 == v2.
		// Because of floating point problems, |v1 - v2| <= epsilon is used to compare
		std::println("XMScalarNearEqual(XM_PI, XM_PIDIV2 * 2, FLT_EPSILON) : {}", XMScalarNearEqual(XM_PI, XM_PIDIV2 * 2, FLT_EPSILON));	// XMScalarNearEqual(XM_PI, XM_PIDIV2 * 2, FLT_EPSILON) : true

		// There are trigonometric functions for XMVECTORs.
		// For example) XMVectorCos() will return { cos(x), cos(y), cos(z), cos(w) }.

		XMVECTOR v = XMVectorSet(XM_PI, XM_2PI, XM_PIDIV2, XM_1DIV2PI);
		Utils::PrintXMVector(v, "v");

		Utils::PrintXMVector(XMVectorCos(v), "XMVectorCos(v)");			// XMVectorCos(v) : { -1, 1, -6.229855e-08, 0.98736155 }
		Utils::PrintXMVector(XMVectorSin(v), "XMVectorSin(v)");			// XMVectorSin(v) : { 0, 0, 1, 0.15848388 }
		Utils::PrintXMVector(XMVectorCosEst(v), "XMVectorCosEst(v)");	// XMVectorCosEst(v) : { -1, 1, -6.229855e-08, 0.9873633 }
		Utils::PrintXMVector(XMVectorSinEst(v), "XMVectorSinEst(v)");	// XMVectorSinEst(v) : { 0, 0, 1, 0.15848391 }
		Utils::PrintXMVector(XMVectorACos(v), "XMVectorACos(v)");		// XMVectorACos(v) : { 0, -0, 0, 1.4109616 }							: ACos : arccos value
		Utils::PrintXMVector(XMVectorASin(v), "XMVectorASin(v)");		// XMVectorASin(v) : { 1.5707964, 1.5707964, 1.5707964, 0.15983474 }	: ASin : arcsin value 

		// void XMVectorSinCos(XMVECTOR* pSin, XMVECTOR* pCos, XMVECTOR v) can compute sin and cose at once.
		XMVECTOR sinValue = {}, cosValue = {};
		XMVectorSinCos(&sinValue, &cosValue, v);
		Utils::PrintXMVector(sinValue, "sin result of XMVectorSinCos(sin, cos, v)");	// sin result of XMVectorSinCos(sin, cos, v) : { 0, 0, 1, 0.15848388 }
		Utils::PrintXMVector(cosValue, "cos result of XMVectorSinCos(sin, cos, v)");	// cos result of XMVectorSinCos(sin, cos, v) : { -1, 1, -6.229855e-08, 0.98736155 }

	}

	std::println();

	// 2. Vector Arithmetic Operation Functions
	{
		std::println("2. Vector Arithmetic Operation Functions");

		// DirectXMath provides various arithmetic operations for XMVECTOR and XMMATRIX.
		// For now, we will take a look XMVECTOR's arithmetic functions.

		XMVECTOR v1 = XMVectorSet(-1.f, -2.f, 3.f, 4.f);
		XMVECTOR v2 = XMVectorSet(-5.f, 6.f, -7.f, 8.f);
		Utils::PrintXMVector(v1, "v1");
		Utils::PrintXMVector(v2, "v2");

		// XMVECTOR XMVectorAbs(XMVECTOR v) will returns absolute value of each element.
		Utils::PrintXMVector(XMVectorAbs(v1), "XMVectorAbs(v1)");	// XMVectorAbs(v1) : { 1, 2, 3, 4 }

		// XMVECTOR XMVectorAdd(XMVECTOR v1, XMVECTOR v2) will returns XMVECTOR contains sum of each element.
		// XMVECTOR XMVectorSubtract(XMVECTOR v1, XMVECTOR v2) will returns XMVECTOR contains subtract of each element.
		// XMVECTOR XMVectorMultiply(XMVECTOR v1, XMVECTOR v2) will returns XMVECTOR contains multiply of each element.
		// XMVECTOR XMVectorDivide(XMVECTOR v1, XMVECTOR v2) will returns XMVECTOR contains division of each element.
		// XMVECTOR XMVectorMod(XMVECTOR v1, XMVECTOR v2) will returns XMVECTOR contains remainder (v1 % v2).
		Utils::PrintXMVector(XMVectorAdd(v1, v2), "XMVectorAdd(v1, v2)");				// XMVectorAdd(v1, v2) : { -6, 4, -4, 12 }
		Utils::PrintXMVector(XMVectorSubtract(v1, v2), "XMVectorSubtract(v1, v2)");		// XMVectorSubtract(v1, v2) : { 4, -8, 10, -4 }
		Utils::PrintXMVector(XMVectorMultiply(v1, v2), "XMVectorMultiply(v1, v2)");		// XMVectorMultiply(v1, v2) : { 5, -12, -21, 32 }
		Utils::PrintXMVector(XMVectorDivide(v1, v2), "XMVectorDivide(v1, v2)");			// XMVectorDivide(v1, v2) : { 0.2, -0.33333334, -0.42857143, 0.5 }
		Utils::PrintXMVector(XMVectorMod(v1, v2), "XMVectorMod(v1, v2)");				// XMVectorMod(v1, v2) : { -1, -2, 3, 4 }

		// XMVECTOR XMVectorScale(XMVECTOR v, float s) will returns scalar multiply of v
		// XMVECTOR XMVectorNegate(XMVECTOR v) will returns v * -1
		Utils::PrintXMVector(XMVectorScale(v1, 5), "XMVectorScale(v1, 5)");		// XMVectorScale(v1, 5) : { -5, -10, 15, 20 }
		Utils::PrintXMVector(XMVectorNegate(v1), "XMVectorNegate(v1)");			// XMVectorNegate(v1) : { 1, 2, -3, -4 }

		// XMVECTOR XMVectorSqrt(XMVECTOR v) will returns square-root of each elements.
		// This one has XMVectorSqrtEst(XMVECTOR v) version, which returns approximate value.
		Utils::PrintXMVector(XMVectorSqrt(v1), "XMVectorSqrt(v1)");			// XMVectorSqrt(v1) : { -nan, -nan, 1.7320508, 2 }		(sqrt of negative value is nan)
		Utils::PrintXMVector(XMVectorSqrtEst(v1), "XMVectorSqrtEst(v1)");	// XMVectorSqrtEst(v1) : { -nan, -nan, 1.7320508, 2 }

		// XMVECTOR XMVectorMultiplyAdd(XMVECTOR v1, XMVECTOR v2, XMVECTOR v3) will return (v1 * v2) + v3 of each elements.
		// result r will be { x1x2 + x3, y1y2 + y3, z1z2 + z3, w1w2 + w3 }
		Utils::PrintXMVector(XMVectorMultiplyAdd(v1, v2, v1), "XMVectorMultiplyAdd(v1, v2, v1)");	// XMVectorMultiplyAdd(v1, v2, v1) : { 4, -14, -18, 36 }

		// XMVECTOR XMVectorClamp(XMVECTOR v, XMVECTOR min, XMVECTOR max) will returns clamped XMVECTOR between [ elementof(min), elementof(max) ].
		// XMVECTOR XMVectorSaturate(XMVECTOR v) will returns clamped XMVECTOR between [0, 1].
		Utils::PrintXMVector(XMVectorSaturate(v1), "XMVectorSaturate(v1)");		// XMVectorSaturate(v1) : { 0, 0, 1, 1 }
		Utils::PrintXMVector(XMVectorClamp(v1, XMVectorSet(-1.f, -1.f, -1.f, -1.f), XMVectorSet(1.f, 1.f, 1.f, 1.f)), "XMVectorClamp(v1, XMVectorSet(-1.f, -1.f, -1.f, -1.f), XMVectorSet(1.f, 1.f, 1.f, 1.f))");	
		// XMVectorClamp(v1, XMVectorSet(-1.f, -1.f, -1.f, -1.f), XMVectorSet(1.f, 1.f, 1.f, 1.f)) : { -1, -1, 1, 1 }
		
		// XMVECTOR XMVectorAddAngles(XMVECTOR v1, XMVECTOR v2) will returns sum of v1 and v2.
		// Diffrece between XMVectorAdd() is XMVectorAddAngles() returns value between [-PI, PI)
		Utils::PrintXMVector(XMVectorAdd(v1, v2), "XMVectorAdd(v1, v2)");				// XMVectorAdd(v1, v2) : { -6, 4, -4, 12 }
		Utils::PrintXMVector(XMVectorAddAngles(v1, v2), "XMVectorAddAngles(v1, v2)");	// XMVectorAddAngles(v1, v2) : { 0.28318548, -2.2831855, 2.2831855, 5.7168145 }
		
		v1 = XMVectorSet(1.2f, -2.4f, 3.6f, -4.8f);
		v2 = XMVectorSet(-1.2f, 2.4f, -3.6f, 4.8f);
		std::println("v1, v2 changed");
		Utils::PrintXMVector(v1, "v1");
		Utils::PrintXMVector(v2, "v2");

		// XMVECTOR XMVectorCeiling(XMVECTOR v) will returns ceiling value of each elements.
		// XMVECTOR XMVectorFloor(XMVECTOR v) will returns floor value of each elements.
		Utils::PrintXMVector(XMVectorCeiling(v1), "XMVectorCeiling(v1)");	// XMVectorCeiling(v1) : { 2, -2, 4, -4 }
		Utils::PrintXMVector(XMVectorFloor(v1), "XMVectorFloor(v1)");		// XMVectorFloor(v1) : { 1, -3, 3, -5 }
		
		// XMVECTOR XMVectorMin(XMVECTOR v1, XMVECTOR v2) will returns XMVECTOR contains min value of each elements.
		// XMVECTOR XMVectorMax(XMVECTOR v1, XMVECTOR v2) will returns XMVECTOR contains max value of each elements.
		Utils::PrintXMVector(XMVectorMin(v1, v2), "XMVectorMin(v1, v2)");  // XMVectorMin(v1, v2) : { -1.2, -2.4, -3.6, -4.8 }
		Utils::PrintXMVector(XMVectorMax(v1, v2), "XMVectorMax(v1, v2)");  // XMVectorMax(v1, v2) : { 1.2, 2.4, 3.6, 4.8 }

		// XMVECTOR XMVectorReciprocal(XMVECTOR v) will returns reciprocal value of each elements. Est version is option.
		// XMVECTOR XMVectorReciprocalSqrt(XMVECTOR v) will returns reciprocal value of sqrt(elements). Est version is option.
		Utils::PrintXMVector(XMVectorReciprocal(v1), "XMVectorReciprocal(v1)");					// XMVectorReciprocal(v1) : { 0.8333333, -0.41666666, 0.2777778, -0.20833333 }
		Utils::PrintXMVector(XMVectorReciprocalEst(v1), "XMVectorReciprocalEst(v1)");			// XMVectorReciprocalEst(v1) : { 0.833374, -0.416687, 0.277771, -0.2083435 }
		Utils::PrintXMVector(XMVectorReciprocalSqrt(v1), "XMVectorReciprocalSqrt(v1)");			// XMVectorReciprocalSqrt(v1) : { 0.9128709, -nan, 0.5270463, -nan }
		Utils::PrintXMVector(XMVectorReciprocalSqrtEst(v1), "XMVectorReciprocalSqrtEst(v1)");	// XMVectorReciprocalSqrtEst(v1) : { 0.91296387, -nan, 0.52697754, -nan }

		// XMVECTOR XMVectorRound(XMVECTOR v) will returns rounded value of each elements.
		// XMVECTOR XMVectorTruncate(XMVECTOR v) will returns int(remove decimal) value of each elements.
		Utils::PrintXMVector(XMVectorRound(v1), "XMVectorRound(v1)");			// XMVectorRound(v1) : { 1, -2, 4, -5 }
		Utils::PrintXMVector(XMVectorTruncate(v1), "XMVectorTruncate(v1)");		// XMVectorTruncate(v1) : { 1, -2, 3, -4 }

		// XMVECTOR XMVectorModAngles(XMVECTOR v) returns remainder from devided by 2PI. (v represent radian angles)
		Utils::PrintXMVector(XMVectorModAngles(v1), "XMVectorModAngles(v1)");	// XMVectorModAngles(v1) : { 1.2, -2.4, -2.6831856, 1.4831853 }

		// XMVECTOR XMVectorIsInfinite(XMVECTOR v) will check if each element is inf and return as vector.
		// XMVECTOR XMVectorIsNaN(XMVECTOR v) will check if each element is nan and return as vector.
		Utils::PrintXMVector(XMVectorIsInfinite(v1), "XMVectorIsInfinite(v1)");						// XMVectorIsInfinite(v1) : { 0, 0, 0, 0 }
		Utils::PrintXMVector(XMVectorIsNaN(XMVectorSqrt(v1)), "XMVectorIsNaN(XMVectorSqrt(v1))");	// XMVectorIsNaN(XMVectorSqrt(v1)) : { 0, -nan, 0, -nan }

	}

	std::println();

	// 3. Vector Comparison Functions
	{
		std::println("3. Vector Comparison Functions");
		// 3-1. Comparing Vectors
		// Like XMColor... Comparison functions, XMVector... Comparison functions is also provided by DirectXMath.
		// This time, We will take a look about some special comparison functions.

		std::println("3-1. Comparing Vectors");

		XMVECTOR v1 = XMVectorSet(1.2f, 2.4f, 3.6f, 4.8f);
		XMVECTOR v2 = XMVectorScale(v1, 2.f);
		Utils::PrintXMVector(v1, "v1");
		Utils::PrintXMVector(v2, "v2");

		// "XMVectorEqual(XMVECTOR v1, XMVECTOR v2)" vs "XMVectorNearEqual(XMVECTOR v1, XMVECTOR v1, float epsilon)"
		// XMVectorEqual is exact comparison between v1, v2. floating-point error will not be consider.
		// XMVectorNearEqual is comparison considering floating-point error. You can use designate epsilon value
		// Return value is not bool value. It will be computed like this:
		// rx = (x1 == x2) ? 0xffffffff : 0
		// ry = (y1 == y2) ? 0xffffffff : 0
		// rz = (z1 == z2) ? 0xffffffff : 0
		// rw = (w1 == w2) ? 0xffffffff : 0
		// I tried to make floating-point error in example below but XM functions is quite precise...

		Utils::PrintXMVector(XMVectorScale(v2, 0.5f), "XMVectorScale(v2, 0.5f))");
		Utils::PrintXMVector(XMVectorEqual(v1, v2), "XMVectorEqual(v1, v2)");		// XMVectorEqual(v1, v2) : { 0, 0, 0, 0 }
		Utils::PrintXMVector(XMVectorEqual(v1, XMVectorScale(v2, 0.5f)), "XMVectorEqual(v1, XMVectorScale(v2, 0.5f))");		// XMVectorEqual(v1, XMVectorScale(v2, 0.5f)) : { -nan, -nan, -nan, -nan }
		Utils::PrintXMVector(XMVectorNearEqual(v1, XMVectorScale(v2, 0.5f), XMVectorReplicate(FLT_EPSILON)), "XMVectorNearEqual(v1, XMVectorScale(v2, 0.5f), XMVectorReplicate(FLT_EPSILON))");	   
		// XMVectorNearEqual(v1, XMVectorScale(v2, 0.5f), XMVectorReplicate(FLT_EPSILON)) : { -nan, -nan, -nan, -nan }

		// XMVector...R() functions are something special. Like XMVectorEqualR(uint32_t *pCR, XMVECTOR v1, XMVECTOR v2)
		// ...R() series functions will return "XM_CRMASK_CR6TRUE" in pCR output parameter if every component of result if 0xffffffff.
		// Else, return XM_CRMASK_CR6FALSEin pCR output parameter.
		// There are special functions for check pCR value
		// bool XMComparisonAllTrue(uint32_t CR) will return true when CR is XM_CRMASK_CR6TRUE.
		// bool XMComparisonAllFalse(uint32_t CR) will return true when CR is XM_CRMASK_CR6FALSE.
		// bool XMComparisonAnyTrue(uint32_t CR) will return true when CR is not XM_CRMASK_CR6FALSE.
		// bool XMComparisonAnyFalse(uint32_t CR) will return true when CR is not XM_CRMASK_CR6TRUE.
		// So, ...R() functions will be use like this:

		uint32_t CR;
		XMVectorEqualR(&CR, v1, XMVectorScale(v2, 0.5f));
		std::cout << std::boolalpha << "XMComparisonAllTrue(CR) : " << XMComparisonAllTrue(CR) << '\n';	// XMComparisonAllTrue(CR) : true

		// Other comparison is same as above.
		// Such as : XMVectorGreator() -> XMVectorGreatorR() / XMVectorLessOrEqual() -> XMVectorLessOrEqualR() / ...

		std::println();

		// 3-2. Comparing 3D Vectors
		// Compare functions for 3D Vectors are similar to the above.
		// 3D Vector comparison will compare vectors except w-element.
		// ...R() series comparison functions for 3D vectors are return CRMASK directly
		// Like this : uint32_t XMVector3EqualR(XMVECTOR v1, XMVECTOR v2)

		std::println("3-2. Comparing 3D Vectors");

		v1 = XMVectorSet(1.f, 2.f, 3.f, 4.f);
		v2 = XMVectorSet(1.f, 2.f, 3.f, 100.f);
		std::println("v1, v2 changed");
		Utils::PrintXMVector(v1, "v1");
		Utils::PrintXMVector(v2, "v2");

		std::cout << "XMComparisonAllTrue(XMVector3EqualR(v1, v2)) : " << XMComparisonAllTrue(XMVector3EqualR(v1, v2)) << '\n';	// XMComparisonAllTrue(XMVector3EqualR(v1, v2)) : true
	}

	std::println();

	// 4. Vector Geometric Functions
	{
		std::println("4. Vector Geometric Functions");

		XMVECTOR v1 = XMVectorSet(1.2f, 2.4f, 3.6f, 4.8f);
		XMVECTOR v2 = XMVectorScale(v1, 1.5f);
		XMVECTOR v3 = XMVectorScale(v2, 2.f);
		Utils::PrintXMVector(v1, "v1");
		Utils::PrintXMVector(v2, "v2");
		Utils::PrintXMVector(v3, "v3");

		// XMVECTOR XMVectorBaryCentric(XMVECTOR p0, p1, p2, float f, float g) will return point on plane whic contains triangle(composed with p0, p1, p2)
		// XMVECTOR XMVectorBaryCentricV(XMVECTOR p0, p1, p2, XMVECTOR f, g) is same as above but Barycentric coordiantes is represented by vectors.
		// point on plane can be represented by Barycentric coordinates(f, g).
		// t must be in between [0, 1]
		// Return value q will be compute as : p0 + f(p1 - p0) + g(p2 - p0).
		// Look at below :
		//
		//	         p1 --- p2
		//	         |     /
		//	         |    /
		// f*(p1-p0) ↑   ↗ g*(p2 - p0)
		//	         |  /
		//	         | /z
		//	         p0
		//

		Utils::PrintXMVector(XMVectorBaryCentric(v1, v2, v3, 0.2, 0.3), "XMVectorBaryCentric(v1, v2, v3, 0.2, 0.3)");	// XMVectorBaryCentric(v1, v2, v3, 0.2, 0.3) : { 2.0400002, 4.0800004, 6.12, 8.160001 }
		Utils::PrintXMVector(XMVectorBaryCentricV(v1, v2, v3, XMVectorReplicate(0.2), XMVectorReplicate(0.3)), "XMVectorBaryCentricV(v1, v2, v3, XMVectorReplicate(0.2), XMVectorReplicate(0.3))");
		// XMVectorBaryCentricV(v1, v2, v3, XMVectorReplicate(0.2), XMVectorReplicate(0.3)) : { 2.0400002, 4.0800004, 6.12, 8.160001 }

		// Special conditions of XMVectorBaryCentric() :
		// if (f <= 0) && (g >= 0) && (f + g <= 1) : q is in triangle
		// if (f == 0) && (g >= 0) && (f + g <= 1) : q is on line (p0,p2) 
		// if (f >= 0) && (g == 0) && (f + g <= 1) : q is on line (p0,p1) 
		// if (f >= 0) && (g >= 0) && (f + g == 1) : q is on line (p1,p2) 


		// XMVECTOR XMVectorLerp(XMVECTOR v0, XMVECTOR v1, float t) will return linear interpolated result between [v0, v1] using parameter t.
		// t must be in between [0, 1]
		Utils::PrintXMVector(XMVectorLerp(v1, v2, 0.5f), "XMVectorLerp(v1, v2, 0.5f)");	// XMVectorLerp(v1, v2, 0.5f) : { 1.5, 3, 4.5, 6 } -> this is center of line(v1, v2)

		// XMVECTOR XMVectorHeimite(XMVECTOR p0, XMVECTOR tangent0, XMVECTOR p1, XMVECETOR tangent1, float t)
		// This will return one point on Heimite Spline created by p0, p1, tangent0, tangent1.
		// Return value q(t) will be compute like this : (2t^3 - 3t^2 + 1)p0 + (t^3 - 2t^2 + t)t0 + (-2t^3 + 3t^2)p1 + (t^3 - t^2)t1
		// t must be in between [0, 1]

		// XMVECTOR XMVectorCatmullRom(XMVECTOR p0, XMVECTOR p1, XMVECTOR p2, XMVECTOR p3, float t)
		// This will return one point on Catmull-Rom Spline created by p0, p1, p2, p3.
		// Return value q(t) will be compute like this : (-1/2t^3 + t^2 - 1/2t)p0 + (3/2t^3 - 5/2t^2 + 1)p1 + (-3/2t^3 + 2t^2 + 1/2t)p2 + (1/2t^3 - 1/2t^2)p3
		// t must be in between [0, 1]

		Utils::PrintXMVector(XMVectorHermite(v1, XMVectorReplicate(1.f), v2, XMVectorReplicate(-1.f), 0.5f), "XMVectorHermite(v1, XMVectorReplicate(1.f), v2, XMVectorReplicate(-1.f), 0.5f)");
		Utils::PrintXMVector(XMVectorCatmullRom(v1, v2, v3, XMVectorReplicate(0.f), 0.5f), "XMVectorCatmullRom(v1, v2, v3, XMVectorReplicate(0.f), 0.5f)");
		// XMVectorHermite(v1, XMVectorReplicate(1.f), v2, XMVectorReplicate(-1.f), 0.5f) : { 1.75, 3.25, 4.75, 6.25 }
		// XMVectorCatmullRom(v1, v2, v3, XMVectorReplicate(0.f), 0.5f) : { 2.9625, 5.925, 8.8875, 11.85 }

		// XMVECTOR XMVectorInBounds(XMVECTOR v, XMVECTOR bounds) checks point v is in bounds and return as vector
		// Return value will be computed as same as comparison functions.
		// This means XMVectorInBoundsR() is also available. You can use this and check using XMComparison...() functions.
		Utils::PrintXMVector(XMVectorInBounds(v1, XMVectorReplicate(3.f)), "XMVectorInBounds(v1, XMVectorReplicate(3.f))");	// XMVectorInBounds(v1, XMVectorReplicate(3.f)) : { -nan, -nan, 0, 0 } <- true ture false false

		// There are special comarison functions for XMVectorInBoundsR() : bool XMComparisonAllInBounds(uint32_t CR) / bool XMComparisonAnyInBounds(uint32_t CR)
		uint32_t CR = 0;
		XMVectorInBoundsR(&CR, v1, XMVectorReplicate(3.f));
		std::println("v1 is in bound(3.f,3.f,3.f,3.f)? : {}", XMComparisonAllInBounds(CR) ? "TRUE" : "FALSE");							 // v1 is in bound(3.f,3.f,3.f,3.f)? : FALSE
		std::println("any of v1 elements are out of bound(3.f,3.f,3.f,3.f)? : {}", XMComparisonAnyOutOfBounds(CR) ? "TRUE" : "FALSE");	 // any of v1 elements are out of bound(3.f,3.f,3.f,3.f)? : TRUE
	}

	std::println();

	// 5. 3D Vector Transformation Functions
	{
		std::println("5. 3D Vector Transformation Functions");

		XMVECTOR v1 = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR v2 = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
		XMVECTOR v3 = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
		XMVECTOR v4 = XMVectorSet(5.0f, 5.0f, 5.0f, 5.0f);
		Utils::PrintXMVector(v1, "v1");
		Utils::PrintXMVector(v2, "v2");
		Utils::PrintXMVector(v3, "v3");
		Utils::PrintXMVector(v4, "v3");

		// The following functions are about 3D vector transformation functions using matrix or quaternion.
		// If functions input is point vector, w-element will be turn into 1.
		// If functions input is direction(normal) vector, w-element will be turn into 0;

		// XMVECTOR XMVector3Transform(XMVECTOR v, XMMATRIX m) returns result of v * m.
		// This will never convert to 3D homogeneous coordinates.
		Utils::PrintXMVector(XMVector3Transform(v4, XMMatrixTranslation(1.f, 2.f, 3.f)), "XMVector3Transform(v4, XMMatrixTranslation(1.f, 2.f, 3.f))");	// XMVector3Transform(v4, XMMatrixTranslation(1.f, 2.f, 3.f)) : { 6, 7, 8, 1 }

		// XMVECTOR XMVector3TransformCoord(XMVECTOR v, XMMATRIX m) makes v's w-element into 1 and homonize result of (v * m)
		// return value r will be always looks like { rx, ry, rz, 1 }
		Utils::PrintXMVector(XMVector3TransformCoord(v4, XMMatrixTranslation(1.f, 2.f, 3.f)), "XMVector3TransformCoord(v4, XMMatrixTranslation(1.f, 2.f, 3.f)");	// XMVector3TransformCoord(v4, XMMatrixTranslation(1.f, 2.f, 3.f) : { 6, 7, 8, 1 }

		// XMVECTOR XMVector3TransformNormal(XMVECTOR v, XMMATRIX m) makes v's w-element into 0 and return result of (v * m)
		// To transform normal vector, transformation matrix must be transposed inverse matrix.
		Utils::PrintXMVector(XMVector3TransformNormal(v4, XMMatrixTranslation(1.f, 2.f, 3.f)), "XMVector3TransformNormal(v4, XMMatrixTranslation(1.f, 2.f, 3.f))");	// XMVector3TransformNormal(v4, XMMatrixTranslation(1.f, 2.f, 3.f)) : { 5, 5, 5, 0 }

		// XMFLOAT3 XMVector3TransformCoordStream(XMFLOAT3* pOutputStream, size_t outputStride, XMFLOAT3* pInputStream, size_t inputStride, size_t vectorCount, XMMATRIX m)
		// XMFLOAT3 XMVector3TransformNormalStream(XMFLOAT3* pOutputStream, size_t outputStride, XMFLOAT3* pInputStream, size_t inputStride, size_t vectorCount, XMMATRIX m)
		// This two above is same as XMVector3TransfromCoord() and XMVector3TransformNormal(), but read array(stream) of vector and return as array.

		std::vector<XMFLOAT3> inputStream = {};
		std::vector<XMFLOAT3> outputCoord(4);
		std::vector<XMFLOAT3> outputNormal(4);

		XMFLOAT3 store1 = {}, store2 = {}, store3 = {}, store4 = {};
		XMStoreFloat3(&store1, v1); inputStream.push_back(store1);
		XMStoreFloat3(&store2, v2); inputStream.push_back(store2);
		XMStoreFloat3(&store3, v3); inputStream.push_back(store3);
		XMStoreFloat3(&store4, v4); inputStream.push_back(store4);

		XMVector3TransformCoordStream(outputCoord.data(), sizeof(XMFLOAT3), inputStream.data(), sizeof(XMFLOAT3), inputStream.size(), XMMatrixTranslation(1.f, 2.f, 3.f));
		XMVector3TransformNormalStream(outputNormal.data(), sizeof(XMFLOAT3), inputStream.data(), sizeof(XMFLOAT3), inputStream.size(), XMMatrixTranslation(1.f, 2.f, 3.f));

		for (int i = 0; i < outputCoord.size(); i++) Utils::PrintXMVector(XMLoadFloat3(&outputCoord[i]), "XMVectorTransformCoordStream's result[" + std::to_string(i) + "]");
		for (int i = 0; i < outputCoord.size(); i++) Utils::PrintXMVector(XMLoadFloat3(&outputCoord[i]), "XMVector3TransformNormalStream's result[" + std::to_string(i) + "]");
		/*
			XMVectorTransformCoordStream's result[0] : { 2, 2, 3, 0 }
			XMVectorTransformCoordStream's result[1] : { 1, 3, 3, 0 }
			XMVectorTransformCoordStream's result[2] : { 1, 2, 4, 0 }
			XMVectorTransformCoordStream's result[3] : { 6, 7, 8, 0 }
			XMVector3TransformNormalStream's result[0] : { 2, 2, 3, 0 }
			XMVector3TransformNormalStream's result[1] : { 1, 3, 3, 0 }
			XMVector3TransformNormalStream's result[2] : { 1, 2, 4, 0 }
			XMVector3TransformNormalStream's result[3] : { 6, 7, 8, 0 }
		*/

		// XMVECTOR XMVector3Rotate(XMVECTOR v, XMVECTOR quaternion) returns rotated v using quaternion.
		// XMVECTOR XMVector3InverseRotate(XMVECTOR v, XMVECTOR quaternion) returns inverse-rotated v using quaternion.

		Utils::PrintXMVector(XMVector3Rotate(v1, XMQuaternionRotationRollPitchYaw(XM_PIDIV4, XM_PIDIV2, XM_PI)), "XMVector3Rotate(v1, XMQuaternionRotationRollPitchYaw(XM_PIDIV4, XM_PIDIV2, XM_PI))");
		Utils::PrintXMVector(XMVector3InverseRotate(v1, XMQuaternionRotationRollPitchYaw(XM_PIDIV4, XM_PIDIV2, XM_PI)), "XMVector3InverseRotate(v1, XMQuaternionRotationRollPitchYaw(XM_PIDIV4, XM_PIDIV2, XM_PI))");
		// XMVector3Rotate(v1, XMQuaternionRotationRollPitchYaw(XM_PIDIV4, XM_PIDIV2, XM_PI)) : { -2.9802322e-08, -8.940697e-08, 0.9999999, 0 }
		// XMVector3InverseRotate(v1, XMQuaternionRotationRollPitchYaw(XM_PIDIV4, XM_PIDIV2, XM_PI)) : { -2.9802322e-08, -0.7071067, 0.7071067, 0 }


		// XMVECTOR XMVector3Project(XMVECTOR v, float viewportX, float viewportY, float viewportWidth, float viewportHeight, float viewportMinZ, float viewportMAxZ, XMMATRIX projection, XMMATRIX view, XMMATRIX world)
		// XMVECTOR XMVector3UnProject(XMVECTOR v, float viewportX, float viewportY, float viewportWidth, float viewportHeight, float viewportMinZ, float viewportMAxZ, XMMATRIX projection, XMMATRIX view, XMMATRIX world)
		// This two functions above are return Projects - Unprojects v
		// The result of XMVector3Projection will be screen-space translated v (orginal v must be in model-space).
		// The result of XMVector3UnProject will be model-space translated v (original v must be in screen-space).

		// We will assume that viewportX, Y will be 0, 0 / width, height = 1920 * 1080 / minZ, maxZ = 1, 100 / camara is in origin of world and looking forward(+z) / fov = 90 / 
		
		XMMATRIX world = XMMatrixMultiply(XMMatrixRotationRollPitchYaw(XM_PIDIV4, XM_PIDIV2, XM_PI), XMMatrixTranslation(1.f, 2.f, 3.f));
		XMMATRIX view = XMMatrixLookAtLH(XMVectorReplicate(0.f), XMVectorSet(0.f, 0.f, 1.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 1.f));
		XMMATRIX proj = XMMatrixPerspectiveFovLH(90.f, 1920 / 1080, 10, 100);

		XMVECTOR v1Projected = XMVector3Project(v1, 0.f, 0.f, 1920, 1080, 10, 100, proj, view, world);
		XMVECTOR v1UnProjected = XMVector3Unproject(v1Projected, 0.f, 0.f, 1920, 1080, 10, 100, proj, view, world);

		Utils::PrintXMVector(v1Projected, "v1Projected");		// v1Projected : { 1108.1696, 373.30923, -140, 0 }
		Utils::PrintXMVector(v1UnProjected, "v1UnProjected");	// v1UnProjected : { 1.0000001, -9.744818e-08, 2.1665748e-07, 1 }

		// XMFLOAT3* XMVector3ProjectStream() / XMFLOAT3* XMVectorUnprojectStream() is also available.

		std::vector<XMFLOAT3> outputProj(4);
		std::vector<XMFLOAT3> outputUnproj(4);

		XMVector3ProjectStream(outputProj.data(), sizeof(XMFLOAT3), inputStream.data(), sizeof(XMFLOAT3), inputStream.size(), 0, 0, 1920, 1080, 10, 100, proj, view, world);
		XMVector3UnprojectStream(outputUnproj.data(), sizeof(XMFLOAT3), outputProj.data(), sizeof(XMFLOAT3), outputProj.size(), 0, 0, 1920, 1080, 10, 100, proj, view, world);

		for (int i = 0; i < outputProj.size(); i++) Utils::PrintXMVector(XMLoadFloat3(&outputProj[i]), "XMVector3ProjectStream's result[" + std::to_string(i) + "]");
		for (int i = 0; i < outputUnproj.size(); i++) Utils::PrintXMVector(XMLoadFloat3(&outputUnproj[i]), "XMVector3UnprojectStream's result[" + std::to_string(i) + "]");
		/*
			XMVector3ProjectStream's result[0] : { 1108.1696, 373.30923, -140, 0 }
			XMVector3ProjectStream's result[1] : { 1017.86383, 396.32443, -223.33334, 0 }
			XMVector3ProjectStream's result[2] : { 1297.2551, 396.32443, -223.33334, 0 }
			XMVector3ProjectStream's result[3] : { 1034.0848, 751.3251, -15.000001, 0 }
			XMVector3UnprojectStream's result[0] : { 1.0000001, -1.192093e-07, 2.384186e-07, 0 }
			XMVector3UnprojectStream's result[1] : { 8.3679524e-08, 0.9999998, 3.576279e-07, 0 }
			XMVector3UnprojectStream's result[2] : { 1.7178299e-07, -4.4703486e-07, 1.0000006, 0 }
			XMVector3UnprojectStream's result[3] : { 5, 4.9999995, 5.000001, 0 }
		*/

	}

	std::println();

	// 6. Matrix Functions
	{
		std::println("6. Matrix Functions");

		// This time, we will take a look about XMMATRIX functions.
	
		// XMMATRIX XMMatrixSet(float m00, ... float m33) initialze XMMATRIX by value.
		XMMATRIX m1 = XMMatrixSet(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f);
		Utils::PrintXMMatrix(m1, "XMMatrixSet()");
		// { 1, 2, 3, 4 }
		// { 5, 6, 7, 8 }
		// { 9, 10, 11, 12 }
		// { 13, 14, 15, 16 }

		// XMMATRIX XMMatrixIdentity() return 4x4 identity matrix.
		XMMATRIX m2 = XMMatrixIdentity();
		Utils::PrintXMMatrix(m2, "XMMatrixIdentity()");
		// { 1, 0, 0, 0 }
		// { 0, 1, 0, 0 }
		// { 0, 0, 1, 0 }
		// { 0, 0, 0, 1 }

		// bool XMMatrixIsIdentity(XMMATRIX m) return true if m is identity matrix.
		// bool XMMatrixIsInfinite(XMMATRIX m) returns true if any of m's elements is inf.
		// bool XMMatrixIsNaN(XMMATRIX m) return true if any of m's elements is NaN.

		std::println("XMMatrixIsIdentity(m1) : {}", XMMatrixIsIdentity(m1) ? "TRUE" : "FALSE");	// XMMatrixIsIdentity(m1) : FALSE
		std::println("XMMatrixIsIdentity(m2) : {}", XMMatrixIsIdentity(m2) ? "TRUE" : "FALSE");	// XMMatrixIsIdentity(m2) : TRUE
		std::println("XMMatrixIsInfinite(m1) : {}", XMMatrixIsInfinite(m1) ? "TRUE" : "FALSE");	// XMMatrixIsInfinite(m1) : FALSE
		std::println("XMMatrixIsNaN(m1) : {}", XMMatrixIsNaN(m1) ? "TRUE" : "FALSE");			// XMMatrixIsNaN(m1) : FALSE

		// Change m1
		m1 = XMMatrixMultiplyTranspose(XMMatrixMultiply(XMMatrixScaling(2.f, 2.f, 2.f), XMMatrixRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4)), XMMatrixTranslation(1.f, 2.f, 3.f));
		Utils::PrintXMMatrix(m1, "Change m1");
		// { -8.810345e-08, 8.810345e-08, -2, 1 }
		// { -1.4142135, -1.4142135, 0, 2 }
		// { -1.4142135, 1.4142135, 1.245971e-07, 3 }
		// { 0, 0, 0, 1 }

		// XMMATRIX XMMatrixInverse(XMVector* determinant, XMMATRIX m) returns inverse matrix of m
		// XMVECTOR XMMatrixDeterminant(XMMATRIX m) return determinant of m
		XMVECTOR detm1 = XMMatrixDeterminant(m1);
		Utils::PrintXMVector(detm1, "XMMatrixDeterminant(m1)");	// XMMatrixDeterminant(m1) : { 7.9999995, 7.9999995, 7.9999995, 7.9999995 }
		Utils::PrintXMMatrix(XMMatrixInverse(&detm1, m1), "XMMatrixInverse(&detm1, m1)");
		// { -2.2025864e-08, -0.3535534, -0.3535534, 1.7677672 }
		// { 2.2025864e-08, -0.3535534, 0.3535534, -0.35355338 }
		// { -0.5, 5.5993563e-16, 3.1149277e-08, 0.49999994 }
		// { 0, 0, 0, 1 }

		// XMMATRIX XMMatrixTranspose(XMMATRIX m) return 4x4 transposed matrix of m.
		Utils::PrintXMMatrix(XMMatrixTranspose(m1), "XMMatrixTranspose(m1)");
		// { -8.810345e-08, -1.4142135, -1.4142135, 0 }
		// { 8.810345e-08, -1.4142135, 1.4142135, 0 }
		// { -2, 0, 1.245971e-07, 0 }
		// { 1, 2, 3, 1 }


		// XMMATRIX XMMatrixMultiply(XMMATRIX m1, XMMATRIX m2) returns result of m1 * m2.
		// XMMATRIX XMMatrixMultiplyTranspose(XMMATRIX m1, XMMATRIX m2) returns Transposed m1 * m2.
		Utils::PrintXMMatrix(XMMatrixMultiply(m1, XMMatrixTranslation(3.f, 2.f, 1.f)), "XMMatrixMultiply(m1, XMMatrixTranslation(3.f, 2.f, 1.f))");
		Utils::PrintXMMatrix(XMMatrixMultiplyTranspose(m1, XMMatrixTranslation(3.f, 2.f, 1.f)), "XMMatrixMultiplyTranspose(m1, XMMatrixTranslation(3.f, 2.f, 1.f))");
		// { 3, 2, -1, 1 }
		// { 4.5857863, 2.5857863, 2, 2 }
		// { 7.5857863, 7.4142137, 3.0000002, 3 }
		// { 3, 2, 1, 1 }
		// 
		// { 3, 4.5857863, 7.5857863, 3 }
		// { 2, 2.5857863, 7.4142137, 2 }
		// { -1, 2, 3.0000002, 1 }
		// { 1, 2, 3, 1 }

		// XMMATRIX XMMatrixRotationX/Y/Z(float angle) return rotation matrix by angle.
		
		Utils::PrintXMMatrix(XMMatrixRotationX(XM_PIDIV4), "XMMatrixRotationX(XM_PIDIV4)");
		Utils::PrintXMMatrix(XMMatrixRotationY(XM_PIDIV4), "XMMatrixRotationY(XM_PIDIV4)");
		Utils::PrintXMMatrix(XMMatrixRotationZ(XM_PIDIV4), "XMMatrixRotationZ(XM_PIDIV4)");

		// XMMatrixRotationX(XM_PIDIV4) :
		// { 1, 0, 0, 0 }
		// { 0, 0.7071067, 0.70710677, 0 }
		// { 0, -0.70710677, 0.7071067, 0 }
		// { 0, 0, 0, 1 }
		// XMMatrixRotationY(XM_PIDIV4) :
		// { 0.7071067, 0, -0.70710677, 0 }
		// { 0, 1, 0, 0 }
		// { 0.70710677, 0, 0.7071067, 0 }
		// { 0, 0, 0, 1 }
		// XMMatrixRotationZ(XM_PIDIV4) :
		// { 0.7071067, 0.70710677, 0, 0 }
		// { -0.70710677, 0.7071067, 0, 0 }
		// { 0, 0, 1, 0 }
		// { 0, 0, 0, 1 }

		// XMMATRIX XMMatrixRotationRollPitchYaw(float pitch, float yaw, float roll) returns rotation matrix using euler angle.
		// XMMATRIX XMMatrixRotationRollPitchYawFromVector(XMVECTOR Angles) are same with above, but parameter is XMVECTOR.

		Utils::PrintXMMatrix(XMMatrixRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4), "XMMatrixRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4)");
		Utils::PrintXMMatrix(XMMatrixRotationRollPitchYawFromVector(XMVectorSet(XM_PI, XM_PIDIV2, XM_PIDIV4, 0.f)), "XMMatrixRotationRollPitchYawFromVector(XMVectorSet(XM_PI, XM_PIDIV2, XM_PIDIV4, 0.f))");
		// XMMatrixRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4) :
		// { -4.4051724e-08, -0.70710677, -0.70710677, 0 }
		// { 4.4051724e-08, -0.70710677, 0.70710677, 0 }
		// { -1, 0, 6.229855e-08, 0 }
		// { 0, 0, 0, 1 }
		// XMMatrixRotationRollPitchYawFromVector(XMVectorSet(XM_PI, XM_PIDIV2, XM_PIDIV4, 0.f)) :
		// { -4.4051724e-08, -0.70710677, -0.70710677, 0 }
		// { 4.4051724e-08, -0.70710677, 0.70710677, 0 }
		// { -1, 0, 6.229855e-08, 0 }
		// { 0, 0, 0, 1 }

		// XMMATRIX XMMatrixRotationQuaternion(XMVECTOR q) returns rotation matrix using quaternion.

		Utils::PrintXMMatrix(XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4)), "XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4))");
		//XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4)) :
		//{ 5.9604645e-08, -0.7071067, -0.7071067, 0 }
		//{ -1.1920929e-07, -0.7071066, 0.7071067, 0 }
		//{ -0.9999999, 1.1920929e-07, 5.9604645e-08, 0 }
		//{ 0, 0, 0, 1 }

		// XMMATRIX XMMatrixTranslation(float a, float b, float 3) returns translation matrix using a,b,c.
		// XMMATRIX XMMatrixTranslationFromVector(XMVECTOR offset) are same with above, but parameter is XMVECTOR.

		Utils::PrintXMMatrix(XMMatrixTranslation(1.f, 2.f, 3.f), "XMMatrixTranslation(1.f, 2.f, 3.f)");
		Utils::PrintXMMatrix(XMMatrixTranslationFromVector(XMVectorSet(1.f, 2.f, 3.f, 1.f)), "XMMatrixTranslationFromVector(XMVectorSet(1.f, 2.f, 3.f, 1.f))");
		// XMMatrixTranslation(1.f, 2.f, 3.f) :
		// { 1, 0, 0, 0 }
		// { 0, 1, 0, 0 }
		// { 0, 0, 1, 0 }
		// { 1, 2, 3, 1 }
		// XMMatrixTranslationFromVector(XMVectorSet(1.f, 2.f, 3.f, 1.f)) :
		// { 1, 0, 0, 0 }
		// { 0, 1, 0, 0 }
		// { 0, 0, 1, 0 }
		// { 1, 2, 3, 1 }

		// XMMATRIX XMMatrixScaling(float a, float b, float 3) returns Scaling matrix using a,b,c.
		// XMMATRIX XMMatrixScalingFromVector(XMVECTOR offset) are same with above, but parameter is XMVECTOR.

		Utils::PrintXMMatrix(XMMatrixScaling(1.f, 2.f, 3.f), "XMMatrixScaling(1.f, 2.f, 3.f)");
		Utils::PrintXMMatrix(XMMatrixScalingFromVector(XMVectorSet(1.f, 2.f, 3.f, 1.f)), "XMMatrixScalingFromVector(XMVectorSet(1.f, 2.f, 3.f, 1.f))");
		// XMMatrixScaling(1.f, 2.f, 3.f) :
		// { 1, 0, 0, 0 }
		// { 0, 2, 0, 0 }
		// { 0, 0, 3, 0 }
		// { 0, 0, 0, 1 }
		// XMMatrixScalingFromVector(XMVectorSet(1.f, 2.f, 3.f, 1.f)) :
		// { 1, 0, 0, 0 }
		// { 0, 2, 0, 0 }
		// { 0, 0, 3, 0 }
		// { 0, 0, 0, 1 }

		// XMMATRIX XMMatrixAffineTransformation(XMVECTOR scaling, XMVECTOR origin, XMVECTOR rotation, XMVECTOR translation) returns affine transform matrix(SRT)
		Utils::PrintXMMatrix(XMMatrixAffineTransformation(XMVectorReplicate(2.f), XMVectorReplicate(0.f), XMQuaternionRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4), XMVectorSet(1.f, 2.f, 3.f, 1.f)),
			"XMMatrixAffineTransformation(XMVectorReplicate(2.f), XMVectorReplicate(0.f), XMQuaternionRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4), XMVectorSet(1.f, 2.f, 3.f, 1.f))");
		// XMMatrixAffineTransformation(XMVectorReplicate(2.f), XMVectorReplicate(0.f), XMVectorSet(XM_PI, XM_PIDIV2, XM_PIDIV4, 0.f), XMVectorSet(1.f, 2.f, 3.f, 1.f)) :
		// { 1.1920929e-07, -1.4142134, -1.4142134, 0 }
		// { -2.3841858e-07, -1.4142132, 1.4142134, 0 }
		// { -1.9999998, 2.3841858e-07, 1.1920929e-07, 0 }
		// { 1, 2, 3, 1 }

		// XMMATRIX XMMatrixTransformation(XMVECTOR sOrigin, XMVECTOR qScale, XMVECTOR scaling, XMVECTOR rOrigin, XMVECTOR qRotation, XMVECTOR translation)
		// sOrigin : center of scaling / qScale : direction of scaling / scaling : amount of scaling / rOrigin : center of rotation / qRotation : rotation quaternion / translation : amount of translation
		// example skip : i cannot understand

		// XMMATRIX XMMatrixDecompose(XMVECTOR *scaling, XMVECTOR *rotation, XMVECTOR* translation, XMMATRIX m)
		// This Function decomposes m and return scaling, rotation, translation using output parameter
		
		XMMATRIX tr = XMMatrixAffineTransformation(XMVectorReplicate(2.f), XMVectorReplicate(0.f), XMQuaternionRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4), XMVectorSet(1.f, 2.f, 3.f, 1.f));
		XMVECTOR decScaling, decRotation, decTranslation;
		XMMatrixDecompose(&decScaling, &decRotation, &decTranslation, tr);
		Utils::PrintXMVector(decScaling, "Scaling decomposed from previous example matrix");
		Utils::PrintXMVector(decRotation, "Rotation decomposed from previous example matrix");
		Utils::PrintXMVector(decTranslation, "Translation decomposed from previous example matrix");
		// Scaling decomposed from previous example matrix : { 1.9999998, 1.9999996, 1.9999998, 0 }
		// Rotation decomposed from previous example matrix : { -0.65328145, 0.2705981, 0.65328145, -0.270598 }
		// Translation decomposed from previous example matrix : { 1, 2, 3, 1 }


		// XMMATRIX XMMatrixLookAtLH(XMVECTOR eye, XMVECTOR at, XMVECTOR up) returns view matrix in LHS system.
		// XMMATRIX XMMatrixLookToLH(XMVECTOR eye, XMVECTOR at, XMVECTOR up) returns view matrix in LHS system. But 2nd parameter is direction of camera forward (originally at - eye is look vector)
		// RHS system version is XMMatrixLookAtRH() / XMMatrixLookToRH()

		XMVECTOR eye = XMVectorSet(0, 0, 0, 0);
		XMVECTOR at = XMVectorSet(0, 1, 0, 0);
		XMVECTOR up = XMVectorSet(0, 1, 0, 1);
		Utils::PrintXMVector(eye, "eye");
		Utils::PrintXMVector(at, "at");
		Utils::PrintXMVector(up, "up");

		Utils::PrintXMMatrix(XMMatrixLookAtLH(eye, at, up), "XMMatrixLookAtLH(eye, at, up)");
		Utils::PrintXMMatrix(XMMatrixLookToLH(eye, XMVectorSubtract(at, eye), up), "XMMatrixLookToLH(eye, XMVectorSubtract(at, eye), up)");	// result will be same
		// XMMatrixLookAtLH(eye, at, up) :
		// { 0, 0, 0, 0 }
		// { 0, 0, 1, 0 }
		// { 0, 0, 0, 0 }
		// { 0, 0, 0, 1 }
		// XMMatrixLookToLH(eye, XMVectorSubtract(at, eye), up) :
		// { 0, 0, 0, 0 }
		// { 0, 0, 1, 0 }
		// { 0, 0, 0, 0 }
		// { 0, 0, 0, 1 }


		// XMMATRIX XMMatrixPerspectiveFovLH(float fov, float aspect, float zn, float zf) returns projection matrix in LHS system.
		// XMMATRIX XMMatrixPerspectiveLH(float w, float h, float zn, float zf) return projection matrix in LHS system. But parameters are diffrent.
		// RHS system version is XMMatrixPerspectiveFovRH() / XMMatrixPerspectiveRH()

		Utils::PrintXMMatrix(XMMatrixPerspectiveFovLH(90, 1920 / 1080, 10, 100), "XMMatrixPerspectiveFovLH(90, 1920 / 1080, 10, 100)");
		Utils::PrintXMMatrix(XMMatrixPerspectiveLH(1920, 1080, 10, 100), "XMMatrixPerspectiveLH(1920, 1080, 10, 100)");
		// XMMatrixPerspectiveFovLH(90, 1920 / 1080, 10, 100) :
		// { 0.6173732, 0, 0, 0 }
		// { 0, 0.6173732, 0, 0 }
		// { 0, 0, 1.1111112, 1 }
		// { 0, 0, -11.111112, 0 }
		// XMMatrixPerspectiveLH(1920, 1080, 10, 100) :
		// { 0.010416667, 0, 0, 0 }
		// { 0, 0.018518519, 0, 0 }
		// { 0, 0, 1.1111112, 1 }
		// { 0, 0, -11.111112, 0 }


		// XMMATRIX XMMatrixReflect(XMVECTOR plane) returns reflection matrix with plane
		// plane : normal is (a, b, c) and distance from origin is d -> we will see later

		XMVECTOR plane = XMPlaneFromPointNormal(XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(1.f, 0.f, 0.f, 0.f));
		Utils::PrintXMVector(plane, "XMPlaneFromPointNormal(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 0.f, 0.f, 0.f))");
		// XMPlaneFromPointNormal(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 0.f, 0.f, 0.f)) : { 1, 0, 0, 0 }

		// plane will be looks like :
		// 
		//     /|
		//    / |
		//   |  |
		//   |  | ---> normal
		//   |  |
		//   |  |    ↖
		//   |  /      ↖
		//   | /         ↖ {-1.f, 1.f, 0.f, 0.f } -> We will test this. Result expected as { 1.f, 1.f, 0.f, 0.f }
		//   |/
		//

		Utils::PrintXMMatrix(XMMatrixReflect(plane), "XMMatrixReflect(plane)");
		Utils::PrintXMVector(XMVector3Transform(XMVectorSet(-1.f, 1.f, 0.f, 0.f), XMMatrixReflect(plane)), "{ -1.f, 1.f, 0.f, 0.f } reflected");
		// XMMatrixReflect(plane) :
		// { -1, 0, 0, 0 }
		// { 0, 1, 0, 0 }
		// { 0, 0, 1, 0 }
		// { 0, 0, 0, 1 }
		// { -1.f, 1.f, 0.f, 0.f } reflected: { 1, 1, 0, 1 }


		// XMMATRIX XMMatrixShadow(XMVECTOR plane, XMVECTOR light) returns result of light projected to plane (planar projection matrix)
		// This one also called Shadow matrix.

		XMVECTOR light = XMVectorSet(1.f, -1.f, 0.f, 0.f);
		XMVECTOR plane2 = XMPlaneFromPointNormal(XMVectorReplicate(0.f), XMVectorSet(0.f, 1.f, 0.f, 0.f));
		Utils::PrintXMVector(light, "light");
		Utils::PrintXMVector(plane2, "XMPlaneFromPointNormal(XMVectorReplicate(0.f), XMVectorSet(0.f, 1.f, 0.f, 0.f))");

		// What will be look like :
		//
		//   ↘ light : { 1.f, -1.f, 0.f, 0.f }
		//     ↘
		//       ↘  • <- { 0.f, 2.f, 0.f, 1.f } -> let's project this to plane
		//     ________________
		//    /               /
		//   /               /
		//   ----------------
		//

		Utils::PrintXMMatrix(XMMatrixShadow(plane2, light), "XMMatrixShadow(plane2, light)");
		Utils::PrintXMVector(XMVector3Transform(XMVectorSet(0.f, 2.f, 0.f, 1.f), XMMatrixShadow(plane2, light)), "XMVector3Transform(XMVectorSet(0.f, 2.f, 0.f, 1.f), XMMatrixShadow(plane2, light)");
		// XMMatrixShadow(plane2, light) :
		// { -1, 0, 0, 0 }
		// { -1, 0, 0, 0 }
		// { 0, 0, -1, 0 }
		// { 0, 0, 0, -1 }
		// XMVector3Transform(XMVectorSet(0.f, 2.f, 0.f, 1.f), XMMatrixShadow(plane2, light) : { -2, 0, 0, -1 }

	}

	std::println();
	
	// 7. Plane Functions
	{
		std::println("7. Plane Functions");

		// DirectXMath provides various functions about plane.
		// Planes can be represented as ax + by + cz + d = 0. This mean point on the plane { x, y, z } satisfiy ax + by + cz + d = 0.
		// { a, b, c } means normal vector of plane and d means distance from origin.
		// So, planes can be represented as {a, b, c, d} 4D vector.

		// To Create Plane, you can use this two functions.
		// XMVECTOR XMPlaneFromPoints(XMVECTOR p0, XMVECTOR p1, XMVECTOR p2) creates plane using 3 points on the plane.
		// XMVECTOR XMPlaneFromPointNormal(XMVECTOR p, XMVECTOR normal) creates plane using one point and normal vector.

		XMVECTOR plane1 = XMPlaneFromPoints(XMVectorSet(1, 1, 1, 1), XMVectorSet(1, 2, 1, 1), XMVectorSet(3, 1, 2, 1));
		XMVECTOR plane2 = XMPlaneFromPointNormal(XMVectorReplicate(1.f), XMVectorSet(1.f, 1.f, 1.f, 0.f));
		Utils::PrintXMVector(plane1, "plane1 initialized : XMPlaneFromPoints(XMVectorSet(1, 1, 1, 1), XMVectorSet(1, 2, 1, 1), XMVectorSet(3, 1, 2, 1))");
		Utils::PrintXMVector(plane2, "plane2 initialized : XMPlaneFromPointNormal(XMVectorReplicate(1.f), XMVectorSet(1.f, 1.f, 1.f, 0.f))");
		// plane1 initialized : XMPlaneFromPoints(XMVectorSet(1, 1, 1, 1), XMVectorSet(1, 2, 1, 1), XMVectorSet(3, 1, 2, 1)) : { 0.4472136, 0, -0.8944272, 0.4472136 }
		// plane2 initialized : XMPlaneFromPointNormal(XMVectorReplicate(1.f), XMVectorSet(1.f, 1.f, 1.f, 0.f)) : { 1, 1, 1, -3 }
		

		// XMVECTOR XMPlaneNormalize(XMVECTOR p) retruns normalized p.
		// This will set length of normal to 1.
		// Est version also available. (XMPlaneNormalizeEst())
		Utils::PrintXMVector(XMPlaneNormalize(plane2), "XMPlaneNormalize(plane2)");			// XMPlaneNormalize(plane2) : { 0.57735026, 0.57735026, 0.57735026, -1.7320509 }
		Utils::PrintXMVector(XMPlaneNormalizeEst(plane2), "XMPlaneNormalizeEst(plane2)");	// XMPlaneNormalizeEst(plane2) : { 0.5772705, 0.5772705, 0.5772705, -1.7318115 }

		// XMVECTOR XMPlaneDot(XMVECTOR p, XMVECTOR v) returns result of dot(p, v).
		// This related to location between point v { vx, vy, vz, vw } and plane p.
		// CAUTION: USE THIS WHEN v IS 4D VECTOR.
		Utils::PrintXMVector(XMPlaneDot(plane2, XMVectorReplicate(2.f)), "XMPlaneDot(plane2, XMVectorReplicate(2.f))"); // XMPlaneDot(plane2, XMVectorReplicate(2.f)) : { 0, 0, 0, 0 } -> point v is placed over the plane.

		// XMVECTOR XMPlaneDotCoord(XMVECTOR p, XMVECTOR v) returns of dot(p, v).
		// Diffrence is v is 3D vector { vx, vy, vz, 1 }. -> v means position vector.
		// if p is normalized, avx + avy + avz + d means shortest distance(vertical) between v and p.
		Utils::PrintXMVector(XMPlaneDotCoord(XMPlaneNormalize(plane2), XMVectorSet(2.f, 2.f, 2.f, 1.f)), "XMPlaneDotCoord(XMPlaneNormalize(plane2), XMVectorSet(2.f, 2.f, 2.f, 1.f))");
		// XMPlaneDotCoord(XMPlaneNormalize(plane2), XMVectorSet(2.f, 2.f, 2.f, 1.f)) : { 1.7320507, 1.7320507, 1.7320507, 1.7320507 }

		// XMVECTOR XMPlaneDotNormal(XMVECTOR p, XMVECTOR v) returns result of dot(p, v).
		// This time, v is { vx, vy, vz, 0 }. -> v means direction vector.
		// Result will represent cos value of angles that v and p's normal vector makes.
		Utils::PrintXMVector(XMPlaneDotNormal(XMPlaneNormalize(plane2), XMVectorSet(2.f, 2.f, 2.f, 0.f)), "XMPlaneDotNormal(XMPlaneNormalize(plane2), XMVectorSet(2.f, 2.f, 2.f, 0.f)");
		// XMPlaneDotNormal(XMPlaneNormalize(plane2), XMVectorSet(2.f, 2.f, 2.f, 0.f) : { 3.4641016, 3.4641016, 3.4641016, 3.4641016 }

		// XMVECTOR XMPlaneDotNormal(XMVECTOR p, XMMATRIX m) transforms p using transform matrix m.
		Utils::PrintXMVector(XMPlaneTransform(plane2, XMMatrixAffineTransformation(XMVectorReplicate(1.f), XMVectorReplicate(0.f), XMVectorReplicate(XM_PIDIV4), XMVectorReplicate(5.f))), "XMPlaneTransform(plane2, XMMatrixAffineTransformation(XMVectorReplicate(1.f), XMVectorReplicate(0.f), XMVectorReplicate(XM_PIDIV4), XMVectorReplicate(5.f)))");
		// XMPlaneTransform(plane2, XMMatrixAffineTransformation(XMVectorReplicate(1.f), XMVectorReplicate(0.f), XMVectorReplicate(XM_PIDIV4), XMVectorReplicate(5.f))) : { -14, -14, -14, -3 }
		// rotate 45 degree by each axis, translate 5.f by each axis.

		// XMVECTOR XMPlaneIntersectLine(XMVECTOR p, XMVECTOR p1, XMVECTOR p2) checks if plane p intersects with line this passes p1 and p2.
		// if line and plane is parallel, returns { QNaN, QNaN, QNaN, QNaN }
		Utils::PrintXMVector(XMPlaneIntersectLine(plane2, XMVectorReplicate(0.f), XMVectorReplicate(5.f)), "XMPlaneIntersectLine(plane2, XMVectorReplicate(0.f), XMVectorReplicate(5.f))");
		// XMPlaneIntersectLine(plane2, XMVectorReplicate(0.f), XMVectorReplicate(5.f)) : { 1, 1, 1, 1 }

		// Let's look when result is parallel
		XMVECTOR plane3 = XMPlaneFromPointNormal(XMVectorSet(0, 0, 0, 1), XMVectorSet(0, 1, 0, 0));
		XMVECTOR parallelP1 = XMVectorSet(0, 1, 0, 0);
		XMVECTOR parallelP2 = XMVectorSet(1, 1, 0, 0);
		Utils::PrintXMVector(XMPlaneIntersectLine(plane3, parallelP1, parallelP2), "Return when parallel");	// Return when parallel : { nan, nan, nan, nan }

		/*
	            parallelP1   parallelP2
		            •           •             y  z
		       /---------------------/        | /
		      /       plane3        /         |/
             /---------------------/          |------x
		*/


		// void XMPlaneIntersectPlane(XMVECTOR* pLP1, XMVECTOR pLP2, XMVECTOR p1, XMVECTOR p2)
		// This functions checks if plane p1 and p2 are intersected
		// intersectd points will create line. pLP1 and pLP2 will return this line as output parameters.
		// If p1 and p2 is parallel, pLP1 and pLP2 will return { NaN, NaN, NaN, NaN }.

		XMVECTOR cp1{}, cp2{};
		XMPlaneIntersectPlane(&cp1, &cp2, plane1, plane2);
		Utils::PrintXMVector(cp1, "p1 of intersected line between plane1 & plane2");
		Utils::PrintXMVector(cp2, "p2 of intersected line between plane1 & plane2");
		// p1 of intersected line between plane1& plane2 : { 0.99999994, 0.99999994, 0.99999994, 0 }
		// p2 of intersected line between plane1& plane2 : { 0.10557276, 2.3416407, 0.55278635, 0 }


		// Let's look when result is parallel
		XMVECTOR plane4 = XMPlaneFromPointNormal(XMVectorSet(0, 1, 0, 1), XMVectorSet(0, 1, 0, 0));
		XMPlaneIntersectPlane(&cp1, &cp2, plane3, plane4);
		Utils::PrintXMVector(cp1, "p1 when parallel");	// p1 when parallel : { nan, nan, nan, nan }
		Utils::PrintXMVector(cp2, "p2 when parallel");	// p2 when parallel : { nan, nan, nan, nan }


		// Functions below are plane comparison functions.
		// Explain when something special.

		// bool XMPlaneEqual(XMVECTOR p1, XMVECTOR p2)
		// bool XMPlaneNearEqual(XMVECTOR p1, XMVECTOR p2, XMVECTOR epsilon) -> This is almost same as other ...NearEqual() functions.
		// bool XMPlaneNorEqual(XMVECTOR p1, XMVECTOR p2)
		// bool XMPlaneIsInfinite(XMVECTOR p) -> returns true if any of elenents are inf
		// bool XMPlaneIsNaN(XMVECTOR p) -> returns true if any of elements are NaN
	}


}

void XMCollisionDetections()
{
	BoundingBox;
	BoundingOrientedBox; 
	BoundingSphere;
	BoundingFrustum;

	// These four structs above are bounding objects provided by DirectXMath
	
	// 1. BoundingBox 
	{
		// BoundingBox is well known as AABB(Axis-Alligned Bounding Box). 
		// Member datas contains center(XMFLOAT3), extents(XMFLOAT3) and CORNER_COUNT(size_t)
		// BoundingBox::extents means diagnoal point from center. This also means size of boundingbox.

		// To create BoundingBox, We can use both constructor and Initialization functions.
		// Constructor of BoundingBox receives position of center(as XMFLOAT3) and extents(as XMFLOAT3)

		// Example mesh of cube.
		std::vector<XMFLOAT3> points = {
			XMFLOAT3{1.f,	1.f,	1.f},
			XMFLOAT3{1.f,	-1.f,	1.f},
			XMFLOAT3{-1.f,	1.f,	1.f},
			XMFLOAT3{-1.f,	-1.f,	1.f},
			XMFLOAT3{1.f,	1.f,	-1.f},
			XMFLOAT3{1.f,	-1.f,	-1.f},
			XMFLOAT3{-1.f,	1.f,	-1.f},
			XMFLOAT3{-1.f,	-1.f,	-1.f},
		};

		BoundingBox aabb2, aabb3, aabb4, aabb5;

		BoundingBox aabb1{ XMFLOAT3{0.f, 0.f, 0.f}, XMFLOAT3{1.f, 1.f, 1.f} };
		Utils::PrintBoundingBox(aabb1, "aabb1");

		// There are 4 types of BoundingBox initialization functions.
		// This version of CreateFromPoints() creates BoundingBox using min point and max point of target BoundingBox.
		BoundingBox::CreateFromPoints(aabb2, XMVectorReplicate(-1.f), XMVectorReplicate(1.f));
		Utils::PrintBoundingBox(aabb2, "aabb2");

		// This version of CreateFromPoints() creates BoundingBox using array of mesh's position.
		BoundingBox::CreateFromPoints(aabb3, points.size(), points.data(), sizeof(XMFLOAT3));
		Utils::PrintBoundingBox(aabb3, "aabb3");

		// CreateFromSphere() Creates cricumscribed BoundingBox of certain BoundingSphere.
		BoundingBox::CreateFromSphere(aabb4, BoundingSphere{ XMFLOAT3{5.f, 5.f, 5.f}, 1.f });
		Utils::PrintBoundingBox(aabb4, "aabb4");

		// CreateMerged() merges 2 BoundingBoxes.
		BoundingBox::CreateMerged(aabb5, aabb3, aabb4);
		Utils::PrintBoundingBox(aabb5, "aabb5");

		// GetCorners(XMFLOAT3* pCorners) returns positions of corners by output parameter.
		std::vector<XMFLOAT3> corners(8);
		aabb5.GetCorners(corners.data());
		std::println("aabb5's corners");
		for (const auto [i, p] : corners | std::views::enumerate) std::println("{} : {{ {}, {}, {} }}", i, p.x, p.y, p.z);

		// There are 2 types of transfrom functions.
		// void Transform(BoundingBox& out, XMMATRIX m) transforms BoundingBox using transform matrix.
		// For most cases, m will be world transfromation matrix which transforms local space to world space.
		aabb1.Transform(aabb1, XMMatrixAffineTransformation(XMVectorReplicate(1.f), XMVectorReplicate(0.f), XMQuaternionRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4), XMVectorSet(1.f, 2.f, 3.f, 1.f)));
		Utils::PrintBoundingBox(aabb1, "transformed aabb1");

		// void Transform(BoundingBox& out, float scale, XMVECTOR rotation, XMVECTOR translation) transforms BoundingBox using given transfrom parameters.
		aabb2.Transform(aabb2, 1.f, XMQuaternionRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4), XMVectorSet(1.f, 2.f, 3.f, 1.f));
		Utils::PrintBoundingBox(aabb2, "transformed aabb2");

	}

	std::println();

	// 2. BoundingOrientedBox
	{
		// BoundingOrientedBox is well known as OBB(Oriented Bounding Box). Which is has own oriented direction by each objects.
		// Member data has Center(XMFLOAT3), Extents(XMFLOAT3), Orientation(XMFLOAT4 quaternion), CORNER_COUNT.

		// To create BoundingOrientedBox, We can use both constructor and Initialization functions.
		// Constructor of BoundingBox receives position of center(as XMFLOAT3), extents(as XMFLOAT3) and orientation(as XMFLOAT4)
		
		XMFLOAT4 orientation;
		XMStoreFloat4(&orientation, XMQuaternionRotationRollPitchYaw(0.f, 0.f, 0.f));
		BoundingOrientedBox obb1{ XMFLOAT3(0,0,0), XMFLOAT3(1,1,1), orientation };
		Utils::PrintBoundingOrientedBox(obb1, "obb1");

		// Example mesh of cube.
		std::vector<XMFLOAT3> points = {
			XMFLOAT3{1.f,	1.f,	1.f},
			XMFLOAT3{1.f,	-1.f,	1.f},
			XMFLOAT3{-1.f,	1.f,	1.f},
			XMFLOAT3{-1.f,	-1.f,	1.f},
			XMFLOAT3{1.f,	1.f,	-1.f},
			XMFLOAT3{1.f,	-1.f,	-1.f},
			XMFLOAT3{-1.f,	1.f,	-1.f},
			XMFLOAT3{-1.f,	-1.f,	-1.f},
		};

		BoundingOrientedBox obb2{}, obb3{};
		// There are 2 types of BoundingBox initialization functions.

		// CreateFromPoints() creates OBB using mesh vertices data.
		BoundingOrientedBox::CreateFromPoints(obb2, points.size(), points.data(), sizeof(XMFLOAT3));
		Utils::PrintBoundingOrientedBox(obb2, "obb2");

		// CreateFromBoundingBox() creates OBB from BoundingBox object.
		BoundingOrientedBox::CreateFromBoundingBox(obb3, BoundingBox{ XMFLOAT3{0,0,0}, XMFLOAT3{1,1,1} });
		Utils::PrintBoundingOrientedBox(obb3, "obb3");

		// GetCorners(XMFLOAT3* pCorners) returns positions of corners by output parameter.
		std::vector<XMFLOAT3> corners(8);
		obb2.GetCorners(corners.data());
		std::println("obb2's corners");
		for (const auto [i, p] : corners | std::views::enumerate) std::println("{} : {{ {}, {}, {} }}", i, p.x, p.y, p.z);

		// Transform functions are same as BoundingBox's transform functions.
		// void Transform(BoundingBox& out, XMMATRIX m) transforms BoundingBox using transform matrix.
		// For most cases, m will be world transfromation matrix which transforms local space to world space.
		obb1.Transform(obb1, XMMatrixAffineTransformation(XMVectorReplicate(1.f), XMVectorReplicate(0.f), XMQuaternionRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4), XMVectorSet(1.f, 2.f, 3.f, 1.f)));
		Utils::PrintBoundingOrientedBox(obb1, "transformed obb1");

		// void Transform(BoundingBox& out, float scale, XMVECTOR rotation, XMVECTOR translation) transforms BoundingBox using given transfrom parameters.
		obb3.Transform(obb3, 1.f, XMQuaternionRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4), XMVectorSet(1.f, 2.f, 3.f, 1.f));
		Utils::PrintBoundingOrientedBox(obb3, "transformed obb3");

	}

	std::println();

	// 3. BoundingSphere
	{
		// Bounding sphere is Bounding object which is cricumscribes objects using sphere.
		// Member datas contains center(XMFLOAT3) and radius(float)

		// Constructor of BoundingSphere is receives center and radius as parameters
		BoundingSphere sp1{ XMFLOAT3{0.f, 0.f, 0.f}, 1.f };
		Utils::PrintBoundingSphere(sp1, "sp1");

		// Example mesh of cube.
		std::vector<XMFLOAT3> points = {
			XMFLOAT3{1.f,	1.f,	1.f},
			XMFLOAT3{1.f,	-1.f,	1.f},
			XMFLOAT3{-1.f,	1.f,	1.f},
			XMFLOAT3{-1.f,	-1.f,	1.f},
			XMFLOAT3{1.f,	1.f,	-1.f},
			XMFLOAT3{1.f,	-1.f,	-1.f},
			XMFLOAT3{-1.f,	1.f,	-1.f},
			XMFLOAT3{-1.f,	-1.f,	-1.f},
		};

		BoundingSphere sp2{}, sp3{}, sp4{}, sp5{};

		// CreateFromPoints() creates BoundingSphere using mesh vertices data.
		BoundingSphere::CreateFromPoints(sp2, points.size(), points.data(), sizeof(XMFLOAT3));
		Utils::PrintBoundingSphere(sp2, "sp2");

		// CreateFromBoundingBox() creates BoundingSphere from BoundingfOrientedBox.
		XMFLOAT4 orientation;
		XMStoreFloat4(&orientation, XMQuaternionRotationRollPitchYaw(0.f, 0.f, 0.f));
		BoundingSphere::CreateFromBoundingBox(sp3, BoundingOrientedBox{ XMFLOAT3{0.f, 0.f, 0.f}, XMFLOAT3{1.f, 1.f, 1.f}, orientation });
		Utils::PrintBoundingSphere(sp3, "sp3");
		
		// CreateFromFrustum() creates BoundingSphere from BoundingFrustum.
		BoundingSphere::CreateFromFrustum(sp4, BoundingFrustum{ XMMatrixPerspectiveFovLH(90, 1920 / 1080, 10, 100) });
		Utils::PrintBoundingSphere(sp4, "sp4");

		// CreateMerged() merges two BoundingSpheres.
		// Created BoundingSphere will contains two merged BoundingSpheres.
		BoundingSphere::CreateMerged(sp5, sp1, sp4);
		Utils::PrintBoundingSphere(sp5, "sp5");


		// Transfrom Functions are same as BoundingBox and BoundingOrientedBox.
		//Therefore, detailed explanation will skip.
		sp1.Transform(sp1, XMMatrixAffineTransformation(XMVectorReplicate(1.f), XMVectorReplicate(0.f), XMQuaternionRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4), XMVectorSet(1.f, 2.f, 3.f, 1.f)));
		Utils::PrintBoundingSphere(sp1, "transformed sp1");

		sp3.Transform(sp3, 1.f, XMQuaternionRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4), XMVectorSet(1.f, 2.f, 3.f, 1.f));
		Utils::PrintBoundingSphere(sp3, "transformed sp3");
	}

	std::println();

	// 4. BoundingFrustum
	{
		// BoundingFrustum represents frustum bounding objects.
		// Therefore, BoundingFrustum is mostly used for camera frustum.
		// Members of BoundingFrustum is :
		// Origin(XMFLOAT3)
		// Orientation(XMFLOAT4)
		// Near, Far, LeftSlope, RightSlope, TopSlope, BottomSlope(float)
		// CORNER_COUNT(size_t)

		// BoundingFrustum has 2 constructors.

		// BoundingFrustum(XMMATRIX Projection, bool rhcoords = false)
		BoundingFrustum f1{ XMMatrixPerspectiveFovLH(90, /* (width / height) */ 800 / 600, 10, 100), false };
		Utils::PrintBoundingFrustum(f1, "f1");

		// BoundingFrustum(XMFLOAT3& origin, XMFLOAT4 orientation, float RightSlope, float LeftSlope, float TopSlope, float BottomSlope, float nearPlane, float farPlane)
		BoundingFrustum f2{ XMFLOAT3{0,0,0}, XMFLOAT4{0,0,0,0}, 1.5, -1.5, 1.5f, -1.5f, 10, 100};
		Utils::PrintBoundingFrustum(f2, "f2");

		// Bounding Frustum has one Initializize Functions
		// void CreateFromMatrix(BoundingFrustum& out, XMMATRIX Projection, bool rhcoords = false)
		// This creates BoundingFrustum from projection matrix. We can also configure coordiante system(RHS <-> LHS).
		BoundingFrustum f3{};
		BoundingFrustum::CreateFromMatrix(f3, XMMatrixOrthographicLH(800, 600, 10, 100), false);
		Utils::PrintBoundingFrustum(f3, "f3");

		// GetCorners(XMFLOAT3* pCorners) returns positions of corners by output parameter.
		std::vector<XMFLOAT3> corners(8);
		f1.GetCorners(corners.data());
		std::println("f1's corners");
		for (const auto [i, p] : corners | std::views::enumerate) std::println("{} : {{ {}, {}, {} }}", i, p.x, p.y, p.z);

		// Transform Functions are same.
		f1.Transform(f1, XMMatrixAffineTransformation(XMVectorReplicate(1.f), XMVectorReplicate(0.f), XMQuaternionRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4), XMVectorSet(1.f, 2.f, 3.f, 1.f)));
		Utils::PrintBoundingFrustum(f1, "transformed f1");

		f3.Transform(f3, 1.f, XMQuaternionRotationRollPitchYaw(XM_PI, XM_PIDIV2, XM_PIDIV4), XMVectorSet(1.f, 2.f, 3.f, 1.f));
		Utils::PrintBoundingFrustum(f3, "transformed f3");
	}

	std::println();

	// 5. Use of Bounding objects
	{
		// 5-0. Return values of collision detections.
		{
			// There are two types of collision detection functions : 1. ContainedBy() | 2. Intersects()

			// ContainedBy() types functions returns ContainmentType type.
			// This enum type are declared in DirectXCollision.h header file.
			// 
			//	enum ContainmentType
			//	{
			//		DISJOINT = 0,
			//		INTERSECTS = 1,
			//		CONTAINS = 2
			//	};
			// DISJOINT means there are no intersection between two bounding objects.
			// INTERSECTS means two objects are intersected(not contained)
			// CONTAINS means one bounding object contains other objects completly.


			// Intersects() types functions returns PlaneIntersection type.
			// This enum type are also declared in DirectXCollision.h header file.
			//	enum PlaneIntersectionType
			//	{
			//		FRONT = 0,
			//		INTERSECTING = 1,
			//		BACK = 2
			//	};
			// FRONT means bounding object is located in front of plane.
			// INTERSECTING means bounding object and plane is intersected.
			// BACK means bounding object is located behind of plane.
			// FRONT and BACK is determined by planes normal.
			
		}

		// 5-1. Triangle Test
		{
			// DirectXMath provides Triangle test functions that check intersection between triangle and bounding objects.
			// We can use this in more precise collision detection algorithms.
			// TriangleTest functions are declared in "TriangleTests" namespace.
			using namespace TriangleTests;

			// ContainedBy(XMVECTOR V0, FXMVECTOR V1, FXMVECTOR V2, GXMVECTOR Plane0, HXMVECTOR Plane1, HXMVECTOR Plane2, CXMVECTOR Plane3, CXMVECTOR Plane4, CXMVECTOR Plane5) 
			// This functions checks if triangle(three XMFLOAT3) is contained by Bounding object composed by given planes.
			// Mostly used for check with BoundingFrustum and triangles.

			BoundingFrustum frustum{ XMMatrixPerspectiveFovLH(90.f, 800 / 600, 10, 100) };
			std::array<XMVECTOR, 3> triangle{
				 XMVectorSet(-1.f, -1.f, 10.f, 1.f),
				 XMVectorSet(1.f, -1.f, 10.f, 1.f),
				 XMVectorSet(0.f, 1.f, 10.f, 1.f)
			};

			std::array<XMVECTOR, 6> planes;
			frustum.GetPlanes(&planes[0], &planes[1], &planes[2], &planes[3], &planes[4], &planes[5]);

			Utils::PrintBoundingFrustum(frustum, "");
			Utils::PrintContainResult(TriangleTests::ContainedBy(triangle[0], triangle[1], triangle[2], planes[0], planes[1], planes[2], planes[3], planes[4], planes[5]));	// CONTAINS
			std::println();

			// Intersects(XMVECTOR v0, XMVECTOR v1, XMVECTOR v2, XMVECTOR plane) checks intersection between certain triangle and plane.

			for (auto plane : planes)
			{
				Utils::PrintPlaneIntersectionResult(TriangleTests::Intersects(triangle[0], triangle[1], triangle[2], plane));
				std::println();
				/*
					BACK
					BACK
					BACK
					BACK
					BACK
					BACK
				*/
			}

			// There's more Intersects() functions are available

			// bool Intersects(XMVECTOR origin, XMVECTOR direction, XMVECTOR v0, XMVECTOR v1, XMVECTOR v2, float& distance)
			// This function checks intersection between triangle and ray(start from origin, ray towards direction)
			// distance means distance between ray's origin to intersected point.
			struct Ray { XMFLOAT3 origin; XMFLOAT3 direction; };
			Ray r{ XMFLOAT3{0,0,0}, XMFLOAT3{0,0,1} };
			float distance{};
			bool bRayCasted = TriangleTests::Intersects(XMLoadFloat3(&r.origin), XMLoadFloat3(&r.direction), triangle[0], triangle[1], triangle[2], distance);
			std::println("ray casted result : {} | distance : {}", bRayCasted ? "TRUE" : "FALSE", distance);	// ray casted result : TRUE | distance : 10

			// bool Intersects(XMVECTOR a0, XMVECTOR a1, XMVECTOR a2, XMVECTOR b0, XMVECTOR b1, XMVECTOR b2)
			// This function checks intersection between two triangles.

			std::array<XMVECTOR, 3> triangle2{
				XMVectorAdd(triangle[0], XMVectorSet(0.f, 0.f, 2.f, 0.f)),
				XMVectorAdd(triangle[1], XMVectorSet(0.f, 0.f, 2.f, 0.f)),
				XMVectorAdd(triangle[2], XMVectorSet(0.f, 0.f, -2.f, 0.f)),
			};

			bool bTriangleIntersected = TriangleTests::Intersects(triangle[0], triangle[1], triangle[2], triangle2[0], triangle2[1], triangle2[2]);
			std::println("Triangles intersection : {}", bTriangleIntersected ? "TRUE" : "FALSE");	// Triangles intersection : TRUE
		}

		std::println();

		// 5-2. intersection Check between bounding objects
		{
			// Every bounding objects has ContainedBy(), Contains() and Intersects() functions.
			// All of this functions are overloaded for every other types of bounding objects each.

			// ContainedBy() function checks if objects is contained by 6 planes.
			// Contains() functions checks caller objects contains others.
			// Intersects() functions checks intersections between two objects. -> Only vs Plane funtion returns PlaneIntersectionType.

			// Like this
			BoundingBox aabb{ XMFLOAT3{0,0,0}, XMFLOAT3{1,1,1} };
			XMFLOAT4 orientation;
			XMStoreFloat4(&orientation, XMQuaternionRotationRollPitchYaw(0.f, 0.f, 0.f));
			BoundingOrientedBox obb{ XMFLOAT3{0,0,0}, XMFLOAT3{1,1,1}, orientation };
			BoundingSphere sphere{ XMFLOAT3{0,0,0}, 1.f };
			BoundingFrustum frustum{ XMMatrixPerspectiveFovLH(90, 800 / 600, 1, 100) };
			std::array<XMVECTOR, 3> triangle{
				 XMVectorSet(-1.f, -1.f, 1.f, 1.f),
				 XMVectorSet(1.f, -1.f, 1.f, 1.f),
				 XMVectorSet(0.f, 1.f, 1.f, 1.f)
			};
			struct Ray { XMFLOAT3 origin; XMFLOAT3 direction; };
			Ray ray{ XMFLOAT3{0,0,0}, XMFLOAT3{0,0,1} };
			float dist{};

			std::array<XMVECTOR, 6> frustumPlanes;
			frustum.GetPlanes(&frustumPlanes[0], &frustumPlanes[1], &frustumPlanes[2], &frustumPlanes[3], &frustumPlanes[4], &frustumPlanes[5]);

			// ContainedBy
			Utils::PrintContainResult(aabb.ContainedBy(frustumPlanes[0], frustumPlanes[1], frustumPlanes[2], frustumPlanes[3], frustumPlanes[4], frustumPlanes[5])); std::println();

			// Contains (6)
			Utils::PrintContainResult(aabb.Contains(aabb)); std::println();										// BoundingBox vs BoundingBox
			Utils::PrintContainResult(aabb.Contains(obb)); std::println();										// BoundingBox vs BoundingOrientedBox
			Utils::PrintContainResult(aabb.Contains(sphere)); std::println();									// BoundingBox vs BoundingSphere
			Utils::PrintContainResult(aabb.Contains(frustum)); std::println();									// BoundingBox vs BoundingFrustum
			Utils::PrintContainResult(aabb.Contains(triangle[0], triangle[1], triangle[2])); std::println();	// BoundingBox vs Triangle
			Utils::PrintContainResult(aabb.Contains(XMVectorSet(0, 0, 0, 1))); std::println();					// BoundingBox vs Point

			// Intersects (7)
			std::println("vs aabb : {}", aabb.Intersects(aabb) ? "TRUE" : "FALSE");																// BoundingBox vs BoundingBox
			std::println("vs obb : {}", aabb.Intersects(obb) ? "TRUE" : "FALSE");																// BoundingBox vs BoundingOrientedBox
			std::println("vs sphere : {}", aabb.Intersects(sphere) ? "TRUE" : "FALSE");															// BoundingBox vs BoundingSphere
			std::println("vs frustum : {}", aabb.Intersects(frustum) ? "TRUE" : "FALSE");														// BoundingBox vs BoundingFrustum
			std::println("vs triangle : {}", aabb.Intersects(triangle[0], triangle[1], triangle[2]) ? "TRUE" : "FALSE");						// BoundingBox vs Triangle
			std::println("vs ray : {}", aabb.Intersects(XMLoadFloat3(&ray.origin), XMLoadFloat3(&ray.direction), dist) ? "TRUE" : "FALSE");		// BoundingBox vs Ray
			Utils::PrintPlaneIntersectionResult(aabb.Intersects(XMPlaneFromPointNormal(XMVectorSet(0, 0, 0, 1), XMVectorSet(0, 1, 0, 0))));		// BoundingBox vs Plane

			// Every Bounding Objects has SAME functions.

			// BoundingOrientedBox
			{
				// ContainedBy
				obb.ContainedBy(frustumPlanes[0], frustumPlanes[1], frustumPlanes[2], frustumPlanes[3], frustumPlanes[4], frustumPlanes[5]);

				// Contains
				obb.Contains(aabb);										// BoundingOrientedBox vs BoundingBox
				obb.Contains(obb);										// BoundingOrientedBox vs BoundingOrientedBox
				obb.Contains(sphere);									// BoundingOrientedBox vs BoundingSphere
				obb.Contains(frustum);									// BoundingOrientedBox vs BoundingFrustum
				obb.Contains(triangle[0], triangle[1], triangle[2]);	// BoundingOrientedBox vs Triangle
				obb.Contains(XMVectorSet(0, 0, 0, 1));					// BoundingOrientedBox vs Point

				// Intersects
				obb.Intersects(aabb);																		// BoundingOrientedBox vs BoundingBox
				obb.Intersects(obb);																		// BoundingOrientedBox vs BoundingOrientedBox
				obb.Intersects(sphere);																		// BoundingOrientedBox vs BoundingSphere
				obb.Intersects(frustum);																	// BoundingOrientedBox vs BoundingFrustum
				obb.Intersects(triangle[0], triangle[1], triangle[2]);										// BoundingOrientedBox vs Triangle
				obb.Intersects(XMLoadFloat3(&ray.origin), XMLoadFloat3(&ray.direction), dist);				// BoundingOrientedBox vs Ray
				obb.Intersects(XMPlaneFromPointNormal(XMVectorSet(0, 0, 0, 1), XMVectorSet(0, 1, 0, 0)));	// BoundingOrientedBox vs Plane
			}
			
			// BoundingSphere
			{
				// ContainedBy
				sphere.ContainedBy(frustumPlanes[0], frustumPlanes[1], frustumPlanes[2], frustumPlanes[3], frustumPlanes[4], frustumPlanes[5]);

				// Contains
				sphere.Contains(aabb);										// BoundingSphere vs BoundingBox
				sphere.Contains(obb);										// BoundingSphere vs BoundingOrientedBox
				sphere.Contains(sphere);									// BoundingSphere vs BoundingSphere
				sphere.Contains(frustum);									// BoundingSphere vs BoundingFrustum
				sphere.Contains(triangle[0], triangle[1], triangle[2]);		// BoundingSphere vs Triangle
				sphere.Contains(XMVectorSet(0, 0, 0, 1));					// BoundingSphere vs Point

				// Intersects
				sphere.Intersects(aabb);																		// BoundingSphere vs BoundingBox
				sphere.Intersects(obb);																			// BoundingSphere vs BoundingOrientedBox
				sphere.Intersects(sphere);																		// BoundingSphere vs BoundingSphere
				sphere.Intersects(frustum);																		// BoundingSphere vs BoundingFrustum
				sphere.Intersects(triangle[0], triangle[1], triangle[2]);										// BoundingSphere vs Triangle
				sphere.Intersects(XMLoadFloat3(&ray.origin), XMLoadFloat3(&ray.direction), dist);				// BoundingSphere vs Ray
				sphere.Intersects(XMPlaneFromPointNormal(XMVectorSet(0, 0, 0, 1), XMVectorSet(0, 1, 0, 0)));	// BoundingSphere vs Plane
			}

			// BoundingSphere
			{
				// ContainedBy
				frustum.ContainedBy(frustumPlanes[0], frustumPlanes[1], frustumPlanes[2], frustumPlanes[3], frustumPlanes[4], frustumPlanes[5]);

				// Contains
				frustum.Contains(aabb);										// BoundingFrustum vs BoundingBox
				frustum.Contains(obb);										// BoundingFrustum vs BoundingOrientedBox
				frustum.Contains(sphere);									// BoundingFrustum vs BoundingSphere
				frustum.Contains(frustum);									// BoundingFrustum vs BoundingFrustum
				frustum.Contains(triangle[0], triangle[1], triangle[2]);	// BoundingFrustum vs Triangle
				frustum.Contains(XMVectorSet(0, 0, 0, 1));					// BoundingFrustum vs Point

				// Intersects
				frustum.Intersects(aabb);																		// BoundingFrustum vs BoundingBox
				frustum.Intersects(obb);																		// BoundingFrustum vs BoundingOrientedBox
				frustum.Intersects(sphere);																		// BoundingFrustum vs BoundingSphere
				frustum.Intersects(frustum);																	// BoundingFrustum vs BoundingFrustum
				frustum.Intersects(triangle[0], triangle[1], triangle[2]);										// BoundingFrustum vs Triangle
				frustum.Intersects(XMLoadFloat3(&ray.origin), XMLoadFloat3(&ray.direction), dist);				// BoundingFrustum vs Ray
				frustum.Intersects(XMPlaneFromPointNormal(XMVectorSet(0, 0, 0, 1), XMVectorSet(0, 1, 0, 0)));	// BoundingFrustum vs Plane
			}
			

		}

	}

}

void XMQuaternionFundamentals()
{

}

int main()
{
	// Vector processing SIMD extension : SSE, SSE2, AVX, AVX2
	// Check if this system supports SIMD for compute DirectXMath
	std::cout << std::boolalpha << "Is this system support SIMD for DirectXMath? : " << XMVerifyCPUSupport() << '\n';

	// 1. Basics
	//XMBasics();

	// 2. 3D Graphics Fundamentals
	//XM3DGraphicsFundametals();

	// 3. Collision Detection
	XMCollisionDetections();

	// 4. Quaternions
	XMQuaternionFundamentals()

}