#include "ObjLoader.h"
#include <fstream>
#include <iostream>
#include "dual_quat_cu.hpp"
#include <math.h>
#define PI 3.14159

using namespace Tbx;
using namespace std;

#define BMP_Header_Length 54  //图像数据在内存块中的偏移量

//v顶点格式：v x y z
//意义：每个顶点的坐标
//vt纹理坐标
//格式：vt u v w
//意义：绘制模型的三角面片时，每个顶点取像素点时对应的纹理图片上的坐标。
// 纹理图片的坐标指的是，纹理图片如果被放在屏幕上显示时，以屏幕左下角为原点的坐标。
//注意：w一般用于形容三维纹理，大部分是用不到的，基本都为0。
//vn顶点法向量
//格式：vn x y z
//意义：绘制模型三角面片时，需要确定三角面片的朝向，整个面的朝向，
//是由构成每个面的顶点对应的顶点法向量的做矢量和决定的（xyz的坐标分别相加再除以3得到的）。
//f 面
//格式 ：f v/vt/vn v/vt/vn v/vt/vn（f 顶点索引 / 纹理坐标索引 / 顶点法向量索引）
//意义：绘制三角面片的依据，每个三角面片由三个f构成，由f可以确定顶点、顶点的对应的纹理坐标（提取纹理图片对应该坐标的像素点）、
//通过三个顶点对应的顶点法向量可以确定三角面的方向。


ObjLoader::ObjLoader(string filename)
{
	string line;
	fstream f;

	f.open(filename, ios::in);

	if (!f.is_open()) {
		cout << "Something Went Wrong When Opening Objfiles" << endl;
	}

	while (!f.eof()) {

		getline(f, line);//拿到obj文件中的一行，作为一个字符串

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
					parameters.push_back(ans);//取出字符串中的元素，以空格切分
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
					for (int j = 0; j < x.length(); j++) {//跳过'/'
						char ch = x[j];
						if (ch != '/') {
							ans += ch;
						}
						else {
							break;
						}
					}
					GLint index1 = atof(ans.c_str());
					index1 = index1--;//因为顶点的索引在obj文件中是从1开始的，而我们存放的顶点vector是从0开始的因此要减1
					vIndexSets1.push_back(index1);
				}

				fSets1.push_back(vIndexSets1);
			}
			if (parameters.size() == 6 && parameters[0] == "f") {
				vector<GLint>vIndexSets2;

				for (int i = 1; i < 6; i++) {
					string x = parameters[i];
					string ans = "";
					for (int j = 0; j < x.length(); j++) {//跳过'/'
						char ch = x[j];
						if (ch != '/') {
							ans += ch;
						}
						else {
							break;
						}
					}
					GLint index2 = atof(ans.c_str());
					index2 = index2--;//因为顶点的索引在obj文件中是从1开始的，而我们存放的顶点vector是从0开始的因此要减1
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

			if (parameters[0] == "v") {//v表示顶点

				vector<GLfloat>Point1;

				for (int i = 1; i < 4; i++) {//从1开始，将顶点的xyz三个坐标放进顶点vector

					GLfloat xyz = atof(parameters[i].c_str());

					Point1.push_back(xyz);
				}

				vSets.push_back(Point1);
			}

			else if (parameters[0] == "f") {//如果是面的话，存放三个顶点的索引

				vector<GLint>vIndexSets;
				vector<GLint>vtIndexSets;

				for (int i = 1; i < 4; i++) {
					string x = parameters[i];
					string ans = "";
					string ans1 = "";
					int count = 1;
					for (int j = 0; j < x.length(); j++) {//跳过'/'
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
					index = index--;//因为顶点的索引在obj文件中是从1开始的，而我们存放的顶点vector是从0开始的因此要减1
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
	model1_net1.GenerateNet(vSets, "C:/Users/reckless/Desktop/3D模型绘制/Boxlist5.txt");
	//model1_net1.GenerateNet(vSets);
	T_boxNet = model1_net1.Getm_boxNet();

	TxtLoader("C:/Users/reckless/Desktop/3D模型绘制/armboxdata.txt");//通过txt读取变化后的四面体网格，vBoxSets_new存储
	TetrahSet(); //获取变化前的四面体网格
	ChangeType();//用于将float类型的点改为CfPoint3D类型
	ChangeTetrahSet();//修改四面体网格的点的坐标

}


// 函数power_of_two用于判断一个整数是不是2的整数次幂，对纹理图片的一个要求
int power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

/* 函数load_texture
* 读取一个BMP文件作为纹理
* 如果失败，返回0，如果成功，返回纹理编号
*/
GLuint ObjLoader::load_texture(const char* file_name) {
	//GLint width, height, total_bytes;
	//GLubyte* pixels = 0;
	//GLuint last_texture_ID = 0, texture_ID = 0;

	//// 打开文件，如果失败，返回
	//FILE* pFile = fopen(file_name, "rb");
	//if (pFile == 0)
	//	return 0;

	//// 读取文件中图象的宽度和高度
	//fseek(pFile, 0x0012, SEEK_SET);
	//fread(&width, 4, 1, pFile);
	//fread(&height, 4, 1, pFile);
	//fseek(pFile, BMP_Header_Length, SEEK_SET);

	//// 计算每行像素所占字节数，并根据此数据计算总像素字节数
	//{
	//	GLint line_bytes = width * 3;
	//	while (line_bytes % 4 != 0)
	//		++line_bytes;
	//	total_bytes = line_bytes * height;
	//}

	//// 根据总像素字节数分配内存
	//pixels = (GLubyte*)malloc(total_bytes);
	//if (pixels == 0)
	//{
	//	fclose(pFile);
	//	return 0;
	//}

	//// 读取像素数据
	//if (fread(pixels, total_bytes, 1, pFile) <= 0)
	//{
	//	free(pixels);
	//	fclose(pFile);
	//	return 0;
	//}

	//// 对就旧版本的兼容，如果图象的宽度和高度不是的整数次方，则需要进行缩放
	//// 若图像宽高超过了OpenGL规定的最大值，也缩放
	//{
	//	GLint max;
	//	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
	//	if (!power_of_two(width)
	//		|| !power_of_two(height)
	//		|| width > max
	//		|| height > max)
	//	{
	//		const GLint new_width = 256;
	//		const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
	//		GLint new_line_bytes, new_total_bytes;
	//		GLubyte* new_pixels = 0;

	//		// 计算每行需要的字节数和总字节数
	//		new_line_bytes = new_width * 3;
	//		while (new_line_bytes % 4 != 0)
	//			++new_line_bytes;
	//		new_total_bytes = new_line_bytes * new_height;

	//		// 分配内存
	//		new_pixels = (GLubyte*)malloc(new_total_bytes);
	//		if (new_pixels == 0)
	//		{
	//			free(pixels);
	//			fclose(pFile);
	//			return 0;
	//		}

	//		// 进行像素缩放
	//		gluScaleImage(GL_RGB,
	//			width, height, GL_UNSIGNED_BYTE, pixels,
	//			new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

	//		// 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
	//		free(pixels);
	//		pixels = new_pixels;
	//		width = new_width;
	//		height = new_height;
	//	}
	//}

	//// 分配一个新的纹理编号
	//glGenTextures(1, &texture_ID);
	//if (texture_ID == 0)
	//{
	//	free(pixels);
	//	fclose(pFile);
	//	return 0;
	//}

	//// 绑定新的纹理，载入纹理并设置纹理参数
	//// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
	//GLint lastTextureID = last_texture_ID;
	//glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
	//glBindTexture(GL_TEXTURE_2D, texture_ID);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//设置纹理跟材质的融合方式
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
	//	GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	//glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定
	//free(pixels);
	//cout << "success" << endl;
	//return texture_ID;



	// 打开文件，如果失败，返回
	FILE* pFile = fopen(file_name, "rb");
	if (pFile == 0)
		return 0;

GLubyte* pixels = 0;
GLuint last_texture_ID = 0, tex_ID = 0;
fseek(pFile, 0x0012, SEEK_SET);
// 读取文件中图象的宽度和高度
GLint width = 0, height = 0, total_bytes = 0;
fread(&width, 4, 1, pFile);
fread(&height, 4, 1, pFile);
fseek(pFile, BMP_Header_Length, SEEK_SET);

// 计算每行像素所占字节数，并根据此数据计算总像素字节数
GLint  line_bytes = width * 3;
while (line_bytes % 4 != 0)
++line_bytes;
total_bytes = line_bytes * height;
// 根据总像素字节数分配内存
pixels = (GLubyte*)malloc(total_bytes);
if (pixels == 0)
{
	fclose(pFile);
	return false;
}
// 读取像素数据
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
	const GLint new_height = 256; // 规定缩放后新的大小
	GLint new_line_bytes, new_total_bytes;
	GLubyte* new_pixels = 0;
	// 计算每行需要的字节数和总字节数
	new_line_bytes = new_width * 3;
	while (new_line_bytes % 4 != 0)
		++new_line_bytes;
	new_total_bytes = new_line_bytes * new_height;
	// 分配内存
	new_pixels = (GLubyte*)malloc(new_total_bytes);
	if (new_pixels == 0)
	{
		fclose(pFile);
		return false;
	}
	// 进行像素缩放
	gluScaleImage(GL_RGB,
		width, height, GL_UNSIGNED_BYTE, pixels,
		new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);
	// 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
	free(pixels);
	pixels = new_pixels;
	width = new_width;
	height = new_height;
}
// 分配一个新的纹理编号
//GLuint tex_ID;
glGenTextures(1, &tex_ID);

if (tex_ID == 0)
{
	free(pixels);
	fclose(pFile);
	return 0;
}
// 绑定新的纹理，载入纹理并设置纹理参数
// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
GLint lastTextureID = last_texture_ID;
glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
glBindTexture(GL_TEXTURE_2D, tex_ID);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定

fclose(pFile);
free(pixels);

return tex_ID;


}


//vBoxSets_new存储txt
void ObjLoader::TxtLoader(string filename) {
	string line;
	fstream f;

	f.open(filename, ios::in);

	if (!f.is_open()) {
		cout << "Something Went Wrong When Opening Txtfiles" << endl;
	}

	while (!f.eof()) {

		getline(f, line);//拿到txt文件中的一行，作为一个字符串

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
					parameters.push_back(ans);//取出字符串中的元素，以,切分
				}
				ans = "";
			}
		}
		if (parameters.size() == 3) {
			vector<GLfloat>Point1;
			for (int i = 0; i < 3; i++) {//从1开始，将顶点的xyz三个坐标放进顶点vector

				GLfloat xyz = atof(parameters[i].c_str());

				Point1.push_back(xyz);
			}

			vBoxSets_new.push_back(Point1);  //存放变化后盒子的点

		}
	}
}
//读取变化前的四面体网格
void ObjLoader::TetrahSet() { 
	BoxNet model1_net2;
	model1_net2.GenerateNet(vSets, "C:/Users/reckless/Desktop/3D模型绘制/Boxlist5.txt");
	TetrahSet_initial = model1_net2.Getcm_boxNet();
}
//用于将x,y,z改为x,z,y坐标（和opengl中保持一致）
void ObjLoader::ChangePoint(CPoint3D& VSets) {
	PointChange.setFpointVec(VSets.X(), VSets.Z(), VSets.Y());

}
//用于将float类型的点改为CfPoint3D类型（其实就是xyz坐标）
void ObjLoader::ChangeType() {
	for (int i = 0; i < vSets.size(); i++) {
		vSetsChangeType.push_back(CFpointVector_3D(vSets[i][0], vSets[i][1], vSets[i][2]));
	}
}
//拉伸约束
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
将相连的顶点，根据两点之间的距离进行拉伸约束
parameters：两顶点的索引；弹性程度；变化前控制网格的坐标        vsets_old=vttest
*/
/*void ObjLoader::StretchConstraint1(int a, int b, float k, vector<vector<GLfloat>>& vSets_old) {
	float d_old;
	float d_new;
	d_old = sqrt(pow(vSets_old[a][0] - vSets_old[b][0], 2) + pow(vSets_old[a][1] - vSets_old[b][1], 2) + pow(vSets_old[a][2] - vSets_old[b][2], 2));
	//d_old = 0.484;
	d_new = sqrt(pow(vSets[a][0] - vSets[b][0], 2) + pow(vSets[a][1] - vSets[b][1], 2) + pow(vSets[a][2] - vSets[b][2], 2));
	float c = 0.5 * k * (d_new - d_old) * (vSets[a][0] - vSets[b][0]) / d_new;//计算两点间的相对位移，
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
	float c = 0.5 * k * (d_new - d_old) * (vSets[0][0] - vSets[0][0]) / d_new;//计算两点间的相对位移，
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
根据控制网格的顶点生成包围球，限制该顶点变化的范围
parameters：控制网格顶点坐标；限制顶点的索引；包围球半径。。检查并调整模型中的某一点，使其到原点的距离不超过一个给定的半径r
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
四面体体积约束，传入四面体的顶点索引  parameters：需要约束的四面体（四个顶点索引），变化前控制网格的坐标；用于确保模型的体积保持在一个给定的范围内，或者满足某种特定的体积约束
*/
void ObjLoader::VolumeConstraint(int a, int b, int c, int d, vector<vector<GLfloat>>& vSets_old) {
	double volumeConstraint;//用于约束的四面体体积(变形前的体积)
	double volume;//变形后四面体体积
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
	//计算四面体体积
	volumeConstraint = abs(vc1 * (vc5 * vc9 - vc8 * vc6) - vc2 * (vc4 * vc9 - vc7 * vc6) + vc3 * (vc4 * vc8 - vc7 * vc5)) / 6.0;
	volume = abs(v1 * (v5 * v9 - v8 * v6) - v2 * (v4 * v9 - v7 * v6) + v3 * (v4 * v8 - v7 * v5)) / 6.0;
	// 通过体积约束更新顶点位置
	double scaleFactor = pow(volumeConstraint / volume, 1.0 / 3.0);
	double centerx = (vSets[a][0] + vSets[b][0] + vSets[c][0] + vSets[d][0]) / 4.0; //计算四面体的中心坐标，除以4是为了计算出每个顶点到中心的平均距离
	double centery = (vSets[a][1] + vSets[b][1] + vSets[c][1] + vSets[d][1]) / 4.0;
	double centerz = (vSets[a][2] + vSets[b][2] + vSets[c][2] + vSets[d][2]) / 4.0;
	vSets[a][0] = centerx + (vSets[a][0] - centerx) * scaleFactor;  //进行线性变换，使其移动到新的位置
	vSets[a][1] = centery + (vSets[a][1] - centery) * scaleFactor;//减去中心坐标是将每个顶点相对于中心点的偏移量减去，得到新的偏移量。
	vSets[a][2] = centerz + (vSets[a][2] - centerz) * scaleFactor;//再乘以比例因子，使得四面体发生变化

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
//基于位置动力学四面体体积约束，传入四面体顶点索引；根据计算结果调整模型位置
void ObjLoader::VolumeConstraintBypbd(int a, int b, int c, int d, vector<vector<GLfloat>>& vSets_old) {
	double volumeConstraintA, volumeConstraintB, volumeConstraintC, volumeConstraintD ;//用于约束的四面体体积（变形前的体积）
	double volumeA, volumeB, volumeC, volumeD;//变形后的体积
	double k = 1;//k为刚度参数
	double sA, sB,sC, sD;//s为比例因子 为体积约束/梯度模的平方和

	//求a的变化梯度
	//a中点b的变化梯度   1/6*b×c
	double grad1bx = ((vSets[b][1] * vSets[c][2]) - (vSets[b][2] * vSets[c][1])) / 6;  //这个梯度表示向量vSets[b]和vSets[c]在x轴方向上的变化率。
	double grad1by = ((vSets[b][2] * vSets[c][0]) - (vSets[b][0] * vSets[c][2])) / 6;
	double grad1bz = ((vSets[b][0] * vSets[c][1]) - (vSets[b][1] * vSets[c][0])) / 6;
	//a中点c的变化梯度   1/6*c×d
	double grad1cx = ((vSets[c][1] * vSets[d][2]) - (vSets[c][2] * vSets[d][1])) / 6;
	double grad1cy = ((vSets[c][2] * vSets[d][0]) - (vSets[c][0] * vSets[d][2])) / 6;
	double grad1cz = ((vSets[c][0] * vSets[d][1]) - (vSets[c][1] * vSets[d][0])) / 6;
	//a中点d的变化梯度   1/6*d×b
	double grad1dx = ((vSets[d][1] * vSets[b][2]) - (vSets[d][2] * vSets[b][1])) / 6;
	double grad1dy = ((vSets[d][2] * vSets[b][0]) - (vSets[d][0] * vSets[b][2])) / 6;
	double grad1dz = ((vSets[d][0] * vSets[b][1]) - (vSets[d][1] * vSets[b][0])) / 6;
	//点a的变化梯度   变化梯度和为0
	double gradax = -(grad1bx + grad1cx + grad1dx);
	double graday = -(grad1by + grad1cy + grad1dy);
	double gradaz = -(grad1bz + grad1cz + grad1dz);

	//求b的变化梯度
	//b中点a的变化梯度  1/6*a×c
	double grad2ax = ((vSets[a][1] * vSets[c][2]) - (vSets[a][2] * vSets[c][1])) / 6;  
	double grad2ay = ((vSets[a][2] * vSets[c][0]) - (vSets[a][0] * vSets[c][2])) / 6;
	double grad2az = ((vSets[a][0] * vSets[c][1]) - (vSets[a][1] * vSets[c][0])) / 6;
	//b中点c的变化梯度   1/6*c×d
	double grad2cx = ((vSets[c][1] * vSets[d][2]) - (vSets[c][2] * vSets[d][1])) / 6;
	double grad2cy = ((vSets[c][2] * vSets[d][0]) - (vSets[c][0] * vSets[d][2])) / 6;
	double grad2cz = ((vSets[c][0] * vSets[d][1]) - (vSets[c][1] * vSets[d][0])) / 6;
	//b中点d的变化梯度   1/6*d×a
	double grad2dx = ((vSets[d][1] * vSets[a][2]) - (vSets[d][2] * vSets[a][1])) / 6;
	double grad2dy = ((vSets[d][2] * vSets[a][0]) - (vSets[d][0] * vSets[a][2])) / 6;
	double grad2dz = ((vSets[d][0] * vSets[a][1]) - (vSets[d][1] * vSets[a][0])) / 6;
	//点b的变化梯度   变化梯度和为0
	double gradbx = -(grad2ax + grad2cx + grad2dx);
	double gradby = -(grad2ay + grad2cy + grad2dy);
	double gradbz = -(grad2az + grad2cz + grad2dz);

	//求c的变化梯度
    //c中点a的变化梯度  1/6*a×b
	double grad3ax = ((vSets[a][1] * vSets[b][2]) - (vSets[a][2] * vSets[b][1])) / 6;  
	double grad3ay = ((vSets[a][2] * vSets[b][0]) - (vSets[a][0] * vSets[b][2])) / 6;
	double grad3az = ((vSets[a][0] * vSets[b][1]) - (vSets[a][1] * vSets[b][0])) / 6;
	//c中点b的变化梯度   1/6*b×d
	double grad3bx = ((vSets[b][1] * vSets[d][2]) - (vSets[b][2] * vSets[d][1])) / 6;
	double grad3by = ((vSets[b][2] * vSets[d][0]) - (vSets[b][0] * vSets[d][2])) / 6;
	double grad3bz = ((vSets[b][0] * vSets[d][1]) - (vSets[b][1] * vSets[d][0])) / 6;
	//c中点d的变化梯度   1/6*d×a
	double grad3dx = ((vSets[d][1] * vSets[a][2]) - (vSets[d][2] * vSets[a][1])) / 6;
	double grad3dy = ((vSets[d][2] * vSets[a][0]) - (vSets[d][0] * vSets[a][2])) / 6;
	double grad3dz = ((vSets[d][0] * vSets[a][1]) - (vSets[d][1] * vSets[a][0])) / 6;
	//点c的变化梯度   变化梯度和为0
	double gradcx = -(grad3ax + grad3bx + grad3dx);
	double gradcy = -(grad3ay + grad3by + grad3dy);
	double gradcz = -(grad3az + grad3bz + grad3dz);


	//求d的变化梯度
    //d中点a的变化梯度  1/6*a×b
	double grad4ax = ((vSets[a][1] * vSets[b][2]) - (vSets[a][2] * vSets[b][1])) / 6;
	double grad4ay = ((vSets[a][2] * vSets[b][0]) - (vSets[a][0] * vSets[b][2])) / 6;
	double grad4az = ((vSets[a][0] * vSets[b][1]) - (vSets[a][1] * vSets[b][0])) / 6;
	//d中点b的变化梯度   1/6*b×c
	double grad4bx = ((vSets[b][1] * vSets[c][2]) - (vSets[b][2] * vSets[c][1])) / 6;
	double grad4by = ((vSets[b][2] * vSets[c][0]) - (vSets[b][0] * vSets[c][2])) / 6;
	double grad4bz = ((vSets[b][0] * vSets[c][1]) - (vSets[b][1] * vSets[c][0])) / 6;
	//d中点c的变化梯度   1/6*c×a
	double grad4cx = ((vSets[c][1] * vSets[a][2]) - (vSets[c][2] * vSets[a][1])) / 6;
	double grad4cy = ((vSets[c][2] * vSets[a][0]) - (vSets[c][0] * vSets[a][2])) / 6;
	double grad4cz = ((vSets[c][0] * vSets[a][1]) - (vSets[c][1] * vSets[a][0])) / 6;
	//点d的变化梯度   变化梯度和为0
	double graddx = -(grad4ax + grad4bx + grad4cx);
	double graddy = -(grad4ay + grad4by + grad4cy);
	double graddz = -(grad4az + grad4bz + grad4cz);


	//vab 表示变形前的向量ab  cvab 表示变形后的向量ab 其它同理
	double vabx = vSets_old[b][0] - vSets_old[a][0];
	double vaby = vSets_old[b][1] - vSets_old[a][1];
	double vabz = vSets_old[b][2] - vSets_old[a][2];//ab

	double vacx = vSets_old[c][0] - vSets_old[a][0];
	double vacy = vSets_old[c][1] - vSets_old[a][1];
	double vacz = vSets_old[c][2] - vSets_old[a][2];//ac

	double vadx = vSets_old[d][0] - vSets_old[a][0];
	double vady = vSets_old[d][1] - vSets_old[a][1];
	double vadz = vSets_old[d][2] - vSets_old[a][2];//ad

	//以B为顶点
	double vbax = vSets_old[a][0] - vSets_old[b][0];
	double vbay = vSets_old[a][1] - vSets_old[b][1];
	double vbaz = vSets_old[a][2] - vSets_old[b][2]; //ba向量

	double vbcx = vSets_old[c][0] - vSets_old[b][0];
	double vbcy = vSets_old[c][1] - vSets_old[b][1];
	double vbcz = vSets_old[c][2] - vSets_old[b][2]; //bc向量

	double vbdx = vSets_old[d][0] - vSets_old[b][0];
	double vbdy = vSets_old[d][1] - vSets_old[b][1];
	double vbdz = vSets_old[d][2] - vSets_old[b][2]; //bd向量

	//以C为顶点
	double vcax = vSets_old[a][0] - vSets_old[c][0];
	double vcay = vSets_old[a][1] - vSets_old[c][1];
	double vcaz = vSets_old[a][2] - vSets_old[c][2]; //ca向量

	double vcbx = vSets_old[b][0] - vSets_old[c][0];
	double vcby = vSets_old[b][1] - vSets_old[c][1];
	double vcbz = vSets_old[b][2] - vSets_old[c][2]; //cb向量

	double vcdx = vSets_old[d][0] - vSets_old[c][0];
	double vcdy = vSets_old[d][1] - vSets_old[c][1];
	double vcdz = vSets_old[d][2] - vSets_old[c][2]; //cd向量

	//以D为顶点
	double vdax = vSets_old[a][0] - vSets_old[d][0];
	double vday = vSets_old[a][1] - vSets_old[d][1];
	double vdaz = vSets_old[a][2] - vSets_old[d][2]; //da向量

	double vdbx = vSets_old[b][0] - vSets_old[d][0];
	double vdby = vSets_old[b][1] - vSets_old[d][1];
	double vdbz = vSets_old[b][2] - vSets_old[d][2]; //db向量

	double vdcx = vSets_old[c][0] - vSets_old[d][0];
	double vdcy = vSets_old[c][1] - vSets_old[d][1];
	double vdcz = vSets_old[c][2] - vSets_old[d][2]; //dc向量


	volumeConstraintA = (((vaby * vacz) - (vabz * vacy)) * vadx + ((vabz * vacx) - (vabx * vacz)) * vady + ((vabx * vacy) - (vaby * vacx)) * vadz) / 6;
	volumeConstraintB = (-((vbay * vbcz) - (vbaz * vbcy)) * vbdx - ((vbaz * vbcx) - (vbax * vbcz)) * vbdy - ((vbax * vbcy) - (vbay * vbcx)) * vbdz) / 6;
	volumeConstraintC = (((vcay * vcbz) - (vcaz * vcby)) * vcdx + ((vcaz * vcbx) - (vcax * vcbz)) * vcdy + ((vcax * vcby) - (vcay * vcbx)) * vcdz) / 6;
	volumeConstraintD = (((vday * vdbz) - (vdaz * vdby)) * vdcx + ((vdaz * vdbx) - (vdax * vdbz)) * vdcy + ((vdax * vdby) - (vday * vdbx)) * vdcz) / 6;

	//以A为顶点
	double cvabx = vSets[b][0] - vSets[a][0];   
	double cvaby = vSets[b][1] - vSets[a][1];
	double cvabz = vSets[b][2] - vSets[a][2]; //ab向量

	double cvacx = vSets[c][0] - vSets[a][0];
	double cvacy = vSets[c][1] - vSets[a][1];
	double cvacz = vSets[c][2] - vSets[a][2]; //ac向量

	double cvadx = vSets[d][0] - vSets[a][0];
	double cvady = vSets[d][1] - vSets[a][1];
	double cvadz = vSets[d][2] - vSets[a][2]; //ad向量

	//以B为顶点
	double cvbax = vSets[a][0] - vSets[b][0];
	double cvbay = vSets[a][1] - vSets[b][1];
	double cvbaz = vSets[a][2] - vSets[b][2]; //ba向量

	double cvbcx = vSets[c][0] - vSets[b][0];
	double cvbcy = vSets[c][1] - vSets[b][1];
	double cvbcz = vSets[c][2] - vSets[b][2]; //bc向量

	double cvbdx = vSets[d][0] - vSets[b][0];
	double cvbdy = vSets[d][1] - vSets[b][1];
	double cvbdz = vSets[d][2] - vSets[b][2]; //bd向量

	//以C为顶点
	double cvcax = vSets[a][0] - vSets[c][0];
	double cvcay = vSets[a][1] - vSets[c][1];
	double cvcaz = vSets[a][2] - vSets[c][2]; //ca向量

	double cvcbx = vSets[b][0] - vSets[c][0];
	double cvcby = vSets[b][1] - vSets[c][1];
	double cvcbz = vSets[b][2] - vSets[c][2]; //cb向量

	double cvcdx = vSets[d][0] - vSets[c][0];
	double cvcdy = vSets[d][1] - vSets[c][1];
	double cvcdz = vSets[d][2] - vSets[c][2]; //cd向量

	//以D为顶点
	double cvdax = vSets[a][0] - vSets[d][0];
	double cvday = vSets[a][1] - vSets[d][1];
	double cvdaz = vSets[a][2] - vSets[d][2]; //da向量

	double cvdbx = vSets[b][0] - vSets[d][0];
	double cvdby = vSets[b][1] - vSets[d][1];
	double cvdbz = vSets[b][2] - vSets[d][2]; //db向量

	double cvdcx = vSets[c][0] - vSets[d][0];
	double cvdcy = vSets[c][1] - vSets[d][1];
	double cvdcz = vSets[c][2] - vSets[d][2]; //dc向量


	volumeA = (((cvaby * cvacz) - (cvabz * cvacy)) * cvadx + ((cvabz * cvacx) - (cvabx * cvacz)) * cvady + ((cvabx * cvacy) - (cvaby * cvacx)) * cvadz) / 6;
	volumeB = (-((cvbay * cvbcz) - (cvbaz * cvbcy)) * cvbdx - ((cvbaz * cvbcx) - (cvbax * cvbcz)) * cvbdy - ((cvbax * cvbcy) - (cvbay * cvbcx)) * cvbdz) / 6;
	volumeC = (((cvcay * cvcbz) - (cvcaz * cvcby)) * cvcdx + ((cvcaz * cvcbx) - (cvcax * cvcbz)) * cvcdy + ((cvcax * cvcby) - (cvcay * cvcbx)) * cvcdz) / 6;
	volumeD = (((cvday * cvdbz) - (cvdaz * cvdby)) * cvdcx + ((cvdaz * cvdbx) - (cvdax * cvdbz)) * cvdcy + ((cvdax * cvdby) - (cvday * cvdbx)) * cvdcz) / 6;

	//s为比例因子 为体积约束/梯度模的平方和
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

	//改变模型点的位置。changA表示点a的修正因子，S是一个比例因子，用于调整四面体的体积变化量，k是另外一个比例因子，是用于调整四面体的形状变化量。
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
	double volumeConstraintA, volumeConstraintB, volumeConstraintC, volumeConstraintD;//用于约束的四面体体积（变形前的体积）
	double volumeA, volumeB, volumeC, volumeD;//变形后的体积
	double k = 1;//k为刚度参数    
	double sA, sB, sC, sD;//s为比例因子 为体积约束/梯度模的平方和


	//求a的变化梯度
	//a中点b的变化梯度   1/6*b×c
	double grad1bx = ((vttest_new[1][1] * vttest_new[2][2]) - (vttest_new[1][2] * vttest_new[2][1])) / 6;  //vttest_new[0][0] a=0,b=1,c=2,d=3
	double grad1by = ((vttest_new[1][2] * vttest_new[2][0]) - (vttest_new[1][0] * vttest_new[2][2])) / 6;
	double grad1bz = ((vttest_new[1][0] * vttest_new[2][1]) - (vttest_new[1][1] * vttest_new[2][0])) / 6;
	//a中点c的变化梯度   1/6*c×d
	double grad1cx = ((vttest_new[2][1] * vttest_new[3][2]) - (vttest_new[2][2] * vttest_new[3][1])) / 6;
	double grad1cy = ((vttest_new[2][2] * vttest_new[3][0]) - (vttest_new[2][0] * vttest_new[3][2])) / 6;
	double grad1cz = ((vttest_new[2][0] * vttest_new[3][1]) - (vttest_new[2][1] * vttest_new[3][0])) / 6;
	//a中点d的变化梯度   1/6*d×b
	double grad1dx = ((vttest_new[3][1] * vttest_new[1][2]) - (vttest_new[3][2] * vttest_new[1][1])) / 6;
	double grad1dy = ((vttest_new[3][2] * vttest_new[1][0]) - (vttest_new[3][0] * vttest_new[1][2])) / 6;
	double grad1dz = ((vttest_new[3][0] * vttest_new[1][1]) - (vttest_new[3][1] * vttest_new[1][0])) / 6;
	//点a的变化梯度   变化梯度和为0
	double gradax = -(grad1bx + grad1cx + grad1dx);
	double graday = -(grad1by + grad1cy + grad1dy);
	double gradaz = -(grad1bz + grad1cz + grad1dz);

	//求b的变化梯度
	//b中点a的变化梯度  1/6*a×c
	double grad2ax = ((vttest_new[0][1] * vttest_new[2][2]) - (vttest_new[0][2] * vttest_new[2][1])) / 6;
	double grad2ay = ((vttest_new[0][2] * vttest_new[2][0]) - (vttest_new[0][0] * vttest_new[2][2])) / 6;
	double grad2az = ((vttest_new[0][0] * vttest_new[2][1]) - (vttest_new[0][1] * vttest_new[2][0])) / 6;
	//b中点c的变化梯度   1/6*c×d
	double grad2cx = ((vttest_new[2][1] * vttest_new[3][2]) - (vttest_new[2][2] * vttest_new[3][1])) / 6;
	double grad2cy = ((vttest_new[2][2] * vttest_new[3][0]) - (vttest_new[2][0] * vttest_new[3][2])) / 6;
	double grad2cz = ((vttest_new[2][0] * vttest_new[3][1]) - (vttest_new[2][1] * vttest_new[3][0])) / 6;
	//b中点d的变化梯度   1/6*d×a
	double grad2dx = ((vttest_new[3][1] * vttest_new[0][2]) - (vttest_new[3][2] * vttest_new[0][1])) / 6;
	double grad2dy = ((vttest_new[3][2] * vttest_new[0][0]) - (vttest_new[3][0] * vttest_new[0][2])) / 6;
	double grad2dz = ((vttest_new[3][0] * vttest_new[0][1]) - (vttest_new[3][1] * vttest_new[0][0])) / 6;
	//点b的变化梯度   变化梯度和为0
	double gradbx = -(grad2ax + grad2cx + grad2dx);
	double gradby = -(grad2ay + grad2cy + grad2dy);
	double gradbz = -(grad2az + grad2cz + grad2dz);

	//求c的变化梯度
	//c中点a的变化梯度  1/6*a×b
	double grad3ax = ((vttest_new[0][1] * vttest_new[1][2]) - (vttest_new[0][2] * vttest_new[1][1])) / 6;
	double grad3ay = ((vttest_new[0][2] * vttest_new[1][0]) - (vttest_new[0][0] * vttest_new[1][2])) / 6;
	double grad3az = ((vttest_new[0][0] * vttest_new[1][1]) - (vttest_new[0][1] * vttest_new[1][0])) / 6;
	//c中点b的变化梯度   1/6*b×d
	double grad3bx = ((vttest_new[1][1] * vttest_new[3][2]) - (vttest_new[1][2] * vttest_new[3][1])) / 6;
	double grad3by = ((vttest_new[1][2] * vttest_new[3][0]) - (vttest_new[1][0] * vttest_new[3][2])) / 6;
	double grad3bz = ((vttest_new[1][0] * vttest_new[3][1]) - (vttest_new[1][1] * vttest_new[3][0])) / 6;
	//c中点d的变化梯度   1/6*d×a
	double grad3dx = ((vttest_new[3][1] * vttest_new[0][2]) - (vttest_new[3][2] * vttest_new[0][1])) / 6;
	double grad3dy = ((vttest_new[3][2] * vttest_new[0][0]) - (vttest_new[3][0] * vttest_new[0][2])) / 6;
	double grad3dz = ((vttest_new[3][0] * vttest_new[0][1]) - (vttest_new[3][1] * vttest_new[0][0])) / 6;
	//点c的变化梯度   变化梯度和为0
	double gradcx = -(grad3ax + grad3bx + grad3dx);
	double gradcy = -(grad3ay + grad3by + grad3dy);
	double gradcz = -(grad3az + grad3bz + grad3dz);


	//求d的变化梯度
	//d中点a的变化梯度  1/6*a×b
	double grad4ax = ((vttest_new[0][1] * vttest_new[1][2]) - (vttest_new[0][2] * vttest_new[1][1])) / 6;
	double grad4ay = ((vttest_new[0][2] * vttest_new[1][0]) - (vttest_new[0][0] * vttest_new[1][2])) / 6;
	double grad4az = ((vttest_new[0][0] * vttest_new[1][1]) - (vttest_new[0][1] * vttest_new[1][0])) / 6;
	//d中点b的变化梯度   1/6*b×c
	double grad4bx = ((vttest_new[1][1] * vttest_new[2][2]) - (vttest_new[1][2] * vttest_new[2][1])) / 6;
	double grad4by = ((vttest_new[1][2] * vttest_new[2][0]) - (vttest_new[1][0] * vttest_new[2][2])) / 6;
	double grad4bz = ((vttest_new[1][0] * vttest_new[2][1]) - (vttest_new[1][1] * vttest_new[2][0])) / 6;
	//d中点c的变化梯度   1/6*c×a
	double grad4cx = ((vttest_new[2][1] * vttest_new[0][2]) - (vttest_new[2][2] * vttest_new[0][1])) / 6;
	double grad4cy = ((vttest_new[2][2] * vttest_new[0][0]) - (vttest_new[2][0] * vttest_new[0][2])) / 6;
	double grad4cz = ((vttest_new[2][0] * vttest_new[0][1]) - (vttest_new[2][1] * vttest_new[0][0])) / 6;
	//点d的变化梯度   变化梯度和为0
	double graddx = -(grad4ax + grad4bx + grad4cx);
	double graddy = -(grad4ay + grad4by + grad4cy);
	double graddz = -(grad4az + grad4bz + grad4cz);


	//vab 表示变形前的向量ab  cvab 表示变形后的向量ab 其它同理                             //vtSetstest[0][0] a=0,b=1,c=2,d=3
	double vabx = vtSetstest[1][0] - vtSetstest[0][0];
	double vaby = vtSetstest[1][1] - vtSetstest[0][1];
	double vabz = vtSetstest[1][2] - vtSetstest[0][2];//ab

	double vacx = vtSetstest[2][0] - vtSetstest[0][0];
	double vacy = vtSetstest[2][1] - vtSetstest[0][1];
	double vacz = vtSetstest[2][2] - vtSetstest[0][2];//ac

	double vadx = vtSetstest[3][0] - vtSetstest[0][0];
	double vady = vtSetstest[3][1] - vtSetstest[0][1];
	double vadz = vtSetstest[3][2] - vtSetstest[0][2];//ad

	//以B为顶点
	double vbax = vtSetstest[0][0] - vtSetstest[1][0];
	double vbay = vtSetstest[0][1] - vtSetstest[1][1];
	double vbaz = vtSetstest[0][2] - vtSetstest[1][2]; //ba向量

	double vbcx = vtSetstest[2][0] - vtSetstest[1][0];
	double vbcy = vtSetstest[2][1] - vtSetstest[1][1];
	double vbcz = vtSetstest[2][2] - vtSetstest[1][2]; //bc向量

	double vbdx = vtSetstest[3][0] - vtSetstest[1][0];
	double vbdy = vtSetstest[3][1] - vtSetstest[1][1];
	double vbdz = vtSetstest[3][2] - vtSetstest[1][2]; //bd向量

	//以C为顶点
	double vcax = vtSetstest[0][0] - vtSetstest[2][0];
	double vcay = vtSetstest[0][1] - vtSetstest[2][1];
	double vcaz = vtSetstest[0][2] - vtSetstest[2][2]; //ca向量

	double vcbx = vtSetstest[1][0] - vtSetstest[2][0];
	double vcby = vtSetstest[1][1] - vtSetstest[2][1];
	double vcbz = vtSetstest[1][2] - vtSetstest[2][2]; //cb向量

	double vcdx = vtSetstest[3][0] - vtSetstest[2][0];
	double vcdy = vtSetstest[3][1] - vtSetstest[2][1];
	double vcdz = vtSetstest[3][2] - vtSetstest[2][2]; //cd向量

	//以D为顶点
	double vdax = vtSetstest[0][0] - vtSetstest[3][0];
	double vday = vtSetstest[0][1] - vtSetstest[3][1];
	double vdaz = vtSetstest[0][2] - vtSetstest[3][2]; //da向量

	double vdbx = vtSetstest[1][0] - vtSetstest[3][0];
	double vdby = vtSetstest[1][1] - vtSetstest[3][1];
	double vdbz = vtSetstest[1][2] - vtSetstest[3][2]; //db向量

	double vdcx = vtSetstest[2][0] - vtSetstest[3][0];
	double vdcy = vtSetstest[2][1] - vtSetstest[3][1];
	double vdcz = vtSetstest[2][2] - vtSetstest[3][2]; //dc向量


	volumeConstraintA = (-((vaby * vacz) - (vabz * vacy)) * vadx - ((vabz * vacx) - (vabx * vacz)) * vady - ((vabx * vacy) - (vaby * vacx)) * vadz) / 6;  //-1/6  多增加一个点后为1/6
	//volumeConstraintB = (-((vbay * vbcz) - (vbaz * vbcy)) * vbdx - ((vbaz * vbcx) - (vbax * vbcz)) * vbdy - ((vbax * vbcy) - (vbay * vbcx)) * vbdz) / 6;  
	volumeConstraintB = (((vbay * vbcz) - (vbaz * vbcy)) * vbdx + ((vbaz * vbcx) - (vbax * vbcz)) * vbdy + ((vbax * vbcy) - (vbay * vbcx)) * vbdz) / 6;   //-1/6
	volumeConstraintC = (-((vcay * vcbz) - (vcaz * vcby)) * vcdx - ((vcaz * vcbx) - (vcax * vcbz)) * vcdy - ((vcax * vcby) - (vcay * vcbx)) * vcdz) / 6;   //-1/6
	volumeConstraintD = (((vday * vdbz) - (vdaz * vdby)) * vdcx + ((vdaz * vdbx) - (vdax * vdbz)) * vdcy + ((vdax * vdby) - (vday * vdbx)) * vdcz) / 6;  //-1/6

	//以A为顶点
	double cvabx = vttest_new[1][0] - vttest_new[0][0];                                   //vttest_new[0][0]、vttest_new[0][1]和vttest_new[0][2]  a=0,b=1,c=2,d=3
	double cvaby = vttest_new[1][1] - vttest_new[0][1];
	double cvabz = vttest_new[1][2] - vttest_new[0][2]; //ab向量

	double cvacx = vttest_new[2][0] - vttest_new[0][0];
	double cvacy = vttest_new[2][1] - vttest_new[0][1];
	double cvacz = vttest_new[2][2] - vttest_new[0][2]; //ac向量

	double cvadx = vttest_new[3][0] - vttest_new[0][0];
	double cvady = vttest_new[3][1] - vttest_new[0][1];
	double cvadz = vttest_new[3][2] - vttest_new[0][2]; //ad向量

	//以B为顶点
	double cvbax = vttest_new[0][0] - vttest_new[1][0];
	double cvbay = vttest_new[0][1] - vttest_new[1][1];
	double cvbaz = vttest_new[0][2] - vttest_new[1][2]; //ba向量

	double cvbcx = vttest_new[2][0] - vttest_new[1][0];
	double cvbcy = vttest_new[2][1] - vttest_new[1][1];
	double cvbcz = vttest_new[2][2] - vttest_new[1][2]; //bc向量

	double cvbdx = vttest_new[3][0] - vttest_new[1][0];
	double cvbdy = vttest_new[3][1] - vttest_new[1][1];
	double cvbdz = vttest_new[3][2] - vttest_new[1][2]; //bd向量

	//以C为顶点
	double cvcax = vttest_new[0][0] - vttest_new[2][0];
	double cvcay = vttest_new[0][1] - vttest_new[2][1];
	double cvcaz = vttest_new[0][2] - vttest_new[2][2]; //ca向量

	double cvcbx = vttest_new[1][0] - vttest_new[2][0];
	double cvcby = vttest_new[1][1] - vttest_new[2][1];
	double cvcbz = vttest_new[1][2] - vttest_new[2][2]; //cb向量

	double cvcdx = vttest_new[3][0] - vttest_new[2][0];
	double cvcdy = vttest_new[3][1] - vttest_new[2][1];
	double cvcdz = vttest_new[3][2] - vttest_new[2][2]; //cd向量

	//以D为顶点
	double cvdax = vttest_new[0][0] - vttest_new[3][0];
	double cvday = vttest_new[0][1] - vttest_new[3][1];
	double cvdaz = vttest_new[0][2] - vttest_new[3][2]; //da向量

	double cvdbx = vttest_new[1][0] - vttest_new[3][0];
	double cvdby = vttest_new[1][1] - vttest_new[3][1];
	double cvdbz = vttest_new[1][2] - vttest_new[3][2]; //db向量

	double cvdcx = vttest_new[2][0] - vttest_new[3][0];
	double cvdcy = vttest_new[2][1] - vttest_new[3][1];
	double cvdcz = vttest_new[2][2] - vttest_new[3][2]; //dc向量


	volumeA = (-((cvaby * cvacz) - (cvabz * cvacy)) * cvadx - ((cvabz * cvacx) - (cvabx * cvacz)) * cvady - ((cvabx * cvacy) - (cvaby * cvacx)) * cvadz) / 6; //-2/3   1/3
	//volumeB = (-((cvbay * cvbcz) - (cvbaz * cvbcy)) * cvbdx - ((cvbaz * cvbcx) - (cvbax * cvbcz)) * cvbdy - ((cvbax * cvbcy) - (cvbay * cvbcx)) * cvbdz) / 6;  
	volumeB = (((cvbay * cvbcz) - (cvbaz * cvbcy)) * cvbdx + ((cvbaz * cvbcx) - (cvbax * cvbcz)) * cvbdy + ((cvbax * cvbcy) - (cvbay * cvbcx)) * cvbdz) / 6;  //-2/3
	volumeC = (-((cvcay * cvcbz) - (cvcaz * cvcby)) * cvcdx - ((cvcaz * cvcbx) - (cvcax * cvcbz)) * cvcdy - ((cvcax * cvcby) - (cvcay * cvcbx)) * cvcdz) / 6;  //-2/3
	volumeD = (((cvday * cvdbz) - (cvdaz * cvdby)) * cvdcx + ((cvdaz * cvdbx) - (cvdax * cvdbz)) * cvdcy + ((cvdax * cvdby) - (cvday * cvdbx)) * cvdcz) / 6;  //-2/3

	//s为比例因子 为体积约束/梯度模的平方和
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

	//改变模型点的位置。changA表示点a的修正因子，S是一个比例因子，用于调整四面体的体积变化量，k是另外一个比例因子，是用于调整四面体的形状变化量。
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


	cvabx = vttest_new[1][0] - vttest_new[0][0];                                   //vttest_new[0][0]、vttest_new[0][1]和vttest_new[0][2]  a=0,b=1,c=2,d=3
	cvaby = vttest_new[1][1] - vttest_new[0][1];
	cvabz = vttest_new[1][2] - vttest_new[0][2]; //ab向量

	 cvacx = vttest_new[2][0] - vttest_new[0][0];
	 cvacy = vttest_new[2][1] - vttest_new[0][1];
	 cvacz = vttest_new[2][2] - vttest_new[0][2]; //ac向量

	 cvadx = vttest_new[3][0] - vttest_new[0][0];
	 cvady = vttest_new[3][1] - vttest_new[0][1];
	 cvadz = vttest_new[3][2] - vttest_new[0][2]; //ad向量

	//以B为顶点
	 cvbax = vttest_new[0][0] - vttest_new[1][0];
	 cvbay = vttest_new[0][1] - vttest_new[1][1];
	  cvbaz = vttest_new[0][2] - vttest_new[1][2]; //ba向量

	  cvbcx = vttest_new[2][0] - vttest_new[1][0];
	  cvbcy = vttest_new[2][1] - vttest_new[1][1];
	  cvbcz = vttest_new[2][2] - vttest_new[1][2]; //bc向量

	  cvbdx = vttest_new[3][0] - vttest_new[1][0];
	  cvbdy = vttest_new[3][1] - vttest_new[1][1];
	  cvbdz = vttest_new[3][2] - vttest_new[1][2]; //bd向量

	//以C为顶点
	  cvcax = vttest_new[0][0] - vttest_new[2][0];
	  cvcay = vttest_new[0][1] - vttest_new[2][1];
	  cvcaz = vttest_new[0][2] - vttest_new[2][2]; //ca向量

	  cvcbx = vttest_new[1][0] - vttest_new[2][0];
	  cvcby = vttest_new[1][1] - vttest_new[2][1];
	  cvcbz = vttest_new[1][2] - vttest_new[2][2]; //cb向量

	  cvcdx = vttest_new[3][0] - vttest_new[2][0];
	  cvcdy = vttest_new[3][1] - vttest_new[2][1];
	  cvcdz = vttest_new[3][2] - vttest_new[2][2]; //cd向量

	//以D为顶点
	  cvdax = vttest_new[0][0] - vttest_new[3][0];
	  cvday = vttest_new[0][1] - vttest_new[3][1];
	  cvdaz = vttest_new[0][2] - vttest_new[3][2]; //da向量

	  cvdbx = vttest_new[1][0] - vttest_new[3][0];
	  cvdby = vttest_new[1][1] - vttest_new[3][1];
	  cvdbz = vttest_new[1][2] - vttest_new[3][2]; //db向量

	  cvdcx = vttest_new[2][0] - vttest_new[3][0];
	  cvdcy = vttest_new[2][1] - vttest_new[3][1];
	  cvdcz = vttest_new[2][2] - vttest_new[3][2]; //dc向量


	volumeA = (-((cvaby * cvacz) - (cvabz * cvacy)) * cvadx - ((cvabz * cvacx) - (cvabx * cvacz)) * cvady - ((cvabx * cvacy) - (cvaby * cvacx)) * cvadz) / 6; //-21/4
	//volumeB = (-((cvbay * cvbcz) - (cvbaz * cvbcy)) * cvbdx - ((cvbaz * cvbcx) - (cvbax * cvbcz)) * cvbdy - ((cvbax * cvbcy) - (cvbay * cvbcx)) * cvbdz) / 6;  
	volumeB = (((cvbay * cvbcz) - (cvbaz * cvbcy)) * cvbdx + ((cvbaz * cvbcx) - (cvbax * cvbcz)) * cvbdy + ((cvbax * cvbcy) - (cvbay * cvbcx)) * cvbdz) / 6;  //-21/4
	volumeC = (-((cvcay * cvcbz) - (cvcaz * cvcby)) * cvcdx - ((cvcaz * cvcbx) - (cvcax * cvcbz)) * cvcdy - ((cvcax * cvcby) - (cvcay * cvcbx)) * cvcdz) / 6;  //-21/4
	volumeD = (((cvday * cvdbz) - (cvdaz * cvdby)) * cvdcx + ((cvdaz * cvdbx) - (cvdax * cvdbz)) * cvdcy + ((cvdax * cvdby) - (cvday * cvdbx)) * cvdcz) / 6;  //-21/4



}



//void ObjLoader::ComputerSkeletonPointRestriction(float k,int a,int b, int c, int d,int e,int f, vector<vector<GLfloat>>& vSets_old, float centers[2][3]) {
//	float dis_old= sqrt(pow(vSets_old[e][0] - centers[f][0], 2) + pow(vSets_old[e][1] - centers[f][1], 2) + pow(vSets_old[e][2] - centers[f][2], 2));
//	double centerx = (vSets[a][0] + vSets[b][0] + vSets[c][0] + vSets[d][0]) / 4.0; //计算四面体的中心坐标，除以4是为了计算出每个顶点到中心的平均距离
//	double centery = (vSets[a][1] + vSets[b][1] + vSets[c][1] + vSets[d][1]) / 4.0;
//	double centerz = (vSets[a][2] + vSets[b][2] + vSets[c][2] + vSets[d][2]) / 4.0;
//	float dis_new= sqrt(pow(vSets[e][0] - centerx, 2) + pow(vSets[e][1] - centery, 2) + pow(vSets[e][2] - centerz, 2));
//	float dis = (dis_new - dis_old) / dis_new;    //相对距离变化
//	vSets[e][0] += 0.5*k * dis*(vSets[e][0] - centerx); //x方向相对距离变化
//	vSets[e][1] += 0.5*k * dis*(vSets[e][1] - centery); //y方向相对距离变化
//	vSets[e][2] +=0.5* k * dis*(vSets[e][2] - centerz); //z方向相对距离变化
//
//}


//修改四面体网格的点的坐标
void ObjLoader::ChangeTetrahSet() {

	if (vSets.size() < 1000) {
		vSets_old.assign(vSets.begin(), vSets.end());
		string line;
		fstream f;
		int count = 0;
		//设置模型动作：读取控制网格变化后的顶点坐标  此处添加控制网格坐标txt数据
		f.open("C:/Users/reckless/Desktop/3D模型绘制/datou2.txt", ios::in);

		if (!f.is_open()) {
			cout << "Something Went Wrong When Opening Txtfiles" << endl;
		}

		while (!f.eof()) {

			getline(f, line);//拿到txt文件中的一行，作为一个字符串

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
						parameters.push_back(ans);//取出字符串中的元素，以,切分
					}
					ans = "";
				}
			}
			if (parameters.size() == 3) {

				for (int i = 0; i < 3; i++) {//从1开始，将顶点的xyz三个坐标放进顶点vector

					GLfloat xyz = atof(parameters[i].c_str());
					vSets[count][i] = xyz;
					if (i == 2) { count++; }
				}

			}
		}

		/*for (int i = 0; i < vSets.size(); i++) {
			vSets[i][2] += 0;
			}*/
			//添加对大幅度抬左手的点约束
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

		//动作为模型右手拉伸时的约束
/*StretchConstraint1(43,48,2.0,vSets_old);
StretchConstraint1(44,47,2.0,vSets_old);
StretchConstraint1(19,24,2.0,vSets_old);
StretchConstraint1(22,23,2.0,vSets_old);*/

		//动作为模型右拉伸时的约束
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
		//四面体体积约束  左腿151-160

		
		
		
		
		
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
	model1_net3.GenerateNet(vSets, "C:/Users/reckless/Desktop/3D模型绘制/Boxlist5.txt");//vBoxSets_new为txtload中的动作
	T_boxNet = model1_net3.Getm_boxNet();
	TetrahSet_aftermove = model1_net3.Getcm_boxNet();
}
//对3D模型顶点进行约束或形变处理。其中，形变的效果是基于弹性系数的，弹性系数越大，形变的效果就越明显。
void ObjLoader::ModelConstraint(vector<vector<GLfloat>>& vSets, vector<vector<GLfloat>>& vSets_old, int range[], int length, float elasticity) {


	for (int i = 0; i < length; i++) {//特别注意：数组作为参数传递后会退化成为指针，无法通过指针直接得到数组大小！！！！
		cout << length << "";
		for (int j = 0; j < tetrahWithV[range[i]].size(); j++) {
			cout << "  " << range[i] << "  " << j << "  ";
			/*cout << "变化前：" << vSets[tetrahWithV[range[i]][j]][0] << "		";
			cout << vSets[tetrahWithV[range[i]][j]][1] << "		";
			cout << vSets[tetrahWithV[range[i]][j]][2] << "\n";*/

			vSets[tetrahWithV[range[i]][j]][0] = vSets[tetrahWithV[range[i]][j]][0] + elasticity * (vSets_old[tetrahWithV[range[i]][j]][0] - vSets[tetrahWithV[range[i]][j]][0]);
			vSets[tetrahWithV[range[i]][j]][1] = vSets[tetrahWithV[range[i]][j]][1] + elasticity * (vSets_old[tetrahWithV[range[i]][j]][1] - vSets[tetrahWithV[range[i]][j]][1]);
			vSets[tetrahWithV[range[i]][j]][2] = vSets[tetrahWithV[range[i]][j]][2] + elasticity * (vSets_old[tetrahWithV[range[i]][j]][2] - vSets[tetrahWithV[range[i]][j]][2]);
			cout << "变化后：" << vSets[tetrahWithV[range[i]][j]][0] << "		";
			cout << vSets[tetrahWithV[range[i]][j]][1] << "		";
			cout << vSets[tetrahWithV[range[i]][j]][2] << "\n";
			cout << "";
		}
	}
}
void dual_quat_deformer(const std::vector<Point3>& in_verts,//网格的顶点
	const std::vector<Vec3>& in_normals, //网格法线的向量
	std::vector<Vec3>& out_verts,//使用双四元数变形后的顶点
	std::vector<Vec3>& out_normals,//使用双四元数变形后的法线
	const std::vector<Dual_quat_cu>& dual_quat,//每个关节的双四元数变换
	const std::vector< std::vector<float> >& weights,//每个顶点的影响权重
	const std::vector< std::vector<int> >& joints_id)//关节ID
{
	for (unsigned v = 0; v < in_verts.size(); ++v)//循环遍历输入顶点向量 in_verts 中的每一个顶点。
	{
		const int nb_joints = weights[v].size(); // Number of joints influencing vertex p  首先获取了当前顶点受影响的关节数量 nb_joints

		// Init dual quaternion with first joint transformation      使用第一个关节变换初始化双四元数
		int   k0 = -1;    //声明了几个变量，用于存储双四元数的混合结果以及其他临时变量。
		float w0 = 0.f;
		Dual_quat_cu dq_blend;   //存储混合结果
		Quat_cu q0;   //存储第一个关节旋转

		if (nb_joints != 0)  //检查当前顶点是否受到任何关节的影响。如果受到影响，将第一个关节的索引和权重存储在变量 k0 和 w0 中，否则将双四元数混合结果设置为单位双四元数。
		{
			k0 = joints_id[v][0];
			w0 = weights[v][0];
		}
		else
			dq_blend = Dual_quat_cu::identity();

		if (k0 != -1) dq_blend = dual_quat[k0] * w0;   //如果第一个关节索引有效，则使用其对应的双四元数与权重进行混合。

		//这部分代码是处理其他关节对当前顶点的影响。它遍历了除了第一个关节之外的其他关节，对每个关节进行相应的双四元数混合，计算出最终的双四元数混合结果。

		int pivot = k0;   //存储了第一个关节索引

		q0 = dual_quat[pivot].rotation();   //将第一个关节的旋转部分存储在 q0 变量中

		// Look up the other joints influencing 'p' if any          
		for (int j = 1; j < nb_joints; j++)  //遍历除第一个关节之外的其他关节，对每个关节进行相应的双四元数混合。
		{
			const int k = joints_id[v][j];
			float w = weights[v][j];
			const Dual_quat_cu& dq = (k == -1) ? Dual_quat_cu::identity() : dual_quat[k];//如果 k 的值等于 -1，则表示顶点 v 不受任何关节的影响，即没有旋转和平移的变换。
			//如果 k 的值不等于 -1，则将 dq 设置为 dual_quat[k]，即顶点 v 受到关节 k 的双四元数变换的影响。
			if (dq.rotation().dot(q0) < 0.f)   //确保多关节旋转时，与第一个关节旋转方向保持一致
				w *= -1.f;

			dq_blend = dq_blend + dq * w;
		}

		//计算对偶四元数变形后的顶点位置和法线，并将结果存储到输出向量 out_verts 和 out_normals 中。
		// Compute animated position
		Vec3 vi = dq_blend.transform(in_verts[v]).to_vec3();
		out_verts[v] = vi;
		// Compute animated normal
		out_normals[v] = dq_blend.rotate(in_normals[v]);
	}
}

void ObjLoader::computerKinTerachbySearch(vector<CPoint3D>& VSet, vector<Tetrah>& TetrahSet_initial, vector<Tetrah>& TetrahSet_aftermove, vector<CPoint3D>& vSet_new) {
	vSets_old.assign(vSets.begin(), vSets.end()); // 将原始顶点集"vSets"复制到"vSet_old"中

	std::vector<Point3> in_verts;
	std::vector<Vec3> in_normals;
	std::vector<std::vector<float>> weights;
	std::vector<std::vector<int>> joints_id;
	std::vector<int> points_indices;

	// 遍历VSet中的每个点
	for (int p = 0; p < VSet.size(); p++) {
		ChangePoint(VSet[p]);
		CPoint3D P = PointChange; // 获取变换后的顶点坐标

		// 计算点P在网格TetrahSet[i]中的k1, k2, k3值
		for (int i = 0; i < TetrahSet_initial.size(); i++) { // 遍历四面体网格
			CPoint3D K = TetrahSet_initial[i].ComputeKcoordinateValues(P); // 计算当前顶点P在初始四面体中的K坐标

			// 根据K(k1, k2, k3) 均在[0, 1]之间，终端循环，记录下i值
			if (K.X() >= 0 && K.X() <= 1 && K.Y() >= 0 && K.Y() <= 1 && K.Z() >= 0 && K.Z() <= 1) { // 检查K坐标是否在0-1范围内
				tetrahWithV[i].push_back(p); // 记录当前顶点P与对应四面体的关联关系
				CPoint3D P_new = TetrahSet_aftermove[i].ComputePcoordinateValutes(K); // 根据K坐标在变换后的四面体中计算新的顶点坐标P_new
				ChangePoint(P_new); // 改变X, Y, Z顺序

				// 如果顶点在特定范围内，则记录这些顶点的信息
				if (i >= 141 && i <= 150) { // 注意这里将 p 改为 i，因为 i 是四面体网格的索引
					in_verts.push_back(Point3(P_new.X(), P_new.Y(), P_new.Z()));
					in_normals.push_back({ 1.0f, 2.0f, 3.0f }); // 假设的法线数据，可以根据需要修改
					weights.push_back({ 1.0f }); // 假设的权重数据，可以根据需要修改
					joints_id.push_back({ 2 }); // 假设的关节ID数据，可以根据需要修改
					points_indices.push_back(p);
				}
				else {
					// 更新顶点集
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

	// 进行对偶四元数的旋转操作
	if (!in_verts.empty()) {
		std::vector<Dual_quat_cu> dual_quat = {
			Tbx::Dual_quat_cu(Quat_cu(1, 0, 0, 0), Quat_cu(0, 0, 0, 0)),
			Tbx::Dual_quat_cu(Quat_cu(0, -1, 0, 1), Quat_cu(0, 0, 0, 0.0)),
			Tbx::Dual_quat_cu(Quat_cu(cos(PI / 8), sin(PI / 8) * 0, sin(PI / 8) * 0, sin(PI / 8) * 1), Quat_cu(0, 0, 0, 0))
			// 添加更多双四元数变换
		};

		std::vector<Vec3> out_verts(in_verts.size());
		std::vector<Vec3> out_normals(in_normals.size());

		// 调用 dual_quat_deformer 函数进行网格变形
		dual_quat_deformer(in_verts, in_normals, out_verts, out_normals, dual_quat, weights, joints_id);

		// 更新变换后的所有顶点坐标
		for (int j = 0; j < out_verts.size(); ++j) {
			CPoint3D P_new = CPoint3D(out_verts[j].x, out_verts[j].y, out_verts[j].z);
			ChangePoint(P_new); // 改变X, Y, Z顺序

			// 更新顶点集
			int p = points_indices[j];
			vSets[p][0] = PointChange.X();
			vSets[p][1] = PointChange.Y();
			vSets[p][2] = PointChange.Z();
			vSet_new.emplace_back(PointChange);
		}
	}
}
void ObjLoader::ComputerSkeletonPointRestriction(vector<CPoint3D>& VSet, vector<Tetrah>& TetrahSet_initial, vector<Tetrah>& TetrahSet_aftermove, vector<CPoint3D>& vSet_new, float k, float centers[1][3]) {
	vSets_old.assign(vSets.begin(), vSets.end());  //将原始顶点集"vSets复制到vSet_old中"
	for (int p = 0; p < VSet.size(); p++) {
		ChangePoint(VSet[p]);
		CPoint3D P = PointChange;   //获取顶点坐标
		//float distanceToCenterOld = sqrt((pow(P.X() - centers[0][0], 2) + pow(P.Y() - centers[0][1], 2)) + pow(P.Z() - centers[0][2], 2));

		// 计算点P在网格TerahSet[i]中的k1,k2,k3值
		for (int i = 0; i < TetrahSet_initial.size(); i++) {
			CPoint3D K = TetrahSet_initial[i].ComputeKcoordinateValues(P);    //计算当前顶点P在初始四面体中的K坐标

			// 根据K(k1, k2, k3)均在[0, 1]之间，终止循环，记录下i值
			if (K.X() >= 0 && K.X() <= 1 && K.Y() >= 0 && K.Y() <= 1 && K.Z() >= 0 && K.Z() <= 1) {
				
				CPoint3D P_new = TetrahSet_aftermove[i].ComputePcoordinateValutes(K);              //根据K坐标在变换后的四面体中计算新的顶点坐标P_new.
				// 引入关节处的限制
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
					ChangePoint(P_new);  //改变X,Y,Z顺序
					vSets[p][0] = PointChange.X();//更新坐标
					vSets[p][1] = PointChange.Y();
					vSets[p][2] = PointChange.Z();
					vSet_new.emplace_back(PointChange); //将新的坐标添加到vSet_new中
					break;

				}
				else {
					ChangePoint(P_new);  //改变X,Y,Z顺序

					vSets[p][0] = PointChange.X();//更新坐标
					vSets[p][1] = PointChange.Y();
					vSets[p][2] = PointChange.Z();
					vSet_new.emplace_back(PointChange); //将新的坐标添加到vSet_new中
					
				}

				break;
			}
		}
	}
}

//void ObjLoader::ComputerSkeletonPointRestriction(vector<CPoint3D>& vSet_new, vector<Tetrah>& TetrahSet_aftermove, float k, float centers[1][3]) {
//	// 创建一个副本，以便在修改过程中保留原始坐标
//	vector<CPoint3D> vSet_copy(vSet_new);
//
//	// 遍历新的顶点集合 vSet_new
//	for (int p = 0; p < vSet_copy.size(); p++) {
//		// 将当前顶点 vSet_copy[p] 进行坐标变换，并存储在 PointChange 中
//		ChangePoint(vSet_copy[p]);
//		CPoint3D P = PointChange;
//
//		// 计算点 P 到关节的距离
//		float distanceToJoint = sqrt((pow(P.X() - centers[0][0], 2) + pow(P.Y() - centers[0][1], 2)) + pow(P.Z() - centers[0][2], 2));
//
//		// 判断是否需要进行关节点限制
//		if (distanceToJoint < 5) {
//			// 计算变形程度，使用指数函数作为变形函数
//			float deformFactor = k * exp(-distanceToJoint);
//
//			// 对点进行变形
//			CPoint3D updatedP(deformFactor * (centers[0][0] - P.X()) + P.X(),
//				deformFactor * (centers[0][1] - P.Y()) + P.Y(),
//				deformFactor * (centers[0][2] - P.Z()) + P.Z());
//
//			// 将更新后的坐标添加到 vSet_new 中
//			vSet_new[p] = updatedP;
//		}
//		// 如果不需要关节点限制，保持原始坐标
//	}
//}







//设置模型点约束变形 对大幅度抬左手动作中的左胸部添加约束
//设置约束的四面体区域
//ModelConstraint(vSets, vSets_old, range, length, 0.8);
//int range1[] = {
//	55,56,57,58,59,68,69,70,71,72 };
//int length1 = sizeof(range1) / sizeof(range1[0]);
//ModelConstraint(vSets, vSets_old, range1, length1, 0.8);

//int range2[] = {
//	68,69,70,71,72 };
//int length2 = sizeof(range2) / sizeof(range2[0]);
//ModelConstraint(vSets, vSets_old, range2, length2, 0);
//画线
void ObjLoader::Draw1() {
	for (int i = 0; i < fSets.size(); i++) {
		glBegin(GL_LINE_LOOP);//开始绘制图元
		GLfloat VN[3];

		//三个顶点
		GLfloat SV1[3];
		GLfloat	SV2[3];
		GLfloat SV3[3];

		if ((fSets[i]).size() != 3) {
			cout << "the fSetsets_Size is not correct" << endl;

		}
		else {
			GLint firstVertexIndex = (fSets[i])[0];//取出顶点索引
			GLint secondVertexIndex = (fSets[i])[1];
			GLint thirdVertexIndex = (fSets[i])[2];

			SV1[0] = (vSets[firstVertexIndex])[0];//第一个顶点
			SV1[1] = (vSets[firstVertexIndex])[1];
			SV1[2] = (vSets[firstVertexIndex])[2];

			SV2[0] = (vSets[secondVertexIndex])[0];//第二个顶点
			SV2[1] = (vSets[secondVertexIndex])[1];
			SV2[2] = (vSets[secondVertexIndex])[2];

			SV3[0] = (vSets[thirdVertexIndex])[0];//第三个顶点
			SV3[1] = (vSets[thirdVertexIndex])[1];
			SV3[2] = (vSets[thirdVertexIndex])[2];

			GLfloat vec1[3], vec2[3], vec3[3];//计算法向量
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

			//glNormal3f(VN[0], VN[1], VN[2]);//绘制法向量
			glVertex3f(SV1[0], SV1[1], SV1[2]);//指定每个顶点的坐标，绘制三角形
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);

		}
		glEnd();
	}




	for (int i = 0; i < fSets1.size(); i++) {
		glBegin(GL_LINE_LOOP);//开始绘制
		GLfloat VN[3];

		//四个顶点
		GLfloat SV1[3];
		GLfloat	SV2[3];
		GLfloat SV3[3];
		GLfloat SV4[3];


		if ((fSets1[i]).size() != 4) {
			cout << "the fSetsets1_Size is not correct" << endl;

		}

		else {

			GLint firstVertexIndex = (fSets1[i])[0];//取出顶点索引
			GLint secondVertexIndex = (fSets1[i])[1];
			GLint thirdVertexIndex = (fSets1[i])[2];
			GLint fourthVertexIndex = (fSets1[i])[3];

			SV1[0] = (vSets[firstVertexIndex])[0];//第一个顶点
			SV1[1] = (vSets[firstVertexIndex])[1];
			SV1[2] = (vSets[firstVertexIndex])[2];

			SV2[0] = (vSets[secondVertexIndex])[0];//第二个顶点
			SV2[1] = (vSets[secondVertexIndex])[1];
			SV2[2] = (vSets[secondVertexIndex])[2];

			SV3[0] = (vSets[thirdVertexIndex])[0];//第三个顶点
			SV3[1] = (vSets[thirdVertexIndex])[1];
			SV3[2] = (vSets[thirdVertexIndex])[2];

			SV4[0] = (vSets[fourthVertexIndex])[0];//第四个顶点
			SV4[1] = (vSets[fourthVertexIndex])[1];
			SV4[2] = (vSets[fourthVertexIndex])[2];

			GLfloat vec1[3], vec2[3], vec3[3];//计算法向量
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

			//glNormal3f(VN[0], VN[1], VN[2]);//绘制法向量
			glVertex3f(SV1[0], SV1[1], SV1[2]);//绘制四边形面片
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);
			glVertex3f(SV4[0], SV4[1], SV4[2]);
		}
		glEnd();
	}



	for (int i = 0; i < fSets2.size(); i++) {
		glBegin(GL_LINE_LOOP);//开始绘制
		GLfloat VN[3];

		//五个顶点
		GLfloat SV1[3];
		GLfloat	SV2[3];
		GLfloat SV3[3];
		GLfloat SV4[3];
		GLfloat SV5[3];


		if ((fSets2[i]).size() != 5) {
			cout << "the fSetsets2_Size is not correct" << endl;

		}

		else {

			GLint firstVertexIndex = (fSets2[i])[0];//取出顶点索引
			GLint secondVertexIndex = (fSets2[i])[1];
			GLint thirdVertexIndex = (fSets2[i])[2];
			GLint fourthVertexIndex = (fSets2[i])[3];
			GLint fifthVertexIndex = (fSets2[i])[4];

			SV1[0] = (vSets[firstVertexIndex])[0];//第一个顶点
			SV1[1] = (vSets[firstVertexIndex])[1];
			SV1[2] = (vSets[firstVertexIndex])[2];

			SV2[0] = (vSets[secondVertexIndex])[0];//第二个顶点
			SV2[1] = (vSets[secondVertexIndex])[1];
			SV2[2] = (vSets[secondVertexIndex])[2];

			SV3[0] = (vSets[thirdVertexIndex])[0];//第三个顶点
			SV3[1] = (vSets[thirdVertexIndex])[1];
			SV3[2] = (vSets[thirdVertexIndex])[2];

			SV4[0] = (vSets[fourthVertexIndex])[0];//第四个顶点
			SV4[1] = (vSets[fourthVertexIndex])[1];
			SV4[2] = (vSets[fourthVertexIndex])[2];

			SV5[0] = (vSets[fifthVertexIndex])[0];//第五个顶点
			SV5[1] = (vSets[fifthVertexIndex])[1];
			SV5[2] = (vSets[fifthVertexIndex])[2];

			GLfloat vec1[3], vec2[3], vec3[3];//计算法向量
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

			//glNormal3f(VN[0], VN[1], VN[2]);//绘制法向量
			glVertex3f(SV1[0], SV1[1], SV1[2]);//绘制多边形面片
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);
			glVertex3f(SV4[0], SV4[1], SV4[2]);
			glVertex3f(SV5[0], SV5[1], SV5[2]);
		}
		glEnd();
	}

}
//画面
void ObjLoader::Draw2() {

	for (int i = 0; i < fSets.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, texModel);
		glBegin(GL_TRIANGLES);//开始绘制三角形
		GLfloat VN[3];

		//三个顶点
		GLfloat SV1[3];
		GLfloat	SV2[3];
		GLfloat SV3[3];

		if ((fSets[i]).size() != 3) {
			cout << "the fSetsets_Size is not correct" << endl;

		}
		else {
			GLint firstVertexIndex = (fSets[i])[0];//取出顶点索引
			GLint secondVertexIndex = (fSets[i])[1];
			GLint thirdVertexIndex = (fSets[i])[2];
			GLint firstVtIndex = (fvtSets[i])[0];//取出纹理索引
			GLint secondVtIndex = (fvtSets[i])[1];
			GLint thirdVtIndex = (fvtSets[i])[2];

			SV1[0] = (vSets[firstVertexIndex])[0];//第一个顶点
			SV1[1] = (vSets[firstVertexIndex])[1];
			SV1[2] = (vSets[firstVertexIndex])[2];

			SV2[0] = (vSets[secondVertexIndex])[0];//第二个顶点
			SV2[1] = (vSets[secondVertexIndex])[1];
			SV2[2] = (vSets[secondVertexIndex])[2];

			SV3[0] = (vSets[thirdVertexIndex])[0];//第三个顶点
			SV3[1] = (vSets[thirdVertexIndex])[1];
			SV3[2] = (vSets[thirdVertexIndex])[2];

			GLfloat vec1[3], vec2[3], vec3[3];//计算法向量
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

			glNormal3f(VN[0], VN[1], VN[2]);//绘制法向量
			glTexCoord2f(vtSets[firstVtIndex][0], vtSets[firstVtIndex][1]);//绘制纹理
			glVertex3f(SV1[0], SV1[1], SV1[2]);//绘制三角面片
			glTexCoord2f(vtSets[secondVtIndex][0], vtSets[secondVtIndex][1]);
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glTexCoord2f(vtSets[thirdVtIndex][0], vtSets[thirdVtIndex][1]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);

		}
		glEnd();
	}

	glBegin(GL_QUADS);//开始绘制四边形
	for (int i = 0; i < fSets1.size(); i++) {

		GLfloat VN[3];

		//四个顶点
		GLfloat SV1[3];
		GLfloat	SV2[3];
		GLfloat SV3[3];
		GLfloat SV4[3];


		if ((fSets1[i]).size() != 4) {
			cout << "the fSetsets1_Size is not correct" << endl;

		}

		else {

			GLint firstVertexIndex = (fSets1[i])[0];//取出顶点索引
			GLint secondVertexIndex = (fSets1[i])[1];
			GLint thirdVertexIndex = (fSets1[i])[2];
			GLint fourthVertexIndex = (fSets1[i])[3];

			SV1[0] = (vSets[firstVertexIndex])[0];//第一个顶点
			SV1[1] = (vSets[firstVertexIndex])[1];
			SV1[2] = (vSets[firstVertexIndex])[2];

			SV2[0] = (vSets[secondVertexIndex])[0];//第二个顶点
			SV2[1] = (vSets[secondVertexIndex])[1];
			SV2[2] = (vSets[secondVertexIndex])[2];

			SV3[0] = (vSets[thirdVertexIndex])[0];//第三个顶点
			SV3[1] = (vSets[thirdVertexIndex])[1];
			SV3[2] = (vSets[thirdVertexIndex])[2];

			SV4[0] = (vSets[fourthVertexIndex])[0];//第四个顶点
			SV4[1] = (vSets[fourthVertexIndex])[1];
			SV4[2] = (vSets[fourthVertexIndex])[2];

			GLfloat vec1[3], vec2[3], vec3[3];//计算法向量
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

			//glNormal3f(VN[0], VN[1], VN[2]);//绘制法向量
			glVertex3f(SV1[0], SV1[1], SV1[2]);//绘制四边形面片
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);
			glVertex3f(SV4[0], SV4[1], SV4[2]);
		}

	}
	glEnd();


	for (int i = 0; i < fSets2.size(); i++) {
		glBegin(GL_POLYGON);//开始绘制多边形，这里是绘制五边形
		GLfloat VN[3];

		//五个顶点
		GLfloat SV1[3];
		GLfloat	SV2[3];
		GLfloat SV3[3];
		GLfloat SV4[3];
		GLfloat SV5[3];


		if ((fSets2[i]).size() != 5) {
			cout << "the fSetsets2_Size is not correct" << endl;

		}

		else {

			GLint firstVertexIndex = (fSets2[i])[0];//取出顶点索引
			GLint secondVertexIndex = (fSets2[i])[1];
			GLint thirdVertexIndex = (fSets2[i])[2];
			GLint fourthVertexIndex = (fSets2[i])[3];
			GLint fifthVertexIndex = (fSets2[i])[4];

			SV1[0] = (vSets[firstVertexIndex])[0];//第一个顶点
			SV1[1] = (vSets[firstVertexIndex])[1];
			SV1[2] = (vSets[firstVertexIndex])[2];

			SV2[0] = (vSets[secondVertexIndex])[0];//第二个顶点
			SV2[1] = (vSets[secondVertexIndex])[1];
			SV2[2] = (vSets[secondVertexIndex])[2];

			SV3[0] = (vSets[thirdVertexIndex])[0];//第三个顶点
			SV3[1] = (vSets[thirdVertexIndex])[1];
			SV3[2] = (vSets[thirdVertexIndex])[2];

			SV4[0] = (vSets[fourthVertexIndex])[0];//第四个顶点
			SV4[1] = (vSets[fourthVertexIndex])[1];
			SV4[2] = (vSets[fourthVertexIndex])[2];

			SV5[0] = (vSets[fifthVertexIndex])[0];//第五个顶点
			SV5[1] = (vSets[fifthVertexIndex])[1];
			SV5[2] = (vSets[fifthVertexIndex])[2];

			GLfloat vec1[3], vec2[3], vec3[3];//计算法向量
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

			//glNormal3f(VN[0], VN[1], VN[2]);//绘制法向量
			glVertex3f(SV1[0], SV1[1], SV1[2]);//绘制多边形面片
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);
			glVertex3f(SV4[0], SV4[1], SV4[2]);
			glVertex3f(SV5[0], SV5[1], SV5[2]);
		}
		glEnd();
	}
}
//画四面体网格
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

		glBegin(GL_LINE_LOOP);//开始绘制
		glColor4ub(0, 0, 0, 255);//设置线条的颜色
		glVertex3f(point1x, point1y, point1z);//指定线条应该通过的点。
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point2x, point2y, point2z);
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point3x, point3y, point3z);
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point1x, point1y, point1z);//绘制三角形
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point2x, point2y, point2z);
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point4x, point4y, point4z);
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point1x, point1y, point1z);//绘制三角形
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point3x, point3y, point3z);
		glColor4ub(0, 0, 0, 255);
		glVertex3f(point4x, point4y, point4z);
		glColor4ub(0, 0, 0, 255);

		glVertex3f(point2x, point2y, point2z);//绘制三角形
		glColor4ub(255, 0, 0, 255);
		glVertex3f(point3x, point3y, point3z);
		glColor4ub(255, 0, 0, 255);
		glVertex3f(point4x, point4y, point4z);
		glEnd();

	}

}
