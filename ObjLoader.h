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

	ObjLoader(string filename);//��ȡobj�ļ�
	//���캯��
	void TxtLoader(string filename);//����ͨ��txt��ȡ�仯�������������
	//void Init();
	void Draw1();//���ƺ������ڻ滭�����߿�
	void Draw2();//����ģ��
	void DrawNet();//�����������������������񼴿�������
	void TetrahSet();//��ȡ�仯ǰ������������
	void ChangeTetrahSet();//�޸�����������ĵ������
	//���ڿ��������Լ��
	void StretchConstraint();//����Լ��
	void StretchConstraint1(int a,int b, float k, vector<vector<GLfloat>>& vSets_old);//����Լ����
	void BoundingSphereConstraint(vector<vector<GLfloat>>& vSets_old, int a, float r);//��Χ��Լ��
	void VolumeConstraint(int a, int b, int c, int d, vector<vector<GLfloat>>& vSets_old);//���������Լ�������������嶥������
	void VolumeConstraintBypbd(int a, int b, int c, int d, vector<vector<GLfloat>>& vSets_old);//����λ�õĶ���ѧ���������Լ�������������嶥������
	void VolumeConstrainttest( float vtSetstest[4][3], float vttest_new[4][3]);
	void StretchConstraintTest(int a,  float k, float vtSetstest[4][3], float vttest_new[4][3]);
	//ʵ��ģ�͵���������������ƶ����ƶ�
	void computerKinTerachbySearch(vector<CPoint3D>& VSets, vector<Tetrah>& TetrahSet_initial, vector<Tetrah>& TetrahSet_aftermove, vector<CPoint3D>& vSet_new);
	void ComputerSkeletonPointRestriction(vector<CPoint3D>& VSet, vector<Tetrah>& TetrahSet_initial, vector<Tetrah>& TetrahSet_aftermove, vector<CPoint3D>& vSet_new, float k, float centers[1][3]);
	void ChangeType();//���ڽ�float���͵ĵ��ΪCPoint3D����
	void ChangePoint(CPoint3D& VSets);//���ڽ�������xyz��Ϊxzy
	//����ģ�͵�Լ��
	void ModelConstraint(vector<vector<GLfloat>>& vSets, vector<vector<GLfloat>>& vSets_old, int range[], int length, float elasticity);//ģ��Լ��
	


	GLuint load_texture(const char* file_name);//��ȡbmp�ļ���Ϊ����

	GLuint texModel;//�������������
	vector<Tetrah> TetrahSet_initial;//�洢�仯ǰ������������
	vector<Tetrah> TetrahSet_aftermove;//�洢�仯�������������
	vector<CPoint3D> vSetsChangeType;//�洢��ΪCPoint3D���͵ĵ�
	vector<CPoint3D> vSet_new;//�м�������ڴ�ź��ӱ仯��ĵ�����
	CPoint3D PointChange;//�м�������ڴ洢����CPoint3D���͵ĵ�
	float vtSetstest[4][3] = {
	{0, 0, 0},  // �� A ������ vtSetstest[0][0]��vtSetstest[0][1]��vtSetstest[0][2]
	{0, 1, 0},  // �� B ������
	{1, 0, 0},  // �� C ������
	{1, 1, 1}   // �� D ������
	};
	float vttest_new[4][3] = {
	{0, 0, 0},  // �� A ������ vttest_new[0][0]��vttest_new[0][1]��vttest_new[0][2]
	{0, 2, 0},  // �� B ������
	{2, 0, 0},  // �� C ������
	{1, 1, 1}   // �� D ������
	};
	float centers[1][3] = {
		{0.2494, 0.0414,1.9663,},
	};
//private:
public:
	//GLint lastFrameTime;
	//GLint animationLastingTime;

	vector<Tetrah> T_boxNet;//��ź��ӵĵ�
	vector<vector<GLfloat>>vSets;//��Ű�Χ���񶥵㣨x,z,y������
	vector<vector<GLfloat>>vSets_old;//��Ű�Χ����仯ǰ�Ķ��㣨x,z,y������
	vector<vector<GLfloat>>vSets_oldtest;
	vector<vector<GLfloat>>vBoxSets_new;//��ű仯��ĺ��ӵ�
	vector<int>tetrahWithV[195];//���178���������ڵ�ģ�͵�����
	vector<vector<GLint>>fSets;//�����������������������
	vector<vector<GLint>>fvtSets;//�������������������������������
	vector<vector<GLint>>fSets1;//������ĸ���������������
	vector<vector<GLint>>fSets2;//����������������������
	//vector<vector<GLfloat>>vnSets;
	vector<vector<GLfloat>>vtSets;//�����������

	int range[115] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,
		15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,
		35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,
		55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,
		75,76,77,78,                                                  //������   ������˫��

		141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,
		161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176                          //������
	};
	int length = sizeof(range) / sizeof(range[0]);



};

