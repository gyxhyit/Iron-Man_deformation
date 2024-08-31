//////////////////////////////////////////////////////////////////////////
//							��ά�������         
// function:         ʵ��������Ĳ���
// specification��   �ṩ���캯�������������أ�����e-6
//					 ��ûʵ�ֲ�\�������ṩ�˺���
// output:			
// time:		      2012,12,12
////////////////////////////////////////////////////////////////////////// 
//							���Ӳ�˼���  Cross
//specification:    ��Ӳ�˺���
//output:             vector
//time��              2020/07/08
///////////////////////////////////////////////////////////////////////

#pragma once

class CFpointVector_3D
{
public:
	enum { x, y, z, dimension_n };
	float m_fpoint[dimension_n];
public:
	CFpointVector_3D(float X = 0, float Y = 0, float Z = 0);
	CFpointVector_3D(const float(&a)[dimension_n]);//���鹹��
	CFpointVector_3D(const CFpointVector_3D& a);//��������
	~CFpointVector_3D(void) {};  //��������
	void setFpointVec(float X, float Y, float Z = 0) {  //���õ�����
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
	friend const CFpointVector_3D  operator-(const CFpointVector_3D a, const CFpointVector_3D b);// ����
	friend  const float operator *(const CFpointVector_3D a, const CFpointVector_3D b);   //��\
	friend const CFpointVector_3D  operator *(const CFpointVector_3D a, const float t);   // ���Գ���
	friend const CFpointVector_3D  operator *(const float t, const CFpointVector_3D a);
	friend const CFpointVector_3D  operator +(const CFpointVector_3D a, const CFpointVector_3D b); //�ӷ�
	CFpointVector_3D  cross(const CFpointVector_3D a); //���
	float* pointer() { return m_fpoint; };
};
typedef CFpointVector_3D CPoint3D;
typedef CFpointVector_3D CVector3D;


