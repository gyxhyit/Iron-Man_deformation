#include "Tetrah.h"



Tetrah::Tetrah(CPoint3D a, CPoint3D b, CPoint3D c, CPoint3D d)
{
	A = a;
	B = b;
	C = c;
	D = d;

	BC = C - B;
	BD = D - B;
	BA = A - B;
	CD = D - C;
	nBCD = BC.cross(BD);//两个向量的叉乘，叉乘返回的数组垂直于两向量
	//if (nBCD * BA <= 0) cout << "Error: The four point share the same plane!" << endl;
}


Tetrah::~Tetrah()
{
}

bool Tetrah::reSet(CPoint3D a, CPoint3D b, CPoint3D c, CPoint3D d) {
	A = a;
	B = b;
	C = c;
	D = d;

	BC = C - B;
	BD = D - B;
	BA = A - B;
	CD = D - C;
	nBCD = BC.cross(BD);
	//if (nBCD * BA <= 0) cout << "Error: The four point share the same plane!" << endl;
	return true;
}

CPoint3D Tetrah::ComputeKcoordinateValues(const CPoint3D& p) {
	if (p == A) return CPoint3D(0, 0, 0);

	CPoint3D Bp = p - B;
	float k1 = 1 - nBCD * Bp / (nBCD * BA);
	if (k1 == 0)
		return CPoint3D(-100, -100, -100);
	CVector3D Ap = p - A;
	CPoint3D P = 1 / k1 * Ap + A;
	if (P == D)
		return CPoint3D(k1, 0, 0);
	CVector3D DP = P - D;
	float k2 = DP.cross(BC) * nBCD / nBCD.lengthofVector();
	if (k2 == 0) {
		//float k3 = DP * BC / (BC * BC);
		return CPoint3D(k1, 0, -100);
	}

	CPoint3D E = 1 / k2 * DP + D;

	CVector3D BE = E - B;
	float k3 = BE * BC / (BC * BC);

	//CVector3D Cp = p - C;
	//CVector3D CB = B - C;
	//CVector3D CA = A - C;
	//CVector3D Ap = p - A;
	//CVector3D AC = C - A;
	//CVector3D AD = D - A;
	//CVector3D Bp = p - B;

	//float k1 = Cp * CD.cross(CB) / (CA * CD.cross(CB));
	//float k2 = Ap * AC.cross(AD) / (CA * CD.cross(CB));
	//float k3 = Bp * BC.cross(BA) / (CA * CD.cross(CB));
	return CPoint3D(k1, k2, k3);
}

CPoint3D Tetrah::ComputePcoordinateValutes(const CPoint3D& k) {
	float k1 = k.X();
	float k2 = k.Y();
	float k3 = k.Z();
	CPoint3D p = (1 - k1) * A + k1 * k2 * (1 - k3) * B + k1 * k2 * k3 * C + k1 * (1 - k2) * D;

	//CPoint3D p = k1 * A + k2 * B + k3 * D + (1 - k1 - k2 - k3) * C;
	return p;

};