#include "ObjLoader.h"
#include <iostream> 
#include <vector>
#include <math.h>
#include "dual_quat_cu.hpp"
#define PI 3.14159

using namespace Tbx;
using namespace std;
//模型路径    box1+moxing+boxlist box8+moxing1+boxlist1  armbox+moxing1+armboxlist box13+moxing1+boxlist1   controlofbasemodel1+basemodel1+boxlist1
string filePath1 = "C:/Users/reckless/Desktop/3D模型绘制/box1.25(2).obj";
ObjLoader objModel1 = ObjLoader(filePath1);
string filePath2 = "C:/Users/reckless/Desktop/3D模型绘制/moxing1.obj";
ObjLoader objModel2 = ObjLoader(filePath2);

//实现移动鼠标观察模型所需变量
static float c = 3.1415926 / 180.0f;
static float r = 1.5f, h = 0.0;
static int degree = 90;
static int oldPosY = -1;
static int oldPosX = -1;
//设置光源
void setLightRes() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//设置面
	glPolygonMode(GL_FRONT, GL_FILL); // 设置正面为填充模式 
	glPolygonMode(GL_BACK, GL_LINE);  // 设置反面为线形模式  GL_LINE
	glFrontFace(GL_CCW);              // 设置逆时针方向为正面
	glEnable(GL_CULL_FACE);		//用来指定剔除指定面,通过剔除面来提高渲染性能
	glShadeModel(GL_SMOOTH);    // 光照平滑

	glEnable(GL_LIGHTING);//启用灯源

	GLfloat light_position[] = { 0.0f, 0.5f, 0.5f, 1.0f };//最后一个参数为0，说明是方向性光源，非0则为位置性光源
	GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 0.3f }; //环境光  光照强度
	GLfloat light_diffuse[] = { 0.7f, 0.7f, 0.7f, 0.7f }; //漫反射光
	GLfloat light_specular[] = { 0.2f, 0.2f, 0.2f, 0.2f };//镜面反射光

	//GLfloat mat_shininess[] = { 50.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//glLightfv可以简单理解为给光源赋值
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);//设置全局环境光+环境光RGBA强度 
	//


	glEnable(GL_LIGHT0); //启用GL_LIGHT0

	glEnable(GL_DEPTH_TEST | GL_LINE_SMOOTH);//启用深度测试|过滤线段的锯齿

	//glEnable(GL_COLOR_MATERIAL);//图形（材料）将根据光线的照耀进行反射。反射要求由函数 glColorMaterial进行设定。


	//glMateriali(GL_FRONT, GL_SHININESS, 60);

	GLfloat mat_ambient[] = { 1.f, 1.f, 1.f, 1.0f };
	GLfloat mat_diffuse[] = { 1.f, 1.f, 1.f, 1.0f };
	GLfloat mat_specular[] = { 1.f, 1.f, 1.f, 1.0f };
	GLfloat mat_shininess[] = { 60 };//材质RGBA镜面指数，数值在0～128范围内
	//设置材质
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);//材质的环境颜色
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);//材质的镜面反射颜色
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess); //镜面反射指数
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);//材质的散射颜色
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_COLOR_MATERIAL);//图形（材料）将根据光线的照耀进行反射。反射要求由函数 glColorMaterial进行设定。
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
//初始化
void init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("ObjLoader");
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);//指明使用的着色技术 即两点在颜色上的过渡
	setLightRes();
	glEnable(GL_DEPTH_TEST);// 开启更新深度缓冲区的功能,它只会再那个像素前方没有东西时，才会绘画这个像素
	//objModel.Init();
}


void display()
{
	glColor3f(1.0, 1.0, 1.0);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);//确定颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//将颜色设置给背景
	glMatrixMode(GL_MODELVIEW);//设置当前矩阵模式 （对模型视景矩阵堆栈应用随后的矩阵操作） 与glLoadIdentity一同使用
	glLoadIdentity();// 重置当前指定的矩阵为单位矩阵
	glTranslatef(-0.5f, -1.8f, -3.0f);//沿着轴进行平移
	//setLightRes();
	glPushMatrix();
	gluLookAt(r * cos(c * degree), h, r * sin(c * degree), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);//视点转换
	//objModel1.Draw1();//绘制初始控制网格
	//objModel2.Draw1();//绘制模型线框
	objModel2.Draw2();//绘制模型实体
	objModel1.DrawNet();//绘制变化后的控制网格
	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	//glViewport(0, 0, width, height);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 200.0f);//fovy是眼睛上下睁开的幅度，角度值，值越小，视野范围越狭小
	//																		//aspect表示裁剪面的宽w高h比，这个影响到视野的截面有多大
	//												//zNear表示近裁剪面到眼睛的距离，zFar表示远裁剪面到眼睛的距离，注意zNear和zFar不能设置设置为负值
	//												
	//glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);    //截图;1、2为视口的左下角;3、4为视口的宽度和高度
	glMatrixMode(GL_PROJECTION);    //将当前矩阵指定为投影矩阵
	glLoadIdentity();
	gluPerspective(75.0, (float)w / h, 1.0, 1000.0); //1、视野在Y-Z平面的角度[0,180];2、投影平面宽度与高度的比率;3、近截剪面到视点的距离;4、远截剪面到视点的距离
	glMatrixMode(GL_MODELVIEW);     //对模型视景矩阵堆栈应用随后的矩阵操作.
}
//移动鼠标360度观察模型
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
	int temp = x - oldPosX; //鼠标在窗口x轴方向上的增量加到视点与x轴的夹角上，就可以左右转
	degree += temp;
	h += 0.03 * (y - oldPosY);  //鼠标在窗口y轴方向上的改变加到视点y的坐标上，就可以上下转
	if (h > 1.0)   h = 1.0;  //对视点y坐标作一些限制，不会使视点太奇怪
	else if (h < -1.0) h = -1.0;
	oldPosX = x, oldPosY = y;  //将此时的坐标作为旧值，为下一次计算增量做准备
}
float CalculateDistance(const CPoint3D& point1, const CPoint3D& point2) {
	return sqrt(pow(point1.X() - point2.X(), 2) + pow(point1.Y() - point2.Y(), 2) + pow(point1.Z() - point2.Z(), 2));
}

void myIdle()
{
	glutPostRedisplay();//移动了物体绘制的画面改变了
}

//void dual_quat_deformer(const std::vector<Point3>& in_verts,//网格的顶点
//	const std::vector<Vec3>& in_normals, //网格法线的向量
//	std::vector<Vec3>& out_verts,//使用双四元数变形后的顶点
//	std::vector<Vec3>& out_normals,//使用双四元数变形后的法线
//	const std::vector<Dual_quat_cu>& dual_quat,//每个关节的双四元数变换
//	const std::vector< std::vector<float> >& weights,//每个顶点的影响权重
//	const std::vector< std::vector<int> >& joints_id)//关节ID
//{
//	for (unsigned v = 0; v < in_verts.size(); ++v)//循环遍历输入顶点向量 in_verts 中的每一个顶点。
//	{
//		const int nb_joints = weights[v].size(); // Number of joints influencing vertex p  首先获取了当前顶点受影响的关节数量 nb_joints
//
//		// Init dual quaternion with first joint transformation      使用第一个关节变换初始化双四元数
//		int   k0 = -1;    //声明了几个变量，用于存储双四元数的混合结果以及其他临时变量。
//		float w0 = 0.f;
//		Dual_quat_cu dq_blend;   //存储混合结果
//		Quat_cu q0;   //存储第一个关节旋转
//
//		if (nb_joints != 0)  //检查当前顶点是否受到任何关节的影响。如果受到影响，将第一个关节的索引和权重存储在变量 k0 和 w0 中，否则将双四元数混合结果设置为单位双四元数。
//		{
//			k0 = joints_id[v][0];
//			w0 = weights[v][0];
//		}
//		else
//			dq_blend = Dual_quat_cu::identity();
//
//		if (k0 != -1) dq_blend = dual_quat[k0] * w0;   //如果第一个关节索引有效，则使用其对应的双四元数与权重进行混合。
//
//		//这部分代码是处理其他关节对当前顶点的影响。它遍历了除了第一个关节之外的其他关节，对每个关节进行相应的双四元数混合，计算出最终的双四元数混合结果。
//
//		int pivot = k0;   //存储了第一个关节索引
//
//		q0 = dual_quat[pivot].rotation();   //将第一个关节的旋转部分存储在 q0 变量中
//
//		// Look up the other joints influencing 'p' if any          
//		for (int j = 1; j < nb_joints; j++)  //遍历除第一个关节之外的其他关节，对每个关节进行相应的双四元数混合。
//		{
//			const int k = joints_id[v][j];
//			float w = weights[v][j];
//			const Dual_quat_cu& dq = (k == -1) ? Dual_quat_cu::identity() : dual_quat[k];//如果 k 的值等于 -1，则表示顶点 v 不受任何关节的影响，即没有旋转和平移的变换。
//			//如果 k 的值不等于 -1，则将 dq 设置为 dual_quat[k]，即顶点 v 受到关节 k 的双四元数变换的影响。
//			if (dq.rotation().dot(q0) < 0.f)   //确保多关节旋转时，与第一个关节旋转方向保持一致
//				w *= -1.f;
//
//			dq_blend = dq_blend + dq * w;
//		}
//
//		//计算对偶四元数变形后的顶点位置和法线，并将结果存储到输出向量 out_verts 和 out_normals 中。
//		// Compute animated position
//		Vec3 vi = dq_blend.transform(in_verts[v]).to_vec3();
//		out_verts[v] = vi;
//		// Compute animated normal
//		out_normals[v] = dq_blend.rotate(in_normals[v]);
//	}
//}

// 将 Tbx::Point3 转换为 CPoint3D 的转换函数
CPoint3D ConvertToPoint3D(const Tbx::Point3& tbxPoint) {
	
	// 返回一个 CPoint3D 对象
	return CPoint3D(tbxPoint.x, tbxPoint.y, tbxPoint.z);
}


int main(int argc, char* argv[])
{

	objModel2.computerKinTerachbySearch(objModel2.vSetsChangeType, objModel1.TetrahSet_initial, objModel1.TetrahSet_aftermove, objModel2.vSet_new);
	//objModel2.ComputerSkeletonPointRestriction(objModel2.vSetsChangeType, objModel1.TetrahSet_initial, objModel1.TetrahSet_aftermove, objModel2.vSet_new,80, objModel2.centers);

	std::vector<CPoint3D> VSet(150);

	//std::vector<Point3> in_verts;                     //顶点
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
	//  添加更多顶点 ...  
	};

	std::vector<Vec3> in_normals = {               //与in_verts顺序相同         数量相同 全设置成0
		{1.0f, 2.0f, 3.0f},
		{2.0f, 2.0f, 2.0f},
		{3.0f, 3.0f, 3.0f},
		{2.0f, 2.0f, 2.0f},
		{2.0f, 2.0f, 2.0f},
		{2.0f, 2.0f, 2.0f},
		{2.0f, 2.0f, 2.0f},
		{2.0f, 2.0f, 2.0f},
		//  添加更多法线 ...  
	};

	// 假设我们有3个关节，每个关节有一个双四元数变换   直接指定两个四元数      数量可以不相同
	std::vector<Dual_quat_cu> dual_quat = {
		Tbx::Dual_quat_cu(Quat_cu(0, 0.006, 3.246, -0.013), Quat_cu(0,0, 0, 0)),          // 颈部
		Tbx::Dual_quat_cu(Quat_cu(0, 0.008, 2.342, 0.062), Quat_cu(0, 0, 0, 0.0)),        //肚子
		Tbx::Dual_quat_cu(Quat_cu(0 ,0.008, 2.118, 0.0468), Quat_cu(0, 0, 0, 0)),         //脊椎根
		Tbx::Dual_quat_cu(Quat_cu(0, -0.2999, 1.9663, 0.0414), Quat_cu(0, 0, 0, 0.0)),   //右腿上
		Tbx::Dual_quat_cu(Quat_cu(0 ,-0.2652, 1.1228, 0.0058), Quat_cu(0, 0, 0, 0)),      //右腿下
		Tbx::Dual_quat_cu(Quat_cu(0 ,-0.0347, 0.8435, 0.0356), Quat_cu(0, 0, 0, 0)),      //9-11  转轴

	};//

	// 不同的权重                 数量相同
	std::vector<std::vector<float>> weights = {
	{1},  // 第一个关节的权重
	{1},  // 第二个关节的权重
	{1}, // 第三个关节的权重
	{1},
	{1},
	{1},
	{1},
	{1},


	// ... 为每个关节添加权重 ...  
	};

	// 假设每个顶点受影响的关节ID分别是0, 1, 2                 数量相同
	std::vector<std::vector<int>> joints_id = {            //顺序与weight相同
		{5}, // 第一个顶点的关节ID  
		{5},// 第二个顶点的关节ID    
		{5},// 第三个顶点的关节ID    
		{5},
		{5},
		{5},
		{5},
		{5},

		// ... 为每个顶点添加关节ID ...  
	};

	//// 初始化输出顶点和法线的向量，大小与输入一致  
	std::vector<Vec3> out_verts(in_verts.size());
	std::vector<Vec3> out_normals(in_normals.size());

	//调用 dual_quat_deformer 函数进行网格变形  
	//dual_quat_deformer(in_verts, in_normals, out_verts, out_normals, dual_quat, weights, joints_id);

	// 替换第128号点
	VSet[128] = ConvertToPoint3D(in_verts[0]);

	// 替换第36号点
	VSet[36] = ConvertToPoint3D(in_verts[1]);

	// 替换第132号点
	VSet[132] = ConvertToPoint3D(in_verts[2]);

	//objModel2.computerKinTerachbySearch(VSet, objModel1.TetrahSet_initial, objModel1.TetrahSet_aftermove, objModel2.vSet_new);



	//// 输出结果  
	//std::cout << "Deformation completed!" << std::endl;

	//// 查看变形后的顶点或法线
	//for (const auto& vert : out_verts) {
	//	std::cout << "Deformed vertex: " << vert << std::endl;
	//}
	///*for (const auto& norm : out_normals) {
	//	std::cout << "Deformed normal: " << norm << std::endl;
	//}*/




	glutInit(&argc, argv);
	init();
	glEnable(GL_TEXTURE_2D);    // 启用纹理
	objModel2.texModel = objModel2.load_texture("C:/Users/reckless/Desktop/3D模型绘制/ironManPicture.bmp");  //加载纹理
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(moseMove);
	glutMotionFunc(changeViewPoint);
	glutIdleFunc(myIdle);
	glutMainLoop();//循环调用
	return 0;
}

