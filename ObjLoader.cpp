#include "ObjLoader.h"
#include <fstream>
#include <iostream>
#include "dual_quat_cu.hpp"
#include <math.h>
#define PI 3.14159

using namespace Tbx;
using namespace std;

#define BMP_Header_Length 54  //ͼ���������ڴ���е�ƫ����

//v�����ʽ��v x y z
//���壺ÿ�����������
//vt��������
//��ʽ��vt u v w
//���壺����ģ�͵�������Ƭʱ��ÿ������ȡ���ص�ʱ��Ӧ������ͼƬ�ϵ����ꡣ
// ����ͼƬ������ָ���ǣ�����ͼƬ�����������Ļ����ʾʱ������Ļ���½�Ϊԭ������ꡣ
//ע�⣺wһ������������ά�����󲿷����ò����ģ�������Ϊ0��
//vn���㷨����
//��ʽ��vn x y z
//���壺����ģ��������Ƭʱ����Ҫȷ��������Ƭ�ĳ���������ĳ���
//���ɹ���ÿ����Ķ����Ӧ�Ķ��㷨��������ʸ���;����ģ�xyz������ֱ�����ٳ���3�õ��ģ���
//f ��
//��ʽ ��f v/vt/vn v/vt/vn v/vt/vn��f �������� / ������������ / ���㷨����������
//���壺����������Ƭ�����ݣ�ÿ��������Ƭ������f���ɣ���f����ȷ�����㡢����Ķ�Ӧ���������꣨��ȡ����ͼƬ��Ӧ����������ص㣩��
//ͨ�����������Ӧ�Ķ��㷨��������ȷ��������ķ���


ObjLoader::ObjLoader(string filename)
{
	string line;
	fstream f;

	f.open(filename, ios::in);

	if (!f.is_open()) {
		cout << "Something Went Wrong When Opening Objfiles" << endl;
	}

	while (!f.eof()) {

		getline(f, line);//�õ�obj�ļ��е�һ�У���Ϊһ���ַ���

		vector<string>parameters;

		string tailMark = " ";

		string ans = "";

		line = line.append(tailMark);

		for (int i = 0; i < line.length(); i++) {

			char ch = line[i];
			if (ch != ' ') {
				ans += ch;
			}
			else {
				if (ans != "") {
					parameters.push_back(ans);//ȡ���ַ����е�Ԫ�أ��Կո��з�
				}
				ans = "";
			}
		}

		//cout << parameters.size() << endl;
		if (parameters.size() != 4) {
			if (parameters.size() == 3 && parameters[0] == "vt") {
				vector<GLfloat>vtSet;
				for (int i = 1; i < 3; i++) {
					GLfloat xy = atof(parameters[i].c_str());
					vtSet.push_back(xy);
				}
				vtSets.push_back(vtSet);
			}
			if (parameters.size() == 5 && parameters[0] == "f") {
				vector<GLint>vIndexSets1;

				for (int i = 1; i < 5; i++) {
					string x = parameters[i];
					string ans = "";
					for (int j = 0; j < x.length(); j++) {//����'/'
						char ch = x[j];
						if (ch != '/') {
							ans += ch;
						}
						else {
							break;
						}
					}
					GLint index1 = atof(ans.c_str());
					index1 = index1--;//��Ϊ�����������obj�ļ����Ǵ�1��ʼ�ģ������Ǵ�ŵĶ���vector�Ǵ�0��ʼ�����Ҫ��1
					vIndexSets1.push_back(index1);
				}

				fSets1.push_back(vIndexSets1);
			}
			if (parameters.size() == 6 && parameters[0] == "f") {
				vector<GLint>vIndexSets2;

				for (int i = 1; i < 6; i++) {
					string x = parameters[i];
					string ans = "";
					for (int j = 0; j < x.length(); j++) {//����'/'
						char ch = x[j];
						if (ch != '/') {
							ans += ch;
						}
						else {
							break;
						}
					}
					GLint index2 = atof(ans.c_str());
					index2 = index2--;//��Ϊ�����������obj�ļ����Ǵ�1��ʼ�ģ������Ǵ�ŵĶ���vector�Ǵ�0��ʼ�����Ҫ��1
					vIndexSets2.push_back(index2);
				}

				fSets2.push_back(vIndexSets2);
			}

			else {
				if (parameters.size() > 5 || parameters.size() < 3) {
					cout << "the size is not correct" << endl;
				}
			}
		}

		else {

			if (parameters[0] == "v") {//v��ʾ����

				vector<GLfloat>Point1;

				for (int i = 1; i < 4; i++) {//��1��ʼ���������xyz��������Ž�����vector

					GLfloat xyz = atof(parameters[i].c_str());

					Point1.push_back(xyz);
				}

				vSets.push_back(Point1);
			}

			else if (parameters[0] == "f") {//�������Ļ�������������������

				vector<GLint>vIndexSets;
				vector<GLint>vtIndexSets;

				for (int i = 1; i < 4; i++) {
					string x = parameters[i];
					string ans = "";
					string ans1 = "";
					int count = 1;
					for (int j = 0; j < x.length(); j++) {//����'/'
						char ch = x[j];
						if (ch != '/' && count % 3 == 1) {
							ans += ch;
						}
						else if (ch != '/' && count % 3 == 2) {
							ans1 += ch;
						}
						else if (ch == '/') {
							count++;
							continue;
						}
					}
					GLint index = atof(ans.c_str());
					index = index--;//��Ϊ�����������obj�ļ����Ǵ�1��ʼ�ģ������Ǵ�ŵĶ���vector�Ǵ�0��ʼ�����Ҫ��1
					vIndexSets.push_back(index);

					GLint index1 = atof(ans1.c_str());
					index1 = index1--;
					vtIndexSets.push_back(index1);
				}

				fSets.push_back(vIndexSets);
				fvtSets.push_back(vtIndexSets);
			}
		}
	}
	f.close();
	vector<GLfloat>Point2;

	BoxNet model1_net1;
	model1_net1.GenerateNet(vSets, "C:/Users/reckless/Desktop/3Dģ�ͻ���/Boxlist5.txt");
	//model1_net1.GenerateNet(vSets);
	T_boxNet = model1_net1.Getm_boxNet();

	TxtLoader("C:/Users/reckless/Desktop/3Dģ�ͻ���/armboxdata.txt");//ͨ��txt��ȡ�仯�������������vBoxSets_new�洢
	TetrahSet(); //��ȡ�仯ǰ������������
	ChangeType();//���ڽ�float���͵ĵ��ΪCfPoint3D����
	ChangeTetrahSet();//�޸�����������ĵ������

}


// ����power_of_two�����ж�һ�������ǲ���2���������ݣ�������ͼƬ��һ��Ҫ��
int power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

/* ����load_texture
* ��ȡһ��BMP�ļ���Ϊ����
* ���ʧ�ܣ�����0������ɹ�������������
*/
GLuint ObjLoader::load_texture(const char* file_name) {
	//GLint width, height, total_bytes;
	//GLubyte* pixels = 0;
	//GLuint last_texture_ID = 0, texture_ID = 0;

	//// ���ļ������ʧ�ܣ�����
	//FILE* pFile = fopen(file_name, "rb");
	//if (pFile == 0)
	//	return 0;

	//// ��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶�
	//fseek(pFile, 0x0012, SEEK_SET);
	//fread(&width, 4, 1, pFile);
	//fread(&height, 4, 1, pFile);
	//fseek(pFile, BMP_Header_Length, SEEK_SET);

	//// ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���
	//{
	//	GLint line_bytes = width * 3;
	//	while (line_bytes % 4 != 0)
	//		++line_bytes;
	//	total_bytes = line_bytes * height;
	//}

	//// �����������ֽ��������ڴ�
	//pixels = (GLubyte*)malloc(total_bytes);
	//if (pixels == 0)
	//{
	//	fclose(pFile);
	//	return 0;
	//}

	//// ��ȡ��������
	//if (fread(pixels, total_bytes, 1, pFile) <= 0)
	//{
	//	free(pixels);
	//	fclose(pFile);
	//	return 0;
	//}

	//// �Ծ;ɰ汾�ļ��ݣ����ͼ��Ŀ�Ⱥ͸߶Ȳ��ǵ������η�������Ҫ��������
	//// ��ͼ���߳�����OpenGL�涨�����ֵ��Ҳ����
	//{
	//	GLint max;
	//	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
	//	if (!power_of_two(width)
	//		|| !power_of_two(height)
	//		|| width > max
	//		|| height > max)
	//	{
	//		const GLint new_width = 256;
	//		const GLint new_height = 256; // �涨���ź��µĴ�СΪ�߳���������
	//		GLint new_line_bytes, new_total_bytes;
	//		GLubyte* new_pixels = 0;

	//		// ����ÿ����Ҫ���ֽ��������ֽ���
	//		new_line_bytes = new_width * 3;
	//		while (new_line_bytes % 4 != 0)
	//			++new_line_bytes;
	//		new_total_bytes = new_line_bytes * new_height;

	//		// �����ڴ�
	//		new_pixels = (GLubyte*)malloc(new_total_bytes);
	//		if (new_pixels == 0)
	//		{
	//			free(pixels);
	//			fclose(pFile);
	//			return 0;
	//		}

	//		// ������������
	//		gluScaleImage(GL_RGB,
	//			width, height, GL_UNSIGNED_BYTE, pixels,
	//			new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

	//		// �ͷ�ԭ�����������ݣ���pixelsָ���µ��������ݣ�����������width��height
	//		free(pixels);
	//		pixels = new_pixels;
	//		width = new_width;
	//		height = new_height;
	//	}
	//}

	//// ����һ���µ�������
	//glGenTextures(1, &texture_ID);
	//if (texture_ID == 0)
	//{
	//	free(pixels);
	//	fclose(pFile);
	//	return 0;
	//}

	//// ���µ������������������������
	//// �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
	//GLint lastTextureID = last_texture_ID;
	//glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
	//glBindTexture(GL_TEXTURE_2D, texture_ID);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//������������ʵ��ںϷ�ʽ
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
	//	GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	//glBindTexture(GL_TEXTURE_2D, lastTextureID);  //�ָ�֮ǰ�������
	//free(pixels);
	//cout << "success" << endl;
	//return texture_ID;



	// ���ļ������ʧ�ܣ�����
	FILE* pFile = fopen(file_name, "rb");
	if (pFile == 0)
		return 0;

GLubyte* pixels = 0;
GLuint last_texture_ID = 0, tex_ID = 0;
fseek(pFile, 0x0012, SEEK_SET);
// ��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶�
GLint width = 0, height = 0, total_bytes = 0;
fread(&width, 4, 1, pFile);
fread(&height, 4, 1, pFile);
fseek(pFile, BMP_Header_Length, SEEK_SET);

// ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���
GLint  line_bytes = width * 3;
while (line_bytes % 4 != 0)
++line_bytes;
total_bytes = line_bytes * height;
// �����������ֽ��������ڴ�
pixels = (GLubyte*)malloc(total_bytes);
if (pixels == 0)
{
	fclose(pFile);
	return false;
}
// ��ȡ��������
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
	free(pixels);
	fclose(pFile);
		return 0;
	}

GLint max;
glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
if (!power_of_two(width)
	|| !power_of_two(height)
	|| width > max
	|| height > max)
{
	const GLint new_width = 256;
	const GLint new_height = 256; // �涨���ź��µĴ�С
	GLint new_line_bytes, new_total_bytes;
	GLubyte* new_pixels = 0;
	// ����ÿ����Ҫ���ֽ��������ֽ���
	new_line_bytes = new_width * 3;
	while (new_line_bytes % 4 != 0)
		++new_line_bytes;
	new_total_bytes = new_line_bytes * new_height;
	// �����ڴ�
	new_pixels = (GLubyte*)malloc(new_total_bytes);
	if (new_pixels == 0)
	{
		fclose(pFile);
		return false;
	}
	// ������������
	gluScaleImage(GL_RGB,
		width, height, GL_UNSIGNED_BYTE, pixels,
		new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);
	// �ͷ�ԭ�����������ݣ���pixelsָ���µ��������ݣ�����������width��height
	free(pixels);
	pixels = new_pixels;
	width = new_width;
	height = new_height;
}
// ����һ���µ�������
//GLuint tex_ID;
glGenTextures(1, &tex_ID);

if (tex_ID == 0)
{
	free(pixels);
	fclose(pFile);
	return 0;
}
// ���µ������������������������
// �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
GLint lastTextureID = last_texture_ID;
glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
glBindTexture(GL_TEXTURE_2D, tex_ID);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
glBindTexture(GL_TEXTURE_2D, lastTextureID);  //�ָ�֮ǰ�������

fclose(pFile);
free(pixels);

return tex_ID;


}


//vBoxSets_new�洢txt
void ObjLoader::TxtLoader(string filename) {
	string line;
	fstream f;

	f.open(filename, ios::in);

	if (!f.is_open()) {
		cout << "Something Went Wrong When Opening Txtfiles" << endl;
	}

	while (!f.eof()) {

		getline(f, line);//�õ�txt�ļ��е�һ�У���Ϊһ���ַ���

		vector<string>parameters;

		string tailMark = ",";

		string ans = "";

		line = line.append(tailMark);

		for (int i = 0; i < line.length(); i++) {

			char ch = line[i];
			if (ch != ',') {
				ans += ch;
			}
			else {
				if (ans != "") {
					parameters.push_back(ans);//ȡ���ַ����е�Ԫ�أ���,�з�
				}
				ans = "";
			}
		}
		if (parameters.size() == 3) {
			vector<GLfloat>Point1;
			for (int i = 0; i < 3; i++) {//��1��ʼ���������xyz��������Ž�����vector

				GLfloat xyz = atof(parameters[i].c_str());

				Point1.push_back(xyz);
			}

			vBoxSets_new.push_back(Point1);  //��ű仯����ӵĵ�

		}
	}
}
//��ȡ�仯ǰ������������
void ObjLoader::TetrahSet() { 
	BoxNet model1_net2;
	model1_net2.GenerateNet(vSets, "C:/Users/reckless/Desktop/3Dģ�ͻ���/Boxlist5.txt");
	TetrahSet_initial = model1_net2.Getcm_boxNet();
}
//���ڽ�x,y,z��Ϊx,z,y���꣨��opengl�б���һ�£�
void ObjLoader::ChangePoint(CPoint3D& VSets) {
	PointChange.setFpointVec(VSets.X(), VSets.Z(), VSets.Y());

}
//���ڽ�float���͵ĵ��ΪCfPoint3D���ͣ���ʵ����xyz���꣩
void ObjLoader::ChangeType() {
	for (int i = 0; i < vSets.size(); i++) {
		vSetsChangeType.push_back(CFpointVector_3D(vSets[i][0], vSets[i][1], vSets[i][2]));
	}
}
//����Լ��
void ObjLoader::StretchConstraint() {
	float k = 1;
	float d19_24 = 0.484;
	float d44_47 = 0.465;
	float d24_48 = 0.424;
	float d22_23 = 0.468;
	float d22_44 = 0.431;
	float d19_22 = 0.268;
	float d23_24 = 0.266;
	float d47_48 = 0.266;
	float d19_43 = 0.396;
	float d43_44 = 0.266;
	float d23_47 = 0.413;
	float d43_48 = 0.488;

	float q19_24x = 0.5 * k * (sqrt(pow(vSets[19][0] - vSets[24][0], 2) +
		pow(vSets[19][1] - vSets[24][1], 2) +
		pow(vSets[19][2] - vSets[24][2], 2)) - d19_22) *
		(vSets[19][0] - vSets[24][0]) / sqrt(pow(vSets[19][0] - vSets[24][0], 2) +
			pow(vSets[19][1] - vSets[24][1], 2) +
			pow(vSets[19][2] - vSets[24][2], 2));
	float q19_24y = 0.5 * k * (sqrt(pow(vSets[19][0] - vSets[24][0], 2) +
		pow(vSets[19][1] - vSets[24][1], 2) +
		pow(vSets[19][2] - vSets[24][2], 2)) - d19_22) *
		(vSets[19][1] - vSets[24][1]) / sqrt(pow(vSets[19][0] - vSets[24][0], 2) +
			pow(vSets[19][1] - vSets[24][1], 2) +
			pow(vSets[19][2] - vSets[24][2], 2));
	float q19_24z = 0.5 * k * (sqrt(pow(vSets[19][0] - vSets[24][0], 2) +
		pow(vSets[19][1] - vSets[24][1], 2) +
		pow(vSets[19][2] - vSets[24][2], 2)) - d19_22) *
		(vSets[19][2] - vSets[24][2]) / sqrt(pow(vSets[19][0] - vSets[24][0], 2) +
			pow(vSets[19][1] - vSets[24][1], 2) +
			pow(vSets[19][2] - vSets[24][2], 2));
	vSets[24][0] += q19_24x;
	vSets[24][1] += q19_24y;
	vSets[24][2] += q19_24z;
}
/*
�������Ķ��㣬��������֮��ľ����������Լ��
parameters������������������Գ̶ȣ��仯ǰ�������������        vsets_old=vttest
*/
/*void ObjLoader::StretchConstraint1(int a, int b, float k, vector<vector<GLfloat>>& vSets_old) {
	float d_old;
	float d_new;
	d_old = sqrt(pow(vSets_old[a][0] - vSets_old[b][0], 2) + pow(vSets_old[a][1] - vSets_old[b][1], 2) + pow(vSets_old[a][2] - vSets_old[b][2], 2));
	//d_old = 0.484;
	d_new = sqrt(pow(vSets[a][0] - vSets[b][0], 2) + pow(vSets[a][1] - vSets[b][1], 2) + pow(vSets[a][2] - vSets[b][2], 2));
	float c = 0.5 * k * (d_new - d_old) * (vSets[a][0] - vSets[b][0]) / d_new;//�������������λ�ƣ�
	cout << c;
	vSets[b][0] += 0.5 * k * (d_new - d_old) * (vSets[a][0] - vSets[b][0]) / d_new;
	vSets[b][1] += 0.5 * k * (d_new - d_old) * (vSets[a][1] - vSets[b][1]) / d_new;
	vSets[b][2] += 0.5 * k * (d_new - d_old) * (vSets[a][2] - vSets[b][2]) / d_new;
	//vSets[b][2] +=tanh(d_old)
}*/
/*void ObjLoader::StretchConstraintTest(int a, int b, float k, float vtSetstest[4][3], float vttest_new[4][3]) {
	float d_old;
	float d_new;
	float p;
	d_old = sqrt(pow(vtSetstest[a][0] - vtSetstest[b][0], 2) + pow(vtSetstest[a][1] - vtSetstest[b][1], 2) + pow(vtSetstest[a][2] - vtSetstest[b][2], 2));
	//d_old = 0.484;
	d_new = sqrt(pow(vttest_new[a][0] - vttest_new[b][0], 2) + pow(vttest_new[a][1] - vttest_new[b][1], 2) + pow(vttest_new[a][2] - vttest_new[b][2], 2));
	p = d_new - d_old;
	float c = 0.5 * k * (d_new - d_old) * (vSets[0][0] - vSets[0][0]) / d_new;//�������������λ�ƣ�
	float z;
	z = tanh(abs(p));
	vttest_new[0][0] += 0.5 * k * p * tanh(abs(p));
	vttest_new[0][1] += 0.5 * k * p * tanh(abs(p));
	vttest_new[0][2] += 0.5 * k * p * tanh(abs(p));
	//vSets[b][2] +=tanh(d_old)
}*/
void ObjLoader::StretchConstraint1(int a, int b,  float k, vector<vector<GLfloat>>& vSets_old) {
	float p_new, p_old, p;
	p_old = sqrt(pow(vSets_old[a][0] - vSets_old[b][0], 2) + pow(vSets_old[a][1] - vSets_old[b][1], 2) + pow(vSets_old[a][2] - vSets_old[b][2], 2));
	p_new = sqrt(pow(vSets[a][0] - vSets[b][0], 2) + pow(vSets[a][1] - vSets[b][1], 2) + pow(vSets[a][2] - vSets[b][2], 2));
	p = (p_new - p_old)/ p_new;
	float z;
	z = tanh(abs(p));
	float p_dis = k * p * tanh(abs(p));
	vSets[b][0] += k * p * tanh(abs(p* (vSets[a][0] - vSets[b][0])));
	vSets[b][1] += k * p * tanh(abs(p* (vSets[a][1] - vSets[b][1])));
	vSets[b][2] += k * p * tanh(abs(p* (vSets[a][2] - vSets[b][2])));
}
//void ObjLoader::StretchConstraintTest(int a, float k, float vtSetstest[4][3], float vttest_new[4][3]) {
//	float p;
//	p= sqrt(pow(vttest_new[a][0] - vtSetstest[a][0], 2) + pow(vttest_new[a][1] - vtSetstest[a][1], 2) + pow(vttest_new[a][2] - vtSetstest[a][2], 2));
//	float z;
//	z = tanh(abs(p));
//	float p_dis=  k * p * tanh(abs(p));
//	vttest_new[a][0] += k * p * tanh(abs(p));
//	vttest_new[a][1] += k * p * tanh(abs(p));
//	vttest_new[a][2] += k * p * tanh(abs(p));
//	//vSets[b][2] +=tanh(d_old)
//}

/*
���ݿ�������Ķ������ɰ�Χ�����Ƹö���仯�ķ�Χ
parameters���������񶥵����ꣻ���ƶ������������Χ��뾶������鲢����ģ���е�ĳһ�㣬ʹ�䵽ԭ��ľ��벻����һ�������İ뾶r
*/
void ObjLoader::BoundingSphereConstraint(vector<vector<GLfloat>>& vSets_old, int a, float r) {
	float length;
	length = sqrt(pow(vSets[a][0] - vSets_old[a][0], 2) + pow(vSets[a][1] - vSets_old[a][1], 2) + pow(vSets[a][2] - vSets_old[a][2], 2));
	if (length >= r) {
		vSets[a][0] = vSets_old[a][0] + (vSets[a][0] - vSets_old[a][0]) * r / length;
		vSets[a][1] = vSets_old[a][1] + (vSets[a][1] - vSets_old[a][1]) * r / length;
		vSets[a][2] = vSets_old[a][2] + (vSets[a][2] - vSets_old[a][2]) * r / length;
	}
}
/*
���������Լ��������������Ķ�������  parameters����ҪԼ���������壨�ĸ��������������仯ǰ������������ꣻ����ȷ��ģ�͵����������һ�������ķ�Χ�ڣ���������ĳ���ض������Լ��
*/
void ObjLoader::VolumeConstraint(int a, int b, int c, int d, vector<vector<GLfloat>>& vSets_old) {
	double volumeConstraint;//����Լ�������������(����ǰ�����)
	double volume;//���κ����������
	double vc1 = vSets_old[b][0] - vSets_old[a][0];  //x2-x1
	double vc2 = vSets_old[c][0] - vSets_old[a][0];//x3-x1
	double vc3 = vSets_old[d][0] - vSets_old[a][0];//x4-x1
	double vc4 = vSets_old[b][1] - vSets_old[a][1];//y2-y1
	double vc5 = vSets_old[c][1] - vSets_old[a][1];//y3-y1
	double vc6 = vSets_old[d][1] - vSets_old[a][1];//y4-y1
	double vc7 = vSets_old[b][2] - vSets_old[a][2];//z2-z1
	double vc8 = vSets_old[c][2] - vSets_old[a][2];//z3-z1
	double vc9 = vSets_old[d][2] - vSets_old[a][2];//z4-z1

	double v1 = vSets[b][0] - vSets[a][0];
	double v2 = vSets[c][0] - vSets[a][0];
	double v3 = vSets[d][0] - vSets[a][0];
	double v4 = vSets[b][1] - vSets[a][1];
	double v5 = vSets[c][1] - vSets[a][1];
	double v6 = vSets[d][1] - vSets[a][1];
	double v7 = vSets[b][2] - vSets[a][2];
	double v8 = vSets[c][2] - vSets[a][2];
	double v9 = vSets[d][2] - vSets[a][2];
	//�������������
	volumeConstraint = abs(vc1 * (vc5 * vc9 - vc8 * vc6) - vc2 * (vc4 * vc9 - vc7 * vc6) + vc3 * (vc4 * vc8 - vc7 * vc5)) / 6.0;
	volume = abs(v1 * (v5 * v9 - v8 * v6) - v2 * (v4 * v9 - v7 * v6) + v3 * (v4 * v8 - v7 * v5)) / 6.0;
	// ͨ�����Լ�����¶���λ��
	double scaleFactor = pow(volumeConstraint / volume, 1.0 / 3.0);
	double centerx = (vSets[a][0] + vSets[b][0] + vSets[c][0] + vSets[d][0]) / 4.0; //������������������꣬����4��Ϊ�˼����ÿ�����㵽���ĵ�ƽ������
	double centery = (vSets[a][1] + vSets[b][1] + vSets[c][1] + vSets[d][1]) / 4.0;
	double centerz = (vSets[a][2] + vSets[b][2] + vSets[c][2] + vSets[d][2]) / 4.0;
	vSets[a][0] = centerx + (vSets[a][0] - centerx) * scaleFactor;  //�������Ա任��ʹ���ƶ����µ�λ��
	vSets[a][1] = centery + (vSets[a][1] - centery) * scaleFactor;//��ȥ���������ǽ�ÿ��������������ĵ��ƫ������ȥ���õ��µ�ƫ������
	vSets[a][2] = centerz + (vSets[a][2] - centerz) * scaleFactor;//�ٳ��Ա������ӣ�ʹ�������巢���仯

	vSets[b][0] = centerx + (vSets[b][0] - centerx) * scaleFactor;
	vSets[b][1] = centery + (vSets[b][1] - centery) * scaleFactor;
	vSets[b][2] = centerz + (vSets[b][2] - centerz) * scaleFactor;

	vSets[c][0] = centerx + (vSets[c][0] - centerx) * scaleFactor;
	vSets[c][1] = centery + (vSets[c][1] - centery) * scaleFactor;
	vSets[c][2] = centerz + (vSets[c][2] - centerz) * scaleFactor;

	vSets[d][0] = centerx + (vSets[d][0] - centerx) * scaleFactor;
	vSets[d][1] = centery + (vSets[d][1] - centery) * scaleFactor;
	vSets[d][2] = centerz + (vSets[d][2] - centerz) * scaleFactor;
}
//����λ�ö���ѧ���������Լ�������������嶥�����������ݼ���������ģ��λ��
void ObjLoader::VolumeConstraintBypbd(int a, int b, int c, int d, vector<vector<GLfloat>>& vSets_old) {
	double volumeConstraintA, volumeConstraintB, volumeConstraintC, volumeConstraintD ;//����Լ�������������������ǰ�������
	double volumeA, volumeB, volumeC, volumeD;//���κ�����
	double k = 1;//kΪ�նȲ���
	double sA, sB,sC, sD;//sΪ�������� Ϊ���Լ��/�ݶ�ģ��ƽ����

	//��a�ı仯�ݶ�
	//a�е�b�ı仯�ݶ�   1/6*b��c
	double grad1bx = ((vSets[b][1] * vSets[c][2]) - (vSets[b][2] * vSets[c][1])) / 6;  //����ݶȱ�ʾ����vSets[b]��vSets[c]��x�᷽���ϵı仯�ʡ�
	double grad1by = ((vSets[b][2] * vSets[c][0]) - (vSets[b][0] * vSets[c][2])) / 6;
	double grad1bz = ((vSets[b][0] * vSets[c][1]) - (vSets[b][1] * vSets[c][0])) / 6;
	//a�е�c�ı仯�ݶ�   1/6*c��d
	double grad1cx = ((vSets[c][1] * vSets[d][2]) - (vSets[c][2] * vSets[d][1])) / 6;
	double grad1cy = ((vSets[c][2] * vSets[d][0]) - (vSets[c][0] * vSets[d][2])) / 6;
	double grad1cz = ((vSets[c][0] * vSets[d][1]) - (vSets[c][1] * vSets[d][0])) / 6;
	//a�е�d�ı仯�ݶ�   1/6*d��b
	double grad1dx = ((vSets[d][1] * vSets[b][2]) - (vSets[d][2] * vSets[b][1])) / 6;
	double grad1dy = ((vSets[d][2] * vSets[b][0]) - (vSets[d][0] * vSets[b][2])) / 6;
	double grad1dz = ((vSets[d][0] * vSets[b][1]) - (vSets[d][1] * vSets[b][0])) / 6;
	//��a�ı仯�ݶ�   �仯�ݶȺ�Ϊ0
	double gradax = -(grad1bx + grad1cx + grad1dx);
	double graday = -(grad1by + grad1cy + grad1dy);
	double gradaz = -(grad1bz + grad1cz + grad1dz);

	//��b�ı仯�ݶ�
	//b�е�a�ı仯�ݶ�  1/6*a��c
	double grad2ax = ((vSets[a][1] * vSets[c][2]) - (vSets[a][2] * vSets[c][1])) / 6;  
	double grad2ay = ((vSets[a][2] * vSets[c][0]) - (vSets[a][0] * vSets[c][2])) / 6;
	double grad2az = ((vSets[a][0] * vSets[c][1]) - (vSets[a][1] * vSets[c][0])) / 6;
	//b�е�c�ı仯�ݶ�   1/6*c��d
	double grad2cx = ((vSets[c][1] * vSets[d][2]) - (vSets[c][2] * vSets[d][1])) / 6;
	double grad2cy = ((vSets[c][2] * vSets[d][0]) - (vSets[c][0] * vSets[d][2])) / 6;
	double grad2cz = ((vSets[c][0] * vSets[d][1]) - (vSets[c][1] * vSets[d][0])) / 6;
	//b�е�d�ı仯�ݶ�   1/6*d��a
	double grad2dx = ((vSets[d][1] * vSets[a][2]) - (vSets[d][2] * vSets[a][1])) / 6;
	double grad2dy = ((vSets[d][2] * vSets[a][0]) - (vSets[d][0] * vSets[a][2])) / 6;
	double grad2dz = ((vSets[d][0] * vSets[a][1]) - (vSets[d][1] * vSets[a][0])) / 6;
	//��b�ı仯�ݶ�   �仯�ݶȺ�Ϊ0
	double gradbx = -(grad2ax + grad2cx + grad2dx);
	double gradby = -(grad2ay + grad2cy + grad2dy);
	double gradbz = -(grad2az + grad2cz + grad2dz);

	//��c�ı仯�ݶ�
    //c�е�a�ı仯�ݶ�  1/6*a��b
	double grad3ax = ((vSets[a][1] * vSets[b][2]) - (vSets[a][2] * vSets[b][1])) / 6;  
	double grad3ay = ((vSets[a][2] * vSets[b][0]) - (vSets[a][0] * vSets[b][2])) / 6;
	double grad3az = ((vSets[a][0] * vSets[b][1]) - (vSets[a][1] * vSets[b][0])) / 6;
	//c�е�b�ı仯�ݶ�   1/6*b��d
	double grad3bx = ((vSets[b][1] * vSets[d][2]) - (vSets[b][2] * vSets[d][1])) / 6;
	double grad3by = ((vSets[b][2] * vSets[d][0]) - (vSets[b][0] * vSets[d][2])) / 6;
	double grad3bz = ((vSets[b][0] * vSets[d][1]) - (vSets[b][1] * vSets[d][0])) / 6;
	//c�е�d�ı仯�ݶ�   1/6*d��a
	double grad3dx = ((vSets[d][1] * vSets[a][2]) - (vSets[d][2] * vSets[a][1])) / 6;
	double grad3dy = ((vSets[d][2] * vSets[a][0]) - (vSets[d][0] * vSets[a][2])) / 6;
	double grad3dz = ((vSets[d][0] * vSets[a][1]) - (vSets[d][1] * vSets[a][0])) / 6;
	//��c�ı仯�ݶ�   �仯�ݶȺ�Ϊ0
	double gradcx = -(grad3ax + grad3bx + grad3dx);
	double gradcy = -(grad3ay + grad3by + grad3dy);
	double gradcz = -(grad3az + grad3bz + grad3dz);


	//��d�ı仯�ݶ�
    //d�е�a�ı仯�ݶ�  1/6*a��b
	double grad4ax = ((vSets[a][1] * vSets[b][2]) - (vSets[a][2] * vSets[b][1])) / 6;
	double grad4ay = ((vSets[a][2] * vSets[b][0]) - (vSets[a][0] * vSets[b][2])) / 6;
	double grad4az = ((vSets[a][0] * vSets[b][1]) - (vSets[a][1] * vSets[b][0])) / 6;
	//d�е�b�ı仯�ݶ�   1/6*b��c
	double grad4bx = ((vSets[b][1] * vSets[c][2]) - (vSets[b][2] * vSets[c][1])) / 6;
	double grad4by = ((vSets[b][2] * vSets[c][0]) - (vSets[b][0] * vSets[c][2])) / 6;
	double grad4bz = ((vSets[b][0] * vSets[c][1]) - (vSets[b][1] * vSets[c][0])) / 6;
	//d�е�c�ı仯�ݶ�   1/6*c��a
	double grad4cx = ((vSets[c][1] * vSets[a][2]) - (vSets[c][2] * vSets[a][1])) / 6;
	double grad4cy = ((vSets[c][2] * vSets[a][0]) - (vSets[c][0] * vSets[a][2])) / 6;
	double grad4cz = ((vSets[c][0] * vSets[a][1]) - (vSets[c][1] * vSets[a][0])) / 6;
	//��d�ı仯�ݶ�   �仯�ݶȺ�Ϊ0
	double graddx = -(grad4ax + grad4bx + grad4cx);
	double graddy = -(grad4ay + grad4by + grad4cy);
	double graddz = -(grad4az + grad4bz + grad4cz);


	//vab ��ʾ����ǰ������ab  cvab ��ʾ���κ������ab ����ͬ��
	double vabx = vSets_old[b][0] - vSets_old[a][0];
	double vaby = vSets_old[b][1] - vSets_old[a][1];
	double vabz = vSets_old[b][2] - vSets_old[a][2];//ab

	double vacx = vSets_old[c][0] - vSets_old[a][0];
	double vacy = vSets_old[c][1] - vSets_old[a][1];
	double vacz = vSets_old[c][2] - vSets_old[a][2];//ac

	double vadx = vSets_old[d][0] - vSets_old[a][0];
	double vady = vSets_old[d][1] - vSets_old[a][1];
	double vadz = vSets_old[d][2] - vSets_old[a][2];//ad

	//��BΪ����
	double vbax = vSets_old[a][0] - vSets_old[b][0];
	double vbay = vSets_old[a][1] - vSets_old[b][1];
	double vbaz = vSets_old[a][2] - vSets_old[b][2]; //ba����

	double vbcx = vSets_old[c][0] - vSets_old[b][0];
	double vbcy = vSets_old[c][1] - vSets_old[b][1];
	double vbcz = vSets_old[c][2] - vSets_old[b][2]; //bc����

	double vbdx = vSets_old[d][0] - vSets_old[b][0];
	double vbdy = vSets_old[d][1] - vSets_old[b][1];
	double vbdz = vSets_old[d][2] - vSets_old[b][2]; //bd����

	//��CΪ����
	double vcax = vSets_old[a][0] - vSets_old[c][0];
	double vcay = vSets_old[a][1] - vSets_old[c][1];
	double vcaz = vSets_old[a][2] - vSets_old[c][2]; //ca����

	double vcbx = vSets_old[b][0] - vSets_old[c][0];
	double vcby = vSets_old[b][1] - vSets_old[c][1];
	double vcbz = vSets_old[b][2] - vSets_old[c][2]; //cb����

	double vcdx = vSets_old[d][0] - vSets_old[c][0];
	double vcdy = vSets_old[d][1] - vSets_old[c][1];
	double vcdz = vSets_old[d][2] - vSets_old[c][2]; //cd����

	//��DΪ����
	double vdax = vSets_old[a][0] - vSets_old[d][0];
	double vday = vSets_old[a][1] - vSets_old[d][1];
	double vdaz = vSets_old[a][2] - vSets_old[d][2]; //da����

	double vdbx = vSets_old[b][0] - vSets_old[d][0];
	double vdby = vSets_old[b][1] - vSets_old[d][1];
	double vdbz = vSets_old[b][2] - vSets_old[d][2]; //db����

	double vdcx = vSets_old[c][0] - vSets_old[d][0];
	double vdcy = vSets_old[c][1] - vSets_old[d][1];
	double vdcz = vSets_old[c][2] - vSets_old[d][2]; //dc����


	volumeConstraintA = (((vaby * vacz) - (vabz * vacy)) * vadx + ((vabz * vacx) - (vabx * vacz)) * vady + ((vabx * vacy) - (vaby * vacx)) * vadz) / 6;
	volumeConstraintB = (-((vbay * vbcz) - (vbaz * vbcy)) * vbdx - ((vbaz * vbcx) - (vbax * vbcz)) * vbdy - ((vbax * vbcy) - (vbay * vbcx)) * vbdz) / 6;
	volumeConstraintC = (((vcay * vcbz) - (vcaz * vcby)) * vcdx + ((vcaz * vcbx) - (vcax * vcbz)) * vcdy + ((vcax * vcby) - (vcay * vcbx)) * vcdz) / 6;
	volumeConstraintD = (((vday * vdbz) - (vdaz * vdby)) * vdcx + ((vdaz * vdbx) - (vdax * vdbz)) * vdcy + ((vdax * vdby) - (vday * vdbx)) * vdcz) / 6;

	//��AΪ����
	double cvabx = vSets[b][0] - vSets[a][0];   
	double cvaby = vSets[b][1] - vSets[a][1];
	double cvabz = vSets[b][2] - vSets[a][2]; //ab����

	double cvacx = vSets[c][0] - vSets[a][0];
	double cvacy = vSets[c][1] - vSets[a][1];
	double cvacz = vSets[c][2] - vSets[a][2]; //ac����

	double cvadx = vSets[d][0] - vSets[a][0];
	double cvady = vSets[d][1] - vSets[a][1];
	double cvadz = vSets[d][2] - vSets[a][2]; //ad����

	//��BΪ����
	double cvbax = vSets[a][0] - vSets[b][0];
	double cvbay = vSets[a][1] - vSets[b][1];
	double cvbaz = vSets[a][2] - vSets[b][2]; //ba����

	double cvbcx = vSets[c][0] - vSets[b][0];
	double cvbcy = vSets[c][1] - vSets[b][1];
	double cvbcz = vSets[c][2] - vSets[b][2]; //bc����

	double cvbdx = vSets[d][0] - vSets[b][0];
	double cvbdy = vSets[d][1] - vSets[b][1];
	double cvbdz = vSets[d][2] - vSets[b][2]; //bd����

	//��CΪ����
	double cvcax = vSets[a][0] - vSets[c][0];
	double cvcay = vSets[a][1] - vSets[c][1];
	double cvcaz = vSets[a][2] - vSets[c][2]; //ca����

	double cvcbx = vSets[b][0] - vSets[c][0];
	double cvcby = vSets[b][1] - vSets[c][1];
	double cvcbz = vSets[b][2] - vSets[c][2]; //cb����

	double cvcdx = vSets[d][0] - vSets[c][0];
	double cvcdy = vSets[d][1] - vSets[c][1];
	double cvcdz = vSets[d][2] - vSets[c][2]; //cd����

	//��DΪ����
	double cvdax = vSets[a][0] - vSets[d][0];
	double cvday = vSets[a][1] - vSets[d][1];
	double cvdaz = vSets[a][2] - vSets[d][2]; //da����

	double cvdbx = vSets[b][0] - vSets[d][0];
	double cvdby = vSets[b][1] - vSets[d][1];
	double cvdbz = vSets[b][2] - vSets[d][2]; //db����

	double cvdcx = vSets[c][0] - vSets[d][0];
	double cvdcy = vSets[c][1] - vSets[d][1];
	double cvdcz = vSets[c][2] - vSets[d][2]; //dc����


	volumeA = (((cvaby * cvacz) - (cvabz * cvacy)) * cvadx + ((cvabz * cvacx) - (cvabx * cvacz)) * cvady + ((cvabx * cvacy) - (cvaby * cvacx)) * cvadz) / 6;
	volumeB = (-((cvbay * cvbcz) - (cvbaz * cvbcy)) * cvbdx - ((cvbaz * cvbcx) - (cvbax * cvbcz)) * cvbdy - ((cvbax * cvbcy) - (cvbay * cvbcx)) * cvbdz) / 6;
	volumeC = (((cvcay * cvcbz) - (cvcaz * cvcby)) * cvcdx + ((cvcaz * cvcbx) - (cvcax * cvcbz)) * cvcdy + ((cvcax * cvcby) - (cvcay * cvcbx)) * cvcdz) / 6;
	volumeD = (((cvday * cvdbz) - (cvdaz * cvdby)) * cvdcx + ((cvdaz * cvdbx) - (cvdax * cvdbz)) * cvdcy + ((cvdax * cvdby) - (cvday * cvdbx)) * cvdcz) / 6;

	//sΪ�������� Ϊ���Լ��/�ݶ�ģ��ƽ����
	sA = (volumeA - volumeConstraintA) / (pow(gradbx, 2) + pow(gradby, 2) + pow(gradbz, 2) +
		pow(gradcx, 2) + pow(gradcy, 2) + pow(gradcz, 2) +
		pow(graddx, 2) + pow(graddy, 2) + pow(graddz, 2) +
		pow(gradax, 2) + pow(graday, 2) + pow(gradaz, 2));
	sB = (volumeB - volumeConstraintB) / (pow(gradbx, 2) + pow(gradby, 2) + pow(gradbz, 2) +
		pow(gradcx, 2) + pow(gradcy, 2) + pow(gradcz, 2) +
		pow(graddx, 2) + pow(graddy, 2) + pow(graddz, 2) +
		pow(gradax, 2) + pow(graday, 2) + pow(gradaz, 2));
	sC = (volumeC - volumeConstraintC) / (pow(gradbx, 2) + pow(gradby, 2) + pow(gradbz, 2) +
		pow(gradcx, 2) + pow(gradcy, 2) + pow(gradcz, 2) +
		pow(graddx, 2) + pow(graddy, 2) + pow(graddz, 2) +
		pow(gradax, 2) + pow(graday, 2) + pow(gradaz, 2));
	sD = (volumeD - volumeConstraintD) / (pow(gradbx, 2) + pow(gradby, 2) + pow(gradbz, 2) +
		pow(gradcx, 2) + pow(gradcy, 2) + pow(gradcz, 2) +
		pow(graddx, 2) + pow(graddy, 2) + pow(graddz, 2) +
		pow(gradax, 2) + pow(graday, 2) + pow(gradaz, 2));

	//�ı�ģ�͵��λ�á�changA��ʾ��a���������ӣ�S��һ���������ӣ����ڵ��������������仯����k������һ���������ӣ������ڵ������������״�仯����
	double changeAx = sA * k * gradax;
	double changeAy = sA * k * graday;
	double changeAz = sA * k * gradaz;

	double changeBx = sB * k * gradbx;
	double changeBy = sB * k * gradby;
	double changeBz = sB * k * gradbz;

	double changeCx = sC * k * gradcx;
	double changeCy = sC * k * gradcy;
	double changeCz = sC * k * gradcz;

	double changeDx = sD * k * graddx;
	double changeDy = sD * k * graddy;
	double changeDz = sD * k * graddz;



	vSets[a][0] = vSets[a][0] + changeAx;
	vSets[a][1] = vSets[a][1] + changeAy;
	vSets[a][2] = vSets[a][2] + changeAz;

	vSets[b][0] = vSets[b][0] + changeBx;
	vSets[b][1] = vSets[b][1] + changeBy;
	vSets[b][2] = vSets[b][2] + changeBz;

	vSets[c][0] = vSets[c][0] + changeCx;
	vSets[c][1] = vSets[c][1] + changeCy;
	vSets[c][2] = vSets[c][2] + changeCz;

	vSets[d][0] = vSets[d][0] + changeDx;
	vSets[d][1] = vSets[d][1] + changeDy;
	vSets[d][2] = vSets[d][2] + changeDz;
}
void ObjLoader::VolumeConstrainttest(float vtSetstest[4][3], float vttest_new[4][3]) {
	double volumeConstraintA, volumeConstraintB, volumeConstraintC, volumeConstraintD;//����Լ�������������������ǰ�������
	double volumeA, volumeB, volumeC, volumeD;//���κ�����
	double k = 1;//kΪ�նȲ���    
	double sA, sB, sC, sD;//sΪ�������� Ϊ���Լ��/�ݶ�ģ��ƽ����


	//��a�ı仯�ݶ�
	//a�е�b�ı仯�ݶ�   1/6*b��c
	double grad1bx = ((vttest_new[1][1] * vttest_new[2][2]) - (vttest_new[1][2] * vttest_new[2][1])) / 6;  //vttest_new[0][0] a=0,b=1,c=2,d=3
	double grad1by = ((vttest_new[1][2] * vttest_new[2][0]) - (vttest_new[1][0] * vttest_new[2][2])) / 6;
	double grad1bz = ((vttest_new[1][0] * vttest_new[2][1]) - (vttest_new[1][1] * vttest_new[2][0])) / 6;
	//a�е�c�ı仯�ݶ�   1/6*c��d
	double grad1cx = ((vttest_new[2][1] * vttest_new[3][2]) - (vttest_new[2][2] * vttest_new[3][1])) / 6;
	double grad1cy = ((vttest_new[2][2] * vttest_new[3][0]) - (vttest_new[2][0] * vttest_new[3][2])) / 6;
	double grad1cz = ((vttest_new[2][0] * vttest_new[3][1]) - (vttest_new[2][1] * vttest_new[3][0])) / 6;
	//a�е�d�ı仯�ݶ�   1/6*d��b
	double grad1dx = ((vttest_new[3][1] * vttest_new[1][2]) - (vttest_new[3][2] * vttest_new[1][1])) / 6;
	double grad1dy = ((vttest_new[3][2] * vttest_new[1][0]) - (vttest_new[3][0] * vttest_new[1][2])) / 6;
	double grad1dz = ((vttest_new[3][0] * vttest_new[1][1]) - (vttest_new[3][1] * vttest_new[1][0])) / 6;
	//��a�ı仯�ݶ�   �仯�ݶȺ�Ϊ0
	double gradax = -(grad1bx + grad1cx + grad1dx);
	double graday = -(grad1by + grad1cy + grad1dy);
	double gradaz = -(grad1bz + grad1cz + grad1dz);

	//��b�ı仯�ݶ�
	//b�е�a�ı仯�ݶ�  1/6*a��c
	double grad2ax = ((vttest_new[0][1] * vttest_new[2][2]) - (vttest_new[0][2] * vttest_new[2][1])) / 6;
	double grad2ay = ((vttest_new[0][2] * vttest_new[2][0]) - (vttest_new[0][0] * vttest_new[2][2])) / 6;
	double grad2az = ((vttest_new[0][0] * vttest_new[2][1]) - (vttest_new[0][1] * vttest_new[2][0])) / 6;
	//b�е�c�ı仯�ݶ�   1/6*c��d
	double grad2cx = ((vttest_new[2][1] * vttest_new[3][2]) - (vttest_new[2][2] * vttest_new[3][1])) / 6;
	double grad2cy = ((vttest_new[2][2] * vttest_new[3][0]) - (vttest_new[2][0] * vttest_new[3][2])) / 6;
	double grad2cz = ((vttest_new[2][0] * vttest_new[3][1]) - (vttest_new[2][1] * vttest_new[3][0])) / 6;
	//b�е�d�ı仯�ݶ�   1/6*d��a
	double grad2dx = ((vttest_new[3][1] * vttest_new[0][2]) - (vttest_new[3][2] * vttest_new[0][1])) / 6;
	double grad2dy = ((vttest_new[3][2] * vttest_new[0][0]) - (vttest_new[3][0] * vttest_new[0][2])) / 6;
	double grad2dz = ((vttest_new[3][0] * vttest_new[0][1]) - (vttest_new[3][1] * vttest_new[0][0])) / 6;
	//��b�ı仯�ݶ�   �仯�ݶȺ�Ϊ0
	double gradbx = -(grad2ax + grad2cx + grad2dx);
	double gradby = -(grad2ay + grad2cy + grad2dy);
	double gradbz = -(grad2az + grad2cz + grad2dz);

	//��c�ı仯�ݶ�
	//c�е�a�ı仯�ݶ�  1/6*a��b
	double grad3ax = ((vttest_new[0][1] * vttest_new[1][2]) - (vttest_new[0][2] * vttest_new[1][1])) / 6;
	double grad3ay = ((vttest_new[0][2] * vttest_new[1][0]) - (vttest_new[0][0] * vttest_new[1][2])) / 6;
	double grad3az = ((vttest_new[0][0] * vttest_new[1][1]) - (vttest_new[0][1] * vttest_new[1][0])) / 6;
	//c�е�b�ı仯�ݶ�   1/6*b��d
	double grad3bx = ((vttest_new[1][1] * vttest_new[3][2]) - (vttest_new[1][2] * vttest_new[3][1])) / 6;
	double grad3by = ((vttest_new[1][2] * vttest_new[3][0]) - (vttest_new[1][0] * vttest_new[3][2])) / 6;
	double grad3bz = ((vttest_new[1][0] * vttest_new[3][1]) - (vttest_new[1][1] * vttest_new[3][0])) / 6;
	//c�е�d�ı仯�ݶ�   1/6*d��a
	double grad3dx = ((vttest_new[3][1] * vttest_new[0][2]) - (vttest_new[3][2] * vttest_new[0][1])) / 6;
	double grad3dy = ((vttest_new[3][2] * vttest_new[0][0]) - (vttest_new[3][0] * vttest_new[0][2])) / 6;
	double grad3dz = ((vttest_new[3][0] * vttest_new[0][1]) - (vttest_new[3][1] * vttest_new[0][0])) / 6;
	//��c�ı仯�ݶ�   �仯�ݶȺ�Ϊ0
	double gradcx = -(grad3ax + grad3bx + grad3dx);
	double gradcy = -(grad3ay + grad3by + grad3dy);
	double gradcz = -(grad3az + grad3bz + grad3dz);


	//��d�ı仯�ݶ�
	//d�е�a�ı仯�ݶ�  1/6*a��b
	double grad4ax = ((vttest_new[0][1] * vttest_new[1][2]) - (vttest_new[0][2] * vttest_new[1][1])) / 6;
	double grad4ay = ((vttest_new[0][2] * vttest_new[1][0]) - (vttest_new[0][0] * vttest_new[1][2])) / 6;
	double grad4az = ((vttest_new[0][0] * vttest_new[1][1]) - (vttest_new[0][1] * vttest_new[1][0])) / 6;
	//d�е�b�ı仯�ݶ�   1/6*b��c
	double grad4bx = ((vttest_new[1][1] * vttest_new[2][2]) - (vttest_new[1][2] * vttest_new[2][1])) / 6;
	double grad4by = ((vttest_new[1][2] * vttest_new[2][0]) - (vttest_new[1][0] * vttest_new[2][2])) / 6;
	double grad4bz = ((vttest_new[1][0] * vttest_new[2][1]) - (vttest_new[1][1] * vttest_new[2][0])) / 6;
	//d�е�c�ı仯�ݶ�   1/6*c��a
	double grad4cx = ((vttest_new[2][1] * vttest_new[0][2]) - (vttest_new[2][2] * vttest_new[0][1])) / 6;
	double grad4cy = ((vttest_new[2][2] * vttest_new[0][0]) - (vttest_new[2][0] * vttest_new[0][2])) / 6;
	double grad4cz = ((vttest_new[2][0] * vttest_new[0][1]) - (vttest_new[2][1] * vttest_new[0][0])) / 6;
	//��d�ı仯�ݶ�   �仯�ݶȺ�Ϊ0
	double graddx = -(grad4ax + grad4bx + grad4cx);
	double graddy = -(grad4ay + grad4by + grad4cy);
	double graddz = -(grad4az + grad4bz + grad4cz);


	//vab ��ʾ����ǰ������ab  cvab ��ʾ���κ������ab ����ͬ��                             //vtSetstest[0][0] a=0,b=1,c=2,d=3
	double vabx = vtSetstest[1][0] - vtSetstest[0][0];
	double vaby = vtSetstest[1][1] - vtSetstest[0][1];
	double vabz = vtSetstest[1][2] - vtSetstest[0][2];//ab

	double vacx = vtSetstest[2][0] - vtSetstest[0][0];
	double vacy = vtSetstest[2][1] - vtSetstest[0][1];
	double vacz = vtSetstest[2][2] - vtSetstest[0][2];//ac

	double vadx = vtSetstest[3][0] - vtSetstest[0][0];
	double vady = vtSetstest[3][1] - vtSetstest[0][1];
	double vadz = vtSetstest[3][2] - vtSetstest[0][2];//ad

	//��BΪ����
	double vbax = vtSetstest[0][0] - vtSetstest[1][0];
	double vbay = vtSetstest[0][1] - vtSetstest[1][1];
	double vbaz = vtSetstest[0][2] - vtSetstest[1][2]; //ba����

	double vbcx = vtSetstest[2][0] - vtSetstest[1][0];
	double vbcy = vtSetstest[2][1] - vtSetstest[1][1];
	double vbcz = vtSetstest[2][2] - vtSetstest[1][2]; //bc����

	double vbdx = vtSetstest[3][0] - vtSetstest[1][0];
	double vbdy = vtSetstest[3][1] - vtSetstest[1][1];
	double vbdz = vtSetstest[3][2] - vtSetstest[1][2]; //bd����

	//��CΪ����
	double vcax = vtSetstest[0][0] - vtSetstest[2][0];
	double vcay = vtSetstest[0][1] - vtSetstest[2][1];
	double vcaz = vtSetstest[0][2] - vtSetstest[2][2]; //ca����

	double vcbx = vtSetstest[1][0] - vtSetstest[2][0];
	double vcby = vtSetstest[1][1] - vtSetstest[2][1];
	double vcbz = vtSetstest[1][2] - vtSetstest[2][2]; //cb����

	double vcdx = vtSetstest[3][0] - vtSetstest[2][0];
	double vcdy = vtSetstest[3][1] - vtSetstest[2][1];
	double vcdz = vtSetstest[3][2] - vtSetstest[2][2]; //cd����

	//��DΪ����
	double vdax = vtSetstest[0][0] - vtSetstest[3][0];
	double vday = vtSetstest[0][1] - vtSetstest[3][1];
	double vdaz = vtSetstest[0][2] - vtSetstest[3][2]; //da����

	double vdbx = vtSetstest[1][0] - vtSetstest[3][0];
	double vdby = vtSetstest[1][1] - vtSetstest[3][1];
	double vdbz = vtSetstest[1][2] - vtSetstest[3][2]; //db����

	double vdcx = vtSetstest[2][0] - vtSetstest[3][0];
	double vdcy = vtSetstest[2][1] - vtSetstest[3][1];
	double vdcz = vtSetstest[2][2] - vtSetstest[3][2]; //dc����


	volumeConstraintA = (-((vaby * vacz) - (vabz * vacy)) * vadx - ((vabz * vacx) - (vabx * vacz)) * vady - ((vabx * vacy) - (vaby * vacx)) * vadz) / 6;  //-1/6  ������һ�����Ϊ1/6
	//volumeConstraintB = (-((vbay * vbcz) - (vbaz * vbcy)) * vbdx - ((vbaz * vbcx) - (vbax * vbcz)) * vbdy - ((vbax * vbcy) - (vbay * vbcx)) * vbdz) / 6;  
	volumeConstraintB = (((vbay * vbcz) - (vbaz * vbcy)) * vbdx + ((vbaz * vbcx) - (vbax * vbcz)) * vbdy + ((vbax * vbcy) - (vbay * vbcx)) * vbdz) / 6;   //-1/6
	volumeConstraintC = (-((vcay * vcbz) - (vcaz * vcby)) * vcdx - ((vcaz * vcbx) - (vcax * vcbz)) * vcdy - ((vcax * vcby) - (vcay * vcbx)) * vcdz) / 6;   //-1/6
	volumeConstraintD = (((vday * vdbz) - (vdaz * vdby)) * vdcx + ((vdaz * vdbx) - (vdax * vdbz)) * vdcy + ((vdax * vdby) - (vday * vdbx)) * vdcz) / 6;  //-1/6

	//��AΪ����
	double cvabx = vttest_new[1][0] - vttest_new[0][0];                                   //vttest_new[0][0]��vttest_new[0][1]��vttest_new[0][2]  a=0,b=1,c=2,d=3
	double cvaby = vttest_new[1][1] - vttest_new[0][1];
	double cvabz = vttest_new[1][2] - vttest_new[0][2]; //ab����

	double cvacx = vttest_new[2][0] - vttest_new[0][0];
	double cvacy = vttest_new[2][1] - vttest_new[0][1];
	double cvacz = vttest_new[2][2] - vttest_new[0][2]; //ac����

	double cvadx = vttest_new[3][0] - vttest_new[0][0];
	double cvady = vttest_new[3][1] - vttest_new[0][1];
	double cvadz = vttest_new[3][2] - vttest_new[0][2]; //ad����

	//��BΪ����
	double cvbax = vttest_new[0][0] - vttest_new[1][0];
	double cvbay = vttest_new[0][1] - vttest_new[1][1];
	double cvbaz = vttest_new[0][2] - vttest_new[1][2]; //ba����

	double cvbcx = vttest_new[2][0] - vttest_new[1][0];
	double cvbcy = vttest_new[2][1] - vttest_new[1][1];
	double cvbcz = vttest_new[2][2] - vttest_new[1][2]; //bc����

	double cvbdx = vttest_new[3][0] - vttest_new[1][0];
	double cvbdy = vttest_new[3][1] - vttest_new[1][1];
	double cvbdz = vttest_new[3][2] - vttest_new[1][2]; //bd����

	//��CΪ����
	double cvcax = vttest_new[0][0] - vttest_new[2][0];
	double cvcay = vttest_new[0][1] - vttest_new[2][1];
	double cvcaz = vttest_new[0][2] - vttest_new[2][2]; //ca����

	double cvcbx = vttest_new[1][0] - vttest_new[2][0];
	double cvcby = vttest_new[1][1] - vttest_new[2][1];
	double cvcbz = vttest_new[1][2] - vttest_new[2][2]; //cb����

	double cvcdx = vttest_new[3][0] - vttest_new[2][0];
	double cvcdy = vttest_new[3][1] - vttest_new[2][1];
	double cvcdz = vttest_new[3][2] - vttest_new[2][2]; //cd����

	//��DΪ����
	double cvdax = vttest_new[0][0] - vttest_new[3][0];
	double cvday = vttest_new[0][1] - vttest_new[3][1];
	double cvdaz = vttest_new[0][2] - vttest_new[3][2]; //da����

	double cvdbx = vttest_new[1][0] - vttest_new[3][0];
	double cvdby = vttest_new[1][1] - vttest_new[3][1];
	double cvdbz = vttest_new[1][2] - vttest_new[3][2]; //db����

	double cvdcx = vttest_new[2][0] - vttest_new[3][0];
	double cvdcy = vttest_new[2][1] - vttest_new[3][1];
	double cvdcz = vttest_new[2][2] - vttest_new[3][2]; //dc����


	volumeA = (-((cvaby * cvacz) - (cvabz * cvacy)) * cvadx - ((cvabz * cvacx) - (cvabx * cvacz)) * cvady - ((cvabx * cvacy) - (cvaby * cvacx)) * cvadz) / 6; //-2/3   1/3
	//volumeB = (-((cvbay * cvbcz) - (cvbaz * cvbcy)) * cvbdx - ((cvbaz * cvbcx) - (cvbax * cvbcz)) * cvbdy - ((cvbax * cvbcy) - (cvbay * cvbcx)) * cvbdz) / 6;  
	volumeB = (((cvbay * cvbcz) - (cvbaz * cvbcy)) * cvbdx + ((cvbaz * cvbcx) - (cvbax * cvbcz)) * cvbdy + ((cvbax * cvbcy) - (cvbay * cvbcx)) * cvbdz) / 6;  //-2/3
	volumeC = (-((cvcay * cvcbz) - (cvcaz * cvcby)) * cvcdx - ((cvcaz * cvcbx) - (cvcax * cvcbz)) * cvcdy - ((cvcax * cvcby) - (cvcay * cvcbx)) * cvcdz) / 6;  //-2/3
	volumeD = (((cvday * cvdbz) - (cvdaz * cvdby)) * cvdcx + ((cvdaz * cvdbx) - (cvdax * cvdbz)) * cvdcy + ((cvdax * cvdby) - (cvday * cvdbx)) * cvdcz) / 6;  //-2/3

	//sΪ�������� Ϊ���Լ��/�ݶ�ģ��ƽ����
	sA = (volumeA - volumeConstraintA) / (pow(gradbx, 2) + pow(gradby, 2) + pow(gradbz, 2) +
		pow(gradcx, 2) + pow(gradcy, 2) + pow(gradcz, 2) +
		pow(graddx, 2) + pow(graddy, 2) + pow(graddz, 2) +
		pow(gradax, 2) + pow(graday, 2) + pow(gradaz, 2));
	sB = (volumeB - volumeConstraintB) / (pow(gradbx, 2) + pow(gradby, 2) + pow(gradbz, 2) +
		pow(gradcx, 2) + pow(gradcy, 2) + pow(gradcz, 2) +
		pow(graddx, 2) + pow(graddy, 2) + pow(graddz, 2) +
		pow(gradax, 2) + pow(graday, 2) + pow(gradaz, 2));
	sC = (volumeC - volumeConstraintC) / (pow(gradbx, 2) + pow(gradby, 2) + pow(gradbz, 2) +
		pow(gradcx, 2) + pow(gradcy, 2) + pow(gradcz, 2) +
		pow(graddx, 2) + pow(graddy, 2) + pow(graddz, 2) +
		pow(gradax, 2) + pow(graday, 2) + pow(gradaz, 2));
	sD = (volumeD - volumeConstraintD) / (pow(gradbx, 2) + pow(gradby, 2) + pow(gradbz, 2) +
		pow(gradcx, 2) + pow(gradcy, 2) + pow(gradcz, 2) +
		pow(graddx, 2) + pow(graddy, 2) + pow(graddz, 2) +
		pow(gradax, 2) + pow(graday, 2) + pow(gradaz, 2));

	//�ı�ģ�͵��λ�á�changA��ʾ��a���������ӣ�S��һ���������ӣ����ڵ��������������仯����k������һ���������ӣ������ڵ������������״�仯����
 //   double changeAx =  k * gradax;
	//double changeAy =  k * graday;
	//double changeAz =  k * gradaz;

	//double changeBx =  k * gradbx;
	//double changeBy =  k * gradby;
	//double changeBz =  k * gradbz;

	//double changeCx =  k * gradcx;
	//double changeCy =  k * gradcy;
	//double changeCz =  k * gradcz;

	//double changeDx =  k * graddx;
	//double changeDy =  k * graddy;
	//double changeDz =  k * graddz;
	// 
	double changeAx = sA * k * gradax;
	double changeAy = sA * k * graday;
	double changeAz = sA * k * gradaz;

	double changeBx = sB * k * gradbx;
	double changeBy = sB * k * gradby;
	double changeBz = sB * k * gradbz;

	double changeCx = sC * k * gradcx;
	double changeCy = sC * k * gradcy;
	double changeCz = sC * k * gradcz;

	double changeDx = sD * k * graddx;
	double changeDy = sD * k * graddy;
	double changeDz = sD * k * graddz;



	vttest_new[0][0] = vttest_new[0][0] + changeAx;
	vttest_new[0][1] = vttest_new[0][1] + changeAy;
	vttest_new[0][2] = vttest_new[0][2] + changeAz;

	vttest_new[1][0] = vttest_new[1][0] + changeBx;
	vttest_new[1][1] = vttest_new[1][1] + changeBy;
	vttest_new[1][2] = vttest_new[1][2] + changeBz;

	vttest_new[2][0] = vttest_new[2][0] + changeCx;
	vttest_new[2][1] = vttest_new[2][1] + changeCy;
	vttest_new[2][2] = vttest_new[2][2] + changeCz;

	vttest_new[3][0] = vttest_new[3][0] + changeDx;
	vttest_new[3][1] = vttest_new[3][1] + changeDy;
	vttest_new[3][2] = vttest_new[3][2] + changeDz;


	cvabx = vttest_new[1][0] - vttest_new[0][0];                                   //vttest_new[0][0]��vttest_new[0][1]��vttest_new[0][2]  a=0,b=1,c=2,d=3
	cvaby = vttest_new[1][1] - vttest_new[0][1];
	cvabz = vttest_new[1][2] - vttest_new[0][2]; //ab����

	 cvacx = vttest_new[2][0] - vttest_new[0][0];
	 cvacy = vttest_new[2][1] - vttest_new[0][1];
	 cvacz = vttest_new[2][2] - vttest_new[0][2]; //ac����

	 cvadx = vttest_new[3][0] - vttest_new[0][0];
	 cvady = vttest_new[3][1] - vttest_new[0][1];
	 cvadz = vttest_new[3][2] - vttest_new[0][2]; //ad����

	//��BΪ����
	 cvbax = vttest_new[0][0] - vttest_new[1][0];
	 cvbay = vttest_new[0][1] - vttest_new[1][1];
	  cvbaz = vttest_new[0][2] - vttest_new[1][2]; //ba����

	  cvbcx = vttest_new[2][0] - vttest_new[1][0];
	  cvbcy = vttest_new[2][1] - vttest_new[1][1];
	  cvbcz = vttest_new[2][2] - vttest_new[1][2]; //bc����

	  cvbdx = vttest_new[3][0] - vttest_new[1][0];
	  cvbdy = vttest_new[3][1] - vttest_new[1][1];
	  cvbdz = vttest_new[3][2] - vttest_new[1][2]; //bd����

	//��CΪ����
	  cvcax = vttest_new[0][0] - vttest_new[2][0];
	  cvcay = vttest_new[0][1] - vttest_new[2][1];
	  cvcaz = vttest_new[0][2] - vttest_new[2][2]; //ca����

	  cvcbx = vttest_new[1][0] - vttest_new[2][0];
	  cvcby = vttest_new[1][1] - vttest_new[2][1];
	  cvcbz = vttest_new[1][2] - vttest_new[2][2]; //cb����

	  cvcdx = vttest_new[3][0] - vttest_new[2][0];
	  cvcdy = vttest_new[3][1] - vttest_new[2][1];
	  cvcdz = vttest_new[3][2] - vttest_new[2][2]; //cd����

	//��DΪ����
	  cvdax = vttest_new[0][0] - vttest_new[3][0];
	  cvday = vttest_new[0][1] - vttest_new[3][1];
	  cvdaz = vttest_new[0][2] - vttest_new[3][2]; //da����

	  cvdbx = vttest_new[1][0] - vttest_new[3][0];
	  cvdby = vttest_new[1][1] - vttest_new[3][1];
	  cvdbz = vttest_new[1][2] - vttest_new[3][2]; //db����

	  cvdcx = vttest_new[2][0] - vttest_new[3][0];
	  cvdcy = vttest_new[2][1] - vttest_new[3][1];
	  cvdcz = vttest_new[2][2] - vttest_new[3][2]; //dc����


	volumeA = (-((cvaby * cvacz) - (cvabz * cvacy)) * cvadx - ((cvabz * cvacx) - (cvabx * cvacz)) * cvady - ((cvabx * cvacy) - (cvaby * cvacx)) * cvadz) / 6; //-21/4
	//volumeB = (-((cvbay * cvbcz) - (cvbaz * cvbcy)) * cvbdx - ((cvbaz * cvbcx) - (cvbax * cvbcz)) * cvbdy - ((cvbax * cvbcy) - (cvbay * cvbcx)) * cvbdz) / 6;  
	volumeB = (((cvbay * cvbcz) - (cvbaz * cvbcy)) * cvbdx + ((cvbaz * cvbcx) - (cvbax * cvbcz)) * cvbdy + ((cvbax * cvbcy) - (cvbay * cvbcx)) * cvbdz) / 6;  //-21/4
	volumeC = (-((cvcay * cvcbz) - (cvcaz * cvcby)) * cvcdx - ((cvcaz * cvcbx) - (cvcax * cvcbz)) * cvcdy - ((cvcax * cvcby) - (cvcay * cvcbx)) * cvcdz) / 6;  //-21/4
	volumeD = (((cvday * cvdbz) - (cvdaz * cvdby)) * cvdcx + ((cvdaz * cvdbx) - (cvdax * cvdbz)) * cvdcy + ((cvdax * cvdby) - (cvday * cvdbx)) * cvdcz) / 6;  //-21/4



}



//void ObjLoader::ComputerSkeletonPointRestriction(float k,int a,int b, int c, int d,int e,int f, vector<vector<GLfloat>>& vSets_old, float centers[2][3]) {
//	float dis_old= sqrt(pow(vSets_old[e][0] - centers[f][0], 2) + pow(vSets_old[e][1] - centers[f][1], 2) + pow(vSets_old[e][2] - centers[f][2], 2));
//	double centerx = (vSets[a][0] + vSets[b][0] + vSets[c][0] + vSets[d][0]) / 4.0; //������������������꣬����4��Ϊ�˼����ÿ�����㵽���ĵ�ƽ������
//	double centery = (vSets[a][1] + vSets[b][1] + vSets[c][1] + vSets[d][1]) / 4.0;
//	double centerz = (vSets[a][2] + vSets[b][2] + vSets[c][2] + vSets[d][2]) / 4.0;
//	float dis_new= sqrt(pow(vSets[e][0] - centerx, 2) + pow(vSets[e][1] - centery, 2) + pow(vSets[e][2] - centerz, 2));
//	float dis = (dis_new - dis_old) / dis_new;    //��Ծ���仯
//	vSets[e][0] += 0.5*k * dis*(vSets[e][0] - centerx); //x������Ծ���仯
//	vSets[e][1] += 0.5*k * dis*(vSets[e][1] - centery); //y������Ծ���仯
//	vSets[e][2] +=0.5* k * dis*(vSets[e][2] - centerz); //z������Ծ���仯
//
//}


//�޸�����������ĵ������
void ObjLoader::ChangeTetrahSet() {

	if (vSets.size() < 1000) {
		vSets_old.assign(vSets.begin(), vSets.end());
		string line;
		fstream f;
		int count = 0;
		//����ģ�Ͷ�������ȡ��������仯��Ķ�������  �˴���ӿ�����������txt����
		f.open("C:/Users/reckless/Desktop/3Dģ�ͻ���/datou2.txt", ios::in);

		if (!f.is_open()) {
			cout << "Something Went Wrong When Opening Txtfiles" << endl;
		}

		while (!f.eof()) {

			getline(f, line);//�õ�txt�ļ��е�һ�У���Ϊһ���ַ���

			vector<string>parameters;

			string tailMark = ",";

			string ans = "";

			line = line.append(tailMark);

			for (int i = 0; i < line.length(); i++) {

				char ch = line[i];
				if (ch != ',') {
					ans += ch;
				}
				else {
					if (ans != "") {
						parameters.push_back(ans);//ȡ���ַ����е�Ԫ�أ���,�з�
					}
					ans = "";
				}
			}
			if (parameters.size() == 3) {

				for (int i = 0; i < 3; i++) {//��1��ʼ���������xyz��������Ž�����vector

					GLfloat xyz = atof(parameters[i].c_str());
					vSets[count][i] = xyz;
					if (i == 2) { count++; }
				}

			}
		}

		/*for (int i = 0; i < vSets.size(); i++) {
			vSets[i][2] += 0;
			}*/
			//��ӶԴ����̧���ֵĵ�Լ��
		/*vSets[73] = vSets_old[73];
		vSets[74] = vSets_old[74];
		vSets[78] = vSets_old[78];

		vSets[94] = vSets_old[94];
		vSets[107] = vSets_old[107];
		vSets[108] = vSets_old[108];
		vSets[117] = vSets_old[117];
		vSets[118] = vSets_old[118];
		vSets[119] = vSets_old[119];
		vSets[120] = vSets_old[120];*/

		/*vSets[119] = vSets_old[119];
		vSets[120] = vSets_old[120];
		vSets[75] = vSets_old[75];
		vSets[92] = vSets_old[92];
		vSets[78] = vSets_old[78];
		vSets[107] = vSets_old[107];
		vSets[108] = vSets_old[108];*/

		//����Ϊģ����������ʱ��Լ��
/*StretchConstraint1(43,48,2.0,vSets_old);
StretchConstraint1(44,47,2.0,vSets_old);
StretchConstraint1(19,24,2.0,vSets_old);
StretchConstraint1(22,23,2.0,vSets_old);*/

		//����Ϊģ��������ʱ��Լ��
		StretchConstraint1(38,40,  0.8, vSets_old);
		StretchConstraint1(37,39, 0.8, vSets_old);
		StretchConstraint1(14,16, 0.8, vSets_old);
		StretchConstraint1(13,15, 0.8, vSets_old);




		//ComputerSkeletonPointRestriction(0.8, 34, 35, 11, 18, 34, 1, vSets_old, centers);
		//ComputerSkeletonPointRestriction(0.8, 34, 35, 11, 18, 36, 1, vSets_old, centers);
		//ComputerSkeletonPointRestriction(0.8, 34, 35, 11, 18, 38, 1, vSets_old, centers);
		//ComputerSkeletonPointRestriction(0.8, 34, 35, 11, 18, 37, 1, vSets_old, centers);
		//ComputerSkeletonPointRestriction(0.8, 34, 35, 11, 18, 12, 1, vSets_old, centers);
		//ComputerSkeletonPointRestriction(0.8, 34, 35, 11, 18, 13, 1, vSets_old, centers);
		//ComputerSkeletonPointRestriction(0.8, 34, 35, 11, 18, 14, 1, vSets_old, centers);
		//ComputerSkeletonPointRestriction(0.8, 34, 35, 11, 18, 8, 1, vSets_old, centers);
		//ComputerSkeletonPointRestriction(0.8, 34, 35, 11, 18, 128, 1, vSets_old, centers);
		//ComputerSkeletonPointRestriction(0.8, 34, 35, 11, 18, 11, 1, vSets_old, centers);



	







		//StretchConstraint();
		//���������Լ��  ����151-160

		
		
		
		
		
		VolumeConstrainttest( vtSetstest, vttest_new);
		
		
		//VolumeConstraintBypbd(89, 103, 106, 105, vSets_old);
		//VolumeConstraintBypbd(86, 89, 87, 103, vSets_old);
		//VolumeConstraintBypbd(89, 87, 88, 105, vSets_old);
		//VolumeConstraintBypbd(89, 87, 103, 105, vSets_old);

		//VolumeConstraintBypbd(87, 103, 102, 101, vSets_old);
		//VolumeConstraintBypbd(85, 103, 104, 101, vSets_old);
		//VolumeConstraintBypbd(86, 85, 87, 103, vSets_old);
		//VolumeConstraintBypbd(85, 87, 84, 101, vSets_old);
		//VolumeConstraintBypbd(85, 87, 103, 101, vSets_old);

		//VolumeConstraintBypbd(107, 100, 99, 101, vSets_old);
		//VolumeConstraintBypbd(85, 100, 104, 101, vSets_old);
		//VolumeConstraintBypbd(109, 85, 107, 100, vSets_old);
		//VolumeConstraintBypbd(85, 107, 84, 101, vSets_old);
		//VolumeConstraintBypbd(85, 107, 100, 101, vSets_old);

		//VolumeConstraintBypbd(107, 100, 99, 92, vSets_old);
		//VolumeConstraintBypbd(75, 100, 77, 92, vSets_old);
		//VolumeConstraintBypbd(109, 75, 107, 100, vSets_old);
		//VolumeConstraintBypbd(75, 107, 108, 92, vSets_old);
		//VolumeConstraintBypbd(75, 107, 100, 92, vSets_old);

		//VolumeConstraintBypbd(117, 90, 77, 92, vSets_old);
		//VolumeConstraintBypbd(117, 75, 77, 92, vSets_old);
		//VolumeConstraintBypbd(117, 75, 108, 92, vSets_old);

		//VolumeConstraintBypbd(117, 90, 76, 77, vSets_old);
		//VolumeConstraintBypbd(117, 74, 76, 77, vSets_old);
		//VolumeConstraintBypbd(117, 74, 75, 77, vSets_old);

	}

	BoxNet model1_net3;
	model1_net3.GenerateNet(vSets, "C:/Users/reckless/Desktop/3Dģ�ͻ���/Boxlist5.txt");//vBoxSets_newΪtxtload�еĶ���
	T_boxNet = model1_net3.Getm_boxNet();
	TetrahSet_aftermove = model1_net3.Getcm_boxNet();
}
//��3Dģ�Ͷ������Լ�����α䴦�����У��α��Ч���ǻ��ڵ���ϵ���ģ�����ϵ��Խ���α��Ч����Խ���ԡ�
void ObjLoader::ModelConstraint(vector<vector<GLfloat>>& vSets, vector<vector<GLfloat>>& vSets_old, int range[], int length, float elasticity) {


	for (int i = 0; i < length; i++) {//�ر�ע�⣺������Ϊ�������ݺ���˻���Ϊָ�룬�޷�ͨ��ָ��ֱ�ӵõ������С��������
		cout << length << "";
		for (int j = 0; j < tetrahWithV[range[i]].size(); j++) {
			cout << "  " << range[i] << "  " << j << "  ";
			/*cout << "�仯ǰ��" << vSets[tetrahWithV[range[i]][j]][0] << "		";
			cout << vSets[tetrahWithV[range[i]][j]][1] << "		";
			cout << vSets[tetrahWithV[range[i]][j]][2] << "\n";*/

			vSets[tetrahWithV[range[i]][j]][0] = vSets[tetrahWithV[range[i]][j]][0] + elasticity * (vSets_old[tetrahWithV[range[i]][j]][0] - vSets[tetrahWithV[range[i]][j]][0]);
			vSets[tetrahWithV[range[i]][j]][1] = vSets[tetrahWithV[range[i]][j]][1] + elasticity * (vSets_old[tetrahWithV[range[i]][j]][1] - vSets[tetrahWithV[range[i]][j]][1]);
			vSets[tetrahWithV[range[i]][j]][2] = vSets[tetrahWithV[range[i]][j]][2] + elasticity * (vSets_old[tetrahWithV[range[i]][j]][2] - vSets[tetrahWithV[range[i]][j]][2]);
			cout << "�仯��" << vSets[tetrahWithV[range[i]][j]][0] << "		";
			cout << vSets[tetrahWithV[range[i]][j]][1] << "		";
			cout << vSets[tetrahWithV[range[i]][j]][2] << "\n";
			cout << "";
		}
	}
}
void dual_quat_deformer(const std::vector<Point3>& in_verts,//����Ķ���
	const std::vector<Vec3>& in_normals, //�����ߵ�����
	std::vector<Vec3>& out_verts,//ʹ��˫��Ԫ�����κ�Ķ���
	std::vector<Vec3>& out_normals,//ʹ��˫��Ԫ�����κ�ķ���
	const std::vector<Dual_quat_cu>& dual_quat,//ÿ���ؽڵ�˫��Ԫ���任
	const std::vector< std::vector<float> >& weights,//ÿ�������Ӱ��Ȩ��
	const std::vector< std::vector<int> >& joints_id)//�ؽ�ID
{
	for (unsigned v = 0; v < in_verts.size(); ++v)//ѭ���������붥������ in_verts �е�ÿһ�����㡣
	{
		const int nb_joints = weights[v].size(); // Number of joints influencing vertex p  ���Ȼ�ȡ�˵�ǰ������Ӱ��Ĺؽ����� nb_joints

		// Init dual quaternion with first joint transformation      ʹ�õ�һ���ؽڱ任��ʼ��˫��Ԫ��
		int   k0 = -1;    //�����˼������������ڴ洢˫��Ԫ���Ļ�Ͻ���Լ�������ʱ������
		float w0 = 0.f;
		Dual_quat_cu dq_blend;   //�洢��Ͻ��
		Quat_cu q0;   //�洢��һ���ؽ���ת

		if (nb_joints != 0)  //��鵱ǰ�����Ƿ��ܵ��κιؽڵ�Ӱ�졣����ܵ�Ӱ�죬����һ���ؽڵ�������Ȩ�ش洢�ڱ��� k0 �� w0 �У�����˫��Ԫ����Ͻ������Ϊ��λ˫��Ԫ����
		{
			k0 = joints_id[v][0];
			w0 = weights[v][0];
		}
		else
			dq_blend = Dual_quat_cu::identity();

		if (k0 != -1) dq_blend = dual_quat[k0] * w0;   //�����һ���ؽ�������Ч����ʹ�����Ӧ��˫��Ԫ����Ȩ�ؽ��л�ϡ�

		//�ⲿ�ִ����Ǵ��������ؽڶԵ�ǰ�����Ӱ�졣�������˳��˵�һ���ؽ�֮��������ؽڣ���ÿ���ؽڽ�����Ӧ��˫��Ԫ����ϣ���������յ�˫��Ԫ����Ͻ����

		int pivot = k0;   //�洢�˵�һ���ؽ�����

		q0 = dual_quat[pivot].rotation();   //����һ���ؽڵ���ת���ִ洢�� q0 ������

		// Look up the other joints influencing 'p' if any          
		for (int j = 1; j < nb_joints; j++)  //��������һ���ؽ�֮��������ؽڣ���ÿ���ؽڽ�����Ӧ��˫��Ԫ����ϡ�
		{
			const int k = joints_id[v][j];
			float w = weights[v][j];
			const Dual_quat_cu& dq = (k == -1) ? Dual_quat_cu::identity() : dual_quat[k];//��� k ��ֵ���� -1�����ʾ���� v �����κιؽڵ�Ӱ�죬��û����ת��ƽ�Ƶı任��
			//��� k ��ֵ������ -1���� dq ����Ϊ dual_quat[k]�������� v �ܵ��ؽ� k ��˫��Ԫ���任��Ӱ�졣
			if (dq.rotation().dot(q0) < 0.f)   //ȷ����ؽ���תʱ�����һ���ؽ���ת���򱣳�һ��
				w *= -1.f;

			dq_blend = dq_blend + dq * w;
		}

		//�����ż��Ԫ�����κ�Ķ���λ�úͷ��ߣ���������洢��������� out_verts �� out_normals �С�
		// Compute animated position
		Vec3 vi = dq_blend.transform(in_verts[v]).to_vec3();
		out_verts[v] = vi;
		// Compute animated normal
		out_normals[v] = dq_blend.rotate(in_normals[v]);
	}
}

void ObjLoader::computerKinTerachbySearch(vector<CPoint3D>& VSet, vector<Tetrah>& TetrahSet_initial, vector<Tetrah>& TetrahSet_aftermove, vector<CPoint3D>& vSet_new) {
	vSets_old.assign(vSets.begin(), vSets.end()); // ��ԭʼ���㼯"vSets"���Ƶ�"vSet_old"��

	std::vector<Point3> in_verts;
	std::vector<Vec3> in_normals;
	std::vector<std::vector<float>> weights;
	std::vector<std::vector<int>> joints_id;
	std::vector<int> points_indices;

	// ����VSet�е�ÿ����
	for (int p = 0; p < VSet.size(); p++) {
		ChangePoint(VSet[p]);
		CPoint3D P = PointChange; // ��ȡ�任��Ķ�������

		// �����P������TetrahSet[i]�е�k1, k2, k3ֵ
		for (int i = 0; i < TetrahSet_initial.size(); i++) { // ��������������
			CPoint3D K = TetrahSet_initial[i].ComputeKcoordinateValues(P); // ���㵱ǰ����P�ڳ�ʼ�������е�K����

			// ����K(k1, k2, k3) ����[0, 1]֮�䣬�ն�ѭ������¼��iֵ
			if (K.X() >= 0 && K.X() <= 1 && K.Y() >= 0 && K.Y() <= 1 && K.Z() >= 0 && K.Z() <= 1) { // ���K�����Ƿ���0-1��Χ��
				tetrahWithV[i].push_back(p); // ��¼��ǰ����P���Ӧ������Ĺ�����ϵ
				CPoint3D P_new = TetrahSet_aftermove[i].ComputePcoordinateValutes(K); // ����K�����ڱ任����������м����µĶ�������P_new
				ChangePoint(P_new); // �ı�X, Y, Z˳��

				// ����������ض���Χ�ڣ����¼��Щ�������Ϣ
				if (i >= 141 && i <= 150) { // ע�����ｫ p ��Ϊ i����Ϊ i �����������������
					in_verts.push_back(Point3(P_new.X(), P_new.Y(), P_new.Z()));
					in_normals.push_back({ 1.0f, 2.0f, 3.0f }); // ����ķ������ݣ����Ը�����Ҫ�޸�
					weights.push_back({ 1.0f }); // �����Ȩ�����ݣ����Ը�����Ҫ�޸�
					joints_id.push_back({ 2 }); // ����Ĺؽ�ID���ݣ����Ը�����Ҫ�޸�
					points_indices.push_back(p);
				}
				else {
					// ���¶��㼯
					vSets[p][0] = PointChange.X();
					vSets[p][1] = PointChange.Y();
					vSets[p][2] = PointChange.Z();
					vSet_new.emplace_back(PointChange);
				}
				break;
			}

			if (i == TetrahSet_initial.size() - 1) {
				cout << "the missing Point: " << endl;
				cout << p << ":P" << P.X() << " " << P.Y() << " " << P.Z() << endl;
			}
		}
	}

	// ���ж�ż��Ԫ������ת����
	if (!in_verts.empty()) {
		std::vector<Dual_quat_cu> dual_quat = {
			Tbx::Dual_quat_cu(Quat_cu(1, 0, 0, 0), Quat_cu(0, 0, 0, 0)),
			Tbx::Dual_quat_cu(Quat_cu(0, -1, 0, 1), Quat_cu(0, 0, 0, 0.0)),
			Tbx::Dual_quat_cu(Quat_cu(cos(PI / 8), sin(PI / 8) * 0, sin(PI / 8) * 0, sin(PI / 8) * 1), Quat_cu(0, 0, 0, 0))
			// ��Ӹ���˫��Ԫ���任
		};

		std::vector<Vec3> out_verts(in_verts.size());
		std::vector<Vec3> out_normals(in_normals.size());

		// ���� dual_quat_deformer ���������������
		dual_quat_deformer(in_verts, in_normals, out_verts, out_normals, dual_quat, weights, joints_id);

		// ���±任������ж�������
		for (int j = 0; j < out_verts.size(); ++j) {
			CPoint3D P_new = CPoint3D(out_verts[j].x, out_verts[j].y, out_verts[j].z);
			ChangePoint(P_new); // �ı�X, Y, Z˳��

			// ���¶��㼯
			int p = points_indices[j];
			vSets[p][0] = PointChange.X();
			vSets[p][1] = PointChange.Y();
			vSets[p][2] = PointChange.Z();
			vSet_new.emplace_back(PointChange);
		}
	}
}
void ObjLoader::ComputerSkeletonPointRestriction(vector<CPoint3D>& VSet, vector<Tetrah>& TetrahSet_initial, vector<Tetrah>& TetrahSet_aftermove, vector<CPoint3D>& vSet_new, float k, float centers[1][3]) {
	vSets_old.assign(vSets.begin(), vSets.end());  //��ԭʼ���㼯"vSets���Ƶ�vSet_old��"
	for (int p = 0; p < VSet.size(); p++) {
		ChangePoint(VSet[p]);
		CPoint3D P = PointChange;   //��ȡ��������
		//float distanceToCenterOld = sqrt((pow(P.X() - centers[0][0], 2) + pow(P.Y() - centers[0][1], 2)) + pow(P.Z() - centers[0][2], 2));

		// �����P������TerahSet[i]�е�k1,k2,k3ֵ
		for (int i = 0; i < TetrahSet_initial.size(); i++) {
			CPoint3D K = TetrahSet_initial[i].ComputeKcoordinateValues(P);    //���㵱ǰ����P�ڳ�ʼ�������е�K����

			// ����K(k1, k2, k3)����[0, 1]֮�䣬��ֹѭ������¼��iֵ
			if (K.X() >= 0 && K.X() <= 1 && K.Y() >= 0 && K.Y() <= 1 && K.Z() >= 0 && K.Z() <= 1) {
				
				CPoint3D P_new = TetrahSet_aftermove[i].ComputePcoordinateValutes(K);              //����K�����ڱ任����������м����µĶ�������P_new.
				// ����ؽڴ�������
				float distanceToCenterNew = sqrt((pow(P_new.X() - centers[0][0], 2) + pow(P_new.Y() - centers[0][1], 2)) + pow(P_new.Z() - centers[0][2], 2));

				if (distanceToCenterNew<0.01) {	
				
					float deformFactor = k * exp(-distanceToCenterNew );
					//CPoint3D updatedP_new = P_new + deformFactor + (CPoint3D(centers[0][0], centers[0][1], centers[0][2]) );
					//P_new = P_new + deformFactor;
					//CPoint3D updatedP_new = P_new + deformFactor;
					CPoint3D updatedP_new(deformFactor * ( centers[0][0]- P_new.X()) + P_new.X(),
						 deformFactor * ( centers[0][1]- P_new.Y()) + P_new.Y() ,
						 deformFactor * ( centers[0][2]- P_new.Z()) + P_new.Z()) ;
					//P_new = updatedP_new;
					P_new = CPoint3D(updatedP_new.X(), updatedP_new.Y(), updatedP_new.Z());
					ChangePoint(P_new);  //�ı�X,Y,Z˳��
					vSets[p][0] = PointChange.X();//��������
					vSets[p][1] = PointChange.Y();
					vSets[p][2] = PointChange.Z();
					vSet_new.emplace_back(PointChange); //���µ�������ӵ�vSet_new��
					break;

				}
				else {
					ChangePoint(P_new);  //�ı�X,Y,Z˳��

					vSets[p][0] = PointChange.X();//��������
					vSets[p][1] = PointChange.Y();
					vSets[p][2] = PointChange.Z();
					vSet_new.emplace_back(PointChange); //���µ�������ӵ�vSet_new��
					
				}

				break;
			}
		}
	}
}

//void ObjLoader::ComputerSkeletonPointRestriction(vector<CPoint3D>& vSet_new, vector<Tetrah>& TetrahSet_aftermove, float k, float centers[1][3]) {
//	// ����һ���������Ա����޸Ĺ����б���ԭʼ����
//	vector<CPoint3D> vSet_copy(vSet_new);
//
//	// �����µĶ��㼯�� vSet_new
//	for (int p = 0; p < vSet_copy.size(); p++) {
//		// ����ǰ���� vSet_copy[p] ��������任�����洢�� PointChange ��
//		ChangePoint(vSet_copy[p]);
//		CPoint3D P = PointChange;
//
//		// ����� P ���ؽڵľ���
//		float distanceToJoint = sqrt((pow(P.X() - centers[0][0], 2) + pow(P.Y() - centers[0][1], 2)) + pow(P.Z() - centers[0][2], 2));
//
//		// �ж��Ƿ���Ҫ���йؽڵ�����
//		if (distanceToJoint < 5) {
//			// ������γ̶ȣ�ʹ��ָ��������Ϊ���κ���
//			float deformFactor = k * exp(-distanceToJoint);
//
//			// �Ե���б���
//			CPoint3D updatedP(deformFactor * (centers[0][0] - P.X()) + P.X(),
//				deformFactor * (centers[0][1] - P.Y()) + P.Y(),
//				deformFactor * (centers[0][2] - P.Z()) + P.Z());
//
//			// �����º��������ӵ� vSet_new ��
//			vSet_new[p] = updatedP;
//		}
//		// �������Ҫ�ؽڵ����ƣ�����ԭʼ����
//	}
//}







//����ģ�͵�Լ������ �Դ����̧���ֶ����е����ز����Լ��
//����Լ��������������
//ModelConstraint(vSets, vSets_old, range, length, 0.8);
//int range1[] = {
//	55,56,57,58,59,68,69,70,71,72 };
//int length1 = sizeof(range1) / sizeof(range1[0]);
//ModelConstraint(vSets, vSets_old, range1, length1, 0.8);

//int range2[] = {
//	68,69,70,71,72 };
//int length2 = sizeof(range2) / sizeof(range2[0]);
//ModelConstraint(vSets, vSets_old, range2, length2, 0);
//����
void ObjLoader::Draw1() {
	for (int i = 0; i < fSets.size(); i++) {
		glBegin(GL_LINE_LOOP);//��ʼ����ͼԪ
		GLfloat VN[3];

		//��������
		GLfloat SV1[3];
		GLfloat	SV2[3];
		GLfloat SV3[3];

		if ((fSets[i]).size() != 3) {
			cout << "the fSetsets_Size is not correct" << endl;

		}
		else {
			GLint firstVertexIndex = (fSets[i])[0];//ȡ����������
			GLint secondVertexIndex = (fSets[i])[1];
			GLint thirdVertexIndex = (fSets[i])[2];

			SV1[0] = (vSets[firstVertexIndex])[0];//��һ������
			SV1[1] = (vSets[firstVertexIndex])[1];
			SV1[2] = (vSets[firstVertexIndex])[2];

			SV2[0] = (vSets[secondVertexIndex])[0];//�ڶ�������
			SV2[1] = (vSets[secondVertexIndex])[1];
			SV2[2] = (vSets[secondVertexIndex])[2];

			SV3[0] = (vSets[thirdVertexIndex])[0];//����������
			SV3[1] = (vSets[thirdVertexIndex])[1];
			SV3[2] = (vSets[thirdVertexIndex])[2];

			GLfloat vec1[3], vec2[3], vec3[3];//���㷨����
			//(x2-x1,y2-y1,z2-z1)
			vec1[0] = SV1[0] - SV2[0];
			vec1[1] = SV1[1] - SV2[1];
			vec1[2] = SV1[2] - SV2[2];

			//(x3-x2,y3-y2,z3-z2)
			vec2[0] = SV1[0] - SV3[0];
			vec2[1] = SV1[1] - SV3[1];
			vec2[2] = SV1[2] - SV3[2];

			//(x3-x1,y3-y1,z3-z1)
			vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
			vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
			vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

			GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

			VN[0] = vec3[0] / D;
			VN[1] = vec3[1] / D;
			VN[2] = vec3[2] / D;

			//glNormal3f(VN[0], VN[1], VN[2]);//���Ʒ�����
			glVertex3f(SV1[0], SV1[1], SV1[2]);//ָ��ÿ����������꣬����������
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);

		}
		glEnd();
	}




	for (int i = 0; i < fSets1.size(); i++) {
		glBegin(GL_LINE_LOOP);//��ʼ����
		GLfloat VN[3];

		//�ĸ�����
		GLfloat SV1[3];
		GLfloat	SV2[3];
		GLfloat SV3[3];
		GLfloat SV4[3];


		if ((fSets1[i]).size() != 4) {
			cout << "the fSetsets1_Size is not correct" << endl;

		}

		else {

			GLint firstVertexIndex = (fSets1[i])[0];//ȡ����������
			GLint secondVertexIndex = (fSets1[i])[1];
			GLint thirdVertexIndex = (fSets1[i])[2];
			GLint fourthVertexIndex = (fSets1[i])[3];

			SV1[0] = (vSets[firstVertexIndex])[0];//��һ������
			SV1[1] = (vSets[firstVertexIndex])[1];
			SV1[2] = (vSets[firstVertexIndex])[2];

			SV2[0] = (vSets[secondVertexIndex])[0];//�ڶ�������
			SV2[1] = (vSets[secondVertexIndex])[1];
			SV2[2] = (vSets[secondVertexIndex])[2];

			SV3[0] = (vSets[thirdVertexIndex])[0];//����������
			SV3[1] = (vSets[thirdVertexIndex])[1];
			SV3[2] = (vSets[thirdVertexIndex])[2];

			SV4[0] = (vSets[fourthVertexIndex])[0];//���ĸ�����
			SV4[1] = (vSets[fourthVertexIndex])[1];
			SV4[2] = (vSets[fourthVertexIndex])[2];

			GLfloat vec1[3], vec2[3], vec3[3];//���㷨����
			//(x2-x1,y2-y1,z2-z1)
			vec1[0] = SV1[0] - SV2[0];
			vec1[1] = SV1[1] - SV2[1];
			vec1[2] = SV1[2] - SV2[2];

			//(x3-x2,y3-y2,z3-z2)
			vec2[0] = SV1[0] - SV3[0];
			vec2[1] = SV1[1] - SV3[1];
			vec2[2] = SV1[2] - SV3[2];

			//(x3-x1,y3-y1,z3-z1)
			vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
			vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
			vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

			GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

			VN[0] = vec3[0] / D;
			VN[1] = vec3[1] / D;
			VN[2] = vec3[2] / D;

			//glNormal3f(VN[0], VN[1], VN[2]);//���Ʒ�����
			glVertex3f(SV1[0], SV1[1], SV1[2]);//�����ı�����Ƭ
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);
			glVertex3f(SV4[0], SV4[1], SV4[2]);
		}
		glEnd();
	}



	for (int i = 0; i < fSets2.size(); i++) {
		glBegin(GL_LINE_LOOP);//��ʼ����
		GLfloat VN[3];

		//�������
		GLfloat SV1[3];
		GLfloat	SV2[3];
		GLfloat SV3[3];
		GLfloat SV4[3];
		GLfloat SV5[3];


		if ((fSets2[i]).size() != 5) {
			cout << "the fSetsets2_Size is not correct" << endl;

		}

		else {

			GLint firstVertexIndex = (fSets2[i])[0];//ȡ����������
			GLint secondVertexIndex = (fSets2[i])[1];
			GLint thirdVertexIndex = (fSets2[i])[2];
			GLint fourthVertexIndex = (fSets2[i])[3];
			GLint fifthVertexIndex = (fSets2[i])[4];

			SV1[0] = (vSets[firstVertexIndex])[0];//��һ������
			SV1[1] = (vSets[firstVertexIndex])[1];
			SV1[2] = (vSets[firstVertexIndex])[2];

			SV2[0] = (vSets[secondVertexIndex])[0];//�ڶ�������
			SV2[1] = (vSets[secondVertexIndex])[1];
			SV2[2] = (vSets[secondVertexIndex])[2];

			SV3[0] = (vSets[thirdVertexIndex])[0];//����������
			SV3[1] = (vSets[thirdVertexIndex])[1];
			SV3[2] = (vSets[thirdVertexIndex])[2];

			SV4[0] = (vSets[fourthVertexIndex])[0];//���ĸ�����
			SV4[1] = (vSets[fourthVertexIndex])[1];
			SV4[2] = (vSets[fourthVertexIndex])[2];

			SV5[0] = (vSets[fifthVertexIndex])[0];//���������
			SV5[1] = (vSets[fifthVertexIndex])[1];
			SV5[2] = (vSets[fifthVertexIndex])[2];

			GLfloat vec1[3], vec2[3], vec3[3];//���㷨����
			//(x2-x1,y2-y1,z2-z1)
			vec1[0] = SV1[0] - SV2[0];
			vec1[1] = SV1[1] - SV2[1];
			vec1[2] = SV1[2] - SV2[2];

			//(x3-x2,y3-y2,z3-z2)
			vec2[0] = SV1[0] - SV3[0];
			vec2[1] = SV1[1] - SV3[1];
			vec2[2] = SV1[2] - SV3[2];

			//(x3-x1,y3-y1,z3-z1)
			vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
			vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
			vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

			GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

			VN[0] = vec3[0] / D;
			VN[1] = vec3[1] / D;
			VN[2] = vec3[2] / D;

			//glNormal3f(VN[0], VN[1], VN[2]);//���Ʒ�����
			glVertex3f(SV1[0], SV1[1], SV1[2]);//���ƶ������Ƭ
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);
			glVertex3f(SV4[0], SV4[1], SV4[2]);
			glVertex3f(SV5[0], SV5[1], SV5[2]);
		}
		glEnd();
	}

}
//����
void ObjLoader::Draw2() {

	for (int i = 0; i < fSets.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, texModel);
		glBegin(GL_TRIANGLES);//��ʼ����������
		GLfloat VN[3];

		//��������
		GLfloat SV1[3];
		GLfloat	SV2[3];
		GLfloat SV3[3];

		if ((fSets[i]).size() != 3) {
			cout << "the fSetsets_Size is not correct" << endl;

		}
		else {
			GLint firstVertexIndex = (fSets[i])[0];//ȡ����������
			GLint secondVertexIndex = (fSets[i])[1];
			GLint thirdVertexIndex = (fSets[i])[2];
			GLint firstVtIndex = (fvtSets[i])[0];//ȡ����������
			GLint secondVtIndex = (fvtSets[i])[1];
			GLint thirdVtIndex = (fvtSets[i])[2];

			SV1[0] = (vSets[firstVertexIndex])[0];//��һ������
			SV1[1] = (vSets[firstVertexIndex])[1];
			SV1[2] = (vSets[firstVertexIndex])[2];

			SV2[0] = (vSets[secondVertexIndex])[0];//�ڶ�������
			SV2[1] = (vSets[secondVertexIndex])[1];
			SV2[2] = (vSets[secondVertexIndex])[2];

			SV3[0] = (vSets[thirdVertexIndex])[0];//����������
			SV3[1] = (vSets[thirdVertexIndex])[1];
			SV3[2] = (vSets[thirdVertexIndex])[2];

			GLfloat vec1[3], vec2[3], vec3[3];//���㷨����
			//(x2-x1,y2-y1,z2-z1)
			vec1[0] = SV1[0] - SV2[0];
			vec1[1] = SV1[1] - SV2[1];
			vec1[2] = SV1[2] - SV2[2];

			//(x3-x2,y3-y2,z3-z2)
			vec2[0] = SV1[0] - SV3[0];
			vec2[1] = SV1[1] - SV3[1];
			vec2[2] = SV1[2] - SV3[2];

			//(x3-x1,y3-y1,z3-z1)
			vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
			vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
			vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

			GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

			VN[0] = vec3[0] / D;
			VN[1] = vec3[1] / D;
			VN[2] = vec3[2] / D;

			glNormal3f(VN[0], VN[1], VN[2]);//���Ʒ�����
			glTexCoord2f(vtSets[firstVtIndex][0], vtSets[firstVtIndex][1]);//��������
			glVertex3f(SV1[0], SV1[1], SV1[2]);//����������Ƭ
			glTexCoord2f(vtSets[secondVtIndex][0], vtSets[secondVtIndex][1]);
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glTexCoord2f(vtSets[thirdVtIndex][0], vtSets[thirdVtIndex][1]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);

		}
		glEnd();
	}

	glBegin(GL_QUADS);//��ʼ�����ı���
	for (int i = 0; i < fSets1.size(); i++) {

		GLfloat VN[3];

		//�ĸ�����
		GLfloat SV1[3];
		GLfloat	SV2[3];
		GLfloat SV3[3];
		GLfloat SV4[3];


		if ((fSets1[i]).size() != 4) {
			cout << "the fSetsets1_Size is not correct" << endl;

		}

		else {

			GLint firstVertexIndex = (fSets1[i])[0];//ȡ����������
			GLint secondVertexIndex = (fSets1[i])[1];
			GLint thirdVertexIndex = (fSets1[i])[2];
			GLint fourthVertexIndex = (fSets1[i])[3];

			SV1[0] = (vSets[firstVertexIndex])[0];//��һ������
			SV1[1] = (vSets[firstVertexIndex])[1];
			SV1[2] = (vSets[firstVertexIndex])[2];

			SV2[0] = (vSets[secondVertexIndex])[0];//�ڶ�������
			SV2[1] = (vSets[secondVertexIndex])[1];
			SV2[2] = (vSets[secondVertexIndex])[2];

			SV3[0] = (vSets[thirdVertexIndex])[0];//����������
			SV3[1] = (vSets[thirdVertexIndex])[1];
			SV3[2] = (vSets[thirdVertexIndex])[2];

			SV4[0] = (vSets[fourthVertexIndex])[0];//���ĸ�����
			SV4[1] = (vSets[fourthVertexIndex])[1];
			SV4[2] = (vSets[fourthVertexIndex])[2];

			GLfloat vec1[3], vec2[3], vec3[3];//���㷨����
			//(x2-x1,y2-y1,z2-z1)
			vec1[0] = SV1[0] - SV2[0];
			vec1[1] = SV1[1] - SV2[1];
			vec1[2] = SV1[2] - SV2[2];

			//(x3-x2,y3-y2,z3-z2)
			vec2[0] = SV1[0] - SV3[0];
			vec2[1] = SV1[1] - SV3[1];
			vec2[2] = SV1[2] - SV3[2];

			//(x3-x1,y3-y1,z3-z1)
			vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
			vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
			vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

			GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

			VN[0] = vec3[0] / D;
			VN[1] = vec3[1] / D;
			VN[2] = vec3[2] / D;

			//glNormal3f(VN[0], VN[1], VN[2]);//���Ʒ�����
			glVertex3f(SV1[0], SV1[1], SV1[2]);//�����ı�����Ƭ
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);
			glVertex3f(SV4[0], SV4[1], SV4[2]);
		}

	}
	glEnd();


	for (int i = 0; i < fSets2.size(); i++) {
		glBegin(GL_POLYGON);//��ʼ���ƶ���Σ������ǻ��������
		GLfloat VN[3];

		//�������
		GLfloat SV1[3];
		GLfloat	SV2[3];
		GLfloat SV3[3];
		GLfloat SV4[3];
		GLfloat SV5[3];


		if ((fSets2[i]).size() != 5) {
			cout << "the fSetsets2_Size is not correct" << endl;

		}

		else {

			GLint firstVertexIndex = (fSets2[i])[0];//ȡ����������
			GLint secondVertexIndex = (fSets2[i])[1];
			GLint thirdVertexIndex = (fSets2[i])[2];
			GLint fourthVertexIndex = (fSets2[i])[3];
			GLint fifthVertexIndex = (fSets2[i])[4];

			SV1[0] = (vSets[firstVertexIndex])[0];//��һ������
			SV1[1] = (vSets[firstVertexIndex])[1];
			SV1[2] = (vSets[firstVertexIndex])[2];

			SV2[0] = (vSets[secondVertexIndex])[0];//�ڶ�������
			SV2[1] = (vSets[secondVertexIndex])[1];
			SV2[2] = (vSets[secondVertexIndex])[2];

			SV3[0] = (vSets[thirdVertexIndex])[0];//����������
			SV3[1] = (vSets[thirdVertexIndex])[1];
			SV3[2] = (vSets[thirdVertexIndex])[2];

			SV4[0] = (vSets[fourthVertexIndex])[0];//���ĸ�����
			SV4[1] = (vSets[fourthVertexIndex])[1];
			SV4[2] = (vSets[fourthVertexIndex])[2];

			SV5[0] = (vSets[fifthVertexIndex])[0];//���������
			SV5[1] = (vSets[fifthVertexIndex])[1];
			SV5[2] = (vSets[fifthVertexIndex])[2];

			GLfloat vec1[3], vec2[3], vec3[3];//���㷨����
			//(x2-x1,y2-y1,z2-z1)
			vec1[0] = SV1[0] - SV2[0];
			vec1[1] = SV1[1] - SV2[1];
			vec1[2] = SV1[2] - SV2[2];

			//(x3-x2,y3-y2,z3-z2)
			vec2[0] = SV1[0] - SV3[0];
			vec2[1] = SV1[1] - SV3[1];
			vec2[2] = SV1[2] - SV3[2];

			//(x3-x1,y3-y1,z3-z1)
			vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
			vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
			vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

			GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

			VN[0] = vec3[0] / D;
			VN[1] = vec3[1] / D;
			VN[2] = vec3[2] / D;

			//glNormal3f(VN[0], VN[1], VN[2]);//���Ʒ�����
			glVertex3f(SV1[0], SV1[1], SV1[2]);//���ƶ������Ƭ
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);
			glVertex3f(SV4[0], SV4[1], SV4[2]);
			glVertex3f(SV5[0], SV5[1], SV5[2]);
		}
		glEnd();
	}
}
//������������
void ObjLoader::DrawNet() {
	for (int i = 0; i < T_boxNet.size(); i++) {//T_boxNet.size()
		Tetrah T_point = T_boxNet[i];
		CPoint3D point1 = T_point.getA();
		CPoint3D point2 = T_point.getB();
		CPoint3D point3 = T_point.getC();
		CPoint3D point4 = T_point.getD();
		float point1x = point1.X();
		float point1y = point1.Y();
		float point1z = point1.Z();

		float point2x = point2.X();
		float point2y = point2.Y();
		float point2z = point2.Z();

		float point3x = point3.X();
		float point3y = point3.Y();
		float point3z = point3.Z();

		float point4x = point4.X();
		float point4y = point4.Y();
		float point4z = point4.Z();

		glBegin(GL_LINE_LOOP);//��ʼ����
		glColor4ub(0, 0, 0, 255);//������������ɫ
		glVertex3f(point1x, point1y, point1z);//ָ������Ӧ��ͨ���ĵ㡣
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point2x, point2y, point2z);
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point3x, point3y, point3z);
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point1x, point1y, point1z);//����������
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point2x, point2y, point2z);
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point4x, point4y, point4z);
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point1x, point1y, point1z);//����������
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point3x, point3y, point3z);
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point4x, point4y, point4z);
		glColor4ub(0, 0, 0, 255);

		glVertex3f(point2x, point2y, point2z);//����������
		glColor4ub(255, 0, 0, 255);
		glVertex3f(point3x, point3y, point3z);
		glColor4ub(255, 0, 0, 255);
		glVertex3f(point4x, point4y, point4z);
		glEnd();

	}

}
