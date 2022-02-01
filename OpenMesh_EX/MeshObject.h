#pragma once

#include <string>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/System/config.h>
#include <OpenMesh/Core/Mesh/Status.hh>
#include<OpenMesh/Core/Utils/RandomNumberGenerator.hh>
#include <Eigen/Dense>
#include <Eigen/Eigen>
#include<Eigen/IterativeLinearSolvers>
#include "Common.h"
#include <time.h>
#include <cstdint>
#include <algorithm>
//#include <iostream>
#include <iomanip>
//#include <Eigen/PaStiXSupport>

typedef OpenMesh::TriMesh_ArrayKernelT<>  TriMesh;
using namespace Eigen;
struct mynode {
	int v_id;
	Eigen::Vector3d pos;
	bool operator<(const mynode& rhs) const noexcept
	{
		// logic here
		return this->v_id < rhs.v_id;
	}
};
struct myedge {
	int e_id;
	int v_id1;
	int v_id2;
	bool operator<(const myedge& rhs) const noexcept
	{
		// logic here
		return this->e_id < rhs.e_id;
	}
};
class MyMesh : public TriMesh
{
public:
	MyMesh();
	~MyMesh();

	int FindVertex(MyMesh::Point pointToFind);
	void ClearMesh();
};

class GLMesh
{
public:
	GLMesh();
	~GLMesh();

	bool Init(std::string fileName);
	void Render();
	bool SaveModel(std::string fileName);
	void GetModel(unsigned int faceID, std::vector<unsigned int> selectedFace);
	MyMesh mesh;
	MyMesh mesh2;
	MyMesh mesh_tmp;
	// generate vertices
	
	GLuint vao,pvao,myvao;
	GLuint ebo,pebo,myebo;
	GLuint vboVertices, vboNormal,p_vboVertices,p_vboNormal,myvbo;
	int mode=1;
	//std::set<std::tuple<int,MyMesh::Point>> my_point;

	std::set<mynode> my_point;
	std::set<myedge> my_wire;

	//std::vector<MyMesh::Point> vertices;
	//std::vector<MyMesh::Point> vertices;

public:

	bool LoadModel(std::string fileName);
	void LoadToShader();
	void LoadToShader2();
	
	
};

struct headnode {
	OpenMesh::ArrayKernel::EdgeHandle edge_id;
	OpenMesh::ArrayKernel::VertexHandle from;
	OpenMesh::ArrayKernel::VertexHandle to;
	double cost;
	bool operator<(const headnode& rhs) const noexcept
	{
		// logic here
		return this->cost < rhs.cost; 
	}
};
struct SK_headnode {
	OpenMesh::ArrayKernel::HalfedgeHandle halfedge_id;
	double cost;
	bool operator<(const SK_headnode& rhs) const noexcept
	{
		// logic here
		return this->cost < rhs.cost;
	}
};
struct weight {
	OpenMesh::ArrayKernel::EdgeHandle edge_id;
	OpenMesh::ArrayKernel::VertexHandle from;
	OpenMesh::ArrayKernel::VertexHandle to;

};
class MeshObject
{
public:
	MeshObject();
	~MeshObject();

	bool Init(std::string fileName);
	bool End(std::string fileName);
	void Render();
	void RenderSelectedFace();
	bool AddSelectedFace(unsigned int faceID);
	void DeleteSelectedFace(unsigned int faceID);
	bool FindClosestPoint(unsigned int faceID, glm::vec3 worldPos, glm::vec3& closestPos);

	void Simplification(int);
	void back();
	OpenMesh::ArrayKernel::VertexHandle Simplification2(OpenMesh::ArrayKernel::EdgeHandle smallest);
	double caculateQ3(OpenMesh::ArrayKernel::VertexHandle , OpenMesh::ArrayKernel::VertexHandle , OpenMesh::ArrayKernel::EdgeHandle);
	Matrix4d caculateQ(OpenMesh::ArrayKernel::VertexHandle);
	void time_s(float time_input);
	void trackbar_simplification(int);

	void Skeleton();
	void CaculateWeight();
	void Fill_In_Matrix(int);
	double WL(int);
	void WH(int);
	void s_num(float s_input);
	int random_point();
	float MeshObject::cotan(OpenMesh::Vec3d a, OpenMesh::Vec3d b);
	void reset();
	void trackbar_skeleton(int);
	//void swap(struct headnode &p1, struct headnode &p2);
	//void MaxHeapify(std::vector<struct headnode> &array, int root, int length);
	//void BuildMaxHeap(std::vector<struct headnode> &array);
	//void HeapSort(std::vector<struct headnode> &array,bool inital);
	//void PrintArray(std::vector<struct headnode> &array);
	//void Find_and_Delete_Position(OpenMesh::ArrayKernel::VertexHandle from, OpenMesh::ArrayKernel::VertexHandle to,double cost);
	OpenMesh::ArrayKernel::EdgeHandle Top(std::set<struct headnode> &array);
	OpenMesh::ArrayKernel::HalfedgeHandle SK_Top(std::set<struct SK_headnode> &array);
	//void Insert(std::vector<struct headnode> &array);
	//bool small(std::vector<struct headnode> &array, float epsilon);
	//OpenMesh::ArrayKernel::EdgeHandle Find(struct weight node, std::vector<struct weight> &array);


	void SK_simplification();
	int SK_simplification2(OpenMesh::ArrayKernel::HalfedgeHandle smallest);
	double SK_caculateQ3(OpenMesh::ArrayKernel::HalfedgeHandle);
	void SK_caculate_dis(OpenMesh::ArrayKernel::VertexHandle);
	Matrix4d SK_caculateQ(OpenMesh::ArrayKernel::VertexHandle);
	bool SK_is_collaspe_ok(OpenMesh::ArrayKernel::HalfedgeHandle smallest);
	void Render_Point();
	void finetune();
	GLMesh model;
	int point_number();
	void trackbar_sk_skeleton(int);

private:
	
	std::vector<unsigned int> selectedFace;
	//std::vector<unsigned int*> fvIDsPtr;
	std::vector<int> elemCount;
	OpenMesh::VPropHandleT<Eigen::Matrix4d> Q;//存Q矩陣
	OpenMesh::EPropHandleT<double> cost;//存cost
	OpenMesh::EPropHandleT<MyMesh::Point> v3; //存v3
	std::set<struct headnode> queue;	//存cost和ehandle的priority queue
	std::vector<OpenMesh::ArrayKernel::VertexHandle> one_ring;//v3的one_ring
	MyMesh::VertexHandle  vhandles_new;//新增的v3
	std::vector<MyMesh> simplification_index;//存每次更新前的mesh
	bool init = true;

	std::vector<MyMesh> generations;//存每次更新前的mesh
	OpenMesh::EPropHandleT<double> weight;//存weight
	std::vector<struct weight> queue2;

	OpenMesh::VPropHandleT<MyMesh::Point> test;
	int time_skeleton ;
	OpenMesh::VPropHandleT<double> wh;
	OpenMesh::VPropHandleT<double> original_one_ring_area;
	std::vector<MyMesh> skeleton_index;
	OpenMesh::VPropHandleT<double> num;
	double wl0=0;
	int start;
	std::vector<int> select_point;
	double s;
	double wl;
	int time_num;

	double tmp_area = 0.0f;


	//骨架部分的簡化
	OpenMesh::VPropHandleT<Eigen::Matrix4d> SK_Q;//存SK_Q矩陣
	OpenMesh::VPropHandleT<double> SK;//存SK_cost
	OpenMesh::HPropHandleT<double> SK_cost;//存SK_cost
	std::set<struct SK_headnode> SK_queue;	//存cost和ehandle的priority queue
	std::vector<MyMesh::VertexHandle>  face_vhandles;//加面時vhandle順序
	std::vector<MyMesh::VertexHandle>  ring;//加面時vhandle順序

	bool flag_SK_simplification=true;	

	std::vector<MyMesh> sk_simplification_index;
	int sk_time_num;
	bool one = true;
};

