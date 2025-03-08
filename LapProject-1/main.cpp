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

#include "Utils.h"
using namespace std::literals;

// 1. Basics of DirectXMath
void XMBasics()
{
	// 1. Constants(radian)
	// DirectXMath provides some constants like radian : more in DirectXMath.h 
	std::println("1. Constants(radian)");
	std::println("XM_PI (��) : {}", XM_PI);
	std::println("XM_2PI (2��) : {}", XM_2PI);
	std::println("XM_1DIVPI (1/��) : {}", XM_1DIVPI);
	std::println("XM_1DIV2PI (1/2��) : {}", XM_1DIVPI);
	std::println("XM_PIDIV2 (��/2) : {}", XM_PIDIV2);
	std::println("XM_PIDIV4 (��/4) : {}", XM_PIDIV2);

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
		// f*(p1-p0) ��   �� g*(p2 - p0)
		//	         |  /
		//	         | /
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

		// There is special comarison functions for XMVectorInBoundsR() : bool XMComparisonAllInBounds(uint32_t CR) / bool XMComparisonAnyInBounds(uint32_t CR)
		uint32_t CR = 0;
		XMVectorInBoundsR(&CR, v1, XMVectorReplicate(3.f));
		std::println("v1 is in bound(3.f,3.f,3.f,3.f)? : {}", XMComparisonAllInBounds(CR) ? "TRUE" : "FALSE");
		std::println("any of v1 elements are out of bound(3.f,3.f,3.f,3.f)? : {}", XMComparisonAnyOutOfBounds(CR) ? "TRUE" : "FALSE");
		
		


	}
}


int main()
{
	// Vector processing SIMD extension : SSE, SSE2, AVX, AVX2
	// Check if this system supports SIMD for compute DirectXMath
	std::cout << std::boolalpha << "Is this system support SIMD for DirectXMath? : " << XMVerifyCPUSupport() << '\n';

	// 1. Basics
	//XMBasics();

	// 2. 3D Graphics Fundamentals
	XM3DGraphicsFundametals();



}