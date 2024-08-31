#pragma once
// 计算四面体坐标系
// 2020 8 13
// 输出四面体坐标系值
//            A
//           /\- D
//          /  \ \
//         /_ __\/
//        B     C
//     Tudorcat
//   @param   CFpointVector_3D A, B, C, D;

#include"CFpointVector_3D.h"
#include<math.h>
#include<iostream>

using namespace std;

class Tetrah
{
	CPoint3D A, B, C, D;
	CVector3D BC, BD, BA, CD;
	CVector3D nBCD;
public:
	Tetrah(CPoint3D a, CPoint3D b, CPoint3D c, CPoint3D d);
	~Tetrah();
	bool reSet(CPoint3D a, CPoint3D b, CPoint3D c, CPoint3D d);
	CPoint3D getA() { return A; };
	CPoint3D getB() { return B; };
	CPoint3D getC() { return C; };
	CPoint3D getD() { return D; };

	CPoint3D ComputeKcoordinateValues(const CPoint3D& p);
	CPoint3D ComputePcoordinateValutes(const CPoint3D& k);
};


