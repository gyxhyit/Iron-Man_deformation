#include "CFpointVector_3D.h"
#include <math.h>
#include <iostream>
#include <exception>

const int dN = 3;
CFpointVector_3D::CFpointVector_3D(float X, float Y, float Z)
{
	if (!m_fpoint)
		throw std::exception("3D Point or Vector Fail Create for memory not enought \r\n");
	m_fpoint[x] = X;
	m_fpoint[y] = Y;
	m_fpoint[z] = Z;
}
CFpointVector_3D::CFpointVector_3D(const float(&a)[dimension_n]) {
	if (!m_fpoint)
		throw std::exception("3D Point or Vector Fail Create for memory not enought \r\n");
	for (int i = 0; i < dimension_n; i++)
		m_fpoint[i] = a[i];
}
CFpointVector_3D::CFpointVector_3D(const CFpointVector_3D& a) {
	if (!m_fpoint)
		throw std::exception("3D Point or Vector Fail Create for memory not enought \r\n");
	for (int i = 0; i < dimension_n; i++)
		m_fpoint[i] = a.m_fpoint[i];
}
float CFpointVector_3D::lengthofVector() {
	float length = 0;
	for (int i = 0; i < dimension_n; i++)
		length += m_fpoint[i] * m_fpoint[i];
	return length;
}
bool CFpointVector_3D::operator==(const CFpointVector_3D& a)const {

	if (&a == this) return 1;
	if (abs(this->X() - a.X()) < 1e-6 &&
		abs(this->Y() - a.Y()) < 1e-6 &&
		abs(this->Z() - a.Z()) < 1e-6)
		return 1;
	else return 0;
}

const CFpointVector_3D  operator-(const CFpointVector_3D a, const CFpointVector_3D b) {
	CFpointVector_3D c;
	for (int i = 0; i < dN; i++)
		c.m_fpoint[i] = a.m_fpoint[i] - b.m_fpoint[i];
	return c;
}
const CFpointVector_3D  operator +(const CFpointVector_3D a, const CFpointVector_3D b) {
	CFpointVector_3D c;
	for (int i = 0; i < dN; i++)
		c.m_fpoint[i] = a.m_fpoint[i] + b.m_fpoint[i];
	return c;
}
const CFpointVector_3D operator * (const CFpointVector_3D a, const  float t) {
	CFpointVector_3D c;
	for (int i = 0; i < dN; i++)
		c.m_fpoint[i] = a.m_fpoint[i] * t;
	return c;
}
const CFpointVector_3D operator * (const  float t, const CFpointVector_3D a) {
	CFpointVector_3D c;
	for (int i = 0; i < dN; i++)
		c.m_fpoint[i] = a.m_fpoint[i] * t;
	return c;
}
const float operator * (const CFpointVector_3D a, const CFpointVector_3D b) {
	float dot = 0;
	for (int i = 0; i < dN; i++)
		dot += a.m_fpoint[i] * b.m_fpoint[i];
	return dot;
}
void CFpointVector_3D::printf() {
	std::cout << "(x,y,z) = (" << m_fpoint[x] << "," << m_fpoint[y] << "," << m_fpoint[z] << ")" << std::endl;
}
void CFpointVector_3D::copyRight() {
	//std::cout<<"CopyRright is preserved to Linjianchu worked in HDG & Perceive lab."<<std::endl
		//<<"Email: linjianchu_13@126.com"<<std::endl;
	std::cout << "CopyRright is preserved to Linjianchu." << std::endl
		<< "Email: linjianchu_13@126.com" << std::endl;

}
CFpointVector_3D  CFpointVector_3D::cross(const CFpointVector_3D b) {
	CFpointVector_3D c;
	c.m_fpoint[x] = this->m_fpoint[y] * b.m_fpoint[z] - this->m_fpoint[z] * b.m_fpoint[y];
	c.m_fpoint[y] = this->m_fpoint[z] * b.m_fpoint[x] - this->m_fpoint[x] * b.m_fpoint[z];
	c.m_fpoint[z] = this->m_fpoint[x] * b.m_fpoint[y] - this->m_fpoint[y] * b.m_fpoint[x];
	return c;
}//²æ³Ë
//   x y z x
//   x y z x