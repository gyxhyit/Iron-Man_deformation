#pragma once
#include"GL/glut.h"
#include"CFpointVector_3D.h"
#include <vector>
#include<string>
#include<fstream>
#include<iostream>
#include<istream>
#include"Tetrah.h"
using namespace std;
class BoxNet {
	vector<Tetrah> m_boxNet;//用于存储xzy顺序的四面体网格的CPoint3D类型的点
	vector<Tetrah> cm_boxNet;//用于存储xyz顺序的四面体网格的CPoint3D类型的点
public:
	void GenerateNet(vector<vector<GLfloat>>vSets);
	bool GenerateNet(vector<vector<GLfloat>>vSets, const std::string& Filename);
	void split(const std::string& s, std::vector<std::string>& tokens, char delim);
	//void DrawNet();
	vector <Tetrah> Getm_boxNet() { return m_boxNet; };
	vector <Tetrah> Getcm_boxNet() { return cm_boxNet; };
	int GetBoxNetSize() { return m_boxNet.size(); }
	Tetrah GetTetrah(int index) {
		if (index < m_boxNet.size()) { return m_boxNet[index]; }
	}
	CPoint3D GetTetrahPointA(int index) {
		if (index < m_boxNet.size()) {
			return m_boxNet[index].getA();
		}
	}
	CPoint3D GetTetrahPointB(int index) {
		if (index < m_boxNet.size()) {
			return m_boxNet[index].getB();
		}
	}
	CPoint3D GetTetrahPointC(int index) {
		if (index < m_boxNet.size()) {
			return m_boxNet[index].getC();
		}
	}
	CPoint3D GetTetrahPointD(int index) {
		if (index < m_boxNet.size()) {
			return m_boxNet[index].getD();
		}
	}
	bool isEmpty() {
		if (m_boxNet.size() == 0) { return true; }
		else { return false; }
	}
};