#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <vector>
#include <string>
#include "BoxNet.h"
#include "Tetrah.h"
using namespace std;

class ObjLoader {

public:

	ObjLoader(string filename);//读取obj文件
	//构造函数
	void TxtLoader(string filename);//用于通过txt读取变化后的四面体网格
	//void Init();
	void Draw1();//绘制函数用于绘画盒子线框
	void Draw2();//绘制模型
	void DrawNet();//绘制四面体网格（四面体网格即控制网格）
	void TetrahSet();//获取变化前的四面体网格
	void ChangeTetrahSet();//修改四面体网格的点的坐标
	//对于控制网格的约束
	void StretchConstraint();//拉伸约束
	void StretchConstraint1(int a,int b, float k, vector<vector<GLfloat>>& vSets_old);//拉伸约束改
	void BoundingSphereConstraint(vector<vector<GLfloat>>& vSets_old, int a, float r);//包围球约束
	void VolumeConstraint(int a, int b, int c, int d, vector<vector<GLfloat>>& vSets_old);//四面体体积约束，传入四面体顶点索引
	void VolumeConstraintBypbd(int a, int b, int c, int d, vector<vector<GLfloat>>& vSets_old);//基于位置的动力学四面体体积约束，传入四面体顶点索引
	void VolumeConstrainttest( float vtSetstest[4][3], float vttest_new[4][3]);
	void StretchConstraintTest(int a,  float k, float vtSetstest[4][3], float vttest_new[4][3]);
	//实现模型点随四面体网格的移动而移动
	void computerKinTerachbySearch(vector<CPoint3D>& VSets, vector<Tetrah>& TetrahSet_initial, vector<Tetrah>& TetrahSet_aftermove, vector<CPoint3D>& vSet_new);
	void ComputerSkeletonPointRestriction(vector<CPoint3D>& VSet, vector<Tetrah>& TetrahSet_initial, vector<Tetrah>& TetrahSet_aftermove, vector<CPoint3D>& vSet_new, float k, float centers[1][3]);
	void ChangeType();//用于将float类型的点改为CPoint3D类型
	void ChangePoint(CPoint3D& VSets);//用于将点坐标xyz改为xzy
	//对于模型的约束
	void ModelConstraint(vector<vector<GLfloat>>& vSets, vector<vector<GLfloat>>& vSets_old, int range[], int length, float elasticity);//模型约束
	


	GLuint load_texture(const char* file_name);//读取bmp文件作为纹理

	GLuint texModel;//定义纹理对象编号
	vector<Tetrah> TetrahSet_initial;//存储变化前的四面体网格
	vector<Tetrah> TetrahSet_aftermove;//存储变化后的四面体网格
	vector<CPoint3D> vSetsChangeType;//存储变为CPoint3D类型的点
	vector<CPoint3D> vSet_new;//中间变量用于存放盒子变化后的单个点
	CPoint3D PointChange;//中间变量用于存储单个CPoint3D类型的点
	float vtSetstest[4][3] = {
	{0, 0, 0},  // 点 A 的坐标 vtSetstest[0][0]、vtSetstest[0][1]和vtSetstest[0][2]
	{0, 1, 0},  // 点 B 的坐标
	{1, 0, 0},  // 点 C 的坐标
	{1, 1, 1}   // 点 D 的坐标
	};
	float vttest_new[4][3] = {
	{0, 0, 0},  // 点 A 的坐标 vttest_new[0][0]、vttest_new[0][1]和vttest_new[0][2]
	{0, 2, 0},  // 点 B 的坐标
	{2, 0, 0},  // 点 C 的坐标
	{1, 1, 1}   // 点 D 的坐标
	};
	float centers[1][3] = {
		{0.2494, 0.0414,1.9663,},
	};
//private:
public:
	//GLint lastFrameTime;
	//GLint animationLastingTime;

	vector<Tetrah> T_boxNet;//存放盒子的点
	vector<vector<GLfloat>>vSets;//存放包围网格顶点（x,z,y）坐标
	vector<vector<GLfloat>>vSets_old;//存放包围网格变化前的顶点（x,z,y）坐标
	vector<vector<GLfloat>>vSets_oldtest;
	vector<vector<GLfloat>>vBoxSets_new;//存放变化后的盒子点
	vector<int>tetrahWithV[195];//存放178个四面体内的模型点索引
	vector<vector<GLint>>fSets;//存放由三个顶点组成面的索引
	vector<vector<GLint>>fvtSets;//存放由三个顶点组成面的纹理坐标索引
	vector<vector<GLint>>fSets1;//存放由四个顶点组成面的索引
	vector<vector<GLint>>fSets2;//存放由五个顶点组成面的索引
	//vector<vector<GLfloat>>vnSets;
	vector<vector<GLfloat>>vtSets;//存放纹理坐标

	int range[115] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,
		15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,
		35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,
		55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,
		75,76,77,78,                                                  //上躯干   不包括双臂

		141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,
		161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176                          //下躯干
	};
	int length = sizeof(range) / sizeof(range[0]);



};

