#include"BoxNet.h"
using namespace std;

void BoxNet::split(const std::string& s, std::vector<std::string>& tokens, char delim) {
	tokens.clear();
	auto string_find_first_not = [s, delim](size_t pos = 0) -> size_t {
		for (size_t i = pos; i < s.size(); i++) {
			if (s[i] != delim) return i;
		}
		return std::string::npos;
	};
	size_t lastPos = string_find_first_not(0);
	size_t pos = s.find(delim, lastPos);
	while (lastPos != std::string::npos) {
		tokens.emplace_back(s.substr(lastPos, pos - lastPos));
		lastPos = string_find_first_not(pos);
		pos = s.find(delim, lastPos);
	}
}

void BoxNet::GenerateNet(vector<vector<GLfloat>>vSets) {
	//头
	vector<GLfloat>a = vSets[92];
	vector<GLfloat>b = vSets[6];
	vector<GLfloat>c = vSets[95];
	vector<GLfloat>d = vSets[90];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[54];
	b = vSets[6];
	c = vSets[95];
	d = vSets[53];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[94];
	b = vSets[95];
	c = vSets[53];
	d = vSets[90];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[2];
	b = vSets[6];
	c = vSets[53];
	d = vSets[90];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[53];
	b = vSets[6];
	c = vSets[95];
	d = vSets[90];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//右胳膊（从下到上）
	//第一个立方体
	a = vSets[40];
	b = vSets[43];
	c = vSets[41];
	d = vSets[19];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[44];
	b = vSets[43];
	c = vSets[41];
	d = vSets[22];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[18];
	b = vSets[41];
	c = vSets[22];
	d = vSets[19];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[23];
	b = vSets[43];
	c = vSets[22];
	d = vSets[19];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[22];
	b = vSets[43];
	c = vSets[41];
	d = vSets[19];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第二个立方体
	a = vSets[39];
	b = vSets[40];
	c = vSets[42];
	d = vSets[20];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[41];
	b = vSets[40];
	c = vSets[42];
	d = vSets[18];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[21];
	b = vSets[42];
	c = vSets[18];
	d = vSets[20];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[19];
	b = vSets[40];
	c = vSets[18];
	d = vSets[20];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[18];
	b = vSets[40];
	c = vSets[42];
	d = vSets[20];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第三个立方体
	a = vSets[38];
	b = vSets[39];
	c = vSets[27];
	d = vSets[45];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[42];
	b = vSets[39];
	c = vSets[27];
	d = vSets[21];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[17];
	b = vSets[27];
	c = vSets[21];
	d = vSets[45];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[20];
	b = vSets[39];
	c = vSets[21];
	d = vSets[45];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[21];
	b = vSets[39];
	c = vSets[27];
	d = vSets[45];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第一个三棱柱
	a = vSets[5];
	b = vSets[45];
	c = vSets[17];
	d = vSets[1];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[45];
	b = vSets[17];
	c = vSets[5];
	d = vSets[38];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[17];
	b = vSets[5];
	c = vSets[38];
	d = vSets[27];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//左胳膊（从下到上）
	//第一个立方体
	a = vSets[77];
	b = vSets[81];
	c = vSets[78];
	d = vSets[65];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[82];
	b = vSets[81];
	c = vSets[78];
	d = vSets[67];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[64];
	b = vSets[78];
	c = vSets[67];
	d = vSets[65];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[68];
	b = vSets[81];
	c = vSets[67];
	d = vSets[65];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[67];
	b = vSets[81];
	c = vSets[78];
	d = vSets[65];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第二个立方体
	a = vSets[80];
	b = vSets[77];
	c = vSets[79];
	d = vSets[66];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[78];
	b = vSets[77];
	c = vSets[79];
	d = vSets[64];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[63];
	b = vSets[79];
	c = vSets[64];
	d = vSets[66];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[65];
	b = vSets[77];
	c = vSets[64];
	d = vSets[66];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[64];
	b = vSets[77];
	c = vSets[79];
	d = vSets[66];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第三个立方体
	a = vSets[69];
	b = vSets[80];
	c = vSets[76];
	d = vSets[62];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[79];
	b = vSets[80];
	c = vSets[76];
	d = vSets[63];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[83];
	b = vSets[76];
	c = vSets[63];
	d = vSets[62];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[66];
	b = vSets[80];
	c = vSets[63];
	d = vSets[62];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[63];
	b = vSets[80];
	c = vSets[76];
	d = vSets[62];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第一个三棱柱
	a = vSets[69];
	b = vSets[83];
	c = vSets[52];
	d = vSets[62];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[83];
	b = vSets[52];
	c = vSets[69];
	d = vSets[76];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[52];
	b = vSets[69];
	c = vSets[76];
	d = vSets[55];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//右腿（由下到上）
	//第一个立方体
	a = vSets[34];
	b = vSets[36];
	c = vSets[35];
	d = vSets[13];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[37];
	b = vSets[36];
	c = vSets[35];
	d = vSets[14];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[12];
	b = vSets[35];
	c = vSets[14];
	d = vSets[13];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[15];
	b = vSets[36];
	c = vSets[14];
	d = vSets[13];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[14];
	b = vSets[36];
	c = vSets[35];
	d = vSets[13];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第二个立方体
	a = vSets[31];
	b = vSets[34];
	c = vSets[33];
	d = vSets[8];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[35];
	b = vSets[34];
	c = vSets[33];
	d = vSets[12];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[11];
	b = vSets[33];
	c = vSets[12];
	d = vSets[8];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[13];
	b = vSets[34];
	c = vSets[12];
	d = vSets[8];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[12];
	b = vSets[34];
	c = vSets[33];
	d = vSets[8];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//左腿（由下到上）
	//第一个立方体
	a = vSets[72];
	b = vSets[74];
	c = vSets[73];
	d = vSets[59];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[75];
	b = vSets[74];
	c = vSets[73];
	d = vSets[60];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[58];
	b = vSets[73];
	c = vSets[60];
	d = vSets[59];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[61];
	b = vSets[74];
	c = vSets[60];
	d = vSets[59];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[60];
	b = vSets[74];
	c = vSets[73];
	d = vSets[59];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第二个立方体
	a = vSets[33];
	b = vSets[72];
	c = vSets[71];
	d = vSets[11];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[73];
	b = vSets[72];
	c = vSets[71];
	d = vSets[58];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[57];
	b = vSets[71];
	c = vSets[58];
	d = vSets[11];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[59];
	b = vSets[72];
	c = vSets[58];
	d = vSets[11];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[58];
	b = vSets[72];
	c = vSets[71];
	d = vSets[11];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//右脚（从左往右）
	//第一个三棱柱
	a = vSets[36];
	b = vSets[14];
	c = vSets[46];
	d = vSets[37];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[14];
	b = vSets[46];
	c = vSets[36];
	d = vSets[15];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[46];
	b = vSets[36];
	c = vSets[15];
	d = vSets[47];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第二个三棱柱
	a = vSets[49];
	b = vSets[37];
	c = vSets[46];
	d = vSets[50];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[37];
	b = vSets[46];
	c = vSets[49];
	d = vSets[36];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[46];
	b = vSets[49];
	c = vSets[36];
	d = vSets[47];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第三个三棱柱
	a = vSets[48];
	b = vSets[50];
	c = vSets[46];
	d = vSets[51];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[50];
	b = vSets[46];
	c = vSets[48];
	d = vSets[49];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[46];
	b = vSets[48];
	c = vSets[49];
	d = vSets[47];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//左脚（从左往右）
	//第一个三棱柱
	a = vSets[86];
	b = vSets[88];
	c = vSets[85];
	d = vSets[89];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[88];
	b = vSets[85];
	c = vSets[86];
	d = vSets[87];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[85];
	b = vSets[86];
	c = vSets[87];
	d = vSets[84];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第二个三棱柱
	a = vSets[84];
	b = vSets[74];
	c = vSets[89];
	d = vSets[85];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[74];
	b = vSets[89];
	c = vSets[84];
	d = vSets[75];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[89];
	b = vSets[84];
	c = vSets[75];
	d = vSets[86];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第三个三棱柱
	a = vSets[84];
	b = vSets[61];
	c = vSets[74];
	d = vSets[85];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[61];
	b = vSets[74];
	c = vSets[84];
	d = vSets[60];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[74];
	b = vSets[84];
	c = vSets[60];
	d = vSets[75];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//身躯（从左往右，从上到下）
	//(1)(2)(3)
	// (4)(5)
	// (6)(7)
	// (8)(9)
	//(10)(11)
	//第一个三棱柱
	a = vSets[5];
	b = vSets[2];
	c = vSets[0];
	d = vSets[1];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[2];
	b = vSets[0];
	c = vSets[5];
	d = vSets[6];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[0];
	b = vSets[5];
	c = vSets[6];
	d = vSets[24];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第二个三棱柱
	a = vSets[6];
	b = vSets[0];
	c = vSets[53];
	d = vSets[2];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[0];
	b = vSets[53];
	c = vSets[6];
	d = vSets[24];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[53];
	b = vSets[6];
	c = vSets[24];
	d = vSets[54];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第三个三棱柱
	a = vSets[24];
	b = vSets[53];
	c = vSets[52];
	d = vSets[0];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[53];
	b = vSets[52];
	c = vSets[24];
	d = vSets[54];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[52];
	b = vSets[24];
	c = vSets[54];
	d = vSets[55];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第一个立方体
	a = vSets[5];
	b = vSets[27];
	c = vSets[24];
	d = vSets[1];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[28];
	b = vSets[27];
	c = vSets[24];
	d = vSets[16];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[0];
	b = vSets[24];
	c = vSets[16];
	d = vSets[1];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[17];
	b = vSets[27];
	c = vSets[16];
	d = vSets[1];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[16];
	b = vSets[27];
	c = vSets[24];
	d = vSets[1];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第二个立方体
	a = vSets[24];
	b = vSets[28];
	c = vSets[55];
	d = vSets[0];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[69];
	b = vSets[28];
	c = vSets[55];
	d = vSets[62];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[52];
	b = vSets[55];
	c = vSets[62];
	d = vSets[0];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[16];
	b = vSets[28];
	c = vSets[62];
	d = vSets[0];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[62];
	b = vSets[28];
	c = vSets[55];
	d = vSets[0];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第三个立方体
	a = vSets[27];
	b = vSets[29];
	c = vSets[28];
	d = vSets[17];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[30];
	b = vSets[29];
	c = vSets[28];
	d = vSets[10];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[16];
	b = vSets[28];
	c = vSets[10];
	d = vSets[17];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[9];
	b = vSets[29];
	c = vSets[10];
	d = vSets[17];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[10];
	b = vSets[29];
	c = vSets[28];
	d = vSets[17];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第四个立方体
	a = vSets[28];
	b = vSets[30];
	c = vSets[69];
	d = vSets[16];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[70];
	b = vSets[30];
	c = vSets[69];
	d = vSets[56];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[62];
	b = vSets[69];
	c = vSets[56];
	d = vSets[16];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[10];
	b = vSets[30];
	c = vSets[56];
	d = vSets[16];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[56];
	b = vSets[30];
	c = vSets[69];
	d = vSets[16];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第五个立方体
	a = vSets[29];
	b = vSets[31];
	c = vSets[30];
	d = vSets[9];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[32];
	b = vSets[31];
	c = vSets[30];
	d = vSets[7];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[10];
	b = vSets[30];
	c = vSets[7];
	d = vSets[9];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[8];
	b = vSets[31];
	c = vSets[7];
	d = vSets[9];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[7];
	b = vSets[31];
	c = vSets[30];
	d = vSets[9];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第六个立方体
	a = vSets[30];
	b = vSets[32];
	c = vSets[70];
	d = vSets[10];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[71];
	b = vSets[32];
	c = vSets[70];
	d = vSets[57];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[56];
	b = vSets[70];
	c = vSets[57];
	d = vSets[10];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[7];
	b = vSets[32];
	c = vSets[57];
	d = vSets[10];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[57];
	b = vSets[32];
	c = vSets[70];
	d = vSets[10];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第四个三棱柱
	a = vSets[31];
	b = vSets[7];
	c = vSets[11];
	d = vSets[8];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[7];
	b = vSets[11];
	c = vSets[31];
	d = vSets[32];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[11];
	b = vSets[31];
	c = vSets[32];
	d = vSets[33];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第五个三棱柱
	a = vSets[71];
	b = vSets[7];
	c = vSets[11];
	d = vSets[57];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[7];
	b = vSets[11];
	c = vSets[71];
	d = vSets[32];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[11];
	b = vSets[71];
	c = vSets[32];
	d = vSets[33];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//胯下（从左往右）
//第一个三棱柱
	a = vSets[79];
	b = vSets[93];
	c = vSets[36];
	d = vSets[129];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[79];
	b = vSets[12];
	c = vSets[36];
	d = vSets[129];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[79];
	b = vSets[131];
	c = vSets[12];
	d = vSets[129];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	//第二个三棱柱
	a = vSets[8];
	b = vSets[34];
	c = vSets[36];
	d = vSets[128];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[8];
	b = vSets[12];
	c = vSets[36];
	d = vSets[128];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));
	a = vSets[8];
	b = vSets[12];
	c = vSets[130];
	d = vSets[128];
	m_boxNet.emplace_back(Tetrah(CPoint3D(a[0], a[1], a[2]), CPoint3D(b[0], b[1], b[2]),
		CPoint3D(c[0], c[1], c[2]), CPoint3D(d[0], d[1], d[2])));

}
bool BoxNet::GenerateNet(vector<vector<GLfloat>>vSets, const std::string& Filename) {

	if (Filename.empty()) {
		std::cout << "1 Error Empty File: Please make sure the file is correct" << std::endl;
		return false;
	}
	std::ifstream Reader;
	Reader.open(Filename.data());
	if (!Reader) {
		std::cout << "2 Error Empty File: Please make sure the file is correct" << std::endl;
		return false;
	}
	std::string line;
	int a, b, c, d;
	while (std::getline(Reader, line)) {
		std::vector<std::string> tokens;
		// skip the comments
		if (line[0] == '#' || line[0] == 'g' || line.length() == 0) continue;
		split(line, tokens, '/');
		if (tokens.size() == 0)
			continue;
		if (tokens.size() == 4) {
			a = (int)atoi(tokens[0].c_str());
			b = (int)atoi(tokens[1].c_str());
			c = (int)atoi(tokens[2].c_str());
			d = (int)atoi(tokens[3].c_str());
			cout << a << " " << b << " " << c << " " << d << endl;

			m_boxNet.emplace_back(Tetrah(CPoint3D(vSets[a][0], vSets[a][1], vSets[a][2]), CPoint3D(vSets[b][0], vSets[b][1], vSets[b][2]),
				CPoint3D(vSets[c][0], vSets[c][1], vSets[c][2]), CPoint3D(vSets[d][0], vSets[d][1], vSets[d][2])));
			cm_boxNet.emplace_back(Tetrah(CPoint3D(vSets[a][0], vSets[a][2], vSets[a][1]), CPoint3D(vSets[b][0], vSets[b][2], vSets[b][1]),
				CPoint3D(vSets[c][0], vSets[c][2], vSets[c][1]), CPoint3D(vSets[d][0], vSets[d][2], vSets[d][1])));
		}
	}
	return true;
}


