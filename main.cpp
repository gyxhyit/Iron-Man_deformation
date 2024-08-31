#include "ObjLoader.h"
#include <iostream> 
#include <vector>
#include <math.h>
#include "dual_quat_cu.hpp"
#define PI 3.14159

using namespace Tbx;
using namespace std;
//ģ��·��    box1+moxing+boxlist box8+moxing1+boxlist1  armbox+moxing1+armboxlist box13+moxing1+boxlist1   controlofbasemodel1+basemodel1+boxlist1
string filePath1 = "C:/Users/reckless/Desktop/3Dģ�ͻ���/box1.25(2).obj";
ObjLoader objModel1 = ObjLoader(filePath1);
string filePath2 = "C:/Users/reckless/Desktop/3Dģ�ͻ���/moxing1.obj";
ObjLoader objModel2 = ObjLoader(filePath2);

//ʵ���ƶ����۲�ģ���������
static float c = 3.1415926 / 180.0f;
static float r = 1.5f, h = 0.0;
static int degree = 90;
static int oldPosY = -1;
static int oldPosX = -1;
//���ù�Դ
void setLightRes() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//������
	glPolygonMode(GL_FRONT, GL_FILL); // ��������Ϊ���ģʽ 
	glPolygonMode(GL_BACK, GL_LINE);  // ���÷���Ϊ����ģʽ  GL_LINE
	glFrontFace(GL_CCW);              // ������ʱ�뷽��Ϊ����
	glEnable(GL_CULL_FACE);		//����ָ���޳�ָ����,ͨ���޳����������Ⱦ����
	glShadeModel(GL_SMOOTH);    // ����ƽ��

	glEnable(GL_LIGHTING);//���õ�Դ

	GLfloat light_position[] = { 0.0f, 0.5f, 0.5f, 1.0f };//���һ������Ϊ0��˵���Ƿ����Թ�Դ����0��Ϊλ���Թ�Դ
	GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 0.3f }; //������  ����ǿ��
	GLfloat light_diffuse[] = { 0.7f, 0.7f, 0.7f, 0.7f }; //�������
	GLfloat light_specular[] = { 0.2f, 0.2f, 0.2f, 0.2f };//���淴���

	//GLfloat mat_shininess[] = { 50.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//glLightfv���Լ����Ϊ����Դ��ֵ
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);//����ȫ�ֻ�����+������RGBAǿ�� 
	//


	glEnable(GL_LIGHT0); //����GL_LIGHT0

	glEnable(GL_DEPTH_TEST | GL_LINE_SMOOTH);//������Ȳ���|�����߶εľ��

	//glEnable(GL_COLOR_MATERIAL);//ͼ�Σ����ϣ������ݹ��ߵ���ҫ���з��䡣����Ҫ���ɺ��� glColorMaterial�����趨��


	//glMateriali(GL_FRONT, GL_SHININESS, 60);

	GLfloat mat_ambient[] = { 1.f, 1.f, 1.f, 1.0f };
	GLfloat mat_diffuse[] = { 1.f, 1.f, 1.f, 1.0f };
	GLfloat mat_specular[] = { 1.f, 1.f, 1.f, 1.0f };
	GLfloat mat_shininess[] = { 60 };//����RGBA����ָ������ֵ��0��128��Χ��
	//���ò���
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);//���ʵĻ�����ɫ
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);//���ʵľ��淴����ɫ
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess); //���淴��ָ��
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);//���ʵ�ɢ����ɫ
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_COLOR_MATERIAL);//ͼ�Σ����ϣ������ݹ��ߵ���ҫ���з��䡣����Ҫ���ɺ��� glColorMaterial�����趨��
	//GLfloat position[] = { 0.0f,30.0f,30.0f,1.0f };
	//GLfloat ambient[] = { 0.0f,0.0f,0.0f,1.0f };
	//GLfloat diffuse[] = { 0.0f, 0.8f, 0.8f, 1.0f };
	//GLfloat specular[] = { 0.0f, 0.6f, 0.6f, 1.0f };
	//GLfloat global_ambient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	//glLightfv(GL_POSITION, GL_POSITION, position);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);

}
//��ʼ��
void init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("ObjLoader");
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);//ָ��ʹ�õ���ɫ���� ����������ɫ�ϵĹ���
	setLightRes();
	glEnable(GL_DEPTH_TEST);// ����������Ȼ������Ĺ���,��ֻ�����Ǹ�����ǰ��û�ж���ʱ���Ż�滭�������
	//objModel.Init();
}


void display()
{
	glColor3f(1.0, 1.0, 1.0);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);//ȷ����ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//����ɫ���ø�����
	glMatrixMode(GL_MODELVIEW);//���õ�ǰ����ģʽ ����ģ���Ӿ������ջӦ�����ľ�������� ��glLoadIdentityһͬʹ��
	glLoadIdentity();// ���õ�ǰָ���ľ���Ϊ��λ����
	glTranslatef(-0.5f, -1.8f, -3.0f);//���������ƽ��
	//setLightRes();
	glPushMatrix();
	gluLookAt(r * cos(c * degree), h, r * sin(c * degree), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);//�ӵ�ת��
	//objModel1.Draw1();//���Ƴ�ʼ��������
	//objModel2.Draw1();//����ģ���߿�
	objModel2.Draw2();//����ģ��ʵ��
	objModel1.DrawNet();//���Ʊ仯��Ŀ�������
	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	//glViewport(0, 0, width, height);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 200.0f);//fovy���۾����������ķ��ȣ��Ƕ�ֵ��ֵԽС����Ұ��ΧԽ��С
	//																		//aspect��ʾ�ü���Ŀ�w��h�ȣ����Ӱ�쵽��Ұ�Ľ����ж��
	//												//zNear��ʾ���ü��浽�۾��ľ��룬zFar��ʾԶ�ü��浽�۾��ľ��룬ע��zNear��zFar������������Ϊ��ֵ
	//												
	//glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);    //��ͼ;1��2Ϊ�ӿڵ����½�;3��4Ϊ�ӿڵĿ�Ⱥ͸߶�
	glMatrixMode(GL_PROJECTION);    //����ǰ����ָ��ΪͶӰ����
	glLoadIdentity();
	gluPerspective(75.0, (float)w / h, 1.0, 1000.0); //1����Ұ��Y-Zƽ��ĽǶ�[0,180];2��ͶӰƽ������߶ȵı���;3�����ؼ��浽�ӵ�ľ���;4��Զ�ؼ��浽�ӵ�ľ���
	glMatrixMode(GL_MODELVIEW);     //��ģ���Ӿ������ջӦ�����ľ������.
}
//�ƶ����360�ȹ۲�ģ��
void moseMove(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		oldPosX = x; oldPosY = y;
	}
}

void changeViewPoint(int x, int y)
{
	//int temp = x - oldPosX;
	//degree += temp;
	//oldPosX = x;
	//oldPosY = y;
	int temp = x - oldPosX; //����ڴ���x�᷽���ϵ������ӵ��ӵ���x��ļн��ϣ��Ϳ�������ת
	degree += temp;
	h += 0.03 * (y - oldPosY);  //����ڴ���y�᷽���ϵĸı�ӵ��ӵ�y�������ϣ��Ϳ�������ת
	if (h > 1.0)   h = 1.0;  //���ӵ�y������һЩ���ƣ�����ʹ�ӵ�̫���
	else if (h < -1.0) h = -1.0;
	oldPosX = x, oldPosY = y;  //����ʱ��������Ϊ��ֵ��Ϊ��һ�μ���������׼��
}
float CalculateDistance(const CPoint3D& point1, const CPoint3D& point2) {
	return sqrt(pow(point1.X() - point2.X(), 2) + pow(point1.Y() - point2.Y(), 2) + pow(point1.Z() - point2.Z(), 2));
}

void myIdle()
{
	glutPostRedisplay();//�ƶ���������ƵĻ���ı���
}

//void dual_quat_deformer(const std::vector<Point3>& in_verts,//����Ķ���
//	const std::vector<Vec3>& in_normals, //�����ߵ�����
//	std::vector<Vec3>& out_verts,//ʹ��˫��Ԫ�����κ�Ķ���
//	std::vector<Vec3>& out_normals,//ʹ��˫��Ԫ�����κ�ķ���
//	const std::vector<Dual_quat_cu>& dual_quat,//ÿ���ؽڵ�˫��Ԫ���任
//	const std::vector< std::vector<float> >& weights,//ÿ�������Ӱ��Ȩ��
//	const std::vector< std::vector<int> >& joints_id)//�ؽ�ID
//{
//	for (unsigned v = 0; v < in_verts.size(); ++v)//ѭ���������붥������ in_verts �е�ÿһ�����㡣
//	{
//		const int nb_joints = weights[v].size(); // Number of joints influencing vertex p  ���Ȼ�ȡ�˵�ǰ������Ӱ��Ĺؽ����� nb_joints
//
//		// Init dual quaternion with first joint transformation      ʹ�õ�һ���ؽڱ任��ʼ��˫��Ԫ��
//		int   k0 = -1;    //�����˼������������ڴ洢˫��Ԫ���Ļ�Ͻ���Լ�������ʱ������
//		float w0 = 0.f;
//		Dual_quat_cu dq_blend;   //�洢��Ͻ��
//		Quat_cu q0;   //�洢��һ���ؽ���ת
//
//		if (nb_joints != 0)  //��鵱ǰ�����Ƿ��ܵ��κιؽڵ�Ӱ�졣����ܵ�Ӱ�죬����һ���ؽڵ�������Ȩ�ش洢�ڱ��� k0 �� w0 �У�����˫��Ԫ����Ͻ������Ϊ��λ˫��Ԫ����
//		{
//			k0 = joints_id[v][0];
//			w0 = weights[v][0];
//		}
//		else
//			dq_blend = Dual_quat_cu::identity();
//
//		if (k0 != -1) dq_blend = dual_quat[k0] * w0;   //�����һ���ؽ�������Ч����ʹ�����Ӧ��˫��Ԫ����Ȩ�ؽ��л�ϡ�
//
//		//�ⲿ�ִ����Ǵ��������ؽڶԵ�ǰ�����Ӱ�졣�������˳��˵�һ���ؽ�֮��������ؽڣ���ÿ���ؽڽ�����Ӧ��˫��Ԫ����ϣ���������յ�˫��Ԫ����Ͻ����
//
//		int pivot = k0;   //�洢�˵�һ���ؽ�����
//
//		q0 = dual_quat[pivot].rotation();   //����һ���ؽڵ���ת���ִ洢�� q0 ������
//
//		// Look up the other joints influencing 'p' if any          
//		for (int j = 1; j < nb_joints; j++)  //��������һ���ؽ�֮��������ؽڣ���ÿ���ؽڽ�����Ӧ��˫��Ԫ����ϡ�
//		{
//			const int k = joints_id[v][j];
//			float w = weights[v][j];
//			const Dual_quat_cu& dq = (k == -1) ? Dual_quat_cu::identity() : dual_quat[k];//��� k ��ֵ���� -1�����ʾ���� v �����κιؽڵ�Ӱ�죬��û����ת��ƽ�Ƶı任��
//			//��� k ��ֵ������ -1���� dq ����Ϊ dual_quat[k]�������� v �ܵ��ؽ� k ��˫��Ԫ���任��Ӱ�졣
//			if (dq.rotation().dot(q0) < 0.f)   //ȷ����ؽ���תʱ�����һ���ؽ���ת���򱣳�һ��
//				w *= -1.f;
//
//			dq_blend = dq_blend + dq * w;
//		}
//
//		//�����ż��Ԫ�����κ�Ķ���λ�úͷ��ߣ���������洢��������� out_verts �� out_normals �С�
//		// Compute animated position
//		Vec3 vi = dq_blend.transform(in_verts[v]).to_vec3();
//		out_verts[v] = vi;
//		// Compute animated normal
//		out_normals[v] = dq_blend.rotate(in_normals[v]);
//	}
//}

// �� Tbx::Point3 ת��Ϊ CPoint3D ��ת������
CPoint3D ConvertToPoint3D(const Tbx::Point3& tbxPoint) {
	
	// ����һ�� CPoint3D ����
	return CPoint3D(tbxPoint.x, tbxPoint.y, tbxPoint.z);
}


int main(int argc, char* argv[])
{

	objModel2.computerKinTerachbySearch(objModel2.vSetsChangeType, objModel1.TetrahSet_initial, objModel1.TetrahSet_aftermove, objModel2.vSet_new);
	//objModel2.ComputerSkeletonPointRestriction(objModel2.vSetsChangeType, objModel1.TetrahSet_initial, objModel1.TetrahSet_aftermove, objModel2.vSet_new,80, objModel2.centers);

	std::vector<CPoint3D> VSet(150);

	//std::vector<Point3> in_verts;                     //����
	//for (const auto& vertex : objModel2.vSet_new) {
	//	Point3 point(vertex.x, vertex.y, vertex.z);
	//	in_verts.push_back(point);
	//}
	std::vector<Point3> in_verts = {
	{-0.560,2.372,0.369},  //132
	{-0.187,2.228,0.67},  //128
	{-0.092,2.067,0.608},  //36
	{-0.414,2.407,0.115},   //34
	{-0.463,1.785,-0.126},  //8
	{-0.050,1.556,-0.153},  //12
	{-0.266,1.552,0.320},   //130
	{-0.652,1.839,0.212},   //134
	//  ��Ӹ��ඥ�� ...  
	};

	std::vector<Vec3> in_normals = {               //��in_verts˳����ͬ         ������ͬ ȫ���ó�0
		{1.0f, 2.0f, 3.0f},
		{2.0f, 2.0f, 2.0f},
		{3.0f, 3.0f, 3.0f},
		{2.0f, 2.0f, 2.0f},
		{2.0f, 2.0f, 2.0f},
		{2.0f, 2.0f, 2.0f},
		{2.0f, 2.0f, 2.0f},
		{2.0f, 2.0f, 2.0f},
		//  ��Ӹ��෨�� ...  
	};

	// ����������3���ؽڣ�ÿ���ؽ���һ��˫��Ԫ���任   ֱ��ָ��������Ԫ��      �������Բ���ͬ
	std::vector<Dual_quat_cu> dual_quat = {
		Tbx::Dual_quat_cu(Quat_cu(0, 0.006, 3.246, -0.013), Quat_cu(0,0, 0, 0)),          // ����
		Tbx::Dual_quat_cu(Quat_cu(0, 0.008, 2.342, 0.062), Quat_cu(0, 0, 0, 0.0)),        //����
		Tbx::Dual_quat_cu(Quat_cu(0 ,0.008, 2.118, 0.0468), Quat_cu(0, 0, 0, 0)),         //��׵��
		Tbx::Dual_quat_cu(Quat_cu(0, -0.2999, 1.9663, 0.0414), Quat_cu(0, 0, 0, 0.0)),   //������
		Tbx::Dual_quat_cu(Quat_cu(0 ,-0.2652, 1.1228, 0.0058), Quat_cu(0, 0, 0, 0)),      //������
		Tbx::Dual_quat_cu(Quat_cu(0 ,-0.0347, 0.8435, 0.0356), Quat_cu(0, 0, 0, 0)),      //9-11  ת��

	};//

	// ��ͬ��Ȩ��                 ������ͬ
	std::vector<std::vector<float>> weights = {
	{1},  // ��һ���ؽڵ�Ȩ��
	{1},  // �ڶ����ؽڵ�Ȩ��
	{1}, // �������ؽڵ�Ȩ��
	{1},
	{1},
	{1},
	{1},
	{1},


	// ... Ϊÿ���ؽ����Ȩ�� ...  
	};

	// ����ÿ��������Ӱ��Ĺؽ�ID�ֱ���0, 1, 2                 ������ͬ
	std::vector<std::vector<int>> joints_id = {            //˳����weight��ͬ
		{5}, // ��һ������Ĺؽ�ID  
		{5},// �ڶ�������Ĺؽ�ID    
		{5},// ����������Ĺؽ�ID    
		{5},
		{5},
		{5},
		{5},
		{5},

		// ... Ϊÿ��������ӹؽ�ID ...  
	};

	//// ��ʼ���������ͷ��ߵ���������С������һ��  
	std::vector<Vec3> out_verts(in_verts.size());
	std::vector<Vec3> out_normals(in_normals.size());

	//���� dual_quat_deformer ���������������  
	//dual_quat_deformer(in_verts, in_normals, out_verts, out_normals, dual_quat, weights, joints_id);

	// �滻��128�ŵ�
	VSet[128] = ConvertToPoint3D(in_verts[0]);

	// �滻��36�ŵ�
	VSet[36] = ConvertToPoint3D(in_verts[1]);

	// �滻��132�ŵ�
	VSet[132] = ConvertToPoint3D(in_verts[2]);

	//objModel2.computerKinTerachbySearch(VSet, objModel1.TetrahSet_initial, objModel1.TetrahSet_aftermove, objModel2.vSet_new);



	//// ������  
	//std::cout << "Deformation completed!" << std::endl;

	//// �鿴���κ�Ķ������
	//for (const auto& vert : out_verts) {
	//	std::cout << "Deformed vertex: " << vert << std::endl;
	//}
	///*for (const auto& norm : out_normals) {
	//	std::cout << "Deformed normal: " << norm << std::endl;
	//}*/




	glutInit(&argc, argv);
	init();
	glEnable(GL_TEXTURE_2D);    // ��������
	objModel2.texModel = objModel2.load_texture("C:/Users/reckless/Desktop/3Dģ�ͻ���/ironManPicture.bmp");  //��������
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(moseMove);
	glutMotionFunc(changeViewPoint);
	glutIdleFunc(myIdle);
	glutMainLoop();//ѭ������
	return 0;
}

