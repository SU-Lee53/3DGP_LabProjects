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

// 1. Basics of DirectXMath
void XMBasicsTest()
{

	// 1. Constants(radian)
	// DirectXMath provides some constants like radian : more in DirectXMath.h 
	std::println("Constants(radian)");
	std::println("XM_PI (¥ð) : {}", XM_PI);
	std::println("XM_2PI (2¥ð) : {}", XM_2PI);
	std::println("XM_1DIVPI (1/¥ð) : {}", XM_1DIVPI);
	std::println("XM_1DIV2PI (1/2¥ð) : {}", XM_1DIVPI);
	std::println("XM_PIDIV2 (¥ð/2) : {}", XM_PIDIV2);
	std::println("XM_PIDIV4 (¥ð/4) : {}", XM_PIDIV2);

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


int main()
{
	// Vector processing SIMD extension : SSE, SSE2, AVX, AVX2
	// Check if this system supports SIMD for compute DirectXMath
	std::cout << std::boolalpha << "Is this system support SIMD for DirectXMath? : " << XMVerifyCPUSupport() << '\n';

	// 1. Basics
	XMBasicsTest();


}