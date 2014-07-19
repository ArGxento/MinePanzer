#pragma once
//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include <vector>
#include <string>
#include <set>
#include <map>
#include <list>
#include <algorithm>
#include <functional>

#include <memory>

#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	文字コード
	MSVC		sizeof(wchar_t)==2
	gcc(cygwin)	sizeof(wchar_t)==2
	gcc(linux)	sizeof(wchar_t)==4
*/
namespace ace
{
#ifdef _WIN32
typedef wchar_t achar;
typedef std::wstring astring;
#else 
typedef char16_t achar;
typedef std::basic_string<char16_t> astring;
#endif
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define ACE_STDCALL __stdcall
# else
#   define ACE_STDCALL
# endif 

# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define ACE_DLLEXPORT __declspec(dllexport)
# else
#   define ACE_DLLEXPORT
# endif 



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct Vector2DF;
struct Vector2DI;
struct Vector3DF;
struct Vector4DF;
struct Matrix43;
struct Matrix44;
struct RectF;
struct RectI;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	最大値取得
*/
template <typename T,typename U>
T Max( T t, U u )
{
	if( t > (T)u )
	{
		return t;
	}
	return u;
}

/**
	@brief	最小値取得
*/
template <typename T,typename U>
T Min( T t, U u )
{
	if( t < (T)u )
	{
		return t;
	}
	return u;
}

/**
	@brief	範囲内値取得
*/
template <typename T,typename U,typename V>
T Clamp( T t, U max_, V min_ )
{
	if( t > (T)max_ )
	{
		t = (T)max_;
	}

	if( t < (T)min_ )
	{
		t = (T)min_;
	}

	return t;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
inline float NormalizeAngle(float angle)
{
	int32_t ofs = (*(int32_t*)&angle & 0x80000000) | 0x3F000000; 
	return (angle - ((int)(angle * 0.159154943f + *(float*)&ofs) * 6.283185307f)); 
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
inline void SinCos(float x, float& s, float& c)
{
	x = NormalizeAngle(x);
	float x2 = x * x;
	float x4 = x * x * x * x;
	float x6 = x * x * x * x * x * x;
	float x8 = x * x * x * x * x * x * x * x;
	float x10 = x * x * x * x * x * x * x * x * x * x;
	s = x * (1.0f - x2 / 6.0f + x4 / 120.0f - x6 / 5040.0f + x8 / 362880.0f - x10 / 39916800.0f);
	c = 1.0f - x2 / 2.0f + x4 / 24.0f - x6 / 720.0f + x8 / 40320.0f - x10 / 3628800.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
@brief	文字コードを変換する。(UTF16 -> UTF8)
@param	dst	[out]	出力配列の先頭ポインタ
@param	dst_size	[in]	出力配列の長さ
@param	src			[in]	入力配列の先頭ポインタ
@return	文字数
*/
int32_t Utf16ToUtf8(int8_t* dst, int32_t dst_size, const int16_t* src);

/**
	@brief	文字コードを変換する。(UTF8 -> UTF16)
	@param	dst			[out]	出力配列の先頭ポインタ
	@param	dst_size	[in]	出力配列の長さ
	@param	src			[in]	入力配列の先頭ポインタ
	@return	文字数
*/
int32_t Utf8ToUtf16(int16_t* dst, int32_t dst_size, const int8_t* src);

std::wstring ToWide(const char* pText);

astring ToAString(const wchar_t* src);

astring ToAString(const char* src);

std::string ToUtf8String(const achar* src);

astring ReplaceAll(const astring text, const achar* from, const achar* to);

astring CombinePath(const achar* rootPath, const achar* path);

#if !_WIN32 && !SWIG
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static astring ReplaceAll(const astring text, const wchar_t* from, const wchar_t* to)
{
	return ReplaceAll(text, ToAString(from).c_str(), ToAString(to).c_str());
}
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#if !SWIG
void ShowMessageBox(const achar* title, const achar* text);

#if _WIN32
#define SPRINTF sprintf_s
#else
#define SPRINTF snprintf
#endif

#define ACE_ASSERT(condition, message) { \
if (!(condition)) { \
	char lbuf[100]; \
	SPRINTF(lbuf, 100, "%d", __LINE__); \
	auto m = ::ace::ToAString(message); \
	auto f = ::ace::ToAString(__FILE__); \
	auto l = ::ace::ToAString(lbuf); \
	auto state = f + ::ace::ToAString("(") + l + ::ace::ToAString(")"); \
	auto m_ = state + ::ace::ToAString("\n") + m; \
	::ace::ShowMessageBox(::ace::ToAString("Assert").c_str(), m_.c_str()); \
	(*((int*)0x0) = 0x0);  } \
}

#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#ifdef _WIN32
inline void Sleep( int32_t ms )
{
	::Sleep( ms );
}
#else
inline void Sleep( int32_t ms )
{
	usleep( 1000 * ms );
}
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
template <class T>
void SafeAddRef(T& t)
{
	if (t != NULL)
	{
		t->AddRef();
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
template <class T>
void SafeRelease(T& t)
{
	if (t != NULL)
	{
		t->Release();
		t = NULL;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
template <class T>
void SafeSubstitute(T& target, T& value)
{
	SafeAddRef(value);
	SafeRelease(target);
	target = value;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
template <typename T>
inline void SafeDelete(T*& p)
{
	if (p != NULL)
	{
		delete (p);
		(p) = NULL;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
template <typename T>
inline void SafeDeleteArray(T*& p)
{
	if (p != NULL)
	{
		delete[] (p);
		(p) = NULL;
	}
}


const float PI = 3.14159265358979f;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static float DegreeToRadian(float degree)
{
	return degree / 180.0f * PI;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static float RadianToDegree(float radian)
{
	return radian / PI * 180.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static std::vector<int8_t> GetBinaryData(astring filePath)
{
	FILE* fp = nullptr;

#if _WIN32
	_wfopen_s(&fp, filePath.c_str(), L"rb");
	if (fp == nullptr) return std::vector<int8_t>();
#else
	fp = fopen(ToUtf8String(filePath.c_str()).c_str(), "rb");
	if (fp == nullptr) return std::vector<int8_t>();
#endif

	fseek(fp, 0, SEEK_END);
	auto size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	std::vector<int8_t> data;
	data.resize(size);

	fread(&(data[0]), 1, size, fp);
	fclose(fp);

	return data;
}
}




//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
		@brief	全ての参照カウンタの値の合計を取得する。
		@return	参照カウンタ合計値
	*/
	int32_t GetGlobalReferenceCount();

	/**
		@brief	全ての参照カウンタの値を取得し、0ならDLLを解放する。
	*/
	void CheckDLL();

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}



//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	/**
		@brief	2次元ベクトル(整数)
		*/
	struct Vector2DI
	{
	public:
		/**
			@brief	X
			*/
		int32_t	X;

		/**
			@brief	Y
			*/
		int32_t	Y;

		/**
			@brief	コンストラクタ
			*/
		Vector2DI();

		/**
			@brief	コンストラクタ
			*/
		Vector2DI(int32_t x, int32_t y);

		bool operator == (const Vector2DI& o);
		bool operator != (const Vector2DI& o);

		Vector2DI operator-();

		Vector2DI operator+(const Vector2DI& right);

		Vector2DI operator-(const Vector2DI& right);

		Vector2DI operator*(const Vector2DI& right);

		Vector2DI operator/(const Vector2DI& right);

		Vector2DI operator*(int32_t right);

		Vector2DI operator/(int32_t right);

		Vector2DI& operator+=(const Vector2DI& right);

		Vector2DI& operator-=(const Vector2DI& right);

		Vector2DI& operator*=(const Vector2DI& right);

		Vector2DI& operator/=(const Vector2DI& right);

		Vector2DI& operator*=(int32_t right);

		Vector2DI& operator/=(int32_t right);
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	/**
		@brief	2次元ベクトル
		*/
	struct Vector2DF
	{
	public:
		/**
			@brief	X
			*/
		float	X;

		/**
			@brief	Y
			*/
		float	Y;

		/**
			@brief	コンストラクタ
			*/
		Vector2DF();

		/**
			@brief	コンストラクタ
			@param	x	X
			@param	y	Y
		*/
		Vector2DF(float x, float y);

		/**
			@brief	このベクトルの長さを取得する。
		*/
		float GetLength() const
		{
			return sqrt(GetSquaredLength());
		}

		/**
		@brief	このベクトルの長さの二乗を取得する。
		*/
		float GetSquaredLength() const
		{
			return X * X + Y * Y;
		}

		/**
			@brief	このベクトルの長さを設定する。
		*/
		void SetLength(float value)
		{
			float length = GetLength();
			(*this) *= (value / length);
		}

		/**
		@brief	このベクトルの単位ベクトルを取得する。
		*/
		Vector2DF GetNormal()
		{
			float length = GetLength();
			return Vector2DF(X / length, Y / length);
		}

		/**
		@brief	このベクトルの単位ベクトル化する。
		*/
		void Normalize()
		{
			float length = GetLength();
			(*this) /= length;
		}

		/**
			@brief	このベクトルの向きを弧度法で取得する。
		*/
		float GetRadian() const
		{
			return atan2(Y, X);
		}

		/**
			@brief	このベクトルの向きを弧度法で設定する。
		*/
		void SetRadian(float value)
		{
			float length = GetLength();
			SinCos(value, Y, X);
			X *= length;
			Y *= length;
		}

		/**
			@brief	このベクトルの向きを度数法で取得する。
		*/
		float GetDegree() const
		{
			return RadianToDegree(atan2(Y, X));
		}

		/**
			@brief	このベクトルの向きを度数法で設定する。
		*/
		void SetDegree(float value)
		{
			float length = GetLength();
			SinCos(DegreeToRadian(value), Y, X);
			X *= length;
			Y *= length;
		}

		bool operator==(const Vector2DF& right);

		bool operator!=(const Vector2DF& right);

		Vector2DF operator-();

		Vector2DF operator+(const Vector2DF& right);

		Vector2DF operator-(const Vector2DF& right);

		Vector2DF operator*(const Vector2DF& right);

		Vector2DF operator/(const Vector2DF& right);

		Vector2DF operator*(float right);

		Vector2DF operator/(float right);

		Vector2DF& operator+=(const Vector2DF& right);

		Vector2DF& operator-=(const Vector2DF& right);

		Vector2DF& operator*=(const Vector2DF& right);

		Vector2DF& operator/=(const Vector2DF& right);

		Vector2DF& operator*=(float right);

		Vector2DF& operator/=(float right);
		
		/**
			@brief	内積を取得する。
			@param	v1	値1
			@param	v2	値2
		*/
		static float Dot(const Vector2DF& v1, const Vector2DF& v2)
		{
			return v1.X * v2.X + v1.Y * v2.Y;
		}

		/**
		@brief	外積を取得する。
		@param	v1	値1
		@param	v2	値2
		*/
		static float Cross(const Vector2DF& v1, const Vector2DF& v2)
		{
			return v1.X * v2.Y - v1.Y * v2.X;
		}

		/**
		@brief	2点間の距離を取得する。
		@param	v1	値1
		@param	v2	値2
		*/
		static float Distance(const Vector2DF& v1, const Vector2DF& v2)
		{
			float dx = v1.X - v2.X;
			float dy = v1.Y - v2.Y;
			return sqrt(dx * dx + dy * dy);
		}
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	3次元ベクトル
*/
struct Vector3DF
{
public:
	/**
		@brief	X
	*/
	float	X;

	/**
		@brief	Y
	*/
	float	Y;

	/**
		@brief	Z
	*/
	float	Z;

	/**
		@brief	コンストラクタ
	*/
	Vector3DF();

	/**
	@brief	コンストラクタ
	@param	x	X
	@param	y	Y
	@param	z	Z
	*/
	Vector3DF( float x, float y, float z );

	bool operator == (const Vector3DF& o);
	bool operator != (const Vector3DF& o);

	Vector3DF operator-();

	Vector3DF operator + ( const Vector3DF& o ) const;

	Vector3DF operator - ( const Vector3DF& o ) const;

	Vector3DF operator * (const Vector3DF& o) const;

	Vector3DF operator / (const Vector3DF& o) const;

	Vector3DF operator * ( const float& o ) const;

	Vector3DF operator / ( const float& o ) const;

	Vector3DF& operator += ( const Vector3DF& o );

	Vector3DF& operator -= ( const Vector3DF& o );

	Vector3DF& operator *= (const Vector3DF& o);

	Vector3DF& operator /= (const Vector3DF& o);

	Vector3DF& operator *= ( const float& o );

	Vector3DF& operator /= ( const float& o );


	/**
	@brief	このベクトルの長さを取得する。
	*/
	float GetLength() const
	{
		return sqrt(GetSquaredLength());
	}

	/**
	@brief	このベクトルの長さの二乗を取得する。
	*/
	float GetSquaredLength() const
	{
		return X * X + Y * Y + Z * Z;
	}

	/**
	@brief	このベクトルの長さを設定する。
	*/
	void SetLength(float value)
	{
		float length = GetLength();
		(*this) *= (value / length);
	}

	/**
	@brief	このベクトルの単位ベクトルを取得する。
	*/
	Vector3DF GetNormal()
	{
		float length = GetLength();
		return Vector3DF(X / length, Y / length, Z / length);
	}

	/**
	@brief	このベクトルの単位ベクトル化する。
	*/
	void Normalize()
	{
		float length = GetLength();
		(*this) /= length;
	}

	/**
		@brief	内積を取得する。
	*/
	static float Dot( const Vector3DF& v1, const Vector3DF& v2 );

	/**
		@brief	外積を取得する。
		@note
		右手系の場合、右手の親指がv1、人差し指がv2としたとき、中指の方向を返す。<BR>
		左手系の場合、左手の親指がv1、人差し指がv2としたとき、中指の方向を返す。<BR>
	*/
	static Vector3DF Cross(const Vector3DF& v1, const Vector3DF& v2 );

	/**
		@brief	2点間の距離を取得する。
	*/
	static float Distance(const Vector3DF& v1, const Vector3DF& v2);
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	4次元ベクトル
	*/
	struct Vector4DF
	{
	public:
		/**
		@brief	X
		*/
		float	X;

		/**
		@brief	Y
		*/
		float	Y;

		/**
		@brief	Z
		*/
		float	Z;

		/**
		@brief	W
		*/
		float	W;

		/**
		@brief	コンストラクタ
		*/
		Vector4DF();

		/**
		@brief	コンストラクタ
		@param	x	X
		@param	y	Y
		@param	z	Z
		@param	w	W
		*/
		Vector4DF(float x, float y, float z, float w);


		/**
		@brief	このベクトルの長さを取得する。
		*/
		float GetLength() const
		{
			return sqrt(GetSquaredLength());
		}

		/**
		@brief	このベクトルの長さの二乗を取得する。
		*/
		float GetSquaredLength() const
		{
			return X * X + Y * Y + Z * Z + W * W;
		}

		/**
		@brief	このベクトルの長さを設定する。
		*/
		void SetLength(float value)
		{
			float length = GetLength();
			(*this) *= (value / length);
		}

		/**
		@brief	このベクトルの単位ベクトルを取得する。
		*/
		Vector4DF GetNormal()
		{
			float length = GetLength();
			return Vector4DF(X / length, Y / length, Z / length, W / length);
		}

		/**
		@brief	このベクトルの単位ベクトル化する。
		*/
		void Normalize()
		{
			float length = GetLength();
			(*this) /= length;
		}

		bool operator == (const Vector4DF& o);
		bool operator != (const Vector4DF& o);

		Vector4DF operator-();

		Vector4DF operator + (const Vector4DF& o) const;

		Vector4DF operator - (const Vector4DF& o) const;

		Vector4DF operator * (const Vector4DF& o) const;

		Vector4DF operator / (const Vector4DF& o) const;

		Vector4DF operator * (const float& o) const;

		Vector4DF operator / (const float& o) const;

		Vector4DF& operator += (const Vector4DF& o);

		Vector4DF& operator -= (const Vector4DF& o);

		Vector4DF& operator *= (const Vector4DF& o);

		Vector4DF& operator /= (const Vector4DF& o);

		Vector4DF& operator *= (const float& o);

		Vector4DF& operator /= (const float& o);

		/**
		@brief	内積を取得する。
		*/
		static float Dot(const Vector4DF& v1, const Vector4DF& v2);

		/**
		@brief	2点間の距離を取得する。
		*/
		static float Distance(const Vector4DF& v1, const Vector4DF& v2);
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

namespace ace
{
	/**
		@brief	3×3行列を表す構造体。
		@note
		[0,0][0,1]
		[1,0][1,1]
		*/
	struct Matrix33
	{
	public:
		/**
			@brief	オブジェクトを生成し、単位行列で初期化する。
		*/
		Matrix33();

		/**
			@brief	行列の値
			*/
		float Values[3][3];

		/**
			@brief	単位行列を設定する。
			*/
		Matrix33& SetIdentity();

		/**
		@brief	転置行列を設定する。
		@return	このインスタンスへの参照
		*/
		Matrix33& SetTransposed();

		/**
		@brief	逆行列を設定する。
		@return	このインスタンスへの参照
		*/
		Matrix33& SetInverted();

		/**
		@brief	逆行列を取得する。
		@return	逆行列
		*/
		Matrix33 GetInverted();

		/**
			@brief	平行移動の行列を設定する。
			*/
		Matrix33& SetTranslation(float x, float y);

		/**
		@brief	回転行列を設定する。
		*/
		Matrix33& SetRotation(float angle);

		/**
			@brief	拡大・縮小行列を設定する。
			*/
		Matrix33& SetScale(float x, float y);

		/**
		@brief	行列でベクトルを変形させる。
		@param	in	変形前ベクトル
		@return	変形後ベクトル
		*/
		Vector2DF Transform2D(const Vector2DF& in) const;

		/**
		@brief	行列でベクトルを変形させる。
		@param	in	変形前ベクトル
		@return	変形後ベクトル
		*/
		Vector3DF Transform3D(const Vector3DF& in) const;

		/**
		@brief	3×3行列の掛け算を計算する。
			*/
		Matrix33 operator*(const Matrix33& right);

		Vector3DF operator*(const Vector3DF& right);
	};
}


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief	4×4行列を表す構造体
	@note
	M * V[x,y,z,1] の形<BR>
	[0,0][0,1][0,2][0,3]
	[1,0][1,1][1,2][1,3]
	[2,0][2,1][2,2][2,3]
	[3,0][3,1][3,2][3,3]
*/
struct Matrix44
{
private:

public:

	/**
		@brief	オブジェクトを生成し、単位行列で初期化する。
	*/
	Matrix44();

	/**
		@brief	行列の値
	*/
	float	Values[4][4];

	/**
		@brief	単位行列を設定する。
		@return	このインスタンスへの参照
	*/
	Matrix44& SetIndentity();

	/**
		@brief	転置行列を設定する。
		@return	このインスタンスへの参照
	*/
	Matrix44& SetTransposed();

	/**
		@brief	逆行列を設定する。
		@return	このインスタンスへの参照
	*/
	Matrix44& SetInverted();

	/**
		@brief	逆行列を取得する。
		@return	逆行列
	*/
	Matrix44 GetInverted();


	/**
		@brief	カメラ行列(右手系)を設定する。
		@param	eye	カメラの位置
		@param	at	カメラの注視点
		@param	up	カメラの上方向
		@return	このインスタンスへの参照
	*/
	Matrix44& SetLookAtRH( const Vector3DF& eye, const Vector3DF& at, const Vector3DF& up );

	/**
		@brief	カメラ行列(左手系)を設定する。
		@param	eye	カメラの位置
		@param	at	カメラの注視点
		@param	up	カメラの上方向
		@return	このインスタンスへの参照
	*/
	Matrix44& SetLookAtLH(const Vector3DF& eye, const Vector3DF& at, const Vector3DF& up);

	/**
		@brief	射影行列(右手系)を設定する。
		@param	ovY	Y方向への視野角(ラジアン)
		@param	aspect	画面のアスペクト比
		@param	zn	最近距離
		@param	zf	最遠距離
		@return	このインスタンスへの参照
	*/
	Matrix44& SetPerspectiveFovRH(float ovY, float aspect, float zn, float zf);

	/**
		@brief	OpenGL用射影行列(右手系)を設定する。
		@param	ovY	Y方向への視野角(ラジアン)
		@param	aspect	画面のアスペクト比
		@param	zn	最近距離
		@param	zf	最遠距離
		@return	このインスタンスへの参照
	*/
	Matrix44& SetPerspectiveFovRH_OpenGL(float ovY, float aspect, float zn, float zf);

	/**
		@brief	射影行列(左手系)を設定する。
		@param	ovY	Y方向への視野角(ラジアン)
		@param	aspect	画面のアスペクト比
		@param	zn	最近距離
		@param	zf	最遠距離
		@return	このインスタンスへの参照
	*/
	Matrix44& SetPerspectiveFovLH(float ovY, float aspect, float zn, float zf);

	/**
		@brief	正射影行列(右手系)を設定する。
		@param	width	横幅
		@param	height	縦幅
		@param	zn	最近距離
		@param	zf	最遠距離
		@return	このインスタンスへの参照
	*/
	Matrix44& SetOrthographicRH(float width, float height, float zn, float zf);

	/**
		@brief	正射影行列(左手系)を設定する。
		@param	width	横幅
		@param	height	縦幅
		@param	zn	最近距離
		@param	zf	最遠距離
		@return	このインスタンスへの参照
	*/
	Matrix44& SetOrthographicLH(float width, float height, float zn, float zf);

	/**
		@brief	並行移動行列を設定する。
		@param	x	X方向移動量
		@param	y	Y方向移動量
		@param	z	Z方向移動量
		@return	このインスタンスへの参照
	*/
	Matrix44& SetTranslation(float x, float y, float z);

	/**
		@brief	X軸回転行列(右手)を設定する。
		@param	angle	X軸回転量(ラジアン)
		@return	このインスタンスへの参照
	*/
	Matrix44& SetRotationX(float angle);

	/**
		@brief	Y軸回転行列(右手)を設定する。
		@param	angle	Y軸回転量(ラジアン)
		@return	このインスタンスへの参照
	*/
	Matrix44& SetRotationY(float angle);

	/**
		@brief	Z軸回転行列(右手)を設定する。
		@param	angle	Z軸回転量(ラジアン)
		@return	このインスタンスへの参照
	*/
	Matrix44& SetRotationZ(float angle);

	/**
	@brief	任意軸の反時計回転行列(右手)を設定する。
	@param	axis	軸
	@param	angle	回転量(ラジアン)
	@return	このインスタンスへの参照
	*/
	Matrix44& SetRotationAxis(const Vector3DF& axis, float angle);

	/**
	@brief	クオータニオンを元に回転行列(右手)を設定する。
	@param	x	クオータニオン
	@param	y	クオータニオン
	@param	z	クオータニオン
	@param	w	クオータニオン
	@return	このインスタンスへの参照
	*/
	Matrix44& SetQuaternion(float x, float y, float z, float w);

	/**
	@brief	拡大行列を設定する。
	@param	x	X方向拡大率
	@param	y	Y方向拡大率
	@param	z	Z方向拡大率
	@return	このインスタンスへの参照
	*/
	Matrix44& SetScale(float x, float y, float z);

	/**
	@brief	行列でベクトルを変形させる。
	@param	in	変形前ベクトル
	@return	変形後ベクトル
	*/
	Vector3DF Transform3D(const Vector3DF& in) const;

	/**
	@brief	行列でベクトルを変形させる。
	@param	in	変形前ベクトル
	@return	変形後ベクトル
	*/
	Vector4DF Transform4D(const Vector4DF& in) const;

	Matrix44 operator * (const Matrix44& right) const;

	Vector3DF operator*(const Vector3DF& right) const;

	Vector4DF operator*(const Vector4DF& right) const;

	/**
		@brief	乗算を行う。
		@param	o	出力先
		@param	in1	行列1
		@param	in2	行列2
		@return	出力先の参照
	*/
	static Matrix44& Mul( Matrix44& o, const Matrix44& in1, const Matrix44& in2 );

};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


#include <array>

namespace ace
{
	/**
		@brief	長方形(整数)の構造体
	*/
	struct RectI
	{
	public:
		/**
			@brief	左上X座標
		*/
		int X;

		/**
			@brief	左上Y座標
		*/
		int Y;

		/**
			@brief	横幅
		*/
		int Width;
		
		/**
			@brief	縦幅
		*/
		int Height;

		/**
			@brief	コンストラクタ
		*/
		RectI();

		/**
			@brief	コンストラクタ
			@param	x	左上X座標
			@param	y	左上Y座標
			@param	width	横幅
			@param	height	縦幅
		*/
		RectI(int x, int y, int width, int height);

		/**
			@brief	左上座標を取得する。
			@return	左上座標
		*/
		Vector2DI GetPosition() const;

		/**
			@brief	大きさを取得する。
			@return	大きさ
		*/
		Vector2DI GetSize() const;

		/**
			@brief	四隅の座標を(X,Y)、(X+Width,Y)、(X+Width,Y+Height)、(X,Y+Height)の順に配列として取得する。
			@return	座標
		*/
		std::array<Vector2DI, 4> GetVertexes() const;

		bool operator == (const RectI& other) const;
	};
}


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include <array>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
	/**
	@brief	長方形(浮動小数点)の構造体
	*/
	struct RectF
	{
	public:
		/**
		@brief	左上X座標
		*/
		float X;

		/**
		@brief	左上Y座標
		*/
		float Y;

		/**
		@brief	横幅
		*/
		float Width;

		/**
		@brief	縦幅
		*/
		float Height;

		/**
		@brief	コンストラクタ
		*/
		RectF();

		/**
		@brief	コンストラクタ
		@param	x	左上X座標
		@param	y	左上Y座標
		@param	width	横幅
		@param	height	縦幅
		*/
		RectF(float x, float y, float width, float height);

		/**
		@brief	左上座標を取得する。
		@return	左上座標
		*/
		Vector2DF GetPosition() const;

		/**
		@brief	大きさを取得する。
		@return	大きさ
		*/
		Vector2DF GetSize() const;

		/**
		@brief	四隅の座標を(X,Y)、(X+Width,Y)、(X+Width,Y+Height)、(X,Y+Height)の順に配列として取得する。
		@return	座標
		*/
		std::array<Vector2DF, 4> GetVertexes() const;
	};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

#include<string>
#if defined(_MSC_VER)

#elif defined(__clang__)
#include<cxxabi.h>
#include<cstdlib>
#include<cassert>
#include<typeinfo>
#if !__has_include(<cxxabi.h>)
#error "cxxabi.h is not found"
#endif
#elif defined(__GNUC__)
#include<cxxabi.h>
#include<cstdlib>
#include<cassert>
#include<typeinfo>
#endif

namespace ace
{
/**
	@brief	型から型名を人間の読める形の文字列として得る
	@detail	対応コンパイラ: msvc, gcc, clang.
*/
template<typename T> std::string GetTypeName()
{
#ifdef _MSC_VER
#ifndef _CPPRTTI
#error "/GR option is needed to get type names"
#endif
	return std::string(typeid(T).name());
#else
	int stat;
	std::string ret;
	char *pc = abi::__cxa_demangle(typeid(T).name(), 0, 0, &stat);
	switch (stat)
	{
	case 0:
		ret = std::string(pc);
		free(pc);
		return ret;
		break;
	case -1:
		assert(!"failed to memory allocation");
		return ret;
		break;
	case -2:
		assert(!"the type name couldn't be demangled");
		return ret;
		break;
	case -3:
		assert(!"there's an illegal argument");
		return ret;
		break;
	default:
		return ret;
		break;
	}
#endif
}

}
#include<cstddef>
#include<cassert>
namespace ace
{

/**
	@brief	確保されたメモリ領域の先頭ポインタと長さから、デバッグ時境界検査つき配列を提供する
	@details	確保されたメモリの所有権は移動しないので、使用する側でメモリを開放する必要がある
*/

template<typename T> class PointerArray
{
public:
	T* m_data; //!< 配列の先頭ポインタ
protected:
	size_t m_length; //!< 配列の長さ
public:
	typedef T Type; //!< 対象となっている型
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
		@brief	配列の長さを得る
	*/
	size_t GetLength() const{return m_length;}
	
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
		@brief	コンストラクタ
		@detail	T*はlength確保した配列と見なす。所有権は移動しない
		@param	idx	[in]	
	*/
	//! 
	PointerArray(T* ptr, size_t length): m_data(ptr), m_length(length){}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
		@brief	idx番目の要素への参照を得る
		@detail	デバッグ時には範囲外チェックがつく
		@param	idx	[in]	要素のインデックス
	*/
	T& At(size_t idx)
	{
		assert(idx >= 0 && idx < m_length);
		return m_data[idx];
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
		@brief	idx番目の要素への参照を得る
		@detail	デバッグ時には範囲外チェックがつく
		@param	idx	[in]	要素のインデックス
	*/
	T& operator[](size_t idx)
	{
		return At(idx);
	}
};
}
#include<cstdint>

#ifdef _WIN32
/* win */
#include<Windows.h>
#else
/* *nix */
#include <sys/time.h>
#endif

namespace ace
{
/**
	@brief	パフォーマンスカウンタの現在の値をナノ秒単位で返す
*/

inline int64_t GetTime()
{
#ifdef _WIN32
	int64_t count, freq;
	if (QueryPerformanceCounter((LARGE_INTEGER*)&count))
	{
		if (QueryPerformanceFrequency((LARGE_INTEGER*)&freq))
		{
			// オーバーフロー対策
			// return count * 1000000 / freq; と等価

			int64_t ret = 0;
			ret = count / freq;
			count -= ret * freq;
			ret *= 1000000;
			ret += count * 1000000 / freq;
			return ret;
		}
	}
	return 0;
#else
	struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000000 + (int64_t)tv.tv_usec;
#endif
}
}
#include<cassert>
#include<cstring>
namespace ace
{

/**
	@brief	型は異なるが同じ構造を持つ2変数の間でコピーする
	@details	T, Uが同じサイズかを検査し、そのサイズ分fromからtoにコピーする
	@warning	とても危険な関数なため、使用の際には細心の注意を要する。
	@param	from	[in]	コピー元のポインタ
	@param	to		[out]	コピー先のポインタ
*/
template<typename T, typename U> void TypeErasureCopy(T const* from, U* to)
{
	
#if __cplusplus >= 201103L
	static_assert(alignof(T) == alignof(U), "arguments 'from' and 'to' must be the same alignment.");
	static_assert(sizeof(T) == sizeof(U), "arguments 'from' and 'to' must be the same size.");
#else
	assert(sizeof(T) == sizeof(U));
#endif 
	memcpy(to, from, sizeof(T));
}
}
//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	回転行列の計算順序
	*/
	enum eRotationOrder
	{
		ROTATION_ORDER_QUATERNION = 10,
		ROTATION_ORDER_XZY = 11,
		ROTATION_ORDER_XYZ = 12,
		ROTATION_ORDER_ZXY = 13,
		ROTATION_ORDER_ZYX = 14,
		ROTATION_ORDER_YXZ = 15,
		ROTATION_ORDER_YZX = 16,
		ROTATION_ORDER_AXIS = 18,
		ROTATION_ORDER_MAX = 0xfffffff
	};

	/**
	@brief	キー間の補間方法
	*/
	enum eInterpolationType
	{
		/**
		@brief	次のキーまで定数で補間
		*/
		INTERPOLATION_TYPE_CONSTANT = 0,

		/**
		@brief	次のキーまで線形で補間
		*/
		INTERPOLATION_TYPE_LINEAR = 1,

		/**
		@brief	次のキーまで三次方程式で補間
		*/
		INTERPOLATION_TYPE_CUBIC = 2,

		/**
		@brief	int型指定
		*/
		INTERPOLATION_TYPE_MAX = 0xfffffff
	};

	/**
	@brief	Fカーブのキーフレーム
	*/
	struct FCurveKeyframe
	{
	public:
		/**
		@brief	時間と値
		*/
		Vector2DF				KeyValue;

		/**
		@brief	制御用ハンドル
		*/
		Vector2DF				LeftHandle;

		/**
		@brief	制御用ハンドル
		*/
		Vector2DF				RightHandle;

		/**
		@brief	補間方法
		*/
		eInterpolationType		InterpolationType;

		FCurveKeyframe()
		{
			InterpolationType = eInterpolationType::INTERPOLATION_TYPE_LINEAR;
		}
	};


	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	色
	*/
	struct Color
	{
	public:

		/**
		@brief	赤
		*/
		uint8_t	R;

		/**
		@brief	緑
		*/
		uint8_t	G;

		/**
		@brief	青
		*/
		uint8_t	B;

		/**
		@brief	透明度
		*/
		uint8_t	A;

		/**
		@brief	コンストラクタ
		*/
		Color();

		/**
		@brief	コンストラクタ
		@param	r	赤
		@param	g	緑
		@param	b	青
		@param	a	透明度
		*/
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

#include<deque>
#include<cstdlib>
#include<algorithm>
#include<cstdint>
#include<climits>
#include<cassert>

namespace ace
{

class BinaryReader
{
protected:
	std::deque<int8_t> data;
public:
	template<typename itrT> void ReadIn(itrT beg, itrT end)
	{
		//std::copy<itrT, std::deque<int8_t>::iterator>(beg, end, data.begin());
		for (itrT itr = beg; itr != end; itr++)
		{
			data.push_back(*itr);
		}

	}

	template<typename T> T Get(){ assert(!"The type cannot be serializable."); return T();};
	bool IsEmpty()const{ return data.empty(); }

};
template<> inline int32_t BinaryReader::Get()
{
	int8_t cs[4];
	for (int i = 0; i < 4; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return static_cast<int32_t>(0);
		}
		cs[i] = data.front();
		data.pop_front();
	}

	return *(static_cast<int32_t*>(static_cast<void*>(cs)));
}

template<> inline int16_t BinaryReader::Get()
{
	int8_t cs[2];
	for (int i = 0; i < 2; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return static_cast<int16_t>(0);
		}
		cs[i] = data.front();
		data.pop_front();
	}

	return *(static_cast<int16_t*>(static_cast<void*>(cs)));
}

template<> inline int8_t BinaryReader::Get()
{
	int8_t c;
	assert(!data.empty());
	if (data.empty()){
		return static_cast<int8_t>(0);
	}
	c = data.front();
	data.pop_front();

	return c;
}

template<> inline uint8_t BinaryReader::Get()
{
	uint8_t c;
	assert(!data.empty());
	if (data.empty()){
		return static_cast<uint8_t>(0);
	}
	c = data.front();
	data.pop_front();

	return c;
}


template<> inline std::string BinaryReader::Get()
{
	int8_t lenCs[4];
	std::string str = std::string();
	for (int i = 0; i < 4; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return str;
		}
		lenCs[i] = data.front();
		data.pop_front();
	}

	int32_t const len = *(static_cast<int32_t*>(static_cast<void*>(lenCs)));

	int8_t chr;

	for (int32_t i = 0; i < len; i++)
	{

		assert(!data.empty());
		if (data.empty()){
			return str;
		}
		chr = data.front();
		data.pop_front();

		str.push_back(*(static_cast<char*>(static_cast<void*>(&chr))));

	}
	return str;
}
template<> inline ace::astring BinaryReader::Get()
{
	int8_t lenCs[4];
	ace::astring astr = ace::astring();
	for (int i = 0; i < 4; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return astr;
		}
		lenCs[i] = data.front();
		data.pop_front();
	}

	int32_t const len =  *(static_cast<int32_t*>(static_cast<void*>(lenCs)));

	int8_t charCs[2];

	for (int32_t i = 0; i < len; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			assert(!data.empty());
			if (data.empty()){
				return astr;
			}
			charCs[j] = data.front();
			data.pop_front();
		}

		astr.push_back(*(static_cast<ace::achar*>(static_cast<void*>(charCs))));

	}
	return astr;
}

template<> inline ace::achar* BinaryReader::Get()
{
	int8_t lenCs[4];
	for (int i = 0; i < 4; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return nullptr;
		}
		lenCs[i] = data.front();
		data.pop_front();
	}

	int32_t const len = *(static_cast<int32_t*>(static_cast<void*>(lenCs)));

	ace::achar *achs = static_cast<ace::achar*>(calloc(len + 1, sizeof(ace::achar)));
	int8_t charCs[2];

	for (int32_t i = 0; i < len; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			assert(!data.empty());
			if (data.empty()){
				return achs;
			}
			charCs[j] = data.front();
			data.pop_front();
		}

		achs[i] = *(static_cast<ace::achar*>(static_cast<void*>(charCs)));

	}
	return achs;
}

template<> inline float BinaryReader::Get()
{
	int8_t cs[4];
	for (int i = 0; i < 4; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return static_cast<float>(0);
		}
		cs[i] = data.front();
		data.pop_front();
	}

	return *(static_cast<float*>(static_cast<void*>(cs)));
}


template<> inline Vector3DF BinaryReader::Get()
{
	Vector3DF v;
	v.X = Get<float>();
	v.Y = Get<float>();
	v.Z = Get<float>();

	return v;

}

template<> inline Vector2DF BinaryReader::Get()
{
	Vector2DF v;
	v.X = Get<float>();
	v.Y = Get<float>();

	return v;
}

template<> inline Color BinaryReader::Get()
{
	Color c;
	c.R = Get<uint8_t>();
	c.G = Get<uint8_t>();
	c.B = Get<uint8_t>();
	c.A = Get<uint8_t>();

	return c;
}

template<> inline FCurveKeyframe BinaryReader::Get()
{
	FCurveKeyframe keyframe;

	keyframe.KeyValue = Get<Vector2DF>();
	keyframe.LeftHandle = Get<Vector2DF>();
	keyframe.RightHandle = Get<Vector2DF>();
	keyframe.InterpolationType = (ace::eInterpolationType)Get<int32_t>();

	return keyframe;
}

template<> inline eRotationOrder BinaryReader::Get()
{
	eRotationOrder rotationOrder;

	rotationOrder = (ace::eRotationOrder)Get<int32_t>();

	return rotationOrder;
}

template<> inline Matrix44 BinaryReader::Get()
{
	Matrix44 m;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			m.Values[j][i] = Get<float>();
		}
	}

	return m;

}

} 
#include<vector>
#include<cstdint>
#include<climits>
#include<fstream>
#include<string>

namespace ace
{

/**
@brief	数値、文字列をバイト列にシリアライズし、ファイルに書き出すクラス
@detail	対応する型はint32_t, int16_t, int8_t, uint8_t, std::string, astring, achar*
@warning	エンディアンは現状非互換、正当性の検査等はないので、
データを読み出す際はBinaryReaderで*書き出した順に*読み出すこと
*/
class BinaryWriter
{
protected:
	std::vector<int8_t> m_data; ///< シリアライズされたバイト列
public:
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	扱うデータが大きいことが予想される場合にあらかじめ容量を確保する
	@param	size	確保するサイズ
	*/

	void Reserve(size_t size)
	{
		m_data.reserve(size);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	シリアライズされたバイト列をconst参照で返す。
	@warning	本クラスのインスタンスより寿命を長く取る際はコピーなどして懸垂参照を避けること
	*/

	std::vector<int8_t> const& Get() const
	{
		return m_data;
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	int32_tをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする整数
	*/
	void Push(int32_t content){
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&content));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	int16_tをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする整数
	*/
	
	void Push(int16_t content){
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&content));
		for (int i = 0; i < 2; i++)
		{
			m_data.push_back(pc[i]);
		}
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	int8_tをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする整数
	*/
	void Push(int8_t content){

		m_data.push_back(content);

	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	uint8_tをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする整数
	*/
	void Push(uint8_t content){

		m_data.push_back(*((uint8_t*)(&content)));

	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	std::stringをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする文字列
	*/
	void Push(std::string const& content)
	{
		// TODO: string::lengthの返値が64bitでもたれていた場合に危険
		int32_t l = (int32_t)content.length();

		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&l));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
		for (int32_t i = 0; i < l; i++)
		{
			m_data.push_back(static_cast<int8_t>(content.at(i)));
		}

	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	astringをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする文字列
	*/
	void Push(ace::astring const& content)
	{
		auto l = content.length();

		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&l));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
		for (uint32_t i = 0; i < l; i++)
		{
			//copy
			achar a = content.at(i);
			int8_t* pcs = static_cast<int8_t*>(static_cast<void*>(&a));
			m_data.push_back(static_cast<int8_t>(pcs[0]));
			m_data.push_back(static_cast<int8_t>(pcs[1]));
		}

	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	achar*をシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする文字列の先頭ポインタ
	@param	size	シリアライズする文字列のサイズ
	*/
	void Push(ace::achar const* content, int32_t size)
	{
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&size));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
		for (int32_t i = 0; i < size; i++)
		{
			//copy
			achar a = content[i];
			int8_t* pcs = static_cast<int8_t*>(static_cast<void*>(&a));
			m_data.push_back(static_cast<int8_t>(pcs[0]));
			m_data.push_back(static_cast<int8_t>(pcs[1]));
		}

	}


	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	floatをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする実数
	*/
	void Push(float content){
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&content));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	ace::Vector3DFをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズするVector
	*/
	void Push(ace::Vector3DF const& content){
		Push(content.X);
		Push(content.Y);
		Push(content.Z);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	ace::Vector2DFをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズするVector
	*/
	void Push(ace::Vector2DF const& content){
		Push(content.X);
		Push(content.Y);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	ace::Matrix44をシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズするVector
	*/
	void Push(ace::Matrix44 const& content){
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				Push(content.Values[j][i]);
			}
		}

	}



	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	保持しているバイト列をストリームに書き出す。
	@param	os	書き出すストリーム
	@return 成功すればtrue, 失敗すればfalse
	*/

	bool WriteOut(std::ostream& os) const
	{
		if (!os){ return false; }
		for (auto itr = m_data.cbegin(); itr != m_data.cend(); itr++)
		{
			os << *itr;
		}
		return true;
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	保持しているバイト列をファイルに書き出す。
	@param	filename	書き出すファイル名
	@param	isAppend	追記するか否か
	@return 成功すればtrue, 失敗すればfalse
	*/
	bool WriteOut(std::string const& filename, bool isAppend = false) const
	{
		std::ofstream os(
			filename,
			std::ios_base::binary | std::ios_base::out | (isAppend ? std::ios_base::app : std::ios_base::out)
		);
		if (!os) {
			return false;
		}
		if (!WriteOut(os)) { return false; }
		os.close();
		return true;
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	デストラクタ
	*/
	virtual ~BinaryWriter(){}
};
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Core;
	class Graphics;
	class Window;
	class Keyboard;
	class Mouse;
	class Log;
	class Profiler;
	class ProfilerViewer;
	class Renderer2D;

	class Joystick;
	class JoystickContainer;
	
	class File;
	class Path;
	class StaticFile;
	class StreamFile;

	class Sound;
	class SoundSource;

	class Texture;
	class Texture2D;
	class RenderTexture2D;
	class CubemapTexture;

	class Shader2D;
	class Material2D;

	class Effect;

	class Model;

	class Font;

	class Chip2D;

	class Mesh;
	class Deformer;

	class AnimationSystem;
	class AnimationClip;
	class AnimationSource;
	class KeyframeAnimation;


	class ObjectSystemFactory;
	class CoreScene;
	class CoreLayer;

	class CoreLayer2D;
	class CoreObject2D;
	class CoreTextureObject2D;
	class CoreCameraObject2D;
	class CoreTextObject2D;
	class CoreEffectObject2D;
	class CoreMapObject2D;

	class CoreLayer3D;
	class CoreObject3D;
	class CoreModelObject3D;
	class CoreCameraObject3D;
	class CoreEffectObject3D;
	class CoreDirectionalLightObject3D;

	class CorePostEffect;

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	enum class WritingDirection :int
	{
		Vertical,
		Horizontal,
	};

	enum eTextureFormat
	{
		TEXTURE_FORMAT_R8G8B8A8_UNORM = 0,
		TEXTURE_FORMAT_R32G32B32A32_FLOAT = 1,
		TEXTURE_FORMAT_R8G8B8A8_UNORM_SRGB = 2,
		TEXTURE_FORMAT_GL_R16G16_FLOAT = 3,
	};

	enum eTextureClassType
	{
		TEXTURE_CLASS_TEXTURE2D = 0,
		TEXTURE_CLASS_RENDERTEXTURE2D = 1,
		TEXTURE_CLASS_CUBEMAPTEXTURE = 2,
		TEXTURE_CLASS_DEPTHBUFFER = 3,

	};

	/**
		@brief	描画時のブレンドモードを表す列挙体
	*/
	enum class AlphaBlend : int32_t
	{
		/// <summary>
		/// 不透明
		/// </summary>
		Opacity = 0,
		/// <summary>
		/// 透明
		/// </summary>
		Blend = 1,
		/// <summary>
		/// 加算
		/// </summary>
		Add = 2,
		/// <summary>
		/// 減算
		/// </summary>
		Sub = 3,
		/// <summary>
		/// 乗算
		/// </summary>
		Mul = 4,
	};

	enum class TextureFilterType : int32_t
	{
		Nearest = 0,
		Linear = 1,
	};

	enum class TextureWrapType : int32_t
	{
		Repeat = 0,
		Clamp = 1,
	};

	enum eCullingType
	{
		CULLING_FRONT = 0,
		CULLING_BACK = 1,
		CULLING_DOUBLE = 2,

		CULLING_DWORD = 0x7fffffff,
	};

	/**
		@brief	描画方法
	*/
	enum class GraphicsDeviceType : int32_t
	{
		Default = 0,		///< 実行環境で最も安定している描画方法(初期化時に使用)
		DirectX11 = 1,		///< DirectX11
		OpenGL = 2,			///< OpenGL
	};

	enum class Object2DType : int32_t
	{
		Unknown,
		Texture,
		Camera,
		Effect,
		Text,
		Map,
	};

	enum eRenderedObject3DType
	{
		RENDERED_OBJECT3D_TYPE_UNKNOWN,
		RENDERED_OBJECT3D_TYPE_MESH,
		RENDERED_OBJECT3D_TYPE_CAMERA,
		RENDERED_OBJECT3D_TYPE_EFFECT,
		RENDERED_OBJECT3D_TYPE_DIRECTIONALLIGHT,
	};

	/**
		@brief	シェーダーに対して外部から設定可能なプロパティの変数の種類
	*/
	enum ShaderVariableType
	{
		SHADER_VARIABLE_TYPE_FLOAT,
		SHADER_VARIABLE_TYPE_VECTOR2DF,
		SHADER_VARIABLE_TYPE_VECTOR3DF,
		SHADER_VARIABLE_TYPE_VECTOR4DF,
		SHADER_VARIABLE_TYPE_TEXTURE2D,
	};

	/**
		@brief	シェーダーに対して外部から設定可能なプロパティの情報
	*/
	class ShaderVariableProperty
	{
	public:
		/**
			@brief	名称
		*/
		astring	Name;

		/**
			@brief	種類
		*/
		ShaderVariableType Type;

		/**
			@brief	定数バッファ上での先頭からのオフセット
		*/
		int32_t	Offset;
	};

	/**
	@brief	3D描画時に表示されるバッファ
	*/
	enum eVisalizedBuffer
	{
		VISALIZED_BUFFER_FINALIMAGE,
		VISALIZED_BUFFER_DIFFUSE,
		VISALIZED_BUFFER_NORMAL,
	};

	/**
		@brief	描画設定のクラス
	*/
	class RenderSettings
	{
	public:
		/**
			@brief	遅延レンダリングから軽量レンダリングに変更し高速に描画するか?
		*/
		bool IsLightweightMode;
		
		/**
			@brief	画面に表示されるバッファ
		*/
		eVisalizedBuffer VisalizedBuffer;

		RenderSettings()
		{
			IsLightweightMode = false;
			VisalizedBuffer = VISALIZED_BUFFER_FINALIMAGE;
		}
	};

	/**
	@brief	参照カウンタのインターフェース
	*/
	class IReference
	{
	public:
		/**
			@brief	参照カウンタを加算する。
			@return	加算後の参照カウンタ
		*/
		virtual int AddRef() = 0;

		/**
		@brief	参照カウンタを取得する。
		@return	参照カウンタ
		*/
		virtual int GetRef() = 0;

		/**
		@brief	参照カウンタを減算する。0になった時、インスタンスを削除する。
		@return	減算後の参照カウンタ
		*/
		virtual int Release() = 0;
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <memory>
#include <atomic>


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	参照カウンタオブジェクト
	*/
	class ReferenceObject
		: public IReference
	{
	private:
		mutable std::atomic<int32_t> m_reference;

	public:
		ReferenceObject();

		virtual ~ReferenceObject();

		virtual int AddRef();

		virtual int GetRef();

		virtual int Release();
	};

#if !SWIG
	/**
	@brief	全ての参照カウンタの値の合計を取得する。
	@return	参照カウンタ合計値
	@note
	実体はEngineに存在するのでそちらを参照する。
	*/
	int32_t GetGlobalReferenceCount();

	/**
	@brief	全ての参照カウンタの値を取得し、0ならDLLを解放する。
	@note
	実体はEngineに存在するのでそちらを参照する。
	*/
	void CheckDLL();

	template <typename T>
	struct ReferenceDeleter
	{
		void operator ()(T* p)
		{
			auto p_ = ((IReference*) p);
			SafeRelease(p_);
		}
	};

	template <typename T>
	struct ReferenceDeleterWithReleaseDLL
	{
		void operator ()(T* p)
		{
			auto p_ = ((IReference*) p);
			SafeRelease(p_);

			auto ref = GetGlobalReferenceCount();
			if (ref == 0)
			{
				CheckDLL();
			}
		}
	};

	template <typename T>
	static std::shared_ptr<T> CreateSharedPtr(T* p)
	{		
		return std::shared_ptr<T>(p, ReferenceDeleter<T>());
	}

	template <typename T>
	static std::shared_ptr<T> CreateSharedPtrWithReleaseDLL(T* p)
	{
		return std::shared_ptr<T>(p, ReferenceDeleterWithReleaseDLL<T>());
	}

#endif

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};

namespace ace{

	enum class MouseButtonState:int
	{
		Push,Pull,Hold,Free
	};

	class IMouseButtonState
	{
	public:
		virtual const MouseButtonState GetButtonState() const = 0;
	};
};


namespace ace{


	class IMouseWheelableButtonState:public IMouseButtonState
	{
	public:

		virtual const MouseButtonState GetButtonState() const = 0;

		virtual const double GetRotation() const = 0;
	};
};


namespace ace{

	class SideButton:public IMouseButtonState
	{
	private:
		MouseButtonState m_mouseButtonState;
	public:
		const MouseButtonState GetButtonState() const;
		SideButton(){}
		SideButton(MouseButtonState mouseButtonState,bool doubleClicked);
	};
};


namespace ace{

	class MiddleButton : public IMouseWheelableButtonState
	{
	private:
		MouseButtonState m_mouseButtonState;
		double m_rotation;
	public:
		const MouseButtonState GetButtonState() const;
		const double GetRotation() const;
		MiddleButton(MouseButtonState mouseButtonState,double rotation);
		MiddleButton(){}
	};
};

#include <memory>
namespace ace{
	class Mouse
	{
	public:
		virtual const SideButton* GetLeftButton() const = 0;


		virtual const SideButton* GetRightButton() const = 0;


		virtual const MiddleButton* GetMiddleButton() const = 0;


		virtual const Vector2DF GetPosition() const = 0;

	};
};

namespace ace{

	enum class Keys:int
	{
		Unknown,
		Space,
		Apostrophe,
		Comma,
		Minus,
		Period,
		Slash,
		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
		Semicolon,
		Equal,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		LeftBracket,
		Backslash,
		RightBracket,
		GraveAccent,
		World1,
		World2,
		Escape,
		Enter,
		Tab,
		Backspace,
		Insert,
		Delete,
		Right,
		Left,
		Down,
		Up,
		PageUp,
		PageDown,
		Home,
		End,
		CapsLock,
		ScrollLock,
		NumLock,
		PrintScreen,
		Pause,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		F16,
		F17,
		F18,
		F19,
		F20,
		F21,
		F22,
		F23,
		F24,
		F25,
		Keypad0,
		Keypad1,
		Keypad2,
		Keypad3,
		Keypad4,
		Keypad5,
		Keypad6,
		Keypad7,
		Keypad8,
		Keypad9,
		KeypadDecimal,
		KeypadDivide,
		KeypadMultiply,
		KeypadSubstract,
		KeypadAdd,
		KeypadEnter,
		KeypadEqual,
		LeftShift,
		LeftControl,
		LeftAlt,
		LeftWin,
		RightShift,
		RightControl,
		RightAlt,
		RightWin,
		Menu,
		Last,
		MAX
	};

	enum class KeyState:int
	{
		Push, Pull, Hold, Free
	};

	class Keyboard
	{
	public:
		virtual const KeyState GetKeyState(Keys key) = 0;
	};

};

namespace ace{

	enum class JoystickButtonState:int
	{
		Push,
		Pull,
		Free,
		Hold
	};

	enum eJoystickButtons
	{
		BUTTON_1
	};

	enum eJoystickAxises
	{
		AXIS_1
	};

	class Joystick
	{
	public:
		Joystick(){}
		virtual ~Joystick(){}

		virtual const ace::achar* GetJoystickName() = 0;


		virtual const int GetButtonsCount() = 0;


		virtual const int GetAxesCount() = 0;


		virtual const JoystickButtonState GetButtonState(int at) = 0;


		virtual const float GetAxisState(int at) = 0;
	};
};



namespace ace{
	class JoystickContainer
	{
	public:
		JoystickContainer(){}
		virtual ~JoystickContainer(){}

		virtual const bool IsPresentAt(int at) = 0;


		virtual Joystick* GetJoystickAt(int at) = 0;
	};
};

#include <string>
#include <memory>

namespace ace
{
	class Log
	{
	public:
		enum eLogLevel
		{
			LOG_ERROR = 1 << 1,
			LOG_CRITICAL = 1 << 2,
			LOG_WARNING = 1 << 3,
			LOG_INFORMATION = 1 << 4,
			LOG_ALL = 2147483647	// INT_MAXと等しい
		};

		virtual ~Log()
		{
		}

		/**
			@brief	ログ ファイルへ文字列を書き込む。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void Write(const achar* text, int level = LOG_ALL) = 0;

		/**
			@brief	ログ ファイルへ文字列を書き込み、改行する。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLine(const achar* text, int level = LOG_ALL) = 0;

		/**
			@brief	ログ ファイルへ強調された文字列を書き込む。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteStrongly(const achar* text, int level = LOG_ALL) = 0;

		/**
			@brief	ログ ファイルへ強調された文字列を書き込み、改行する。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLineStrongly(const achar* text, int level = LOG_ALL) = 0;

		/**
			@brief	ログ ファイルへ見出しとして文字列を書き込む。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHeading(const achar* text, int level = LOG_ALL) = 0;

#if !_WIN32 && !SWIG
		/**
		@brief	ログ ファイルへ文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void Write(const wchar_t* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLine(const wchar_t* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteStrongly(const wchar_t* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLineStrongly(const wchar_t* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ見出しとして文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHeading(const wchar_t* text, int level = LOG_ALL) = 0;
#endif

#ifndef SWIG
	public:
		/**
		@brief	ログ ファイルへ文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void Write(const char* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLine(const char* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteStrongly(const char* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLineStrongly(const char* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ見出しとして文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHeading(const char* text, int level = LOG_ALL) = 0;
#endif

		/**
			@brief	ログ ファイルへ水平線を書き込む。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHorizontalRule(int level = LOG_ALL) = 0;

		/**
			@brief	ログ ファイルへ表形式(<table> 要素)での書き込みを開始する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void BeginTable(int level = LOG_ALL) = 0;

		/**
			@brief	ログ ファイルへの表形式(<table> 要素)での書き込みを終了する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void EndTable(int level = LOG_ALL) = 0;

		/**
			@brief	表形式での出力において、新しい行に移動する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void ChangeRow(int level = LOG_ALL) = 0;

		/**
			@brief	表形式での出力において、新しい列に移動する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void ChangeColumn(int level = LOG_ALL) = 0;

		/**
			@brief	ログの出力が可能であるかどうかを表す真偽値を取得する。
			@param	level	[in]	ログの出力が可能か?
		*/
		virtual bool IsValid() = 0;

		/**
			@brief	ログの出力レベルを設定する。ログ出力メソッドに指定した出力レベルがこの値より小さければログが出力される。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void SetOutputLevel(int level) = 0;
	};
}
#include <list>

namespace ace
{
	/**
		@brief	プログラムの実行状況(計算時間や、使っているプロセッサ)を記録するクラス。
	*/
	class Profiler : public ReferenceObject
	{
	protected:
		Profiler(){}
		virtual ~Profiler(){}

	public:
		/**
			@brief	計算の開始時間と、使用するプロセッサ番号を記録する。
			@param	id		プロファイリングを識別するID。Start関数は、IDの一致するEnd関数と対応する。
		*/
		virtual void Start(int id) = 0;

		/**
			@brief	計算の終了時間を記録し、プロファイリング結果を蓄積する。
			@param	id		プロファイリングを識別するID。End関数は、IDの一致するStart関数と対応する。
		*/
		virtual void End(int id) = 0;
	};
}

#include <iterator>
#include <vector>
#include <memory>

namespace ace
{
	class File : public IReference
	{
	private:
		// static ファイルとstream ファイルの参照を持つ
	public:
		virtual ~File() { }
		virtual void SetRootDirectories(const astring& path) = 0;
		virtual void SetRootDirectories(const astring& path, const astring& path2) = 0;
		virtual void SetRootDirectories(const astring& path, const astring& path2, const astring& path3) = 0;
		virtual void GetRootDirectories(std::vector<std::reference_wrapper<Path>>& rootPathes) const = 0;
		virtual void EnumerateFiles(const astring& path) const = 0;
		virtual void EnumerateFiles(const astring& path, const astring& searchPattern) const = 0;
		virtual void EnumerateFiles(const astring& path, const astring& searchPattern, bool isRecursive) const = 0;
		virtual bool Exists(const astring& path) const = 0;
		virtual StaticFile* CreateStaticFile(const astring& path) = 0;
		virtual StreamFile* CreateStreamFile(const astring& path) = 0;
	};
}


namespace ace
{
	class Path
	{
	public:
		virtual ~Path() { };
		virtual astring ToAstring() const = 0;
		virtual std::iterator < std::forward_iterator_tag, Path > Begin() const = 0;
		virtual std::iterator < std::forward_iterator_tag, Path > End() const = 0;
	};
}



namespace ace
{
	class StaticFile : public IReference
	{
	private:
	public:
		virtual ~StaticFile() { };
	};
}


namespace ace
{
	class StreamFile : public IReference
	{
	private:
	public:
		virtual ~StreamFile() { };
	};
}



namespace ace
{
	/**
	@brief	音源のクラス
	*/
	class SoundSource
		: public IReference
	{
	protected:
		SoundSource() {}
		~SoundSource() {}

	public:

		/**
		@brief	ループポイントを設定する。
		@param	loopStart	ループ戻り地点(秒)
		@param	loopEnd		ループ開始地点(秒)
		*/
		virtual void SetLoopPoint(float loopStart, float loopEnd) = 0;

		/**
		@brief	音の長さを取得する。
		@return	長さ(秒)
		*/
		virtual float GetLength() = 0;
	};
}



namespace ace
{
	/**
		@brief	音を管理するクラス
	*/
	class Sound
		: public IReference
	{
		friend class Accessor;

	protected:
		Sound() {}
		~Sound() {}

		virtual SoundSource* CreateSoundSource_(const achar* path, bool isDecompressed) = 0;
	public:
		
#if !SWIG
		/**
			@brief	音を読み込む。
			@param	path	パス
			@param	isDecompressed	解凍するか?
			@return	音源
		*/
		std::shared_ptr<SoundSource> CreateSoundSource(const achar* path, bool isDecompressed)
		{
			return CreateSharedPtrWithReleaseDLL(CreateSoundSource_(path, isDecompressed));
		}
#endif

		/**
			@brief	音を再生する。
			@param	soundSource	音源
			@return	ID
		*/
		virtual int32_t Play(SoundSource* soundSource) = 0;

#if !SWIG
		/**
		@brief	音を再生する。
		@param	soundSource	音源
		@return	ID
		*/
		int32_t Play(std::shared_ptr<SoundSource> soundSource)
		{
			return Play(soundSource.get());
		}
#endif

		/**
		@brief	音が再生中か、取得する。
		@param	id	ID
		@return	再生中か?
		*/
		virtual bool IsPlaying(int32_t id) = 0;

		/**
		@brief	全ての再生中の音を停止する。
		*/
		virtual void StopAll() = 0;

		/**
		@brief	指定した音を停止する。
		@param	id	ID
		*/
		virtual void Stop(int32_t id) = 0;

		/**
		@brief	指定した音を一時停止する。
		@param	id	ID
		*/
		virtual void Pause(int32_t id) = 0;

		/**
		@brief	指定した一時停止中の音の一時停止を解除する。
		@param	id	ID
		*/
		virtual void Resume(int32_t id) = 0;

		/**
		@brief	指定した音の音量を設定する。
		@param	id	ID
		@param	volume	音量(0.0～1.0)
		*/
		virtual void SetVolume(int32_t id, float volume) = 0;

		/**
		@brief	指定した音をフェードインさせる。
		@param	id	ID
		@param	second	フェードインに使用する時間(秒)
		*/
		virtual void FadeIn(int32_t id, float second) = 0;

		/**
		@brief	指定した音をフェードアウトさせる。
		@param	id	ID
		@param	second	フェードアウトに使用する時間(秒)
		*/
		virtual void FadeOut(int32_t id, float second) = 0;
	};
}



namespace ace
{
	class Texture
		: public IReference
	{
	protected:
		Texture() {}
		virtual ~Texture() {}

	public:

		/**
		@brief	テクスチャのクラスの種類を取得する。
		@return	種類
		*/
		virtual eTextureClassType GetType() = 0;
	};
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	struct TextureLockInfomation
	{
		void* Pixels;
		int Pitch;
		Vector2DI Size;
	};

	class Texture2D
		: public Texture
	{
	protected:
		eTextureClassType	m_type;
		
	public:
		Texture2D(){}
		virtual ~Texture2D(){}

		/**
			@brief	テクスチャのサイズを取得する。
			@return	サイズ
		*/
		virtual Vector2DI GetSize() const = 0;

		/**
			@brief	テクスチャのフォーマットを取得する。
			@return	フォーマット
		*/
		virtual eTextureFormat GetFormat() const = 0;

		/**
		@brief	テクスチャをファイルに保存する。
		@param	path	出力先
		@return	成否
		*/
		virtual bool Save(const achar* path) = 0;

		/**
			@brief	テクスチャをロックし編集可能にする。
			@param	info	テクスチャ情報
			@return	成否
		*/
		virtual bool Lock(TextureLockInfomation& info) = 0;

		/**
		@brief	テクスチャをアンロックする。
		*/
		virtual void Unlock() = 0;

		/**
		@brief	テクスチャのクラスの種類を取得する。
		@return	種類
		*/
		virtual eTextureClassType GetType() { return TEXTURE_CLASS_TEXTURE2D; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class RenderTexture2D
		: public Texture2D
	{
	public:
		RenderTexture2D(){}
		virtual ~RenderTexture2D(){}

		/**
		@brief	テクスチャのサイズを取得する。
		@return	サイズ
		*/
		virtual Vector2DI GetSize() const = 0;

		/**
		@brief	テクスチャのクラスの種類を取得する。
		@return	種類
		*/
		virtual eTextureClassType GetType() { return TEXTURE_CLASS_RENDERTEXTURE2D; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}



namespace ace
{
	/**
	@brief	キューブマップ
	*/
	class CubemapTexture
		: public Texture
	{
	protected:
		CubemapTexture() {}
		virtual ~CubemapTexture() {}

	public:

		/**
		@brief	テクスチャのクラスの種類を取得する。
		@return	種類
		*/
		virtual eTextureClassType GetType() override { return TEXTURE_CLASS_CUBEMAPTEXTURE; }
	};
}


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	シェーダー(2D)
	*/
	class Shader2D
		: public IReference
	{
	protected:
		Shader2D(){}
		virtual ~Shader2D(){}
	public:
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Material2D
		: public IReference
	{
		friend class Accessor;

	protected:
		virtual Texture2D* GetTexture2D_(const achar* name) = 0;		
		virtual Shader2D* GetShader2D_() = 0;

		Material2D(){}
		virtual ~Material2D(){}
	public:
		virtual float GetFloat(const achar* name) = 0;
		virtual void SetFloat(const achar* name, float value) = 0;

		virtual Vector2DF GetVector2DF(const achar* name) = 0;
		virtual void SetVector2DF(const achar* name, Vector2DF value) = 0;

		virtual Vector3DF GetVector3DF(const achar* name) = 0;
		virtual void SetVector3DF(const achar* name, Vector3DF value) = 0;

		virtual Vector4DF GetVector4DF(const achar* name) = 0;
		virtual void SetVector4DF(const achar* name, Vector4DF value) = 0;

		virtual void SetTexture2D(const achar* name, Texture2D* value) = 0;
		virtual void SetShader2D(Shader2D* shader) = 0;

		virtual TextureFilterType GetTextureFilterType(const achar* name) = 0;
		virtual void SetTextureFilterType(const achar* name, TextureFilterType filter) = 0;

		virtual TextureWrapType GetTextureWrapType(const achar* name) = 0;
		virtual void SetTextureWrapType(const achar* name, TextureWrapType wrap) = 0;

#if! SWIG
		std::shared_ptr<Texture2D> GetTexture2D(const achar* name)
		{
			auto v = GetTexture2D_(name);
			SafeAddRef(v);
			return CreateSharedPtrWithReleaseDLL(v);
		}

		void SetTexture2D(const achar* name, std::shared_ptr<Texture2D> value)
		{
			SetTexture2D(name, value.get());
		}

		void SetTexture2D(const achar* name, std::shared_ptr<RenderTexture2D> value)
		{
			SetTexture2D(name, value.get());
		}

		std::shared_ptr<Shader2D> GetShader2D()
		{
			auto v = GetShader2D_();
			SafeAddRef(v);
			return CreateSharedPtrWithReleaseDLL(v);
		}

		void SetShader2D(std::shared_ptr<Shader2D> shader)
		{
			SetShader2D(shader.get());
		}
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}


namespace ace
{
	/**
	@brief	フォントの情報が記録されているクラス
	*/
	class Font
		:public IReference
	{
	private:

	protected:
		Font(){}
		virtual ~Font(){}
	public:
		/**
		@brief	描画テキストと描画方向を与えると、その文字の描画領域を返す。
		@param	text	描画テキスト
		@param	writingDirection	描画方向
		@return	文字の描画領域
		*/
		virtual Vector2DI CalcTextureSize(achar* text, WritingDirection writingDirection) = 0;
	};
}



namespace ace
{
	/**
		@brief	エフェクトの情報が記録されているクラス
	*/
	class Effect
		: public IReference
	{
	private:

	protected:
		Effect(){}
		virtual ~Effect(){}
	public:

	};
}


namespace ace
{
	/**
	@brief	フォントの情報が記録されているクラス
	*/
	class Chip2D
		:public IReference
	{
	private:

	protected:
		Chip2D(){}
		virtual ~Chip2D(){}
	public:
		virtual Texture2D* GetTexture() const = 0;
		virtual void SetTexture(Texture2D* texture) = 0;

		virtual const RectF GetSrc() const = 0;
		virtual void SetSrc(RectF src) = 0;

		virtual const Color GetColor() const = 0;
		virtual void SetColor(Color color) = 0;

		virtual const bool GetTurnLR() const = 0;
		virtual void SetTurnLR(bool turnLR) = 0;

		virtual const bool GetTurnUL() const = 0;
		virtual void SetTurnUL(bool turnUL) = 0;

		virtual const AlphaBlend GetAlphaBlendMode() const = 0;
		virtual void SetAlphaBlendMode(AlphaBlend alphaBlend) = 0;
	};
}




namespace ace
{
	/**
		@brief	メッシュを変形させるためのクラス
	*/
	class Deformer
		: public IReference
	{
	protected:
		Deformer() {}
		virtual ~Deformer() {}

	public:	
		/**
			@brief	ボーンを追加する。
			@param	name	ボーンの名称
			@param	parentBoneIndex	親ボーンのインデックス(親がない場合は-1)
			@param	rotationOrder	ボーンの回転行列の計算方法
			@param	localMat	ボーンのローカル変形行列
		*/
		virtual void AddBone(const achar* name, int32_t parentBoneIndex, eRotationOrder rotationOrder, const Matrix44& localMat) = 0;
	};
}





namespace ace
{
	/**
		@brief	3Dメッシュのクラス
	*/
	class Mesh
		: public IReference
	{
	protected:
		Mesh(){}
		virtual ~Mesh(){}
	public:

		/**
			@brief	頂点を追加する。
			@param	position	座標
			@param	normal		法線
			@param	binormal	従法線
			@param	uv1			UV1
			@param	uv2			UV2
			@param	color		頂点色
			@param	boneWeights	ボーンのウエイト
			@param	boneIndexes	ボーンのインデックス
		*/
		virtual void AddVertex(
			const Vector3DF& position,
			const Vector3DF& normal,
			const Vector3DF& binormal,
			const Vector2DF& uv1,
			const Vector2DF& uv2,
			const Color& color,
			int32_t boneWeights,
			int32_t boneIndexes) = 0;

		/**
			@brief	面を追加する。
			@param	index1	頂点インデックス1
			@param	index2	頂点インデックス2
			@param	index3	頂点インデックス3
			@param	materialIndex	材質インデックス
		*/
		virtual void AddFace(int32_t index1, int32_t index2, int32_t index3, int32_t materialIndex) = 0;

		/**
			@brief	ボーンとの接続設定を追加する。
			@param	targetIndex	対象ボーンインデックス
			@param	boneToMesh	ボーンの行列をメッシュの行列に変換する行列
		*/
		virtual void AddBoneConnector(int32_t targetIndex, const Matrix44& boneToMesh) = 0;

		/**
			@brief	材質を追加する。
			@return	材質のインデックス
		*/
		virtual int32_t AddMaterial() = 0;

		/**
			@brief	設定した値をGPUに送信する。
		*/
		virtual void SendToGPUMemory() = 0;

		/**
		@brief	内部シェーダーを使用する場合のカラーテクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		virtual void SetColorTexture(int32_t materialIndex, Texture2D* texture) = 0;

		/**
		@brief	内部シェーダーを使用する場合の法線テクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		virtual void SetNormalTexture(int32_t materialIndex, Texture2D* texture) = 0;

		/**
		@brief	内部シェーダーを使用する場合のスペキュラテクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		virtual void SetSpecularTexture(int32_t materialIndex, Texture2D* texture) = 0;
	};
};



namespace ace
{
	/**
		@brief	3Dモデルクラス
	*/
	class Model
		: public IReference
	{
	protected:
		Model(){}
		virtual ~Model(){}
	public:

	};
}




namespace ace
{
	/**
		@brief	キーフレームで指定するアニメーションのクラス
	*/
	class KeyframeAnimation
		: public IReference
	{
	protected:
		KeyframeAnimation() {}
		virtual ~KeyframeAnimation() {}

	public:
		/**
			@brief	名称を取得する。
			@return	名称
		*/
		virtual const achar* GetName() = 0;

		/**
			@brief	名称を指定する。
			@param	name	名称
		*/
		virtual void SetName(const achar* name) = 0;

		/**
			@brief	キーフレームを追加する。
			@param	kf	キーフレーム
		*/
		virtual void AddKeyframe(const FCurveKeyframe& kf) = 0;

		/**
			@brief	指定した時間の値を取得する。
			@param	time	時間
			@return	値
		*/
		virtual float GetValue(float time) = 0;
	};
}



namespace ace
{
	/**
		@brief	アニメーションの元データのクラス
	*/
	class AnimationSource
		: public IReference
	{
	public:
		AnimationSource() {}
		virtual ~AnimationSource() {}

		/**
			@brief	ボーンごとのアニメーションを追加する。
			@param	keyframeAnimation	アニメーション
		*/
		virtual void AddAnimation(KeyframeAnimation* keyframeAnimation) = 0;

		/**
		@brief	ボーンごとのアニメーションを削除する。
		@param	keyframeAnimation	アニメーション
		*/
		virtual void RemoveAnimation(KeyframeAnimation* keyframeAnimation) = 0;

#if !SWIG
		/**
		@brief	ボーンごとのアニメーションを追加する。
		@param	keyframeAnimation	アニメーション
		*/
		void AddAnimation(std::shared_ptr<KeyframeAnimation> keyframeAnimation)
		{
			AddAnimation(keyframeAnimation.get());
		}

		/**
		@brief	ボーンごとのアニメーションを削除する。
		@param	keyframeAnimation	アニメーション
		*/
		void RemoveAnimation(std::shared_ptr<KeyframeAnimation> keyframeAnimation)
		{
			RemoveAnimation(keyframeAnimation.get());
		}
#endif
	};
}





namespace ace
{
	/**
		@brief	アニメーションソースを元に情報が付加されたアニメーションクラス
	*/
	class AnimationClip
		: public IReference
	{
		friend class Accessor;

	protected:
		virtual AnimationSource* GetSource_() = 0;
	public:

		AnimationClip() {}
		virtual ~AnimationClip() {}

		/**
			@brief	ソースを設定する。
			@param	source	ソース
		*/
		virtual void SetSource(AnimationSource* source) = 0;

#if !SWIG
		/**
		@brief	ソースを取得する。
		@return	source	ソース
		*/
		std::shared_ptr<AnimationSource> GetSource()
		{
			auto o = GetSource_();
			SafeAddRef(o);
			return CreateSharedPtrWithReleaseDLL(o);
		}

		/**
		@brief	ソースを設定する。
		@param	source	ソース
		*/
		void SetSource(std::shared_ptr<AnimationSource>& source)
		{
			SetSource(source.get());
		}
#endif
	};
}



namespace ace
{
	/**
		@brief	アニメーションに関するインスタンスを生成するためのクラス
	*/
	class AnimationSystem
	{
		friend class Accessor;

	protected:
		AnimationSystem(){}
		virtual ~AnimationSystem(){}

		virtual AnimationClip* CreateAnimationClip_() = 0;

		virtual AnimationSource* CreateAnimationSource_() = 0;

		virtual KeyframeAnimation* CreateKeyframeAnimation_() = 0;

	public:

#if !SWIG
		/**
			@brief	アニメーションクリップのインスタンスを生成する。
			@return	インスタンス
		*/
		std::shared_ptr<AnimationClip> CreateAnimationClip()
		{
			auto o = CreateAnimationClip_();
			return CreateSharedPtrWithReleaseDLL(o);
		}

		/**
		@brief	アニメーションソースのインスタンスを生成する。
		@return	インスタンス
		*/
		std::shared_ptr<AnimationSource> CreateAnimationSource()
		{
			auto o = CreateAnimationSource_();
			return CreateSharedPtrWithReleaseDLL(o);
		}

		/**
		@brief	キーフレームアニメーションのインスタンスを生成する。
		@return	インスタンス
		*/
		std::shared_ptr<KeyframeAnimation> CreateKeyframeAnimation()
		{
			auto o = CreateKeyframeAnimation_();
			return CreateSharedPtrWithReleaseDLL(o);
		}
#endif
	};
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class Graphics
	: public ReferenceObject
{
protected:
	virtual Texture2D* CreateTexture2D_(const achar* path) = 0;
	virtual Texture2D* CreateEmptyTexture2D_(int32_t width, int32_t height, eTextureFormat format) = 0;
	virtual RenderTexture2D* CreateRenderTexture2D_(int32_t width, int32_t height, eTextureFormat format) = 0;
	virtual CubemapTexture* CreateCubemapTextureFrom6ImageFiles_(const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom) = 0;
	virtual Shader2D* CreateShader2D_( const achar* shaderText) = 0;
	virtual Material2D* CreateMaterial2D_(Shader2D* shader) = 0;
	virtual Mesh* CreateMesh_() = 0;
	virtual Deformer* CreateDeformer_() = 0;
	virtual Model* CreateModel_(const achar* path) = 0;
	virtual Effect* CreateEffect_(const achar* path) = 0;
	virtual Font* CreateFont_(const achar* path) = 0;
	virtual Chip2D* CreateChip2D_() = 0;

public:
	Graphics(){}
	virtual ~Graphics(){}

#ifndef SWIG

	/**
	@brief	テクスチャを生成する。
	@param	path	パス
	@return	テクスチャ
	*/
	std::shared_ptr<Texture2D> CreateTexture2D(const achar* path) { return CreateSharedPtrWithReleaseDLL(CreateTexture2D_(path)); }

	/**
	@brief	テクスチャを生成する。
	@param	width	横幅
	@param	height	縦幅
	@param	format	フォーマット
	@return	テクスチャ
	*/
	std::shared_ptr<Texture2D> CreateEmptyTexture2D(int32_t width, int32_t height, eTextureFormat format)
	{
		return CreateSharedPtrWithReleaseDLL(CreateEmptyTexture2D_(width, height, format));
	}

	/**
	@brief	描画先として指定可能なテクスチャを生成する。
	@param	width	横幅
	@param	height	縦幅
	@param	format	フォーマット
	@return	テクスチャ
	*/
	std::shared_ptr<RenderTexture2D> CreateRenderTexture2D(int32_t width, int32_t height, eTextureFormat format){ return CreateSharedPtrWithReleaseDLL(CreateRenderTexture2D_(width, height, format)); }

	/**
	@brief	6枚の画像ファイルからキューブマップテクスチャを生成する。
	@param	front	前方向の画像ファイルへの相対パス
	@param	left	左方向の画像ファイルへの相対パス
	@param	back	後ろ方向の画像ファイルへの相対パス
	@param	right	右方向の画像ファイルへの相対パス
	@param	top		上方向の画像ファイルへの相対パス
	@param	bottom	下方向の画像ファイルへの相対パス
	@return	キューブマップ
	*/
	std::shared_ptr<CubemapTexture> CreateCubemapTextureFrom6ImageFiles(const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom)
	{
		return CreateSharedPtrWithReleaseDLL(
			CreateCubemapTextureFrom6ImageFiles_(
			front, left, back, right, top, bottom));
	}

	/**
	@brief	シェーダー(2D)を生成する。
	@param	shaderText						シェーダーのコード
	@return	シェーダー(2D)
	*/
	std::shared_ptr<Shader2D> CreateShader2D(const achar* shaderText)
	{
		return CreateSharedPtrWithReleaseDLL(CreateShader2D_(shaderText));
	}

	/**
	@brief	マテリアル(2D)を生成する。
	@param	shader	シェーダー
	@return	マテリアル(2D)
	*/
	std::shared_ptr<Material2D> CreateMaterial2D(std::shared_ptr<Shader2D> shader)
	{
		auto material = CreateMaterial2D_(shader.get());
		return CreateSharedPtrWithReleaseDLL(material);
	}

	/**
	@brief	メッシュを生成する。
	@return	メッシュ
	*/
	std::shared_ptr<Mesh> CreateMesh()
	{
		auto mesh = CreateMesh_();
		return CreateSharedPtrWithReleaseDLL(mesh);
	}

	/**
	@brief	デフォーマーを生成する。
	@return	デフォーマー
	*/
	std::shared_ptr<Deformer> CreateDeformer()
	{
		auto deformer = CreateDeformer_();
		return CreateSharedPtrWithReleaseDLL(deformer);
	}

	/**
	@brief	モデルを生成する。
	@param	path	パス
	@return	モデル
	*/
	std::shared_ptr<Model> CreateModel(const achar* path)
	{
		auto model = CreateModel_(path);
		return CreateSharedPtrWithReleaseDLL(model);
	}

	/**
	@brief	エフェクトを生成する。
	@param	path	パス
	@return	エフェクト
	*/
	std::shared_ptr<Effect> CreateEffect(const achar* path)
	{
		auto effect = CreateEffect_(path);
		return CreateSharedPtrWithReleaseDLL(effect);
	}

	/**
	@brief	2Dチップを生成する。
	@param	texture	テクスチャ
	@return	2Dチップ
	*/
	std::shared_ptr<Chip2D> CreateChip2D()
	{
		auto chip = CreateChip2D_();
		return CreateSharedPtrWithReleaseDLL(chip);
	}

#undef CreateFont
	/**
	@brief	フォントを生成する。
	@param	path	パス
	@return	フォント
	*/
	std::shared_ptr<Font> CreateFont(const achar* path)
	{
		auto font = CreateFont_(path);
		return CreateSharedPtrWithReleaseDLL(font);
	}

#endif

	/**
	@brief	描画ランタイムの種類を取得する。
	@return	種類
	*/
	virtual GraphicsDeviceType GetGraphicsDeviceType() const = 0;

};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
#if !SWIG
	typedef void(ACE_STDCALL *CoreFuncPtr)(Core*);
#endif

	class Core
		: public ReferenceObject
	{
	private:

	public:

#if !SWIG
		/**
			@brief	インスタンスが破棄された際に呼ばれる関数ポインタを設定する。
			@param	func	破棄された際に呼ばれる関数
		*/
		virtual void SetRemovedFunctionPpointer(CoreFuncPtr func) = 0;
#endif

		/**
		@brief	初期化を行う。
		@param	title	タイトル
		@param	width	横幅
		@param	height	縦幅
		@param	isFullScreen	フルスクリーンで起動するかどうか?
		@param	isOpenGLMode	OpenGLで起動するかどうか?
		@param	isMultithreadingMode	マルチスレッドモードを使用するかどうか?
		@return	成否
		*/
		virtual bool Initialize(const achar* title, int32_t width, int32_t height, bool isFullScreen, bool isOpenGLMode, bool isMultithreadingMode) = 0;

		/**
		@brief	初期化を行う。
		@param	handle1	ハンドル
		@param	handle2	ハンドル
		@param	width	横幅
		@param	height	縦幅
		@param	isOpenGLMode	OpenGLで起動するかどうか?
		@param	isMultithreadingMode	マルチスレッドモードを使用するかどうか?
		@return	成否
		*/
		virtual bool InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height, bool isOpenGLMode, bool isMultithreadingMode) = 0;

		/**
		@brief	イベントを実行し、進行可否を判断する。
		@return	進行可能か?
		*/
		virtual bool DoEvents() = 0;

		/**
		@brief	更新処理を行う。
		*/
		virtual void Update() = 0;

		/**
		@brief	終了処理を行う。
		*/
		virtual void Terminate() = 0;

		/**
		@brief	更新されたリソースのリロードを行う。
		*/
		virtual void Reload() = 0;

		/**
			@brief	描画機能の準備をする。
		*/
		virtual void BeginDrawing() = 0;

		/**
			@brief	描画機能の使用を完了する。
		*/
		virtual void EndDrawing() = 0;

		/**
			@brief	現在のシーンを描画します。
		*/
		virtual void Draw() = 0;

		/**
			@brief	描画対象となるシーンを変更します。
		*/
		virtual void ChangeScene(CoreScene* scene) = 0;

		/**
			@brief	ウインドウを閉じる。
		*/
		virtual void Close() = 0;

		/**
			@brief	スクリーンショットをpngとして保存する。
			@param	path	出力先
		*/
		virtual void TakeScreenshot(const achar* path) = 0;

		/**
			@brief	現在のFPSを取得する。
			@return FPS
		*/
		virtual float GetCurrentFPS() = 0;

		/**
			@brief	目標FPSを取得する。
			@return	FPS
		*/
		virtual int32_t GetTargetFPS() = 0;

		/**
			@brief	目標FPSを設定する。
			@param	fps	FPS
		*/
		virtual void SetTargetFPS(int32_t fps) = 0;

		/**
			@brief	Windowsの場合、ウインドウのハンドルを返す。
			@return	ウインドウハンドル
		*/
		virtual void* GetWindowHandle() const = 0;

		/**
		@brief キーボードの入力を扱うKeyboardクラスを取得する。
		*/
		virtual Keyboard* GetKeyboard() = 0;

		/**
		@brief マウスの入力を扱うMouseクラスを取得する。
		*/
		virtual Mouse* GetMouse() = 0;

		/**
		@brief 接続されているジョイスティックの入力を扱うJoystickContainerクラスを取得する。
		*/
		virtual JoystickContainer* GetJoystickContainer() = 0;

		/**
		@brief 通常のファイルとパックファイルに対する読み込みをサポートするFileクラスを取得する。
		*/
		virtual File* GetFile() = 0;

		/**
			@brief	ログ出力を扱うLogクラスを取得する。
		*/
		virtual Log* GetLogger() = 0;

		/**
			@brief	プロファイリング機能を提供するProfilerクラスを取得する。
		*/
		virtual Profiler* GetProfiler() = 0;

		/**
			@brief	描画を扱うグラフィックスクラスを取得する。
		*/
		virtual Graphics* GetGraphics() = 0;

		virtual Sound* GetSound() = 0;

		virtual ObjectSystemFactory* GetObjectSystemFactory() = 0;

		virtual AnimationSystem* GetAnimationSyatem() = 0;
	};
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};


namespace ace
{
	enum eChildMode
	{
		CHILD_MODE_POSITION = 0,
		CHILD_MODE_ALL = 1,
		CHILD_MODE_NOTHING = 2,
	};
}


#include <vector>
#include <memory>

namespace ace
{
	class Engine;

	/**
		@brief	ポストエフェクトを適用するクラス
	*/
	class PostEffect
	{
		friend class Engine;
		friend class Layer;
		friend class CameraObject3D;
		friend class PostEffectGrayScale;
		friend class PostEffectGaussianBlur;
		friend class PostEffectLightBloom;
		friend class PostEffectSepia;

	private:
		std::shared_ptr<CorePostEffect>	m_corePostEffect;

		CorePostEffect* GetCoreObject() const;

	public:
		PostEffect();
		virtual ~PostEffect();

		/**
		@brief	オーバーライドして、毎フレーム描画される処理を記述できる。
		*/
		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) {}

		/**
		@brief	マテリアルを用いてテクスチャに画像を描画する。
		@param	target		描画先
		@param	material	マテリアル
		*/
		void DrawOnTexture2DWithMaterial(std::shared_ptr<RenderTexture2D> target, std::shared_ptr<Material2D> material);
	};
}


#include<memory>
#include <vector>


namespace ace
{

	/**
	@brief	グレースケール化するポストエフェクトクラス
	*/

	class PostEffectGrayScale : public PostEffect
	{
	private:
		std::shared_ptr<ace::Material2D>	material2d;

	public:
		PostEffectGrayScale();
		virtual ~PostEffectGrayScale() = default;

		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;
	};

}

#include<memory>
#include <vector>


namespace ace
{

	/**
	@brief	セピア調化するポストエフェクトクラス
	*/

	class PostEffectSepia : public PostEffect
	{
	private:
		std::shared_ptr<ace::Material2D>	material2d;
	public:
		PostEffectSepia();
		virtual ~PostEffectSepia() = default;

		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;
	};

}

#include<memory>
#include <vector>


namespace ace
{

	/**
	@brief	ガウスぼかしをかけるポストエフェクトクラス
	*/
	class PostEffectGaussianBlur : public PostEffect
	{
	private:
		std::shared_ptr<ace::Material2D>	material2dX;
		std::shared_ptr<ace::Material2D>	material2dY;
		std::shared_ptr<RenderTexture2D>	tempTexture;

		float intensity = 5.0f;

	public:
		PostEffectGaussianBlur();
		virtual ~PostEffectGaussianBlur() = default;

		/**
		@brief ぼかしの強さを設定する。
		@detail 実質的にはガウス関数の分散にあたる値の平方根
		*/
		void SetIntensity(float const value){ if (value <= 0.0f){ return; }intensity = value; }

		
		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;
	};

}

#include<memory>
#include <vector>


namespace ace
{

	/**
		@brief	輝度の高い画素のみをぼかして加算するポストエフェクト
	*/
	class PostEffectLightBloom : public PostEffect
	{
	private:
		std::shared_ptr<ace::Material2D>	material, material2dX, material2dY;

		std::shared_ptr<RenderTexture2D>	tempTexture;
		std::shared_ptr<RenderTexture2D>	copiedTexture;

		float intensity = 5.0f;
		float threshold = 1.0f;
		float power = 1.0f;

	public:
		PostEffectLightBloom();
		virtual ~PostEffectLightBloom() = default;

		/**
		@brief ぼかしの強さを設定する。
		@note
		実質的にはガウス関数の分散にあたる値の平方根。
		*/
		void SetIntensity(float const value){ if (value <= 0.0f){ return; }intensity = value; }

		/**
		@brief	ぼかすピクセルの閾値を設定する。
		@note
		この閾値を超えた値がぼかされ加算される。
		*/
		void SetThreshold(float const value){ threshold = value; }

		/**
			@brief	ぼかされた値を加算する強さを指定する。
			@note
			この値とぼかされた値を乗算した値が加算される。
		*/
		void SetPower(float const value) { power = value; }

		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;
	};

}



namespace ace
{
	class Object2D;

	/**
		@brief	ace::Object2D クラスに登録することができるコンポーネント。
	*/
	class Object2DComponent
	{
		friend class Object2D;

	private:
		Object2D* m_owner;
		bool m_isUpdated;
		bool m_isAlive;

		void Update();
		void SetOwner(Object2D* value);

	protected:
		/**
			@brief	オーバーライドして、このコンポーネントの更新処理を記述できる。
		*/
		virtual void OnUpdate() = 0;

	public:
		typedef std::shared_ptr<Object2DComponent> Ptr;

		/**
			@brief	コンストラクタ
		*/
		Object2DComponent();
		/**
			@brief	デストラクタ
		*/
		virtual ~Object2DComponent()
		{
		}

		/**
			@brief	このインスタンスを保持している2Dオブジェクトのインスタンスを取得する。
		*/
		Object2D* GetOwner() const;

		/**
			@brief	このコンポーネントが更新されるかどうかを取得する。
		*/
		bool GetIsUpdated() const;
		/**
			@brief	このコンポーネントが更新されるかどうかを設定する。
		*/
		void SetIsUpdated(bool value);

		/**
			@brief	このコンポーネントが実行中かどうかを取得する。Vanishメソッドによって破棄された時に false を返します。
		*/
		bool GetIsAlive() const;

		/**
			@brief	このコンポーネントを破棄します。
		*/
		void Vanish();
	};
}




namespace ace
{
	class Layer2D;

	/**
		@brief	ace::Layer2D クラスに登録することができるコンポーネント。
	*/
	class Layer2DComponent
	{
		friend class Layer2D;

	private:
		Layer2D* m_layer;
		bool m_isUpdated;
		bool m_isAlive;

		void Update();
		void SetOwner(Layer2D* value);

	protected:
		/**
			@brief	オーバーライドして、このコンポーネントの更新処理を記述できる。
		*/
		virtual void OnUpdate() = 0;

	public:
		typedef std::shared_ptr<Layer2DComponent> Ptr;

		/**
			@brief	コンストラクタ
		*/
		Layer2DComponent();
		/**
			@brief	デストラクタ
		*/
		virtual ~Layer2DComponent()
		{
		}

		/**
			@brief	このインスタンスを保持しているレイヤーのインスタンスを取得する。
		*/
		Layer2D* GetOwner() const;

		/**
			@brief	このコンポーネントが更新されるかどうかを取得する。
		*/
		bool GetIsUpdated() const;
		/**
			@brief	このコンポーネントが更新されるかどうかを設定する。
		*/
		void SetIsUpdated(bool value);

		/**
			@brief	このコンポーネントが実行中かどうかを取得する。Vanishメソッドによって破棄された時に false を返します。
		*/
		bool GetIsAlive() const;

		/**
			@brief	このコンポーネントを破棄します。
		*/
		void Vanish();
	};
}




namespace ace
{
	class Scene;

	/**
		@brief	ace::Scene クラスに登録することができるコンポーネント。
	*/
	class SceneComponent
	{
		friend class Scene;

	private:
		Scene* m_scene;
		bool m_isUpdated;
		bool m_isAlive;

		void Update();
		void SetOwner(Scene* value);

	protected:
		/**
			@brief	オーバーライドして、このコンポーネントの更新処理を記述できる。
		*/
		virtual void OnUpdate() = 0;

	public:
		typedef std::shared_ptr<SceneComponent> Ptr;

		/**
			@brief	コンストラクタ
		*/
		SceneComponent();
		/**
			@brief	デストラクタ
		*/
		virtual ~SceneComponent()
		{
		}

		/**
			@brief	このインスタンスを保持しているシーンのインスタンスを取得する。
		*/
		Scene* GetOwner() const;

		/**
			@brief	このコンポーネントが更新されるかどうかを取得する。
		*/
		bool GetIsUpdated() const;
		/**
			@brief	このコンポーネントが更新されるかどうかを設定する。
		*/
		void SetIsUpdated(bool value);

		/**
			@brief	このコンポーネントが実行中かどうかを取得する。Vanishメソッドによって破棄された時に false を返します。
		*/
		bool GetIsAlive() const;

		/**
			@brief	このコンポーネントを破棄します。
		*/
		void Vanish();
	};
}

#include <memory>
#include <map>


namespace ace
{
	class Scene;

	/**
		@brief	オブジェクトの更新と描画を管理するレイヤーの機能を提供する抽象クラス。
	*/
	class Layer
	{
		friend class Scene;

	public:
		typedef std::shared_ptr<Layer> Ptr;

	protected:
		std::shared_ptr<CoreLayer>	m_commonObject;
		bool m_isUpdated;
		bool m_isDrawn;

		std::vector<std::shared_ptr<PostEffect>>	m_postEffects;

	private:
		Scene* m_scene;

		virtual void SetScene(Scene* scene);

		std::shared_ptr<CoreLayer> GetCoreLayer() const;

		virtual void BeginUpdateting() = 0;
		virtual void EndUpdateting() = 0;

		virtual void Update() = 0;
		virtual void DrawAdditionally() = 0;

		void BeginDrawing();
		void EndDrawing();
		void Draw();

	protected:
		virtual void OnUpdated() = 0;
		virtual void OnUpdating() = 0;

	public:
		Layer();
		virtual ~Layer();

		/**
			@brief	このインスタンスを管理している ace.Scene クラスのインスタンスを取得する。
			@return	シーン
		*/
		virtual Scene* GetScene() const;

		/**
			@brief	レイヤーの更新を実行するかどうか取得する。
		*/
		bool GetIsUpdated() const;

		/**
			@brief	レイヤーの更新を実行するかどうか設定する。
			@param	value	レイヤーの更新を実行するかどうか
		*/
		void SetIsUpdated(bool value);

		/**
			@brief	レイヤーを描画するどうか取得する。
		*/
		bool GetIsDrawn() const;
		/**
		@brief	レイヤーを描画するどうか設定する。
		@param	value	レイヤーを描画するどうか
		*/
		void SetIsDrawn(bool value);

		/**
		@brief	このレイヤーの描画優先度を取得する。この値が大きいほど手前に描画される。
		*/
		int GetDrawingPriority() const;

		/**
		@brief	このレイヤーの描画優先度を設定する。この値が大きいほど手前に描画される。
		@param	value	優先度
		*/
		void SetDrawingPriority(int value);

		/**
		@brief	ポストエフェクトを追加する。
		@param	postEffect	ポストエフェクト
		*/
		void AddPostEffect(const std::shared_ptr<PostEffect>& postEffect);

		/**
		@brief	ポストエフェクトを全て消去する。
		*/
		void ClearPostEffects();

	};
}

#include <memory>
#include <list>

namespace ace
{
	class Layer2D;

	/**
		@brief	画面上に描画される2Dオブジェクトを表すクラス。
	*/
	class Object2D
	{
		friend class Layer2D;

	public:
		typedef std::shared_ptr<Object2D> Ptr;

	private:
		Layer2D* m_owner;
		std::list<Object2D::Ptr> m_children;
		std::map<astring, Object2DComponent::Ptr> m_components;
		bool m_isUpdated;
		bool m_isDrawn;

		void Start();
		void Update();
		void SetLayer(Layer2D* layer);
		virtual CoreObject2D* GetCoreObject() const = 0;

		void UpdateComponents();

	protected:
		/**
			@brief	オーバーライドして、このオブジェクトの初期化処理を記述できる。
		*/
		virtual void OnStart();
		/**
			@brief	オーバーライドして、このオブジェクトの更新処理を記述できる。
		*/
		virtual void OnUpdate();
		/**
			@brief	オーバーライドして、このオブジェクトに対する追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally();

		/**
		@brief	追加のテクスチャを描画する。
		*/
		void DrawSpriteAdditionally(Vector2DF pos1, Vector2DF pos2, Vector2DF pos3, Vector2DF pos4,
			Color col1, Color col2, Color col3, Color col4,
			Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, Vector2DF uv4,
			Texture2D* texture, AlphaBlend alphaBlend, int32_t priority);

	public:
		/**
			@brief	コンストラクタ
		*/
		Object2D();
		/**
			@brief	デストラクタ
		*/
		virtual ~Object2D();

		/**
			@brief	このオブジェクトが更新されるかどうかを取得する。
		*/
		bool GetIsUpdated() const;
		/**
			@brief	このオブジェクトが更新されるかどうかを設定する。
		*/
		void SetIsUpdated(bool value);

		/**
			@brief	このオブジェクトが描画されるかどうかを取得する。
		*/
		bool GetIsDrawn() const;

		/**
			@brief	このオブジェクトが描画されるかどうかを設定する。
		*/
		void SetIsDrawn(bool value);

		/**
			@brief	このオブジェクトが実行中かどうかを取得する。Vanishメソッドによって破棄された時に false を返す。
		*/
		bool GetIsAlive() const;

		/**
			@brief	このオブジェクトを破棄する。
		*/
		void Vanish();

		/**
			@brief	このオブジェクトを保持しているレイヤーを取得する。
		*/
		Layer2D* GetLayer() const;

		/**
			@brief	指定した2Dオブジェクトを子オブジェクトとしてこのインスタンスに追加する。
			@param	child	追加する子オブジェクト
			@param	mode	子オブジェクトの同期モード
		*/
		void AddChild(const Object2D::Ptr& child, eChildMode mode);
		/**
			@brief	指定した子オブジェクトをこのインスタンスから削除する。
			@param	child	削除する子オブジェクト
		*/
		void RemoveChild(const Object2D::Ptr& child);
		/**
			@brief	このオブジェクトが保持している子オブジェクトを含むコンテナを取得する。
		*/
		const std::list<Object2D::Ptr>& GetChildren() const;

		/**
			@brief	指定したコンポーネントをこのインスタンスに追加する。
			@param	component	追加するコンポーネント
			@param	key			コンポーネントに関連付けるキー
		*/
		void AddComponent(const Object2DComponent::Ptr& component, astring key);
		/**
			@brief	指定したキーを持つコンポーネントを取得する。
			@param	key		取得するコンポーネントを示すキー
		*/
		Object2DComponent::Ptr& GetComponent(astring key);
		/**
			@brief	指定したコンポーネントを削除する。
			@param	key		削除するコンポーネントを示すキー
		*/
		void RemoveComponent(astring key);


		/**
			@brief	このオブジェクトを描画する位置を取得する。
		*/
		Vector2DF GetPosition() const;
		/**
			@brief	このオブジェクトを描画する位置を設定する。
		*/
		void SetPosition(Vector2DF position);
		/**
			@brief	このオブジェクトを描画する実際の位置を取得する。親子関係がある場合に、親の位置を考慮した値を取得できる。
		*/
		Vector2DF GetGlobalPosition();

		/**
			@brief	このオブジェクトを描画する際の回転角度を取得する。
		*/
		float GetAngle() const;
		/**
			@brief	このオブジェクトを描画する際の回転角度を設定する。
		*/
		void SetAngle(float value);

		/**
			@brief	このオブジェクトを描画する際の拡大率を取得する。
		*/
		Vector2DF GetScale() const;
		/**
			@brief	このオブジェクトを描画する際の拡大率を設定する。
		*/
		void SetScale(Vector2DF value);
	};
}
#include <memory>

namespace ace
{
	class TextureObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreTextureObject2D> m_coreObject;
		std::shared_ptr<Texture2D> m_texturePtr;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<TextureObject2D> Ptr;

		/**
			@brief	コンストラクタ
		*/
		TextureObject2D();
		/**
			@brief	デストラクタ
		*/
		virtual ~TextureObject2D();


		/**
			@brief	描画するテクスチャを取得する。
		*/
		std::shared_ptr<Texture2D> GetTexture() const;
		/**
			@brief	描画するテクスチャを設定する。
		*/
		void SetTexture(std::shared_ptr<Texture2D> texture);

		/**
		@brief	テクスチャ上の描画範囲を取得する。
		*/
		RectF GetSrc() const;

		/**
		@brief	テクスチャ上の描画範囲を設定する。
		@param value 描画するテクスチャ上の範囲。(フィールドのいずれかが0より小さかったらテクスチャ全体を描画する。)
		*/
		void SetSrc(RectF value);

		/**
			@brief	描画する際の中心座標を取得する。
		*/
		Vector2DF GetCenterPosition() const;
		/**
			@brief	描画する際の中心座標を設定する。
		*/
		void SetCenterPosition(Vector2DF position);

		/**
			@brief	描画する際に合成する色を取得する。
		*/
		Color GetColor() const;
		/**
			@brief	描画する際に合成する色を設定する。
		*/
		void SetColor(Color color);

		/**
			@brief	左右反転して描画するかどうかを取得する。
		*/
		bool GetTurnLR() const;
		/**
			@brief	左右反転して描画するかどうかを設定する。
		*/
		void SetTurnLR(bool turnLR);

		/**
			@brief	上下反転して描画するかどうかを取得する。
		*/
		bool GetTurnUL() const;
		/**
			@brief	上下反転して描画するかどうかを設定する。
		*/
		void SetTurnUL(bool turnUL);

		/**
			@brief	このオブジェクトの描画優先度を取得する。
		*/
		int GetDrawingPriority() const;
		/**
			@brief	このオブジェクトの描画優先度を設定する。
		*/
		void SetDrawingPriority(int priority);

		/**
			@brief	このオブジェクトのブレンディング モードを取得する。
		*/
		AlphaBlend GetAlphaBlendMode() const;
		/**
			@brief	このオブジェクトのブレンディング モードを設定する。
		*/
		void SetAlphaBlendMode(AlphaBlend alphaBlend);
	};
}


namespace ace
{
	/**
		@brief	2Dレイヤーの一部を描画するためのカメラ
	*/
	class CameraObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreCameraObject2D> m_coreObject;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<CameraObject2D> Ptr;

		CameraObject2D();
		virtual ~CameraObject2D();

		/**
			@brief	描画元の領域を取得する。
			@return	描画元
		*/
		RectI GetSrc() const;

		/**
		@brief	描画元の領域を設定する。
		@param	value	描画元
		*/
		void SetSrc(RectI value);

		/**
		@brief	描画先の領域を取得する。
		@return	描画先
		*/
		RectI GetDst() const;

		/**
		@brief	描画先の領域を設定する。
		@param	value	描画先
		*/
		void SetDst(RectI value);
	};
}


namespace ace
{
	class TextObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreTextObject2D> m_coreObject;
		std::shared_ptr<Font> m_font;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<TextObject2D> Ptr;

		TextObject2D();
		virtual ~TextObject2D();

		/**
		@brief	このオブジェクトを描画する際の描画方向を取得する。
		*/
		const WritingDirection GetWritingDirection() const;

		/**
		@brief	このオブジェクトを描画する際のフォントハンドルを取得する。
		*/
		const std::shared_ptr<Font> GetFont() const;

		/**
		@brief 描画するテキストを取得する。
		*/
		const astring GetText() const;

		/**
		@brief	このオブジェクトの原点位置を取得する。この位置が、描画する際の描画・拡縮・回転の中心となる。
		*/
		const Vector2DF GetCenterPosition() const;

		/**
		@brief	このオブジェクトを描画する際に文字画像に合成する色を取得する。
		*/
		const Color GetColor() const;

		/**
		@brief	このオブジェクトを描画する際に左右反転するかどうかの真偽値を取得する。
		*/
		const bool GetTurnLR() const;

		/**
		@brief	このオブジェクトを描画する際に上下反転するかどうかの真偽値を取得する。
		*/
		const bool GetTurnUL() const;

		/**
		@brief	このオブジェクトの描画優先度を取得します。
		*/
		const int GetDrawingPriority() const;

		/**
		@brief	このオブジェクトを描画する際のブレンドモードを取得します。
		*/
		const AlphaBlend GetAlphaBlendMode() const;

		/**
		@brief	このオブジェクトを描画する際の描画方向を設定します。
		*/
		void SetWritingDirection(WritingDirection writingDirection);

		/**
		@brief	このオブジェクトを描画する際のフォントハンドルを設定する。
		*/
		void SetFont(std::shared_ptr<Font> font);

		/**
		@brief 描画するテキストを設定する。
		*/
		void SetText(astring text);

		/**
		@brief	このオブジェクトの原点位置を設定する。この位置が、描画する際の描画・拡縮・回転の中心となる。
		*/
		void SetCenterPosition(Vector2DF position);

		/**
		@brief	このオブジェクトを描画する際にテクスチャに合成する色を設定する。
		*/
		void SetColor(Color color);

		/**
		@brief	このオブジェクトを描画する際に左右反転するかどうかの真偽値を設定する。
		*/
		void SetTurnLR(bool turnLR);

		/**
		@brief	このオブジェクトを描画する際に上下反転するかどうかの真偽値を設定する。
		*/
		void SetTurnUL(bool turnUL);

		/**
		@brief	このオブジェクトの描画優先度を設定する。
		*/
		void SetDrawingPriority(int priority);

		/**
		@brief	このオブジェクトを描画する際のブレンドモードを設定する。
		*/
		void SetAlphaBlendMode(AlphaBlend alphaBlend);

	};
}


namespace ace
{
	class EffectObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreEffectObject2D> m_coreObject;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<EffectObject2D> Ptr;

		EffectObject2D();
		virtual ~EffectObject2D();

		/**
		@brief	描画に使用するエフェクトを設定する。
		@param	effect	エフェクト
		*/
		void SetEffect(std::shared_ptr<Effect> effect);


		/**
		@brief	設定されている全てのエフェクトを再生する。
		*/
		void Play();

		/**
		@brief	このオブジェクトから再生されたエフェクトを全て停止する。
		*/
		void Stop();

		/**
		@brief	このオブジェクトから再生されたエフェクトのルートを全て停止する。
		*/
		void StopRoot();

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか取得する。
		@return	フラグ
		*/
		bool GetSyncEffects();

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか設定する。
		@param	value	フラグ
		*/
		void SetSyncEffects(bool value);

		/**
		@brief	このオブジェクトから再生されるエフェクトの向きを取得する。
		@return	value	回転量(度)
		*/
		float GetEffectRotation() const;

		/**
		@brief	このオブジェクトから再生されるエフェクトの向き(度)を指定する。
		@param	value	回転量(度)
		@note
		2D表示だとエフェクトが画面に対して前後に表示されるように作成されていた場合、予期せぬ見た目で表示されてしまうことがある。
		そのため、Y軸を中心にエフェクトを回転させ2Dとして違和感のない表示ができるようにする。
		*/
		void SetEffectRotation(float value);
	};
}
#include <memory>

namespace ace
{
	class MapObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreMapObject2D> m_coreObject;
		std::set<std::shared_ptr<Chip2D>> m_chips;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<MapObject2D> Ptr;

		/**
		@brief	コンストラクタ
		*/
		MapObject2D();
		/**
		@brief	デストラクタ
		*/
		virtual ~MapObject2D();

		/**
		@brief	このオブジェクトの原点位置を取得する。この位置が、描画する際の描画・拡縮・回転の中心となる。
		*/
		const Vector2DF GetCenterPosition() const;

		/**
		@brief	このオブジェクトの描画優先度を取得します。
		*/
		const int GetDrawingPriority() const;

		/**
		@brief	このオブジェクトの原点位置を設定する。この位置が、描画する際の描画・拡縮・回転の中心となる。
		*/
		void SetCenterPosition(Vector2DF position);

		/**
		@brief	このオブジェクトの描画優先度を設定する。
		*/
		void SetDrawingPriority(int priority);

		/**
		@brief	このオブジェクトに描画チップを追加する。
		*/
		const bool AddChip(std::shared_ptr<Chip2D> chip);

		/**
		@brief	このオブジェクトから描画チップを削除する。
		*/
		const bool RemoveChip(std::shared_ptr<Chip2D> chip);
	};
}
#include <memory>
#include <list>
#include <map>

namespace ace
{
	/**
		@brief	2Dオブジェクトの更新と描画を管理するレイヤーの機能を提供する抽象クラス。
	*/
	class Layer2D : public Layer
	{
		friend class Scene;

	public:
		typedef std::shared_ptr<Layer2D> Ptr;

	private:
		std::shared_ptr<CoreLayer2D>	m_coreLayer;
		std::list<Object2D::Ptr>		m_objects;
		std::map<astring, Layer2DComponent::Ptr> m_components;
		std::vector<Object2D::Ptr>		beVanished;

		void BeginUpdateting();
		void EndUpdateting();

		void DrawAdditionally();
		void Update();
		void UpdateObjects();
		void UpdateComponents();

	protected:
		/**
			@brief	オーバーライドして、このレイヤーの初期化処理を記述できる。
		*/
		virtual void OnUpdating();
		/**
			@brief	オーバーライドして、このレイヤーの更新処理を記述できる。
		*/
		virtual void OnUpdated();
		/**
			@brief	オーバーライドして、このレイヤーの描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally();

	public:
		/**
			@brief	コンストラクタ
		*/
		Layer2D();
		/**
			@brief	デストラクタ
		*/
		virtual ~Layer2D();

		/**
			@brief	指定した2Dオブジェクトをこのインスタンスに追加する。
			@param	object	追加するオブジェクト
		*/
		void AddObject(const Object2D::Ptr& object);
		/**
			@brief	指定した2Dオブジェクトをこのインスタンスから削除する。
			@param	object	削除するオブジェクト
		*/
		void RemoveObject(const Object2D::Ptr& object);

		/**
			@brief	指定したコンポーネントをこのインスタンスに追加する。
			@param	component	追加するコンポーネント
			@param	key			コンポーネントに関連付けるキー
		*/
		void AddComponent(const Layer2DComponent::Ptr& component, astring key);

		/**
			@brief	指定したキーを持つコンポーネントを取得する。
			@param	key		取得するコンポーネントを示すキー
		*/
		Layer2DComponent::Ptr& GetComponent(astring key);

		/**
			@brief	指定したコンポーネントをこのインスタンスから削除する。
			@param	key		削除するコンポーネントを示すキー
		*/
		void RemoveComponent(astring key);

		/**
		@brief	追加のテクスチャを描画する。
		*/
		void DrawSpriteAdditionally(Vector2DF pos1, Vector2DF pos2, Vector2DF pos3, Vector2DF pos4,
			Color col1, Color col2, Color col3, Color col4,
			Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, Vector2DF uv4,
			Texture2D* texture, AlphaBlend alphaBlend, int32_t priority);
	};
}



namespace ace
{
	class Layer3D;

	/**
		@brief	更新・描画処理を行う単位となる3Dオブジェクトの機能を提供するクラス
	*/
	class Object3D
	{
		friend class Layer3D;

	public:
		typedef std::shared_ptr<Object3D> Object3DPtr;

	private:

		Layer3D*		m_owner;

		bool m_isUpdated;
		bool m_isDrawn;
		bool m_isAlive;
		
		void Start();
		void Update();
		void SetLayer(Layer3D* layer);
		CoreObject3D* GetCoreObject() const;

	protected:
		CoreObject3D*	m_commonObject;

		/**
		@brief	ユーザーはオーバーライドしてはいけない。
		*/
		virtual void OnUpdateInternal() {}

	protected:

		/**
			@brief	オーバーライドして、この3Dオブジェクトの初期化処理を記述することができる。
		*/
		virtual void OnStart() = 0;

		/**
			@brief	オーバーライドして、この2Dオブジェクトの更新処理を記述することができる。
		*/
		virtual void OnUpdate() = 0;

		/**
			@brief	将来的に削除予定
		*/
		virtual void OnDrawAdditionally() = 0;

	public:
		/**
			@brief	コンストラクタ
		*/
		Object3D();

		/**
			@brief	デストラクタ
		*/
		virtual ~Object3D();

		/**
		@brief	オブジェクトの更新を実行するかどうか取得する。
		*/
		bool GetIsUpdated() const;

		/**
		@brief	オブジェクトの更新を実行するかどうか設定する。
		@param	value	オブジェクトの更新を実行するかどうか
		*/
		void SetIsUpdated(bool value);

		/**
		@brief	オブジェクトを描画するどうか取得する。
		*/
		bool GetIsDrawn() const;
		/**
		@brief	オブジェクトを描画するどうか設定する。
		@param	value	オブジェクトを描画するどうか
		*/
		void SetIsDrawn(bool value);

		/**
		@brief	オブジェクトが生存しているか取得する。
		@return	生存しているか
		*/
		bool GetIsAlive() const;

		/**
		@brief	オブジェクトを破棄する。
		*/
		void Vanish();

		/**
		@brief	このインスタンスを管理している ace.Layer3D クラスのインスタンスを取得する。
		*/
		Layer3D* GetLayer() const;

		/**
			@brief	このインスタンスの親に対する現在の座標を取得する。
			@return	座標
		*/
		Vector3DF GetPosition() const;

		/**
			@brief	このインスタンスの親に対する現在の座標を設定する。
			@param	position	座標
		*/
		void SetPosition(Vector3DF position);

		/**
		@brief	このインスタンスの親に対する現在の角度を取得する。
		@return	角度
		*/
		Vector3DF GetRotation() const;

		/**
		@brief	このインスタンスの親に対する現在の角度を設定する。
		@param	angle	角度
		*/
		void SetRotation(Vector3DF angle);

		/**
		@brief	このインスタンスの親に対する現在の拡大率を取得する。
		@return	拡大率
		*/
		Vector3DF GetScale() const;

		/**
		@brief	このインスタンスの親に対する現在の拡大率を設定する。
		@param	scale	拡大率
		*/
		void SetScale(Vector3DF scale);
	};
}



namespace ace
{
	class PostEffect;

	/**
	@brief	カメラの機能を提供するクラス
	*/
	class CameraObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreCameraObject3D> m_coreObject;

		std::vector<std::shared_ptr<PostEffect>>	m_postEffects;

		void OnUpdateInternal() override;

	protected:
		/**
		@brief	オーバーライドして、このオブジェクトの初期化処理を記述できる。
		*/
		virtual void OnStart() {};

		/**
		@brief	オーバーライドして、毎フレーム実行される更新処理を記述できる。
		*/
		virtual void OnUpdate() {};

		/**
		@brief	オーバーライドして、追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally() {};

	public:
		CameraObject3D();
		virtual ~CameraObject3D();

		/**
			@brief	画面サイズを取得する。
			@return	画面サイズ
		*/
		Vector2DI GetWindowSize() const;

		/**
			@brief	画面サイズを設定する。
			@param	size	画面サイズ
		*/
		void SetWindowSize(const Vector2DI& size);

		/**
			@brief	カメラが見る座標を取得する。
			@return	カメラが見る座標
		*/
		Vector3DF GetFocus();

		/**
			@brief	カメラが見る座標を設定する。
			@param	focus	カメラが見る座標
		*/
		void SetFocus(const Vector3DF& focus);

		/**
		@brief	画角を取得する。
		@return	画角
		*/
		float GetFieldOfView();

		/**
		@brief	画角を設定する。
		@param	fov	画角
		*/
		void SetFieldOfView(float fov);

		/**
		@brief	画角を取得する。
		@return	画角
		*/
		float GetZFar();

		/**
		@brief	画角を設定する。
		@param	zfar	画角
		*/
		void SetZFar(float zfar);

		/**
		@brief	描画する最遠距離を取得する。
		@return	描画する最遠距離
		*/
		float GetZNear();

		/**
		@brief	描画する最近距離を設定する。
		@param	znear	描画する最近距離
		*/
		void SetZNear(float znear);

		/**
		@brief	ポストエフェクトを追加する。
		@param	postEffect	ポストエフェクト
		*/
		void AddPostEffect(const std::shared_ptr<PostEffect>& postEffect);

		/**
		@brief	ポストエフェクトを全て消去する。
		*/
		void ClearPostEffects();

	};
}



namespace ace
{
	/**
	@brief	無限遠光源の機能を提供するクラス
	*/
	class DirectionalLightObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreDirectionalLightObject3D> m_coreObject;

	protected:
		/**
		@brief	オーバーライドして、このオブジェクトの初期化処理を記述できる。
		*/
		virtual void OnStart() {};

		/**
		@brief	オーバーライドして、毎フレーム実行される更新処理を記述できる。
		*/
		virtual void OnUpdate() {};

		/**
		@brief	オーバーライドして、追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally() {};

	public:
		DirectionalLightObject3D();
		virtual ~DirectionalLightObject3D();

		/**
			@brief	光源の色を取得する。
			@return	光源の色
		*/
		Color GetColor();
		/**
		@brief	光源の色を設定する。
		@param	color	光源の色
		*/
		void SetColor(const Color& color);
	};
}



namespace ace
{
	/**
	@brief	エフェクトの機能を提供するクラス
	*/
	class EffectObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreEffectObject3D> m_coreObject;

	protected:
		/**
		@brief	オーバーライドして、このオブジェクトの初期化処理を記述できる。
		*/
		virtual void OnStart() {};

		/**
		@brief	オーバーライドして、毎フレーム実行される更新処理を記述できる。
		*/
		virtual void OnUpdate() {};

		/**
		@brief	オーバーライドして、追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally() {};

	public:
		EffectObject3D();
		virtual ~EffectObject3D();

		/**
			@brief	描画に使用するエフェクトを設定する。
			@param	effect	エフェクト
		*/
		void SetEffect(std::shared_ptr<Effect> effect);


		/**
		@brief	設定されている全てのエフェクトを再生する。
		*/
		void Play();

		/**
		@brief	このオブジェクトから再生されたエフェクトを全て停止する。
		*/
		void Stop();

		/**
		@brief	このオブジェクトから再生されたエフェクトのルートを全て停止する。
		*/
		void StopRoot();

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか取得する。
		@return	フラグ
		*/
		bool GetSyncEffects();

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか設定する。
		@param	value	フラグ
		*/
		void SetSyncEffects(bool value);
	};
}



namespace ace
{
	/**
	@brief	メッシュを表示する3Dオブジェクトの機能を提供するクラス
	*/
	class ModelObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreModelObject3D> m_coreObject;

	protected:
		/**
		@brief	オーバーライドして、このオブジェクトの初期化処理を記述できる。
		*/
		virtual void OnStart() {};

		/**
		@brief	オーバーライドして、毎フレーム実行される更新処理を記述できる。
		*/
		virtual void OnUpdate() {};

		/**
		@brief	オーバーライドして、追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally() {};

	public:
		ModelObject3D();
		virtual ~ModelObject3D();

		/**
		@brief	描画に使用するモデルを設定する。
		@param	model	モデル
		*/
		void SetModel(std::shared_ptr<Model>& model);

		/**
		@brief	描画に使用するメッシュを追加する。
		@param	meshGroupIndex	メッシュグループのインデックス
		@param	mesh	メッシュ
		*/
		void AddMesh(std::shared_ptr<Mesh> mesh);

		/**
		@brief	描画に使用するデフォーマーを設定する。
		@param	meshGroupIndex	メッシュグループのインデックス
		@param	deformer	デフォーマー
		*/
		void SetDeformer(std::shared_ptr<Deformer> deformer);

		/**
		@brief	アニメーションを再生する。
		@param	name	アニメーションの名称
		*/
		void PlayAnimation(const achar* name);
	};
}



namespace ace
{
	/**
		@brief	3Dオブジェクトの更新と描画を管理するレイヤーの機能を提供するクラス
	*/
	class Layer3D
		: public Layer
	{
		friend class Scene;

	public:
		typedef std::shared_ptr<Object3D> ObjectPtr;
	
	private:

		std::shared_ptr<CoreLayer3D>	m_coreLayer;
		std::list<ObjectPtr>			m_objects;

		void BeginUpdateting();
		void EndUpdateting();

		void DrawAdditionally();
		void Update();

	protected:
		virtual void OnUpdating();
		virtual void OnUpdated();
		virtual void OnDrawAdditionally();

	public:
		/**
			@brief	コンストラクタ
		*/
		Layer3D(RenderSettings settings = RenderSettings());

		/**
			@brief	デストラクタ
		*/
		virtual ~Layer3D();

		/**
			@brief	描画設定を取得する。
			@return	描画設定
		*/
		RenderSettings GetRenderSettings() const;

		/**
			@brief	描画設定を設定する。
			@param	settings	描画設定
		*/
		void SetRenderSettings(RenderSettings settings);

		/**
			@brief	このレイヤーに指定した3Dオブジェクトを追加する。
			@param	object	追加する3Dオブジェクト
		*/
		void AddObject(const ObjectPtr& object);
		
		/**
			@brief	このレイヤーから指定した3Dオブジェクトを削除する。
			@param	object	削除される3Dオブジェクト
		*/
		void RemoveObject(const ObjectPtr& object);

		/**
			@brief	空方向の環境光の色を設定する。
			@param	color	色
		*/
		void SetSkyAmbientColor(Color color);

		/**
		@brief	地面方向の環境光の色を設定する。
		@param	color	色
		*/
		void SetGroundAmbientColor(Color color);

	};
}




namespace ace
{
	class Engine;
	class Layer;

	/**
		@brief	画面を表し、レイヤーを管理するクラス。
	*/
	class Scene
	{
		friend class Engine;
		friend class Layer;

	public:
		typedef std::shared_ptr<Layer> LayerPtr;
		typedef std::shared_ptr<SceneComponent> ComponentPtr;
		typedef std::shared_ptr<Scene> Ptr;

	private:
		std::shared_ptr<CoreScene> m_coreScene;
		std::list<LayerPtr> m_layersToDraw;
		std::list<LayerPtr> m_layersToUpdate;
		std::map<astring, SceneComponent::Ptr> m_components;

		void Draw();

		void Update();
		void UpdateComponents();

	protected:
		/**
			@brief	オーバーライドして、このシーンを更新する直前の処理を記述できる。
		*/
		virtual void OnUpdating();
		/**
			@brief	オーバーライドして、このシーンを更新した直後の処理を記述できる。
		*/
		virtual void OnUpdated();

	public:
		/**
			@brief	コンストラクタ
		*/
		Scene();
		/**
			@brief	デストラクタ
		*/
		virtual ~Scene();

		/**
			@brief	描画先のフォーマットがHDRか取得する。
			@return	HDRか?
		*/
		bool GetHDRMode() const;

		/**
			@brief	描画先のフォーマットをHDRにするか設定する。
			@param	value	HDRか?
		*/
		void SetHDRMode(bool value);

		/**
			@brief	指定したレイヤーをこのインスタンスに追加する。
			@param	layer	追加するレイヤー
		*/
		void AddLayer(const Layer::Ptr& layer);
		/**
			@brief	指定したレイヤーをこのインスタンスから削除する。
			@param	layer	削除するレイヤー
		*/
		void RemoveLayer(const Layer::Ptr& layer);

		/**
			@brief	指定したコンポーネントをこのインスタンスに追加する。
			@param	component	追加するコンポーネント
			@param	key			コンポーネントに関連付けるキー
		*/
		void AddComponent(const SceneComponent::Ptr& component, astring key);
		/**
			@brief	キーの示すコンポーネントをこのインスタンスから取得する。
			@param	key		取得するコンポーネントを示すキー
		*/
		ComponentPtr& GetComponent(astring key);
		/**
			@brief	キーの示すコンポーネントをこのインスタンスから削除する。
			@param	key		削除するコンポーネントを示すキー
		*/
		void RemoveComponent(astring key);

		/**
			@brief	シーンの最終的な描画内容を表示する三角形を追加する。(非推奨)
			@param	pos1	座標1
			@param	uv1		UV1
			@param	col1	色1
			@param	pos2	座標2
			@param	uv2		UV2
			@param	col2	色2
			@param	pos3	座標3
			@param	uv3		UV3
			@param	col3	色3
		*/
		void AddDrawnTriangle(
			const Vector2DF& pos1, const Vector2DF& uv1, const Color& col1,
			const Vector2DF& pos2, const Vector2DF& uv2, const Color& col2,
			const Vector2DF& pos3, const Vector2DF& uv3, const Color& col3);

		/**
			@brief	全てのレイヤーとポストエフェクトが描画され終わった画面をテクスチャとして取得する。
			@return	画面
			@note
			テクスチャの内容はシーンが描画されるたびに変わる。
			主にシーン遷移の際に使用する。
		*/
		std::shared_ptr<RenderTexture2D> GetEffectedScreen();
	};
}



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	/**
	@brief	初期化時に設定するオプションのクラス
	*/
	struct EngineOption
	{
		/**
		@brief	フルスクリーンで起動するか?
		*/
		bool			IsFullScreen = false;

		/**
		@brief	描画に使用するデバイス
		*/
		GraphicsDeviceType	GraphicsDevice = GraphicsDeviceType::Default;

		/**
		@brief	マルチスレッドモードを使用するか?
		@note
		現在、マルチスレッドモードは開発中であり、実行速度は上昇するが、バグが多発する可能性がある。
		*/
		bool			IsMultithreadingMode = false;
	};


	class Engine
	{
	public:
		typedef std::shared_ptr<Scene> ScenePtr;

		static Core*					m_core;

		static Keyboard* m_keyboard ;
		static Mouse* m_mouse ;
		static JoystickContainer* m_joystickContainer ;
		static Log* m_logger ;
		static Profiler* m_profiler ;
		static Sound*	m_sound;
		static Graphics* m_graphics ;
		static ObjectSystemFactory* m_objectSystemFactory ;
		static AnimationSystem* m_animationSyatem ;


		static std::shared_ptr<Scene>	m_currentScene;
		static std::shared_ptr<Scene>	m_nextScene;

	private:
		static bool HasDLL(const char* path);
		static bool CheckDLL();
		static bool GenerateCore();

		Engine();
		~Engine();

	public:
		/**
			@brief	初期化を行う。
			@param	title	タイトル
			@param	width	横幅
			@param	height	縦幅
			@param	option	オプション
			@return	成否
		*/
		static bool Initialize(const achar* title, int32_t width, int32_t height, EngineOption option);

		/**
			@brief	初期化を行う。
			@param	handle1	ハンドル
			@param	handle2	ハンドル
			@param	width	横幅
			@param	height	縦幅
			@param	option	オプション
			@return	成否
		*/
		static bool InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height, EngineOption option);

		/**
			@brief	イベントを実行し、進行可否を判断する。
			@return	進行可能か?
		*/
		static bool DoEvents();

		/**
			@brief	更新処理を行う。
		*/
		static void Update();

		/**
			@brief	終了処理を行う。
		*/
		static void Terminate();

		/**
			@brief	描画する対象となるシーンを変更する。
		*/
		static void ChangeScene(std::shared_ptr<Scene>& scene);

		/**
		@brief	スクリーンショットをpngとして保存する。
		@param	path	出力先
		*/
		static void TakeScreenshot(const achar* path);

		/**
		@brief	現在のFPSを取得する。
		@return FPS
		*/
		static float GetCurrentFPS();

		/**
		@brief	目標FPSを取得する。
		@return	FPS
		*/
		static int32_t GetTargetFPS();

		/**
		@brief	目標FPSを設定する。
		@param	fps	FPS
		*/
		static void SetTargetFPS(int32_t fps);

		/**
			@brief	ウインドウを閉じる。
		*/
		static void Close();

		/**
		@brief	Windowsの場合、ウインドウのハンドルを取得する。
		@return	ウインドウハンドル
		*/
		static void* GetWindowHandle();

		/**
		@brief キーボードクラスを取得する。
		@return キーボード
		*/
		static Keyboard* GetKeyboard();

		/**
		@brief マウスクラスを取得する。
		@return マウス
		*/
		static Mouse* GetMouse();

		/**
		@brief	ログクラスを取得する。
		@return	ログクラス
		*/
		static Log* GetLogger();

		/**
		@brief	プロファイラクラスを取得する。
		@return	プロファイラクラス
		*/
		static Profiler* GetProfiler();

		/**
		@brief	ジョイスティックコンテナクラスを取得する。
		@return	ジョイスティックコンテナクラス
		*/
		static JoystickContainer* GetJoystickContainer();

		/**
		@brief	Graphicsクラスを取得する。
		*/
		static Graphics* GetGraphics();

		/**
		@brief	Soundクラスを取得する。
		*/
		static Sound* GetSound();

		/**
		@brief	AnimationSyatemクラスを取得する。
		preturn	AnimationSyatemクラス
		*/
		static AnimationSystem* GetAnimationSyatem();

#if _WIN32

#else
		/**
			@brief	初期化を行う。
			@param	title	タイトル
			@param	width	横幅
			@param	height	縦幅
			@param	option	オプション
			@return	成否
		*/
		static bool Initialize(const wchar_t* title, int32_t width, int32_t height, EngineOption option)
		{
			return Initialize( ToAString(title).c_str(), width, height, option );
		}
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

