#include "MeshObject.h"
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <map>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cstdint>
#define Quad
//#define Harmonic

struct OpenMesh::VertexHandle const OpenMesh::PolyConnectivity::InvalidVertexHandle;

#pragma region MyMesh

MyMesh::MyMesh()
{
	request_face_normals();
	request_vertex_normals();
	request_vertex_status();
	request_face_status();
	request_edge_status();
}

MyMesh::~MyMesh()
{

}

int MyMesh::FindVertex(MyMesh::Point pointToFind)
{
	int idx = -1;
	for (MyMesh::VertexIter v_it = vertices_begin(); v_it != vertices_end(); ++v_it)
	{
		MyMesh::Point p = point(*v_it);
		if (pointToFind == p)
		{
			idx = v_it->idx();
			break;
		}
	}

	return idx;
}

void MyMesh::ClearMesh()
{
	if (!faces_empty())
	{
		for (MyMesh::FaceIter f_it = faces_begin(); f_it != faces_end(); ++f_it)
		{
			delete_face(*f_it, true);
		}

		garbage_collection();
	}
}

#pragma endregion

#pragma region GLMesh

GLMesh::GLMesh()
{

}

GLMesh::~GLMesh()
{

}

bool GLMesh::Init(std::string fileName)
{
	if (LoadModel(fileName))
	{
		LoadToShader();
		mesh2=mesh;
		return true;
	}
	return false;
}

/*void GLMesh::Render()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, mesh.n_faces() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}*/


bool GLMesh::LoadModel(std::string fileName)
{
	OpenMesh::IO::Options ropt;
	if (OpenMesh::IO::read_mesh(mesh, fileName, ropt))
	{
		if (!ropt.check(OpenMesh::IO::Options::VertexNormal) && mesh.has_vertex_normals())
		{
			mesh.request_face_normals();
			mesh.request_edge_status();
			//mesh.request_edge_position();
			mesh.update_normals();
			mesh.release_face_normals();
		}

		return true;
	}

	return false;
}
bool GLMesh::SaveModel(std::string fileName)
{
	bool isSave = false;
	if (OpenMesh::IO::write_mesh(mesh, fileName))
	{
		isSave = true;
	}
	return isSave;
}

void GLMesh::LoadToShader()
{
	//std::cout << "enterShader: "<< mesh.n_vertices() << std::endl;
	std::vector<MyMesh::Point> vertices;
	//std::vector<Eigen::Vector3d> vertices;
	vertices.reserve(mesh.n_vertices());
	for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		vertices.push_back((mesh.point(*v_it)));
		//std::cout << *v_it<<" mesh.point(*v_it)" << mesh.point(*v_it) << std::endl;
		//MyMesh::Point p = mesh.point(*v_it);
	}
	std::vector<MyMesh::Normal> normals;
	normals.reserve(mesh.n_vertices());
	for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		normals.push_back(mesh.normal(*v_it));
	}
	//std::cout << "enterShader3: "<< mesh.n_faces() << std::endl;
	//std::cout << "enterShader4" << mesh.n_edges() << std::endl;
	//std::cout << "enterShader5" << mesh.n_halfedges() << std::endl;
	std::vector<unsigned int> indices;
	if (mode == 1)
	{
		indices.reserve(mesh.n_faces() * 3);
		for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
		{
			for (MyMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
			{
				indices.push_back(fv_it->idx());

			}
		}
	}
	std::vector<unsigned int> indices2;
	indices2.reserve(mesh.n_edges() * 2);
	for (MyMesh::EdgeIter e_it = mesh.edges_begin(); e_it != mesh.edges_end(); ++e_it)
	{
		MyMesh::HalfedgeHandle now = mesh.halfedge_handle(*e_it, 0);
		indices2.push_back((mesh.to_vertex_handle(now)).idx());
		indices2.push_back((mesh.from_vertex_handle(now)).idx());	
		//std::cout << mesh.n_edges() <<"(mesh.to_vertex_handle(now)).idx()" << (mesh.to_vertex_handle(now)).idx() <<std::endl;
		//std::cout << mesh.n_edges()<< "(mesh.from_vertex_handle(now)).idx()" << (mesh.from_vertex_handle(now)).idx() << std::endl;
	}
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::Point) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	//std::cout << "enterShader6" << std::endl;
	glGenBuffers(1, &vboNormal);
	glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::Normal) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	//std::cout << "enterShader7" << std::endl;

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	if (mode == 1)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);
	}
	else if (mode == 2)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices2.size(), &indices2[0], GL_STATIC_DRAW);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//std::cout << "enterShader8" << std::endl;
	glGenVertexArrays(1, &pvao);
	glBindVertexArray(pvao);
	//std::cout << "enterShader5" << std::endl;
	glGenBuffers(1, &p_vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, p_vboVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::Point) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	//std::cout << "enterShader6" << std::endl;
	glGenBuffers(1, &p_vboNormal);
	glBindBuffer(GL_ARRAY_BUFFER, p_vboNormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::Normal) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	//std::cout << "enterShader7" << std::endl;

	glGenBuffers(1, &pebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices2.size(), &indices2[0], GL_STATIC_DRAW);	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	std::cout << "LoadToShader" << std::endl;
}
void GLMesh::LoadToShader2()
{
	//std::cout << "enterShader: "<< mesh.n_vertices() << std::endl;
	struct mynode my_node;
	for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		my_node.v_id = (*v_it).idx();
		my_node.pos(0) = mesh.point(*v_it)[0];
		my_node.pos(1) = mesh.point(*v_it)[1];
		my_node.pos(2) = mesh.point(*v_it)[2];
		my_point.insert(my_node);
	}
	struct myedge my_edge;
	for (MyMesh::EdgeIter e_it = mesh.edges_begin(); e_it != mesh.edges_end(); ++e_it)
	{
		MyMesh::HalfedgeHandle now = mesh.halfedge_handle(*e_it, 0);

		my_edge.e_id = (*e_it).idx();
		my_edge.v_id1 = (mesh.to_vertex_handle(now)).idx();
		my_edge.v_id2 = (mesh.from_vertex_handle(now)).idx();
		my_wire.insert(my_edge);
	}
	std::vector<Eigen::Vector3d> vertices2;
	vertices2.reserve(my_point.size());
	for (auto it= my_point.begin();it!=my_point.end();++it)
	{
		vertices2.push_back(it->pos);
		std::cout << my_point.size() <<" it->pos " << it->pos << std::endl;
	
	}

	std::vector<unsigned int> indices3;
	indices3.reserve(my_wire.size()*2);
	for (auto it = my_wire.begin(); it != my_wire.end(); ++it)
	{
		indices3.push_back(it->v_id1);
		indices3.push_back(it->v_id2);
		std::cout << my_wire.size() << " it->v_id1 " << it->v_id1 << std::endl;
		std::cout << my_wire.size() << " it->v_id2 " << it->v_id2 << std::endl;
	}
	glGenVertexArrays(1, &myvao);
	glBindVertexArray(myvao);

	glGenBuffers(1, &myvbo);
	glBindBuffer(GL_ARRAY_BUFFER, myvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Eigen::Vector3d) * vertices2.size(), &vertices2[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	////std::cout << "enterShader6" << std::endl;
	//glGenBuffers(1, &p_vboNormal);
	//glBindBuffer(GL_ARRAY_BUFFER, p_vboNormal);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::Normal) * normals.size(), &normals[0], GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(1);
	////std::cout << "enterShader7" << std::endl;

	glGenBuffers(1, &myebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices3.size(), &indices3[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	std::cout << "LoadToShader2" << std::endl;
}

#pragma endregion

MeshObject::MeshObject()
{

}

MeshObject::~MeshObject()
{
}

bool MeshObject::Init(std::string fileName)
{
	selectedFace.clear();
	return model.Init(fileName);
}
bool MeshObject::End(std::string fileName)
{
	//selectedFace.clear();

	return model.SaveModel(fileName);
}
void MeshObject::Skeleton()
{
	generations.push_back(model.mesh);
	//std::cout << "time" << time_skeleton << std::endl;
	int inner_point_num=0;	
	CaculateWeight();
	//std::cout << "22" << std::endl;
	for (MyMesh::VertexIter v_it = model.mesh.vertices_begin(); v_it != model.mesh.vertices_end(); ++v_it)
	{		
		inner_point_num++;
	}
	//srand(time(NULL));
	//int min = 0;
	//int max = inner_point_num-1;
	//int select_point_num =0;
	//select_point.clear();
	//for (int i = 0; i < select_point_num; i++)
	//{
	//	//std::cout << "i: " << i << std::endl;
	//	int x = rand() % (max - min + 1) + min;
	//	if (std::find(select_point.begin(), select_point.end(), x) == select_point.end())
	//	{			
	//		select_point.push_back(x);
	//	}
	//	else
	//	{
	//		i--;
	//	}
	//	//std::cout << "i_new: " << i << std::endl;
	//}	
	std::cout << "inner_point_num: "<< inner_point_num << std::endl;
	std::cout << "time_skeleton: " << time_skeleton << std::endl;
	model.mesh.add_property(num);
	if (time_skeleton == 0)
	{
		skeleton_index.push_back(model.mesh);
		model.mesh.add_property(wh);
		model.mesh.add_property(original_one_ring_area);
	}
	//Fill_In_Matrix(inner_point_num,select_point_num);
	Fill_In_Matrix(inner_point_num);
	time_skeleton++;
	skeleton_index.push_back(model.mesh);
	model.LoadToShader();
	/*std::cout << "++++++++++++++++++++++" << std::endl;
	for (MyMesh::FaceIter v_it = model.mesh.faces_begin(); v_it != model.mesh.faces_end(); ++v_it)
	{
		std::cout << *v_it << std::endl;
	}*/
	//model.mesh2 = model.mesh;

}
void MeshObject::CaculateWeight()
{
	//struct weight id;
	double cotan_alpha = 0;
	double cotan_beta = 0;
	double wij = 0;
	const double eps = 1e-0f;
	const double cotan_max = cos(eps) / sin(eps);
	model.mesh.add_property(weight);
	for (MyMesh::EdgeIter e_it = model.mesh.edges_begin(); e_it != model.mesh.edges_end(); ++e_it)
	{
		if (!model.mesh.is_boundary(*e_it))
		{
			//ehandle.push_back(*e_it);
			TriMesh::HalfedgeHandle heh, heh2;
			OpenMesh::ArrayKernel::VertexHandle   vertex_j, vertex_j_next, vertex_j_prev, vertex_i;
			heh = model.mesh.halfedge_handle(*e_it, 1);//左邊
			heh2 = model.mesh.halfedge_handle(*e_it, 0);//右邊

			vertex_j = model.mesh.from_vertex_handle(heh);//<---------------------------------------------vertex_j
			vertex_i = model.mesh.to_vertex_handle(heh);//<---------------------------------------------vertex_i									
			vertex_j_next = model.mesh.to_vertex_handle(model.mesh.next_halfedge_handle(heh));//<---------------------------------------------vertex_j_next
			vertex_j_prev = model.mesh.to_vertex_handle(model.mesh.next_halfedge_handle(heh2));//<---------------------------------------------vertex_j_prev
			/*OpenMesh::ArrayKernel::VertexHandle p1 =model.mesh.opposite_vh(heh);
			OpenMesh::ArrayKernel::VertexHandle p2 = model.mesh.opposite_he_opposite_vh(heh);*/

			//std::cout<<"i: "<< vertex_i <<"; j: "<< vertex_j<<"; j_next: "<< vertex_j_next <<"; j_prev: "<< vertex_j_prev<<"; p1: "<<p1<<"; p2: "<<p2<<std::endl;
			
			OpenMesh::Vec3d v1 =(OpenMesh::Vec3d)(model.mesh.point(vertex_j_next) - model.mesh.point(vertex_j)); v1.normalize();
			OpenMesh::Vec3d v2 = (OpenMesh::Vec3d)(model.mesh.point(vertex_j_next) - model.mesh.point(vertex_i)); v2.normalize();
			if (OpenMesh::dot(v1, v2) > 1)
			{
				cotan_alpha= std::acos(1.0);
			}
			else if (OpenMesh::dot(v1, v2) < -1)
			{
				cotan_alpha = std::acos(-1.0);
			}
			else 
			{
			   cotan_alpha = std::acos(OpenMesh::dot(v1, v2));
			}
			OpenMesh::Vec3d v_1(v1[0], v1[1], v1[2]);
			OpenMesh::Vec3d v_2(v2[0], v2[1], v2[2]);
			OpenMesh::Vec3d v3 = (OpenMesh::Vec3d)(model.mesh.point(vertex_j_prev) - model.mesh.point(vertex_j)); v3.normalize();
			OpenMesh::Vec3d v4 = (OpenMesh::Vec3d)(model.mesh.point(vertex_j_prev) - model.mesh.point(vertex_i)); v4.normalize();
			if (OpenMesh::dot(v3, v4) > 1)
			{
				cotan_beta = std::acos(1.0);
			}
			else if (OpenMesh::dot(v3, v4) < -1)
			{
				cotan_beta = std::acos(-1.0);
			}
			else
			{
				cotan_alpha = std::acos(OpenMesh::dot(v3, v4));
			}
			OpenMesh::Vec3d v_3(v3[0], v3[1], v3[2]);
			OpenMesh::Vec3d v_4(v4[0], v4[1], v4[2]);
			double cotan1 = cotan(v_1, v_2);
			double cotan2 = cotan(v_3, v_4);

			/*OpenMesh::Vec3f v1 = model.mesh.point(vertex_i) - model.mesh.point(vertex_j);
			OpenMesh::Vec3f v2 = model.mesh.point(vertex_i) - model.mesh.point(vertex_j_prev);
			OpenMesh::Vec3f v3 = model.mesh.point(vertex_j_prev) - model.mesh.point(vertex_j);
			OpenMesh::Vec3f v4 = model.mesh.point(vertex_i) - model.mesh.point(vertex_j_next);
			OpenMesh::Vec3f v5 = model.mesh.point(vertex_j_next) - model.mesh.point(vertex_j);
			double e1 = (double)v1.norm();
			double e2 = (double)v2.norm();
			double e3 = (double)v3.norm();
			double e4 = (double)v4.norm();
			double e5 = (double)v5.norm();
			cotan_alpha = fabs((e3*e3 + e2 * e2 - e1 * e1) / (2.0f*e3*e2));
			cotan_beta = fabs((e4*e4 + e5 * e5 - e1 * e1) / (2.0f*e4*e5));
			double cotan1 = cotan_alpha / sqrt(1.0f - cotan_alpha * cotan_alpha);
			double cotan2 = cotan_beta / sqrt(1.0f - cotan_beta * cotan_beta);*/
			//wij = (cotan1 + cotan2) / 2.0f;
			
			wij = (cotan1 + cotan2) ;
			/*if (isnan(wij)) {
				wij = 0.0f;
			}
			if (wij >= cotan_max) {
				wij = cotan_max;
			}*/
			/*if (wij <= -cotan_max) {
				wij = -cotan_max;
			}*/
			//wij = std::clamp(wij, -cotan_max, cotan_max);
			model.mesh.property(weight, *e_it) = wij;
			//std::cout << vertex_j << " and " << vertex_i << std::endl;
			//std::cout << *e_it << " weight= " << model.mesh.property(weight, *e_it) << std::endl;
		}
	}
	//檢查weights
	for (MyMesh::EdgeIter e_it = model.mesh.edges_begin(); e_it != model.mesh.edges_end(); ++e_it)
	{
		if (model.mesh.is_boundary(*e_it))
		{
			model.mesh.property(weight, *e_it) == 0;
		}
		//std::cout << "====================================" << std::endl;
		//std::cout << *e_it << " weight= " << model.mesh.property(weight, *e_it) << std::endl;
	}
}
void MeshObject::Fill_In_Matrix(int inner_point_num)
{
	//int start = clock();
	//std::cout << "time" << time_skeleton << std::endl;
	OpenMesh::VPropHandleT<double> weight_total; 
	model.mesh.add_property(weight_total);
	double wl = WL(time_skeleton);
	std::cout << "wl" << wl << std::endl;
	WH(time_skeleton);
	double time=0;
	
	//OpenMesh::VPropHandleT<std::vector<OpenMesh::ArrayKernel::EdgeHandle>> total; //存v3
	for (MyMesh::VertexIter v_it = model.mesh.vertices_begin(); v_it != model.mesh.vertices_end(); ++v_it)
	{
		for (MyMesh::VertexEdgeIter ve_it = model.mesh.ve_iter(*v_it); ve_it.is_valid(); ++ve_it)
		{
			model.mesh.property(weight_total, *v_it) += model.mesh.property(weight, *ve_it);
			//time++;
		}
		//model.mesh.property(num, *v_it) = -(1/time);	
		//time = 0;
	}		
	
	//std::cout << "+++++++++++++++++++++++" << std::endl;
	int start = clock();
	int size = inner_point_num;
	//SparseMatrix<double,Eigen::ColMajor> A(size+ select_point_num, size);
	SparseMatrix<double, Eigen::ColMajor> A(size *2, size);
	for (int i = 0; i < size ; i++)
	{
		
		//A.insert(i, i) = (-(model.mesh.property(weight_total, (OpenMesh::ArrayKernel::VertexHandle)i)))*wl;
		A.insert(i, i) = (-(model.mesh.property(weight_total, (OpenMesh::ArrayKernel::VertexHandle)i)))*wl;
		//A.insert(i, i) = -1 * wl;
		//A.insert(i, i) = 1 ;
		
		A.insert(i + size, i) = 1* model.mesh.property(wh, (OpenMesh::ArrayKernel::VertexHandle)i);
		//A.insert(i, i) = (-(model.mesh.property(weight_total, (OpenMesh::ArrayKernel::VertexHandle)i)));
		for (MyMesh::VertexVertexIter vv_it = model.mesh.vv_iter((OpenMesh::ArrayKernel::VertexHandle)i); vv_it.is_valid(); ++vv_it)
		{			
			//model.mesh.property(weight, *vv_it);
			//for (int j = 0; j < sqrt(A.size() / 2); j++)
			//{
				//if ((OpenMesh::ArrayKernel::VertexHandle)j == *vv_it)
				//{					
					MyMesh::HalfedgeHandle heh= model.mesh.find_halfedge((OpenMesh::ArrayKernel::VertexHandle)i, *vv_it);
					OpenMesh::ArrayKernel::EdgeHandle now = model.mesh.edge_handle(heh);					
					
					A.insert( (*vv_it).idx(),i) = model.mesh.property(weight, now) *wl;
					
					//A.insert(i, j) = model.mesh.property(weight, now);

					//A.insert(size + i, j) = 1 * model.mesh.property(wh, (OpenMesh::ArrayKernel::VertexHandle)i);
					//break;
				//}
			//}
		}
		
	}	
	//std::cout << "x" << A << std::endl;
	/*for (int i = size; i < size+ select_point_num; i++)
	{			
		A.insert(i , select_point[i-size]) = 1 * model.mesh.property(wh, (OpenMesh::ArrayKernel::VertexHandle)select_point[i - size]);		
	}*/
	int end = clock();
	//std::cout << "time" << end - start << std::endl;
	//std::cout << A.row(1) << std::endl;
	//std::cout << A.row(size+1) << std::endl;
	
	A.makeCompressed();
	//MatrixXd B(size*2,3);
	//std::cout << size << std::endl;
	//std::cout << B.size() << std::endl;
	//for (int i = 0; i < B.size()/6; i++)
	//{
	//	//std::cout << i << std::endl;
	//	B(i,0) = 0;
	//	B(i + size, 0) = model.mesh.point((OpenMesh::ArrayKernel::VertexHandle)i)[0];
	//	B(i, 1) = 0;
	//	B(i + size, 1) = model.mesh.point((OpenMesh::ArrayKernel::VertexHandle)i)[1];
	//	B(i, 2) = 0;
	//	B(i + size, 2) = model.mesh.point((OpenMesh::ArrayKernel::VertexHandle)i)[2];
	//}
	start = clock();
	//MatrixXd B(size + select_point_num, 1);
	MatrixXd B(size *2, 1);
	//MatrixXd B2(size + select_point_num, 1);
	MatrixXd B2(size * 2, 1);
	//MatrixXd B3(size + select_point_num, 1);
	MatrixXd B3(size * 2, 1);
	for (int i = 0; i < size; i++)
	{		
		B(i, 0) = 0;
		B2(i, 0) = 0;
		B3(i, 0) = 0;
		/*if (i < select_point_num )
		{
			B(i + size, 0) = (double)model.mesh.point((OpenMesh::ArrayKernel::VertexHandle)select_point[i])[0] * model.mesh.property(wh, (OpenMesh::ArrayKernel::VertexHandle)select_point[i]);
			B2(i + size, 0) = (double)model.mesh.point((OpenMesh::ArrayKernel::VertexHandle)select_point[i])[1] * model.mesh.property(wh, (OpenMesh::ArrayKernel::VertexHandle)select_point[i]);
			B3(i + size, 0) = (double)model.mesh.point((OpenMesh::ArrayKernel::VertexHandle)select_point[i])[2] * model.mesh.property(wh, (OpenMesh::ArrayKernel::VertexHandle)select_point[i]);
		}*/
		//B3(i + size, 0) = model.mesh.point((OpenMesh::ArrayKernel::VertexHandle)i)[2];
		
		B(i + size, 0) = model.mesh.point((OpenMesh::ArrayKernel::VertexHandle)i)[0] * model.mesh.property(wh, (OpenMesh::ArrayKernel::VertexHandle)i);
		B2(i + size, 0) = model.mesh.point((OpenMesh::ArrayKernel::VertexHandle)i)[1] * model.mesh.property(wh, (OpenMesh::ArrayKernel::VertexHandle)i);
		B3(i + size, 0) = model.mesh.point((OpenMesh::ArrayKernel::VertexHandle)i)[2]* model.mesh.property(wh, (OpenMesh::ArrayKernel::VertexHandle)i);

		//B(i + size, 0) = model.mesh.point((OpenMesh::ArrayKernel::VertexHandle)i)[0];
		//B2(i + size, 0) = model.mesh.point((OpenMesh::ArrayKernel::VertexHandle)i)[1];
		//B3(i + size, 0) = model.mesh.point((OpenMesh::ArrayKernel::VertexHandle)i)[2];
	}
	end = clock();
	//std::cout << "timeB" << end - start << std::endl;	
	std::cout << "Start Solve Least Square Solution" << std::endl;
	//std::cout <<   (A.transpose()*A).row(1) << std::endl;
	start = clock();
	Eigen::SimplicialCholesky<SparseMatrix<double >> chol(A.transpose()*A);
	//Eigen::SimplicialLLT<SparseMatrix<double >> chol(A.transpose()*A);
	end = clock();
	//std::cout << "timeLLT" << end - start << std::endl;
	start = clock();
	MatrixXd X = chol.solve(A.transpose()*B);
	MatrixXd Y = chol.solve(A.transpose()*B2);
	MatrixXd Z = chol.solve(A.transpose()*B3);
	end = clock();

	//std::cout << "x" << X << std::endl;
	//std::cout << "timelinearSolver" << end - start << std::endl;
	//std::cout << "point number" << inner_point_num << std::endl;
	std::cout << "Solve Least Square Solution End" << std::endl;
	OpenMesh::Vec3d new_v;	
	for (MyMesh::VertexIter v_it = model.mesh.vertices_begin(); v_it != model.mesh.vertices_end(); ++v_it)
	{
		if (isnan(X((*v_it).idx())) == false && isnan(Y((*v_it).idx())) == false && isnan(Z((*v_it).idx())) == false)
		{
			new_v = OpenMesh::Vec3d(X((*v_it).idx()), Y((*v_it).idx()), Z((*v_it).idx()));
			//model.mesh.point(*v_it) = new_v;
			model.mesh.point(*v_it)[0] = X((*v_it).idx());
			model.mesh.point(*v_it)[1] = Y((*v_it).idx());
			model.mesh.point(*v_it)[2] = Z((*v_it).idx());
			new_v = OpenMesh::Vec3d(0, 0, 0);
		}
	}
	
}
void MeshObject::trackbar_skeleton(int index)
{
	model.mesh = skeleton_index[index];
	std::cout << "check" << std::endl;
	model.LoadToShader();
}
void MeshObject::s_num(float s_f)
{
	s = s_f;
}
double MeshObject::WL(int time)
{
	//std::cout << "time" << time << std::endl;
	//std::cout <<"S"<< s << std::endl;
	//int s = 40;
	//float wl=wl0;
	//std::cout <<"wl"<< wl << std::endl;
	if (time == 0)
	{
		double total_area = 0.0;
		int num = 0;
		for (MyMesh::FaceIter f_it = model.mesh.faces_begin(); f_it != model.mesh.faces_end(); ++f_it) {
			//std::cout << num << std::endl;
			total_area += model.mesh.calc_sector_area(model.mesh.halfedge_handle(*f_it));
			
			MyMesh::FaceVertexIter fv_it = model.mesh.fv_iter(*f_it);
			//for (MyMesh::FaceVertexIter fv_it = model.mesh.fv_iter(); fv_it.is_valid(); ++fv_it)
			//{	

			MyMesh::Point P = model.mesh.point(fv_it);  ++fv_it;

			MyMesh::Point Q = model.mesh.point(fv_it);  ++fv_it;

			MyMesh::Point R = model.mesh.point(fv_it);
	
			/*float a = sqrt((pow(P[0]- Q[0], 2) + pow(P[1]- Q[1], 2) + pow(P[2]- Q[2], 2)));
			float b = sqrt((pow(Q[0]- R[0], 2) + pow(Q[1]- R[1], 2) + pow(Q[2]- R[2], 2)));
			float c = sqrt((pow(R[0]- P[0], 2) + pow(R[1]- P[1], 2) + pow(R[2]- P[2], 2)));
			float s = (a+b+c)/2;
			total_area += sqrt(s*(s - a)*(s - b)*(s - c));*/

			//total_area += (((Q - P) % (R - P)).norm()) * 0.5/3 ;    // norm: compute euclidean norm, return Scalar
		//} 
			//std::cout << total_area << std::endl;
			num++;
		}
		//std::cout << "========================================" << std::endl;
		//std::cout << " total_area/num: " << total_area / num << std::endl;
		//std::cout << total_area  << std::endl;
		wl = pow(10,-3)*sqrt(total_area / model.mesh.n_faces());
		
		
	}
	else 
	{
		wl = wl * s;		
	}
	//std::cout << "wl_f" << wl << std::endl;
	return wl;
}
void MeshObject::WH(int time)
{
	//std::cout << "time" << time << std::endl;
	if (time == 0)
	{
		for (MyMesh::VertexIter v_it = model.mesh.vertices_begin(); v_it != model.mesh.vertices_end(); ++v_it)
		{
			model.mesh.property(wh, *v_it) = 1.0f;
			//std::cout << model.mesh.property(wh, *v_it) << std::endl;
		}
				
		for (MyMesh::VertexIter v_it = model.mesh.vertices_begin(); v_it != model.mesh.vertices_end(); ++v_it)
		{		
			double original_one_ring = 0.0;
			for (MyMesh::VertexFaceIter vf_it = model.mesh.vf_iter(*v_it); vf_it; ++vf_it)
			{
				original_one_ring += model.mesh.calc_sector_area(model.mesh.halfedge_handle(*vf_it));
				//MyMesh::FaceVertexIter fv_it = model.mesh.fv_iter(*vf_it);
				
				/*const MyMesh::Point P = model.mesh.point(*fv_it);  ++fv_it;
				const MyMesh::Point Q = model.mesh.point(*fv_it);  ++fv_it;
				const MyMesh::Point R = model.mesh.point(*fv_it);*/				
				//original_one_ring += (((Q - P) % (R - P)).norm()) * 0.5f ;    // norm: compute euclidean norm, return Scalar

			}

			model.mesh.property(original_one_ring_area, *v_it) = original_one_ring;
			//std::cout << "original_one_ring" << original_one_ring << std::endl;
		}			
	}
	else 
    //if(time > 0&& time <5)
	{	
		//tmp_area = 0.0f;
		//int num = 0;
		for (MyMesh::VertexIter v_it = model.mesh.vertices_begin(); v_it != model.mesh.vertices_end(); ++v_it) 
		{
			double one_ring = 0.0;
			for (MyMesh::VertexFaceIter vf_it = model.mesh.vf_iter(*v_it); vf_it; ++vf_it)
			{
				one_ring += model.mesh.calc_sector_area(model.mesh.halfedge_handle(*vf_it));
				//MyMesh::FaceVertexIter fv_it = model.mesh.fv_iter(*vf_it);			
				//const MyMesh::Point& P = model.mesh.point(fv_it);  ++fv_it;
				//const MyMesh::Point& Q = model.mesh.point(fv_it);  ++fv_it;
				//const MyMesh::Point& R = model.mesh.point(fv_it);

				//one_ring += (((Q - P) % (R - P)).norm() * 0.5f)/3 ;    // norm: compute euclidean norm, return Scalar
			}
			//tmp_area += one_ring;
			//num++;
			model.mesh.property(wh, *v_it) =  sqrt(model.mesh.property(original_one_ring_area, *v_it) / one_ring);
			/*double tmp_wh = pow(model.mesh.property(original_one_ring_area, *v_it) / one_ring,3);
			if (tmp_wh > sqrt(model.mesh.property(original_one_ring_area, *v_it) / one_ring)) {
				model.mesh.property(wh, *v_it) = tmp_wh;
			}*/
			if (one_ring < 0.000000005)
			{
				model.mesh.property(wh, *v_it) = wl;
				//std::cout << "true" << std::endl;
			}
		}
		//tmp_area=tmp_area / num*0.1;
	}	
}
void MeshObject::Simplification(int time_num)
{
	int inner_point_num = 0;	
	//CaculateWeight();
	for (MyMesh::VertexIter v_it = model.mesh.vertices_begin(); v_it != model.mesh.vertices_end(); ++v_it)
	{
		inner_point_num++;
	}
	std::cout << "point number:  "<< inner_point_num << std::endl;
	//if (!model.mesh2.faces_empty())
	//{
	//	model.mesh = model.mesh2;
	//	//std::cout << "change" << std::endl;
	//}
	if (init == true)
	{
		simplification_index.push_back(model.mesh);
		init = false;
	}
	//generations.push_back(model.mesh);
	//算Q
	int start = clock();
	model.mesh.add_property(Q);
	for (MyMesh::VertexIter v_it = model.mesh.vertices_begin(); v_it != model.mesh.vertices_end(); ++v_it)
	{
		//std::cout << "num: "<< *v_it << std::endl;
		model.mesh.property(Q, *v_it) = caculateQ(*v_it);
	}
	int end = clock();
	std::cout << "Q:　" << end - start << std::endl;
	//
	start = clock();
	model.mesh.add_property(v3);
	model.mesh.add_property(cost);
	queue.clear();
	//std::cout<<"queue"<<queue.size()<<std::endl;
	struct headnode num;
	double cost_v;
	for (MyMesh::EdgeIter e_it = model.mesh.edges_begin(); e_it != model.mesh.edges_end(); ++e_it)
	{
		//std::cout << "AAAAAAAAAAAAAA" << std::endl;
		//std::cout <<"old"<< *e_it << std::endl;
		if (!model.mesh.is_boundary(*e_it))
		{
			TriMesh::HalfedgeHandle heh;
			heh = model.mesh.halfedge_handle(*e_it, 0);		
			if (model.mesh.is_collapse_ok(heh) == true)
			{
				cost_v = caculateQ3(model.mesh.from_vertex_handle(heh), model.mesh.to_vertex_handle(heh), *e_it);
				model.mesh.property(cost, *e_it) = cost_v;
				num.from = model.mesh.from_vertex_handle(heh);
				num.to = model.mesh.to_vertex_handle(heh);
				num.cost = model.mesh.property(cost, *e_it);
				queue.insert(num);
			}
			
		}
	}		
	end = clock();
	std::cout << "Q3:　" << end - start << std::endl;
	start = clock();
	//HeapSort(queue,true);
	end = clock();
	//std::cout << "queue:　" << end - start << std::endl;
	std::cout << "queue.size():　" << queue.size() << std::endl;
	start = clock();
	for (int i = 0; i < time_num; i++)
	{
		inner_point_num--;
		//std::cout << "time"<<i << std::endl;
		//start = clock();
		if (queue.size() < 50 )
		{
			break;
		}
		else
		{
			//start = clock();
			OpenMesh::ArrayKernel::EdgeHandle smallest = Top(queue);
			//std::cout << "TOP"<< smallest << std::endl;
			//end = clock();
			//std::cout << "TOP:　" << end - start << std::endl;
			OpenMesh::ArrayKernel::VertexHandle point = Simplification2(smallest);
			if (point == (OpenMesh::ArrayKernel::VertexHandle) - 1)
			{
				//std::cout << "can't callaspe" << std::endl;
				i--;
			}
			else
			{
				//int start2 = clock();
				//start = clock();
			
				//vhandles_new = model.mesh.add_vertex(model.mesh.property(v3, smallest));
				////
				////one_ring.clear();
				//for (MyMesh::VertexVertexCCWIter vv_it = model.mesh.vv_ccwiter(point); vv_it.is_valid(); ++vv_it)
				//{
				//	//std::cout << " =========== CW   " << *vv_it << std::endl;
				//	one_ring.push_back(*vv_it);
				//}
				//model.mesh.point(point) = model.mesh.point(vhandles_new);
				//std::vector<OpenMesh::ArrayKernel::EdgeHandle> after_collaspe;
				//after_collaspe.clear();
				//for (MyMesh::VertexEdgeIter ve_it = model.mesh.ve_iter(point); ve_it.is_valid(); ++ve_it)
				//{
				//	//std::cout << " ===========    " << *ve_it << std::endl;
				//	after_collaspe.push_back(*ve_it);
				//}
				//for (int i = 0; i < after_collaspe.size(); i++)
				//{
				//	//std::cout << " ===========    " << after_collaspe[i] << std::endl;
				//	model.mesh.delete_edge(after_collaspe[i], false);
				//}
				//face_vhandles.clear();
				//for (int i = 0; i < one_ring.size(); i++)
				//{
				//	//std::cout <<"裡"<< one_ring[i] << std::endl;
				//	if (i == one_ring.size() - 1)
				//	{
				//		face_vhandles.clear();
				//		face_vhandles.push_back(one_ring[i]);
				//		face_vhandles.push_back(one_ring[0]);
				//		face_vhandles.push_back(vhandles_new);
				//		OpenMesh::ArrayKernel::FaceHandle tmp_face = model.mesh.add_face(face_vhandles);
				//		model.mesh.set_normal(tmp_face, model.mesh.calc_face_normal(tmp_face));
				//	}
				//	else if (i < one_ring.size() - 1)
				//	{
				//		face_vhandles.clear();
				//		face_vhandles.push_back(one_ring[i]);
				//		face_vhandles.push_back(one_ring[i + 1]);
				//		face_vhandles.push_back(vhandles_new);
				//		OpenMesh::ArrayKernel::FaceHandle tmp_face = model.mesh.add_face(face_vhandles);
				//		model.mesh.set_normal(tmp_face, model.mesh.calc_face_normal(tmp_face));
				//	}

				//}
				
				
				////model.mesh.update_vertex_normals();
				////std::cout << "-------------------------" << std::endl;
				////model.mesh.set_normal(vhandles_new, model.mesh.calc_vertex_normal(vhandles_new));
				///*MyMesh::Normal normal;
				//int i = 0;
				//for (MyMesh::VertexFaceIter vf_it = model.mesh.vf_iter(vhandles_new); vf_it.is_valid(); ++vf_it)
				//{
				//	i++;
				//	normal += model.mesh.normal(*vf_it);

				//}*/

				model.mesh.update_normal(vhandles_new);				
				for (int i = 0; i < one_ring.size(); i++)
				{
					for (MyMesh::VertexFaceIter vf_it = model.mesh.vf_iter(one_ring[i]); vf_it.is_valid(); ++vf_it)
					{
						model.mesh.update_normal(*vf_it);
					}
					model.mesh.update_normal(one_ring[i]);
				}
				
				for (int i = 0; i < one_ring.size(); i++)
				{
					for (MyMesh::VertexVertexIter vv_it = model.mesh.vv_iter(one_ring[i]); vv_it.is_valid(); ++vv_it)
					{
						model.mesh.update_normal(*vv_it);
					}
				}
				
				for (int i = 0; i < one_ring.size(); i++)
				{
					model.mesh.property(Q, one_ring[i]) = caculateQ(one_ring[i]);
				}
				
				model.mesh.property(Q, (OpenMesh::ArrayKernel::VertexHandle)vhandles_new) = caculateQ((OpenMesh::ArrayKernel::VertexHandle)vhandles_new);
				std::vector<OpenMesh::ArrayKernel::EdgeHandle> New;
				New.clear();
				
				int start3 = clock();
				//int s123 = 0;
				for (int i = 0; i < one_ring.size(); i++)
				{
					for (MyMesh::VertexVertexIter vv_it = model.mesh.vv_iter(one_ring[i]); vv_it.is_valid(); ++vv_it)
					{
						TriMesh::HalfedgeHandle heh = model.mesh.find_halfedge(one_ring[i], *vv_it);
						TriMesh::EdgeHandle heh2 = model.mesh.edge_handle(heh);						
						TriMesh::VertexHandle from = model.mesh.from_vertex_handle(heh);
						TriMesh::VertexHandle to = model.mesh.to_vertex_handle(heh);

						if (std::find(New.begin(), New.end(), heh2) == New.end())
						{
							if (*vv_it != vhandles_new )
							{
								//s123++;
								//Find_and_Delete_Position(one_ring[i], *vv_it,model.mesh.property(cost, heh2));
								//std::cout<<"from"<< model.mesh.from_vertex_handle(heh) <<std::endl;
								//std::cout << "to" << model.mesh.to_vertex_handle(heh) << std::endl;
								struct headnode find;
								find.cost = model.mesh.property(cost, heh2);
								find.to = model.mesh.to_vertex_handle(heh);
								find.from = model.mesh.from_vertex_handle(heh);
								queue.erase(find);
							}

							cost_v = caculateQ3(model.mesh.from_vertex_handle(heh), model.mesh.to_vertex_handle(heh), heh2);
							model.mesh.property(cost, heh2) = cost_v;
							New.push_back(heh2);
						}						
						//Insert(queue);
					}				
				}				
				//std::cout << "sssssssss:　" << s123 << std::endl;
				int end3 = clock();
				//std::cout << "Delete3:　" << end3 - start3 << std::endl;
				
				for (int i = 0; i < New.size(); i++)
				{
					TriMesh::HalfedgeHandle heh = model.mesh.halfedge_handle(New[i], 0);
					num.from = model.mesh.from_vertex_handle(heh);
					num.to = model.mesh.to_vertex_handle(heh);
					num.cost = model.mesh.property(cost, New[i]);
					queue.insert(num);
				}	
				
				
			}
		}
		//}
	}
	end = clock();
	std::cout << "END:　" << end - start << std::endl;
	model.mesh.delete_isolated_vertices();	
	model.mesh.garbage_collection();
	std::cout<<"queue.size():　"<<queue.size()<<std::endl;
	model.mesh.remove_property(Q);
	model.mesh.remove_property(v3);
	model.mesh.remove_property(cost);
	/*model.mesh2.clean();
	model.mesh2.garbage_collection();
	model.mesh2 = model.mesh;	*/
	simplification_index.push_back(model.mesh);
	point_number();
	model.LoadToShader();
}
OpenMesh::ArrayKernel::VertexHandle MeshObject::Simplification2(OpenMesh::ArrayKernel::EdgeHandle smallest)
{
	//int start = clock();
	//model.mesh_tmp.clean();
	//model.mesh_tmp = model.mesh;
	OpenMesh::ArrayKernel::VertexHandle from, to,test;
	std::vector<OpenMesh::ArrayKernel::EdgeHandle> need_to_delete;
	need_to_delete.clear();
	TriMesh::HalfedgeHandle heh_smallest;
	heh_smallest = model.mesh.halfedge_handle(smallest, 0);
	test=to = model.mesh.to_vertex_handle(heh_smallest);
	from = model.mesh.from_vertex_handle(heh_smallest);
	std::vector<OpenMesh::ArrayKernel::VertexHandle> one_ring_t;
	std::vector<OpenMesh::ArrayKernel::VertexHandle> one_ring_f;
	one_ring_f.clear();
	one_ring_t.clear();
	for (MyMesh::VertexVertexCCWIter vv_it = model.mesh.vv_ccwiter(to); vv_it.is_valid(); ++vv_it)
	{
		one_ring_t.push_back(*vv_it);
	}
	for (MyMesh::VertexVertexCCWIter vv_it = model.mesh.vv_ccwiter(from); vv_it.is_valid(); ++vv_it)
	{
		one_ring_f.push_back(*vv_it);
	}
	std::vector<OpenMesh::ArrayKernel::VertexHandle> same_point;
	for (int i = 0; i < one_ring_t.size(); i++)
	{
		/*std::cout << "tttttttttttttttt" << std::endl;
		std::cout << "tt: " << one_ring_t[i] << std::endl;*/
		if (!(std::find(one_ring_f.begin(), one_ring_f.end(), one_ring_t[i]) == one_ring_f.end()))
		{
			same_point.push_back(one_ring_t[i]);

		}
	}

	float top=0;
	float down=0;
	for (int i = 0; i < one_ring_f.size(); i++)
	{
		if (i == one_ring_f.size() - 1)
		{
			float cosa = (model.mesh.point(from) - model.mesh.point(one_ring_f[i]) | model.mesh.point(one_ring_f[i - 1]) - model.mesh.point(one_ring_f[i])) / ((model.mesh.point(from) - model.mesh.point(one_ring_f[i])).norm()*(model.mesh.point(one_ring_f[i - 1]) - model.mesh.point(one_ring_f[i])).norm());
			float cosb =( model.mesh.point(one_ring_f[0]) - model.mesh.point(one_ring_f[i]) | model.mesh.point(from) - model.mesh.point(one_ring_f[i]) )/ ((model.mesh.point(one_ring_f[0]) - model.mesh.point(one_ring_f[i])).norm()*(model.mesh.point(from) - model.mesh.point(one_ring_f[i])).norm());
			float cosaa = acos(cosa) * 180 / 3.1415926;
			float cosbb = acos(cosb) * 180 / 3.1415926;	
			//std::cout << "角度: " << cosaa << std::endl;
			//std::cout << "角度: " << cosbb << std::endl;	
			if (one_ring_f[i] == same_point[0])
			{
				top += cosaa;
				top += cosbb;
				//std::cout << "+"<< cosaa << std::endl;
			}
			else if (one_ring_f[i] == same_point[1])
			{
				down += cosaa;
				down += cosbb;
				//std::cout << "-" << cosaa << std::endl;
			}
			//std::cout << "角度" << " : " << cosaa + cosbb << std::endl;
			if ((cosaa + cosbb > 180) && one_ring_f[i] !=  to)
			{
				//std::cout << "false" << " : " << cosaa + cosbb << std::endl;
				return (OpenMesh::ArrayKernel::VertexHandle) - 1;
				break;
			}
			else
			{
				//std::cout << "true" << " : " << cosaa + cosbb << std::endl;
			}
		}
		else if (i > 0 && i < one_ring_f.size() - 1)
		{
			float cosa = (model.mesh.point(from) - model.mesh.point(one_ring_f[i]) | model.mesh.point(one_ring_f[i - 1]) - model.mesh.point(one_ring_f[i])) / ((model.mesh.point(from) - model.mesh.point(one_ring_f[i])).norm()*(model.mesh.point(one_ring_f[i - 1]) - model.mesh.point(one_ring_f[i])).norm());
			float cosb = (model.mesh.point(one_ring_f[i + 1]) - model.mesh.point(one_ring_f[i]) | model.mesh.point(from) - model.mesh.point(one_ring_f[i])) / ((model.mesh.point(one_ring_f[i + 1]) - model.mesh.point(one_ring_f[i])).norm()*(model.mesh.point(from) - model.mesh.point(one_ring_f[i])).norm());
			
			float cosaa = acos(cosa) * 180 / 3.1415926;
			float cosbb = acos(cosb) * 180 / 3.1415926;
			//std::cout << "12角度: " << cosaa << std::endl;
			//std::cout << "12角度: " << cosbb << std::endl;
			if (one_ring_f[i] == same_point[0])
			{
				top += cosaa;
				top += cosbb;
				//std::cout << "+" << cosaa << std::endl;
			}
			else if (one_ring_f[i] == same_point[1])
			{
				down += cosaa;
				down += cosbb;
				//std::cout << "-" << cosaa << std::endl;
			}
			//std::cout << "角度" << " : " << cosaa + cosbb << std::endl;
			if ((cosaa + cosbb > 180) && one_ring_f[i] != to)
			{
				//std::cout << "false" << " : " << cosaa + cosbb << std::endl;
				return (OpenMesh::ArrayKernel::VertexHandle) - 1;
				break;
				
			}
			else
			{
				//std::cout << "true" << " : " << cosaa + cosbb << std::endl;
			}
		}
		else if (i == 0)
		{
			float cosa = (model.mesh.point(from) - model.mesh.point(one_ring_f[i]) | model.mesh.point(one_ring_f[one_ring_f.size() - 1]) - model.mesh.point(one_ring_f[i])) / ((model.mesh.point(from) - model.mesh.point(one_ring_f[i])).norm()*(model.mesh.point(one_ring_f[one_ring_f.size() - 1]) - model.mesh.point(one_ring_f[i])).norm());
			float cosb = (model.mesh.point(one_ring_f[i + 1]) - model.mesh.point(one_ring_f[i]) | model.mesh.point(from) - model.mesh.point(one_ring_f[i])) / ((model.mesh.point(one_ring_f[i + 1]) - model.mesh.point(one_ring_f[i])).norm()*(model.mesh.point(from) - model.mesh.point(one_ring_f[i])).norm());
			float cosaa = acos(cosa) * 180 / 3.1415926;
			float cosbb = acos(cosb) * 180 / 3.1415926;
			//std::cout << "角度: " << cosaa << std::endl;
			//std::cout << "角度: " << cosbb << std::endl;
			if (one_ring_f[i] == same_point[0])
			{
				top += cosaa;
				top += cosbb;
				//std::cout << "+" << cosaa << std::endl;
			}
			else if (one_ring_f[i] == same_point[1])
			{
				down += cosaa;
				down += cosbb;
				//std::cout << "-" << cosaa << std::endl;
			}
			//std::cout << "角度" << " : " << cosaa + cosbb << std::endl;
			if ((cosaa + cosbb > 180) && one_ring_f[i] !=to)
			{
				//std::cout << "false" << " : " << cosaa + cosbb << std::endl;
				return (OpenMesh::ArrayKernel::VertexHandle) - 1;
				break;
				
			}
			else
			{
				//std::cout << "true" << " : " << cosaa + cosbb << std::endl;
			}
		}
	}
	for (int i = 0; i < one_ring_t.size(); i++)
	{		
		if (i == one_ring_t.size() - 1)
		{

			float cosa = (model.mesh.point(to) - model.mesh.point(one_ring_t[i]) | model.mesh.point(one_ring_t[i - 1]) - model.mesh.point(one_ring_t[i])) / ((model.mesh.point(from) - model.mesh.point(one_ring_t[i])).norm()*(model.mesh.point(one_ring_t[i - 1]) - model.mesh.point(one_ring_t[i])).norm());
			float cosb = (model.mesh.point(one_ring_t[0]) - model.mesh.point(one_ring_t[i]) | model.mesh.point(to) - model.mesh.point(one_ring_t[i])) / ((model.mesh.point(one_ring_t[0]) - model.mesh.point(one_ring_t[i])).norm()*(model.mesh.point(to) - model.mesh.point(one_ring_t[i])).norm());
			float cosaa = acos(cosa) * 180 / 3.1415926;
			float cosbb = acos(cosb) * 180 / 3.1415926;
			//std::cout << "角度: " << cosaa << std::endl;
			//std::cout << "角度: " << cosbb << std::endl;
			if (one_ring_t[i] == same_point[0])
			{

				top+=cosaa ;
				top +=cosbb;
				//std::cout << "+" << cosaa << std::endl;
			}
			else if (one_ring_t[i] == same_point[1])
			{
				down+=cosa;
				down+=cosb;
				//std::cout << "-" << cosaa << std::endl;
			}
			//std::cout << "角度" << " : " << cosaa + cosbb << std::endl;
			if ((cosaa + cosbb > 180) && one_ring_t[i] != from)
			{
				//std::cout << "false" << " : " << cosaa + cosbb << std::endl;
				return (OpenMesh::ArrayKernel::VertexHandle) - 1;
				break;
			}
			else
			{
				//std::cout << "true" << " : " << cosaa + cosbb << std::endl;
			}
		}
		else if (i > 0 && i < one_ring_t.size() - 1)
		{
			float cosa = (model.mesh.point(to) - model.mesh.point(one_ring_t[i]) | model.mesh.point(one_ring_t[i - 1]) - model.mesh.point(one_ring_t[i])) / ((model.mesh.point(to) - model.mesh.point(one_ring_t[i])).norm()*(model.mesh.point(one_ring_t[i - 1]) - model.mesh.point(one_ring_t[i])).norm());
			float cosb = (model.mesh.point(one_ring_t[i + 1]) - model.mesh.point(one_ring_t[i]) | model.mesh.point(to) - model.mesh.point(one_ring_t[i])) / ((model.mesh.point(one_ring_t[i + 1]) - model.mesh.point(one_ring_t[i])).norm()*(model.mesh.point(to) - model.mesh.point(one_ring_t[i])).norm());
			
			float cosaa = acos(cosa) * 180 / 3.1415926;
			float cosbb = acos(cosb) * 180 / 3.1415926;
			//std::cout << "角度: " << cosaa << std::endl;
			//std::cout << "角度: " << cosbb << std::endl;
			if (one_ring_t[i] == same_point[0])
			{
				top += cosaa;
				top += cosbb;
				//std::cout << "+" << cosaa << std::endl;
			}
			else if (one_ring_t[i] == same_point[1])
			{
				down += cosaa;
				down += cosbb;
				//std::cout << "-" << cosaa << std::endl;
			}
			//std::cout << "角度" << " : " << cosaa + cosbb << std::endl;
			if ((cosaa + cosbb > 180) && one_ring_t[i] != from)
			{
				//std::cout << "false" << " : " << cosaa + cosbb << std::endl;
				return (OpenMesh::ArrayKernel::VertexHandle) - 1;
				break;				
			}
			else
			{
				//std::cout << "true" << " : " << cosaa + cosbb << std::endl;
			}
		}
		else if (i == 0)
		{
			float cosa =( model.mesh.point(to) - model.mesh.point(one_ring_t[i]) | model.mesh.point(one_ring_t[one_ring_t.size() - 1]) - model.mesh.point(one_ring_t[i])) / ((model.mesh.point(to) - model.mesh.point(one_ring_t[i])).norm()*(model.mesh.point(one_ring_t[one_ring_t.size() - 1]) - model.mesh.point(one_ring_t[i])).norm());
			float cosb = (model.mesh.point(one_ring_t[i + 1]) - model.mesh.point(one_ring_t[i]) | model.mesh.point(to) - model.mesh.point(one_ring_t[i])) / ((model.mesh.point(one_ring_t[i + 1]) - model.mesh.point(one_ring_t[i])).norm()*(model.mesh.point(to) - model.mesh.point(one_ring_t[i])).norm());
			float cosaa = acos(cosa) * 180 / 3.1415926;
			float cosbb = acos(cosb) * 180 / 3.1415926;
			//std::cout << "角度: " << cosaa << std::endl;
			//std::cout << "角度: " << cosbb << std::endl;
			if (one_ring_t[i] == same_point[0])
			{
				top += cosaa;
				top += cosbb;
				//std::cout << "+" << cosaa << std::endl;
			}
			else if (one_ring_t[i] == same_point[1])
			{
				down += cosaa;
				down += cosbb;
				//std::cout << "-" << cosaa << std::endl;
			}
			//std::cout << "角度" << " : " << cosaa + cosbb << std::endl;
			if ((cosaa+cosbb >180) && one_ring_t[i] != from)
			{
				//std::cout << "false" << " : " << cosaa + cosbb << std::endl;
				return (OpenMesh::ArrayKernel::VertexHandle) - 1;
				break;				
			}
			else
			{
				//std::cout << "true" << " : " << cosaa + cosbb << std::endl;
			}
		}
	}
	float cos0=acos(model.mesh.point(to) - model.mesh.point(same_point[0]) | model.mesh.point(from) - model.mesh.point(same_point[0]) / (model.mesh.point(to) - model.mesh.point(same_point[0])).norm()*(model.mesh.point(from) - model.mesh.point(same_point[0])).norm()) * 180 / 3.1415926;
	float cos1=acos(model.mesh.point(to) - model.mesh.point(same_point[1]) | model.mesh.point(from) - model.mesh.point(same_point[1]) / (model.mesh.point(to) - model.mesh.point(same_point[1])).norm()*(model.mesh.point(from) - model.mesh.point(same_point[1])).norm()) * 180 / 3.1415926;
	//std::cout << "角度" << " : " << top - cos0 << std::endl;
	//std::cout << "角度" << " : " << down - cos1 << std::endl;
	if (top - cos0 > 180)
	{
		//std::cout << "samefalse" << " : " << top - cos0 << std::endl;
	    return (OpenMesh::ArrayKernel::VertexHandle) - 1;
	}
	if (down - cos1 > 180)
	{
		//std::cout << "samefalse" << " : " << down - cos1  << std::endl;
		return (OpenMesh::ArrayKernel::VertexHandle) - 1;
	}

	for (MyMesh::VertexEdgeIter ve_it = model.mesh.ve_iter(model.mesh.to_vertex_handle(heh_smallest)); ve_it.is_valid(); ++ve_it)
	{
		//need_to_delete.push_back(*ve_it);
		if (std::find(need_to_delete.begin(), need_to_delete.end(), *ve_it) == need_to_delete.end())
		{
			need_to_delete.push_back(*ve_it);
		}
	}
	for (MyMesh::VertexEdgeIter ve_it = model.mesh.ve_iter(model.mesh.from_vertex_handle(heh_smallest)); ve_it.is_valid(); ++ve_it)
	{
		//need_to_delete.push_back(*ve_it);
		if (std::find(need_to_delete.begin(), need_to_delete.end(), *ve_it) == need_to_delete.end())
		{
			need_to_delete.push_back(*ve_it);
		}
	}
			
	for (MyMesh::VertexOHalfedgeIter voh_it = model.mesh.voh_iter(model.mesh.from_vertex_handle(heh_smallest)); voh_it.is_valid(); ++voh_it)
	{			   
		if (model.mesh.to_vertex_handle(*voh_it) == model.mesh.to_vertex_handle(heh_smallest) )
		{
			if (model.mesh.is_collapse_ok(*voh_it) == false)
			{
				need_to_delete.clear();
				break;
			}
			else if (model.mesh.is_collapse_ok(*voh_it)==true)
			{
				for (int i = 0; i < need_to_delete.size(); i++)
				{
					//std::cout << "need_to_delete[i]" << need_to_delete[i] << std::endl;
					TriMesh::HalfedgeHandle heh_need_delete = model.mesh.halfedge_handle(need_to_delete[i], 0);
					struct headnode find;
					find.cost = model.mesh.property(cost, need_to_delete[i]);						
					find.to = model.mesh.to_vertex_handle(heh_need_delete);							
					find.from = model.mesh.from_vertex_handle(heh_need_delete);
					queue.erase(find);
					//Find_and_Delete_Position(model.mesh.from_vertex_handle(heh_need_delete), model.mesh.to_vertex_handle(heh_need_delete),model.mesh.property(cost, need_to_delete[i]));
				}
				//model.mesh.point(from) = model.mesh.point(to);
				//model.mesh.calc_sector_angle();
				//MyMesh::Point old_v_from = model.mesh.point(from);
				//MyMesh::Point old_v_to = model.mesh.point(from);
				//std::cout << *voh_it << " inital: "<< "from: " << model.mesh.point(model.mesh.from_vertex_handle(*voh_it)) << " to: " << model.mesh.point(model.mesh.to_vertex_handle(*voh_it)) << std::endl;
				model.mesh.collapse(*voh_it);
				//std::cout << *voh_it<<" old: "<< "from: " << model.mesh.point(model.mesh.from_vertex_handle(*voh_it))<<" to: "<< model.mesh.point(model.mesh.to_vertex_handle(*voh_it)) << std::endl;
				//model.mesh.point(model.mesh.from_vertex_handle(*voh_it)) = old_v_from;
				//model.mesh.point(model.mesh.to_vertex_handle(*voh_it)) = old_v_to;
				//std::cout << *voh_it << " new: " <<"from: " << model.mesh.point(model.mesh.from_vertex_handle(*voh_it)) << " to: " << model.mesh.point(model.mesh.to_vertex_handle(*voh_it)) << std::endl;
				break;
			}
		}
	}
	vhandles_new = model.mesh.add_vertex(model.mesh.property(v3, smallest));

	one_ring.clear();
	for (MyMesh::VertexVertexCCWIter vv_it = model.mesh.vv_ccwiter(test); vv_it.is_valid(); ++vv_it)
	{
		//std::cout << " =========== CW   " << *vv_it << std::endl;
		one_ring.push_back(*vv_it);
	}
	//MyMesh::Point old_v = model.mesh.point(test);
	model.mesh.point(test) = model.mesh.point(vhandles_new);

	if (need_to_delete.size() == 0)
	{
		//std::cout << "return -1" << std::endl;
		return (OpenMesh::ArrayKernel::VertexHandle)-1;
	}
	else
	{
		//std::cout << "return test" << std::endl;
		return test;
	}
}
void MeshObject::back()
{
	if (generations.size() != 0) {
		model.mesh2 = generations[generations.size() - 1];
		model.mesh = generations[generations.size() - 1];
		generations.erase(generations.end() - 1);
		std::cout << "check" << std::endl;
		model.LoadToShader();
	}
}
void MeshObject::trackbar_simplification(int index)
{
	model.mesh=simplification_index[index];
	point_number();
	model.LoadToShader();
}
void MeshObject::trackbar_sk_skeleton(int index)
{
	model.mesh = sk_simplification_index[index];
	point_number();
	model.LoadToShader();
}
void MeshObject::time_s(float time_input)
{
	sk_time_num=time_input;
}
void MeshObject::reset()
{
	time_skeleton = 0;
	flag_SK_simplification = true;
	//model.mesh = model.mesh2;
	init = true;
	simplification_index.clear();
	//sk_simplification_index.clear();
	skeleton_index.clear();
	model.mesh.remove_property(SK_Q);

}
Matrix4d MeshObject::caculateQ( OpenMesh::ArrayKernel::VertexHandle  v_it)
{
	double x = model.mesh.point(v_it)[0];
	double y = model.mesh.point(v_it)[1];
	double z = model.mesh.point(v_it)[2];
	int i=0;
	double a = model.mesh.normal(v_it)[0];
	double b = model.mesh.normal(v_it)[1];
	double c = model.mesh.normal(v_it)[2];
	auto  test = model.mesh.normal(v_it);
	test.normalize();
	a = test[0];
	b = test[1];
	c = test[2];
	double d = -test[0] * x - test[1] * y - test[2] * z;
	Matrix4d Q1(4, 4);
	Q1(0, 0) = a * a;  Q1(0, 1) = a * b;  Q1(0, 2) = a * c;  Q1(0, 3) = a * d;
	Q1(1, 0) = a * b;  Q1(1, 1) = b * b;  Q1(1, 2) = b * c;  Q1(1, 3) = b * d;
	Q1(2, 0) = a * c;  Q1(2, 1) = c * b;  Q1(2, 2) = c * c;  Q1(2, 3) = c * d;
	Q1(3, 0) = a * d;  Q1(3, 1) = d * b;  Q1(3, 2) = d * c;  Q1(3, 3) = d * d;
	//std::cout <<"qq1q11q1q1qq11q1q"<< Q1 << std::endl;
	return Q1;
}
double MeshObject::caculateQ3(OpenMesh::ArrayKernel::VertexHandle v_it1, OpenMesh::ArrayKernel::VertexHandle  v_it2 ,OpenMesh::ArrayKernel::EdgeHandle e_it)
{
	
	double cost;
	Matrix4d Q3(4, 4);
	//std::cout << "11111111111111111111111" << std::endl;
	//std::cout << model.mesh.point(v_it1) << std::endl;
	//std::cout << model.mesh.point(v_it2) << std::endl;
	Q3 = model.mesh.property(Q, v_it1) + model.mesh.property(Q, v_it2);
	//std::cout << "Q3"<< Q3 << std::endl;
	Matrix4d Q4(4, 4);
	Q4(0, 0) = Q3(0, 0);  Q4(0, 1) = Q3(0, 1);  Q4(0, 2) = Q3(0, 2);  Q4(0, 3) = Q3(0, 3);
	Q4(1, 0) = Q3(0, 1);  Q4(1, 1) = Q3(1, 1);  Q4(1, 2) = Q3(1, 2);  Q4(1, 3) = Q3(1, 3);
	Q4(2, 0) = Q3(0, 2);  Q4(2, 1) = Q3(1, 2);  Q4(2, 2) = Q3(2, 2);  Q4(2, 3) = Q3(2, 3);
	Q4(3, 0) = 0;         Q4(3, 1) = 0;         Q4(3, 2) = 0;         Q4(3, 3) = 1;
	Vector4d B(4);
	B(0) = B(1) = B(2) = 0;  B(3) = 1;
	Vector4d x;	
	//std::cout << "33333333333333" << std::endl;
	if (Q4.determinant()!=0&& (Q4.inverse()*Q4).determinant()>0.95&& (Q4.inverse()*Q4).determinant() < 1.05&&model.mesh.n_vertices()>3000)
	{
		//x = Q4.bdcSvd(ComputeThinU | ComputeThinV).solve(B);
		//x=Q4.colPivHouseholderQr().solve(B);
		//x = (Q4.transpose()*Q4).ldlt().solve(Q4.transpose()*B);
	//	x = Q4.completeOrthogonalDecomposition().pseudoInverse()*B;
		//Q4 = Q4.completeOrthogonalDecomposition();
		//x = Q4 * B;
		x = Q4.inverse()*B;
		//std::cout << " xxxxxxxxxxxxxx" << std::endl;
	//	std::cout <<" Q4.inverse()" << std::endl;
	//	std::cout << Q4.completeOrthogonalDecomposition().pseudoInverse() << std::endl;
	//	std::cout << "determinant()!=0:    " << std::endl;
	//	std::cout << x    << std::endl;
	//	std::cout << "determinant()!=0:  determinant()!=0: determinant()!=0: determinant()!=0: determinant()!=0: determinant()!=0: determinant()!=0:   " << std::endl;
	//	
	}
	else 
	{
	
		OpenMesh::Vec3d tmp= OpenMesh::Vec3d(model.mesh.point(v_it2) - model.mesh.point(v_it1)) / 2 + (OpenMesh::Vec3d)model.mesh.point(v_it1);
		x(0) = tmp[0]; x(1) = tmp[1]; x(2) = tmp[2]; x(3) = 1;
		//std::cout << " xxxxxxxxxxxxxx2222222222" << std::endl;
		//std::cout << "determinant()==0:    "  << std::endl;
	}
		//std::cout << "444444444444444444" << std::endl;
	//std::cout << e_it << " : " << "Q4Q4Q4Q4Q4Q4Q:    " << Q4 << std::endl;
	cost=x.transpose()*Q3*x;
	if (isnan(cost))
	{
		OpenMesh::Vec3d tmp = OpenMesh::Vec3d(model.mesh.point(v_it2) - model.mesh.point(v_it1)) / 2 + (OpenMesh::Vec3d)model.mesh.point(v_it1);
		x(0) = tmp[0]; x(1) = tmp[1]; x(2) = tmp[2]; x(3) = 1;
		cost = x.transpose()*Q3*x;
		//std::cout <<  "isnan:    "  << std::endl;
	}	
	//std::cout << "5555555555555555555555" << std::endl;
	MyMesh::Point v;
	v[0] = x(0);  v[1] = x(1);  v[2] = x(2);
	
	model.mesh.property(v3, e_it) = v;
	//std::cout << "6666666666666666666666" << std::endl;
	//std::cout <<"v3v3v3v3v3vv33333vv333vb3:    "<< e_it <<" : "<< model.mesh.property(v3, e_it) << std::endl;
	//std::cout << cost << std::endl;	
	return cost;
}
void MeshObject::SK_simplification()
{
	int inner_point_num = 0;
	//CaculateWeight();
	for (MyMesh::VertexIter v_it = model.mesh.vertices_begin(); v_it != model.mesh.vertices_end(); ++v_it)
	{
		inner_point_num++;
	}
	std::cout << "point number:  " << inner_point_num << std::endl;
	if (flag_SK_simplification == true)
	{

		model.mesh.add_property(SK_Q);
		sk_simplification_index.push_back(model.mesh);
		flag_SK_simplification = false;
	}
	model.mesh.add_property(SK);
	for (MyMesh::VertexIter v_it = model.mesh.vertices_begin(); v_it != model.mesh.vertices_end(); ++v_it)
	{
		model.mesh.property(SK_Q, *v_it) = SK_caculateQ(*v_it);
		SK_caculate_dis(*v_it);
	}
	model.mesh.add_property(SK_cost);
	SK_queue.clear();

	struct SK_headnode sk_num;
	double cost_v;
	for (MyMesh::VertexIter v_it = model.mesh.vertices_begin(); v_it != model.mesh.vertices_end(); ++v_it)
	{
		for (MyMesh::VertexOHalfedgeIter voh_it = model.mesh.voh_iter(v_it); voh_it.is_valid(); ++voh_it)
		{
			cost_v = SK_caculateQ3(*voh_it);
			model.mesh.property(SK_cost, *voh_it) = cost_v;
			//std::cout<< model.mesh.property(SK_cost, *voh_it) <<std::endl;
			sk_num.halfedge_id = *voh_it;
			sk_num.cost = model.mesh.property(SK_cost, *voh_it);
			SK_queue.insert(sk_num);
		}
	}

	std::cout << "queue.size():　" << SK_queue.size() << std::endl;
	for (int i = 0; i < sk_time_num; i++)
	{
		inner_point_num--;
		if (SK_queue.size() < 10)
		{			
			break;
		}
		//else if (one==true&&inner_point_num < 3000)
		//{
		//	std::cout << "false　" <<  std::endl;
		//	//one = false;
		//	break;
		//}
		else
		{
			//if (inner_point_num == 4)
			//{
		        //model.mesh.delete_edge((OpenMesh::ArrayKernel::EdgeHandle)1);
				//model.mesh.delete_face((OpenMesh::ArrayKernel::FaceHandle)1);
				//model.mesh.delete_face((OpenMesh::ArrayKernel::FaceHandle)2);
			//	break;
			//}

			//這裡
			//std::cout << "queue.size():　" << SK_queue.size() << std::endl;
	
				
			OpenMesh::ArrayKernel::HalfedgeHandle smallest = SK_Top(SK_queue);
			
			//bool flag=SK_is_collaspe_ok(smallest);
			//std::cout << "smallest:　" << smallest << std::endl;
			//這裡
		   if ((model.mesh.is_collapse_ok(smallest)==true)&& model.mesh.property(SK_cost, smallest)<0.0005)
			{
				int detect = SK_simplification2(smallest);
			//	//std::cout << detect << std::endl;
			}	
		   else if(model.mesh.property(SK_cost, smallest) >0.0005)
		   {
			   //std::cout << "smallest:　" << smallest << std::endl;
			   break;
		   }
			//else 
			/*if ((model.mesh.is_collapse_ok(smallest) == false) )
			{*/
				/*OpenMesh::ArrayKernel::VertexHandle to = model.mesh.to_vertex_handle(smallest);
				OpenMesh::ArrayKernel::VertexHandle from = model.mesh.from_vertex_handle(smallest);
				MyMesh::Point new_from = (model.mesh.point(from) - model.mesh.point(to))*0.1+ model.mesh.point(to);
				model.mesh.point(from) = new_from;*/
			//}
		}
	}
	model.mesh.delete_isolated_vertices();
	model.mesh.garbage_collection();
	model.mesh.update_face_normals();
	model.mesh.update_vertex_normals();
	model.mesh.remove_property(SK);
	model.mesh.remove_property(SK_cost);
	model.mesh2.clean();
	model.mesh2.garbage_collection();
	model.mesh2 = model.mesh;
	sk_simplification_index.push_back(model.mesh);
	model.LoadToShader();
}
void MeshObject::finetune()
{
	model.mesh.delete_face((OpenMesh::ArrayKernel::FaceHandle)420);
	model.mesh.delete_face((OpenMesh::ArrayKernel::FaceHandle)419);
	model.mesh.delete_face((OpenMesh::ArrayKernel::FaceHandle)178);
	model.mesh.delete_face((OpenMesh::ArrayKernel::FaceHandle)173);
	model.mesh.delete_face((OpenMesh::ArrayKernel::FaceHandle)457);
	model.mesh.delete_face((OpenMesh::ArrayKernel::FaceHandle)456);
	model.mesh.delete_face((OpenMesh::ArrayKernel::FaceHandle)269);
	model.mesh.delete_face((OpenMesh::ArrayKernel::FaceHandle)99);
	model.mesh.delete_face((OpenMesh::ArrayKernel::FaceHandle)59);
	model.mesh.delete_face((OpenMesh::ArrayKernel::FaceHandle)58);
	model.mesh.delete_face((OpenMesh::ArrayKernel::FaceHandle)57);
	model.mesh.update_face_normals();
	model.mesh.update_vertex_normals();
	model.mesh.delete_isolated_vertices();
	model.mesh.garbage_collection();
	model.mesh2.clean();
	model.mesh2.garbage_collection();
	model.mesh2 = model.mesh;
	model.LoadToShader();
}
int MeshObject::SK_simplification2(OpenMesh::ArrayKernel::HalfedgeHandle smallest)
{
	OpenMesh::ArrayKernel::VertexHandle from, to, test;
	std::vector<OpenMesh::ArrayKernel::HalfedgeHandle> need_to_delete;
	std::vector<OpenMesh::ArrayKernel::EdgeHandle> need_to_delete_edge;
	need_to_delete.clear();
	need_to_delete_edge.clear();
	test = to = model.mesh.to_vertex_handle(smallest);
	from = model.mesh.from_vertex_handle(smallest);

	//std::vector<OpenMesh::ArrayKernel::VertexHandle> ring_vertex;
	//ring_vertex.clear();
	for (MyMesh::VertexEdgeIter ve_it = model.mesh.ve_iter(to); ve_it.is_valid(); ++ve_it)
	{

		OpenMesh::ArrayKernel::HalfedgeHandle he0 = model.mesh.halfedge_handle(*ve_it, 0);
		OpenMesh::ArrayKernel::HalfedgeHandle he1 = model.mesh.halfedge_handle(*ve_it, 1);
		if (std::find(need_to_delete.begin(), need_to_delete.end(), he0) == need_to_delete.end())
		{
			need_to_delete.push_back(he0);
		}
		if (std::find(need_to_delete.begin(), need_to_delete.end(), he1) == need_to_delete.end())
		{
			need_to_delete.push_back(he1);
		}
		/*if (std::find(need_to_delete_edge.begin(), need_to_delete_edge.end(), *ve_it) == need_to_delete_edge.end())
		{
			need_to_delete_edge.push_back(*ve_it);
		}	*/
	}

	for (MyMesh::VertexEdgeIter ve_it = model.mesh.ve_iter(from); ve_it.is_valid(); ++ve_it)
	{
		OpenMesh::ArrayKernel::HalfedgeHandle he0 = model.mesh.halfedge_handle(*ve_it, 0);
		OpenMesh::ArrayKernel::HalfedgeHandle he1 = model.mesh.halfedge_handle(*ve_it, 1);
		if (std::find(need_to_delete.begin(), need_to_delete.end(), he0) == need_to_delete.end())
		{
			need_to_delete.push_back(he0);
		}
		if (std::find(need_to_delete.begin(), need_to_delete.end(), he1) == need_to_delete.end())
		{
			need_to_delete.push_back(he1);
		}
		/*if (std::find(need_to_delete_edge.begin(), need_to_delete_edge.end(), *ve_it) == need_to_delete_edge.end())
		{
			need_to_delete_edge.push_back(*ve_it);
		}*/
	}
	///*for (MyMesh::VertexVertexIter vv_it = model.mesh.vv_iter(to); vv_it.is_valid(); ++vv_it)
	//{

	//	if (std::find(ring_vertex.begin(), ring_vertex.end(), *vv_it) == ring_vertex.end())
	//	{
	//		ring_vertex.push_back(*vv_it);
	//	}
	//}*/
	///*for (MyMesh::VertexVertexIter vv_it = model.mesh.vv_iter(from); vv_it.is_valid(); ++vv_it)
	//{
	//	if (std::find(ring_vertex.begin(), ring_vertex.end(), *vv_it) == ring_vertex.end()&&*vv_it!=to)
	//	{
	//		ring_vertex.push_back(*vv_it);
	//	}
	//}*/

	//MyMesh::Point new_point = (model.mesh.point(to) + model.mesh.point(from)) / 2;
	/*for (int i = 0; i < need_to_delete_edge.size(); i++)
	{
		model.mesh.delete_edge(need_to_delete_edge[i],false);
	} */

	///*int num = 0;
	//for (int i = 0; i < ring_vertex.size(); i++)
	//{
	//	if (model.mesh.is_isolated(ring_vertex[i]))
	//	{
	//		num++;
	//	}
	//}
	//std::cout <<"num: "<< num << std::endl;*/
	
	if (true)
	{
		for (int i = 0; i < need_to_delete.size(); i++)
		{
			struct SK_headnode find;
			find.cost = model.mesh.property(SK_cost, need_to_delete[i]);
			find.halfedge_id = need_to_delete[i];
			SK_queue.erase(find);
		}
	//	//OpenMesh::ArrayKernel::VertexHandle new_handle = model.mesh.add_vertex(new_point);
	//	ring.clear();
	//	for (MyMesh::VertexIter v_it = model.mesh.vertices_begin(); v_it != model.mesh.vertices_end(); ++v_it)
	//	{

	//		if (!(model.mesh.status(*v_it).deleted()) && model.mesh.is_boundary(*v_it) && !(model.mesh.is_isolated(*v_it)))
	//		{
	//			//std::cout << "=============================================================================" << std::endl;
	//			//std::cout << "*v_it is_boundary: " << *v_it << std::endl;
	//			OpenMesh::ArrayKernel::HalfedgeHandle now_0 = model.mesh.halfedge_handle(*v_it);
	//			OpenMesh::ArrayKernel::VertexHandle now_0_from = model.mesh.from_vertex_handle(now_0);
	//			OpenMesh::ArrayKernel::HalfedgeHandle next_0 = model.mesh.next_halfedge_handle(now_0);
	//			OpenMesh::ArrayKernel::VertexHandle next_0_from = model.mesh.from_vertex_handle(next_0);
	//			//std::cout << "now_0: " << now_0 <<  " next_0: " << next_0  << std::endl;
	//			//std::cout << "now_0_from: " << now_0_from << " next_0_from: " << next_0_from << std::endl;
	//			ring.push_back(next_0_from);
	//			while (next_0 != now_0) {
	//				next_0 = model.mesh.next_halfedge_handle(next_0);
	//				//std::cout  << " next_0: " << next_0  << std::endl;
	//				next_0_from = model.mesh.from_vertex_handle(next_0);
	//				//std::cout <<  " next_0_from: " << next_0_from << std::endl;
	//				//if(next_0_from!=to)
	//				ring.push_back(next_0_from);
	//			}
	//			break;
	//		}
	//	}
	//	face_vhandles.clear();
	//	for (int i = 0; i < ring.size(); i++)
	//	{
	//		//std::cout <<"裡"<< one_ring[i] << std::endl;
	//		if (i == ring.size() - 1)
	//		{
	//			face_vhandles.clear();
	//			face_vhandles.push_back(ring[i]);
	//			face_vhandles.push_back(ring[0]);
	//			face_vhandles.push_back(to);
	//			OpenMesh::ArrayKernel::FaceHandle tmp_face = model.mesh.add_face(face_vhandles);
	//			//model.mesh.set_normal(tmp_face, model.mesh.calc_face_normal(tmp_face));
	//		}
	//		else if (i < ring.size() - 1)
	//		{
	//			face_vhandles.clear();
	//			face_vhandles.push_back(ring[i]);
	//			face_vhandles.push_back(ring[i + 1]);
	//			face_vhandles.push_back(to);
	//			OpenMesh::ArrayKernel::FaceHandle tmp_face = model.mesh.add_face(face_vhandles);
	//			//model.mesh.set_normal(tmp_face, model.mesh.calc_face_normal(tmp_face));
	//		}
	//	}
	//	/*model.mesh.update_normal(new_handle);
	//	for (int i = 0; i < ring.size(); i++)
	//	{
	//		for (MyMesh::VertexFaceIter vf_it = model.mesh.vf_iter(ring[i]); vf_it.is_valid(); ++vf_it)
	//		{
	//			model.mesh.update_normal(*vf_it);
	//		}
	//		model.mesh.update_normal(ring[i]);
	//	}*/
	////}
	////else
	////{
	////	need_to_delete.clear();
	////}
	//////std::cout << "444444444444444444 " << std::endl;
	/////*for (int i = 0; i < ring.size(); i++)
	////{
	////	for (MyMesh::VertexVertexIter vv_it = model.mesh.vv_iter(ring[i]); vv_it.is_valid(); ++vv_it)
	////	{
	////		model.mesh.update_normal(*vv_it);
	////	}
	////}*/
		model.mesh.collapse(smallest);
		/*OpenMesh::ArrayKernel::VertexHandle to = model.mesh.to_vertex_handle(smallest);
		OpenMesh::ArrayKernel::VertexHandle from = model.mesh.from_vertex_handle(smallest);
		MyMesh::Point new_from = (model.mesh.point(from) - model.mesh.point(to))*0.1 + model.mesh.point(to);
		model.mesh.point(from) = new_from;*/
	}
	else
	{
		need_to_delete.clear();
	}
	
	
	model.mesh.property(SK_Q, (OpenMesh::ArrayKernel::VertexHandle)to) = model.mesh.property(SK_Q, (OpenMesh::ArrayKernel::VertexHandle)to)+ model.mesh.property(SK_Q, (OpenMesh::ArrayKernel::VertexHandle)from);
	SK_caculate_dis(to);
	double cost_v;
	struct SK_headnode NEW;
	for (MyMesh::VertexOHalfedgeIter voh_it = model.mesh.voh_iter(to); voh_it.is_valid(); ++voh_it)
	{
		cost_v = SK_caculateQ3(*voh_it);
		model.mesh.property(SK_cost, *voh_it) = cost_v;
		NEW.halfedge_id = *voh_it;
		NEW.cost = model.mesh.property(SK_cost, *voh_it);
		SK_queue.insert(NEW);	
	}
	for (MyMesh::VertexIHalfedgeIter vih_it = model.mesh.vih_iter(to); vih_it.is_valid(); ++vih_it)
	{
		cost_v = SK_caculateQ3(*vih_it);
		model.mesh.property(SK_cost, *vih_it) = cost_v;
		NEW.halfedge_id = *vih_it;
		NEW.cost = model.mesh.property(SK_cost, *vih_it);
		SK_queue.insert(NEW);		
	}
	

	if (need_to_delete.size() == 0)
	{
		return  - 1;
	}
	else
	{
		return 0;
	}
	
}
Matrix4d MeshObject::SK_caculateQ(OpenMesh::ArrayKernel::VertexHandle  v_it)
{
	//std::cout << "00000000000000 " << std::endl;
	Matrix4d Q= Matrix4d::Zero();
	MatrixXd K(3,4);
	//std::cout << "111111111111111 " << std::endl;
	for (MyMesh::VertexOHalfedgeIter voh_it = model.mesh.voh_iter(v_it); voh_it.is_valid(); ++voh_it)
	{
		OpenMesh::ArrayKernel::VertexHandle to = model.mesh.to_vertex_handle(voh_it);
		OpenMesh::Vec3d a = (OpenMesh::Vec3d)(model.mesh.point(to) - model.mesh.point(v_it));
		a.normalize();
		OpenMesh::Vec3d b = a % (OpenMesh::Vec3d)model.mesh.point(v_it);
		//std::cout <<"b"<< b << std::endl;
		//std::cout << "222222222222222222 " << std::endl;
		K << 0,-a[2],a[1],-b[0],
			a[2],0,-a[0],-b[1],
			-a[1],a[0],0,-b[2];
		//std::cout << "333333333333333333 " << std::endl;
		Q += (K.transpose()*K);
		//std::cout << "44444444444444" << std::endl;
	}		
	return Q;
}
void MeshObject::SK_caculate_dis(OpenMesh::ArrayKernel::VertexHandle v_it)
{
	double total_distance = 0;
	for (MyMesh::VertexOHalfedgeIter voh_it = model.mesh.voh_iter(v_it); voh_it.is_valid(); ++voh_it)
	{
		OpenMesh::ArrayKernel::VertexHandle to = model.mesh.to_vertex_handle(*voh_it);
		OpenMesh::Vec3d adj_distance = (OpenMesh::Vec3d)(model.mesh.point(v_it) - model.mesh.point(to));
		total_distance += sqrt(adj_distance[0] * adj_distance[0] + adj_distance[1] * adj_distance[1] + adj_distance[2] * adj_distance[2]);
	}
	model.mesh.property(SK,v_it)= total_distance;
	
}
double MeshObject::SK_caculateQ3(OpenMesh::ArrayKernel::HalfedgeHandle he_it)
{
	double cost;

	OpenMesh::ArrayKernel::VertexHandle from = model.mesh.from_vertex_handle(he_it);
	OpenMesh::ArrayKernel::VertexHandle to = model.mesh.to_vertex_handle(he_it);
	Matrix4d Q1 = model.mesh.property(SK_Q, from);
	Matrix4d Q2 = model.mesh.property(SK_Q, to);
	Vector4d p(model.mesh.point(to)[0], model.mesh.point(to)[1], model.mesh.point(to)[2], 1);
	MatrixXd Fa = p.transpose()*Q1*p + p.transpose()*Q2*p;
	//std::cout<<he_it<<" : "<<Fa<<std::endl;
	OpenMesh::Vec3d dis = (OpenMesh::Vec3d)(model.mesh.point(from) - model.mesh.point(to));
	double Fb = model.mesh.property(SK,from)* sqrt(dis[0] * dis[0] + dis[1] * dis[1] + dis[2] * dis[2]);
	//std::cout << he_it << " : " << Fb << std::endl;
	cost = 1.0f*Fa(0,0) + 0.1f*Fb;
	return cost;
}
void MeshObject::Render()
{
	glBindVertexArray(model.vao);
	if (model.mode == 1)
	{
		glDrawElements(GL_TRIANGLES, model.mesh.n_faces() * 3, GL_UNSIGNED_INT, 0);
	}
	else if (model.mode == 2)
	{
		glDrawElements(GL_LINES, model.mesh.n_edges() * 2, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);		
}
void MeshObject::Render_Point()
{
	glBindVertexArray(model.pvao);
	if (model.mode == 1)
	{
		glDrawElements(GL_LINES, model.mesh.n_edges() * 2, GL_UNSIGNED_INT, 0);
	}
	else if (model.mode == 2)
	{
		glDrawElements(GL_POINTS, model.mesh.n_edges() * 2, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);	
	/*glBindVertexArray(model.myvao);
	glBindBuffer(GL_ARRAY_BUFFER, model.myvbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.myebo);
	glDrawElements(GL_POINTS, model.my_wire.size()*2, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);*/

}

void MeshObject::RenderSelectedFace()
{
	if (selectedFace.size() > 0)
	{

		std::vector<unsigned int*> offsets(selectedFace.size());
		for (int i = 0; i < offsets.size(); ++i)
		{
			offsets[i] = (GLuint*)(selectedFace[i] * 3 * sizeof(GLuint));
		}

		std::vector<int> count(selectedFace.size(), 3);

		glBindVertexArray(model.vao);
		glMultiDrawElements(GL_TRIANGLES, &count[0], GL_UNSIGNED_INT, (const GLvoid **)&offsets[0], selectedFace.size());
		glBindVertexArray(0);
	}
}

bool MeshObject::AddSelectedFace(unsigned int faceID)
{	
	if (std::find(selectedFace.begin(), selectedFace.end(), faceID) == selectedFace.end() &&
		faceID >= 0 && faceID < model.mesh.n_faces())
	{
		selectedFace.push_back(faceID);
		return true;
	}
	return false;
}


void MeshObject::DeleteSelectedFace(unsigned int faceID)
{
	selectedFace.erase(std::remove(selectedFace.begin(), selectedFace.end(), faceID), selectedFace.end());
}

bool MeshObject::FindClosestPoint(unsigned int faceID, glm::vec3 worldPos, glm::vec3& closestPos)
{
	OpenMesh::FaceHandle fh = model.mesh.face_handle(faceID);
	if (!fh.is_valid())
	{
		return false;
	}

	double minDistance = 0.0;
	MyMesh::Point p(worldPos.x, worldPos.y, worldPos.z);
	MyMesh::FVIter fv_it = model.mesh.fv_iter(fh);
	MyMesh::VertexHandle closestVH = *fv_it;
	MyMesh::Point v1 = model.mesh.point(*fv_it);
	++fv_it;

	minDistance = (p - v1).norm();
	for (; fv_it.is_valid(); ++fv_it)
	{
		MyMesh::Point v = model.mesh.point(*fv_it);
		double distance = (p - v).norm();
		if (minDistance > distance)
		{
			minDistance = distance;
			closestVH = *fv_it;
		}
	}
	MyMesh::Point closestPoint = model.mesh.point(closestVH);
	closestPos.x = closestPoint[0];
	closestPos.y = closestPoint[1];
	closestPos.z = closestPoint[2];
	return true;
}


//void MeshObject::swap(struct headnode &p1, struct headnode &p2) {
//	struct headnode temp = p1;
//	p1 = p2;
//	p2 = temp;
//}

//void MeshObject::MaxHeapify(std::vector<struct headnode> &array, int root, int length) {
//
//	int left = 2 * root,      // 取得left child
//		right = 2 * root + 1,     // 取得right child
//		largest;                // largest用來記錄包含root與child, 三者之中Key最大的node
//
//	if (left <= length && array[left].cost < array[root].cost)
//		largest = left;
//	else
//		largest = root;
//
//	if (right <= length && array[right].cost < array[largest].cost)
//		largest = right;
//
//	if (largest != root) {                         // 如果目前root的Key不是三者中的最大
//		swap(array[largest], array[root]);         // 就調換root與三者中Key最大的node之位置
//		MaxHeapify(array, largest, length);        // 調整新的subtree成Max Heap
//	}
//}
//
//void MeshObject::BuildMaxHeap(std::vector<struct headnode> &array) {
//
//	for (int i = (int)array.size() / 2; i >= 1; i--) {
//		MaxHeapify(array, i, (int)array.size() - 1);     // length要減一, 因為heap從1開始存放資料
//	}
//}
//
//void MeshObject::HeapSort(std::vector<struct headnode> &array,bool inital) {
//	
//	if (inital == true)
//	{
//		array[0].cost = 0;                   // 將index(0)閒置
//		array[0].from = (OpenMesh::ArrayKernel::VertexHandle) - 1;
//		array[0].to = (OpenMesh::ArrayKernel::VertexHandle) - 1;
//	}
//	//std::cout << "aaaaaaaaaa" << std::endl;
//	BuildMaxHeap(array);                                // 將array調整成Max Heap
//	//std::cout << "bbbbbbbbbbbb" << std::endl;
//	//int size = (int)array.size() - 1;                    // size用來記錄「目前要處理的矩陣」之長度
//	//for (int i = (int)array.size() - 1; i >= 2; i--) {
//	//	swap(array[1], array[i]);                       // 將最大值放到array的最後一個位置
//	//	size--;
//	//	MaxHeapify(array, 1, size);                     // 調整「忽略最後一個位置」的矩陣
//	//}
//
//
//	//std::cout << "ccccccccccccc" << std::endl;
//	if (inital == true)
//	{
//		array.erase(array.begin());                         // 將index(0)刪除
//	}
//}
//
//
//void MeshObject::Find_and_Delete_Position(OpenMesh::ArrayKernel::VertexHandle from, OpenMesh::ArrayKernel::VertexHandle to,double cost) {
//	
//	if (cost < (queue[0].cost + queue[queue.size() - 1].cost) / 2)
//	{
//		for (int i = 0; i < queue.size(); i++)
//		{
//			if (cost == queue[i].cost)
//			{
//				if ((from == queue[i].from&& to == queue[i].to) || (to == queue[i].from&& from == queue[i].to))
//				{
//					//swap(queue[0], queue[i]);
//					//queue.erase(queue.begin());
//					queue.erase(queue.begin() + i);
//					break;
//				}
//			}
//
//		}
//	}
//	else
//	{
//		for (int i = queue.size() - 1; i >= 0; i--)
//		{
//			if (cost == queue[i].cost)
//			{
//				if ((from == queue[i].from&& to == queue[i].to) || (to == queue[i].from&& from == queue[i].to))
//				{
//					//swap(queue[0], queue[i]);
//					//queue.erase(queue.begin());
//					queue.erase(queue.begin() + i);
//					break;
//				}
//			}
//
//		}
//	}
//	
//}

OpenMesh::ArrayKernel::EdgeHandle MeshObject::Top(std::set<struct headnode> &array)
{
	
	MyMesh::HalfedgeHandle heh = model.mesh.find_halfedge(array.begin()->from, array.begin()->to);
	OpenMesh::ArrayKernel::EdgeHandle smallest = model.mesh.edge_handle(heh);		
	array.erase(array.begin());
	
	return smallest;
}
OpenMesh::ArrayKernel::HalfedgeHandle MeshObject::SK_Top(std::set<struct SK_headnode> &array)
{

	OpenMesh::ArrayKernel::HalfedgeHandle smallest = array.begin()->halfedge_id;
	array.erase(array.begin());
	return smallest;
}

//bool MeshObject::small(std::vector<struct headnode> &array,float epsilon)
//{
//	float smallest_cost = array[0].cost;
//	//std::cout << array[0].cost << std::endl;
//	if (array[0].cost < epsilon)
//		return true;
//	else
//		return false;
//}


//void MeshObject::Insert(std::vector<struct headnode> &array)
//{
//	for (int i = array.size() - 1; i > 0; i--)
//	{
//		if (array[i].cost < array[i - 1].cost)
//		{
//			swap(array[i], array[i-1]);
//		}	
//		else 
//		{
//			break;
//		}
//	}
//}



//OpenMesh::ArrayKernel::EdgeHandle MeshObject::Find(struct weight node, std::vector<struct weight> &array)
//{
//	for (int i = 0; i < array.size(); i++) {
//		if ((array[i].from == node.from&& array[i].to == node.to)|| (array[i].from == node.to&& array[i].to == node.from)){
//			return array[i].edge_id;
//		}
//
//	}
//}
//void MeshObject::PrintArray(std::vector<struct headnode> &array) {
//	for (int i = 0; i < array.size(); i++) {
//		//std::cout << array[i] << " ";
//	}
//	std::cout << std::endl;
//}

float MeshObject::cotan(OpenMesh::Vec3d a, OpenMesh::Vec3d b) {
	//std::cout << (a | b) / ((a%b).norm()) << std::endl;
	return (a | b) / ((a%b).norm());
}

bool  MeshObject::SK_is_collaspe_ok(OpenMesh::ArrayKernel::HalfedgeHandle smallest)
{

	OpenMesh::ArrayKernel::VertexHandle to_smallest = model.mesh.to_vertex_handle(smallest);
	OpenMesh::ArrayKernel::VertexHandle from_smallest = model.mesh.from_vertex_handle(smallest);
	std::vector<OpenMesh::ArrayKernel::VertexHandle> test_ring;
	for (MyMesh::VertexVertexIter vv_it = model.mesh.vv_iter(to_smallest); vv_it.is_valid(); ++vv_it)
	{
		if (std::find(test_ring.begin(), test_ring.end(), *vv_it) == test_ring.end())
		{
			test_ring.push_back(*vv_it);
		}
	}
	for (MyMesh::VertexVertexIter vv_it = model.mesh.vv_iter(from_smallest); vv_it.is_valid(); ++vv_it)
	{
		if (std::find(test_ring.begin(), test_ring.end(), *vv_it) == test_ring.end())
		{
			test_ring.push_back(*vv_it);
		}
	}
	for (int i = 0; i < test_ring.size(); i++)
	{
		int num = 0;
		int num_e = 0;
		for (MyMesh::VertexFaceIter vf_it = model.mesh.vf_iter(test_ring[i]); vf_it.is_valid(); ++vf_it)
		{
			num++;
		}
		for (MyMesh::VertexEdgeIter ve_it = model.mesh.ve_iter(test_ring[i]); ve_it.is_valid(); ++ve_it)
		{
			num_e++;
		}
		if ((num == 2 && num_e == 4)|| num <2 ||(num_e==num*2))
		{
			return false;
		}
	}
	int num = 0;
	int num_e = 0;
	for (MyMesh::VertexFaceIter vf_it = model.mesh.vf_iter(to_smallest); vf_it.is_valid(); ++vf_it)
	{
		num++;
	}
	for (MyMesh::VertexEdgeIter ve_it = model.mesh.ve_iter(to_smallest); ve_it.is_valid(); ++ve_it)
	{
		num_e++;
	}
	if ((num == 2&& num_e==4) || num < 2 || (num_e == num * 2))
	{
		return false;
	}
	num = 0;
	num_e = 0;
	for (MyMesh::VertexFaceIter vf_it = model.mesh.vf_iter(from_smallest); vf_it.is_valid(); ++vf_it)
	{
		num++;
	}
	for (MyMesh::VertexEdgeIter ve_it = model.mesh.ve_iter(from_smallest); ve_it.is_valid(); ++ve_it)
	{
		num_e++;
	}
	if ((num == 2&&num_e==4) || num < 2 || (num_e == num * 2))
	{
		return false;
	}		
	return true;
	/*OpenMesh::ArrayKernel::HalfedgeHandle opp_smallest = model.mesh.opposite_halfedge_handle(smallest);

	OpenMesh::ArrayKernel::HalfedgeHandle smallest_loop = model.mesh.next_halfedge_handle(model.mesh.next_halfedge_handle(model.mesh.next_halfedge_handle(smallest)));
	OpenMesh::ArrayKernel::HalfedgeHandle opp_smallest_loop = model.mesh.next_halfedge_handle(model.mesh.next_halfedge_handle(model.mesh.next_halfedge_handle(opp_smallest)));
	if (smallest_loop != smallest)
	{
		return false;
	}
	else if (opp_smallest_loop != opp_smallest)
	{
		return false;
	}
	else
	{
		return true;
	}*/
}

int MeshObject::point_number()
{
	int point_num = model.mesh.n_vertices();
	return point_num;
}

