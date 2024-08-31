//////////////////////////////////////////////////////////////////////////
//							三维点操作类         
// function:         实现向量类的操作
// specification：   提供构造函数，操作符重载，精度e-6
//					 但没实现叉乗，仅是提供了函数
// output:			
// time:		      2012,12,12
////////////////////////////////////////////////////////////////////////// 
//							增加叉乘计算  Cross
//specification:    添加叉乘函数
//output:             vector
//time：              2020/07/08
///////////////////////////////////////////////////////////////////////

#pragma once

class CFpointVector_3D
{
public:
	enum { x, y, z, dimension_n };
	float m_fpoint[dimension_n];
public:
	CFpointVector_3D(float X = 0, float Y = 0, float Z = 0);
	CFpointVector_3D(const float(&a)[dimension_n]);//数组构造
	CFpointVector_3D(const CFpointVector_3D& a);//拷贝构造
	~CFpointVector_3D(void) {};  //析构函数
	void setFpointVec(float X, float Y, float Z = 0) {  //设置点坐标
		m_fpoint[x] = X;
		m_fpoint[y] = Y;
		m_fpoint[z] = Z;
	};
	float lengthofVector();
	const float X()const { return m_fpoint[x]; };
	const float Y()const { return m_fpoint[y]; };
	const float Z()const { return m_fpoint[z]; };
	CFpointVector_3D& operator=(const CFpointVector_3D& a) {
		if (&a == this) return *this;
		m_fpoint[x] = a.m_fpoint[x];
		m_fpoint[y] = a.m_fpoint[y];
		m_fpoint[z] = a.m_fpoint[z];
		return *this;
	}
	bool operator==(const CFpointVector_3D& a) const;
	//bool const operator==(const CFpointVector_3D &a);
	void printf();
	static void copyRight();
	friend const CFpointVector_3D  operator-(const CFpointVector_3D a, const CFpointVector_3D b);// 减法
	friend  const float operator *(const CFpointVector_3D a, const CFpointVector_3D b);   //点乗
	friend const CFpointVector_3D  operator *(const CFpointVector_3D a, const float t);   // 乘以常数
	friend const CFpointVector_3D  operator *(const float t, const CFpointVector_3D a);
	friend const CFpointVector_3D  operator +(const CFpointVector_3D a, const CFpointVector_3D b); //加法
	CFpointVector_3D  cross(const CFpointVector_3D a); //叉乘
	float* pointer() { return m_fpoint; };
};
typedef CFpointVector_3D CPoint3D;
typedef CFpointVector_3D CVector3D;


