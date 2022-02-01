#pragma once

//#include "GL/glew.h"
//#include "GLFW/glfw3.h"
//#include "ResourcePath.h"
#include "MeshObject.h"
#include "DrawModelShader.h"
#include "drawpoint.h"
//#include "PickingShader.h"
//#include "PickingTexture.h"
//#include "DrawPickingFaceShader.h"
#include "ViewManager.h"
#include "DotNetUtilities.h"
//#include "DrawPointShader.h"
//#include "Mesh/GUA_OM.h"
//#include "Mesh/DP.h"
//#include "Mesh/XForm.h"
//#include "Mesh/GLCamera.h"

//const std::string ProjectName = "OpenMesh_EX";
glm::vec3 worldPos;
bool updateFlag = false;

GLuint faceID=0;
//bool isRightButtonPress = false;
GLuint currentFaceID = 0;
int currentMouseX = 0;
int currentMouseY = 0;
int windowWidth = 800;
int windowHeight = 600; 
int selectionMode = 1;
GLuint			program;			// shader program
glm::mat4		proj_matrix;		// projection matrix
float			aspect;
ViewManager		meshWindowCam;
GLuint texture;
MeshObject model;

// shaders
DrawModelShader drawModelShader;
drawpoint drawpoints;
//DrawPickingFaceShader drawPickingFaceShader;
//PickingShader pickingShader;
//PickingTexture pickingTexture;
//DrawPointShader drawPointShader;

// vbo for drawing point
GLuint vboPoint;


//Tri_Mesh *mesh;

//xform xf;
//GLCamera camera;
//float fov = 0.7f;

/*static const Mouse::button physical_to_logical_map[] = {
	Mouse::NONE, Mouse::ROTATE, Mouse::MOVEXY, Mouse::MOVEZ,
	Mouse::MOVEZ, Mouse::MOVEXY, Mouse::MOVEXY, Mouse::MOVEXY,
};
Mouse::button Mouse_State = Mouse::ROTATE;*/

namespace OpenMesh_EX {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO:  在此加入建構函式程式碼
			//
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadModelToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  openModelDialog;
	private: System::Windows::Forms::SaveFileDialog^  saveModelDialog;
	private: System::Windows::Forms::ToolStripMenuItem^  saveModelToolStripMenuItem;
	private: HKOGLPanel::HKOGLPanelControl^  hkoglPanelControl1;


	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;

	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::TrackBar^  trackBar1;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::TrackBar^  trackBar2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip2;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip3;
	private: System::Windows::Forms::TrackBar^  trackBar3;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::Label^  label3;
	private: System::ComponentModel::IContainer^  components;


	protected:

	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			HKOGLPanel::HKCOGLPanelCameraSetting^  hkcoglPanelCameraSetting2 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^  hkcoglPanelPixelFormat2 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openModelDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveModelDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->trackBar3 = (gcnew System::Windows::Forms::TrackBar());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->trackBar2 = (gcnew System::Windows::Forms::TrackBar());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->contextMenuStrip2 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->contextMenuStrip3 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->BeginInit();
			this->panel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1000, 24);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->loadModelToolStripMenuItem,
					this->saveModelToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(38, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// loadModelToolStripMenuItem
			// 
			this->loadModelToolStripMenuItem->Name = L"loadModelToolStripMenuItem";
			this->loadModelToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->loadModelToolStripMenuItem->Text = L"Load Model";
			this->loadModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::loadModelToolStripMenuItem_Click);
			// 
			// saveModelToolStripMenuItem
			// 
			this->saveModelToolStripMenuItem->Name = L"saveModelToolStripMenuItem";
			this->saveModelToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->saveModelToolStripMenuItem->Text = L"Save Model";
			this->saveModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::saveModelToolStripMenuItem_Click);
			// 
			// openModelDialog
			// 
			this->openModelDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::openModelDialog_FileOk);
			// 
			// saveModelDialog
			// 
			this->saveModelDialog->DefaultExt = L"obj";
			this->saveModelDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::saveModelDialog_FileOk);
			// 
			// hkoglPanelControl1
			// 
			this->hkoglPanelControl1->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			hkcoglPanelCameraSetting2->Far = 1000;
			hkcoglPanelCameraSetting2->Fov = 45;
			hkcoglPanelCameraSetting2->Near = -1000;
			hkcoglPanelCameraSetting2->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting2;
			this->hkoglPanelControl1->Cursor = System::Windows::Forms::Cursors::Default;
			this->hkoglPanelControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->hkoglPanelControl1->Location = System::Drawing::Point(0, 24);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcoglPanelPixelFormat2->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat2->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat2->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat2;
			this->hkoglPanelControl1->Size = System::Drawing::Size(1000, 600);
			this->hkoglPanelControl1->TabIndex = 2;
			this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl1_Load);
			this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl1_Paint);
			this->hkoglPanelControl1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::hkoglPanelControl1_KeyPress);
			this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseDown);
			this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseMove);
			this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseWheel);
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(43, 395);
			this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 50000, 0, 0, 0 });
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(120, 22);
			this->numericUpDown1->TabIndex = 5;
			this->numericUpDown1->ValueChanged += gcnew System::EventHandler(this, &MyForm::numericUpDown1_ValueChanged);
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->Location = System::Drawing::Point(43, 86);
			this->numericUpDown2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 50000, 0, 0, 0 });
			this->numericUpDown2->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 196608 });
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(125, 22);
			this->numericUpDown2->TabIndex = 6;
			this->numericUpDown2->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 196608 });
			this->numericUpDown2->ValueChanged += gcnew System::EventHandler(this, &MyForm::numericUpDown2_ValueChanged);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(9, 31);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(87, 39);
			this->button1->TabIndex = 3;
			this->button1->Text = L"Skeleton";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(9, 334);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(87, 39);
			this->button2->TabIndex = 4;
			this->button2->Text = L"Simplification";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->textBox2);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->trackBar3);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->textBox1);
			this->panel1->Controls->Add(this->button4);
			this->panel1->Controls->Add(this->trackBar2);
			this->panel1->Controls->Add(this->trackBar1);
			this->panel1->Controls->Add(this->checkBox1);
			this->panel1->Controls->Add(this->button3);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->button2);
			this->panel1->Controls->Add(this->numericUpDown1);
			this->panel1->Controls->Add(this->button1);
			this->panel1->Controls->Add(this->numericUpDown2);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Right;
			this->panel1->Location = System::Drawing::Point(791, 24);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(209, 600);
			this->panel1->TabIndex = 9;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(137, 190);
			this->textBox2->Name = L"textBox2";
			this->textBox2->ShortcutsEnabled = false;
			this->textBox2->Size = System::Drawing::Size(60, 22);
			this->textBox2->TabIndex = 17;
			this->textBox2->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox2_TextChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(135, 162);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(65, 12);
			this->label3->TabIndex = 16;
			this->label3->Text = L"剩餘點數量";
			// 
			// trackBar3
			// 
			this->trackBar3->Location = System::Drawing::Point(43, 250);
			this->trackBar3->Maximum = 15;
			this->trackBar3->Name = L"trackBar3";
			this->trackBar3->Size = System::Drawing::Size(104, 45);
			this->trackBar3->TabIndex = 15;
			this->trackBar3->Scroll += gcnew System::EventHandler(this, &MyForm::trackBar3_Scroll);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(132, 315);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(65, 12);
			this->label2->TabIndex = 14;
			this->label2->Text = L"剩餘點數量";
			this->label2->Click += gcnew System::EventHandler(this, &MyForm::label2_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(137, 344);
			this->textBox1->Name = L"textBox1";
			this->textBox1->ShortcutsEnabled = false;
			this->textBox1->Size = System::Drawing::Size(60, 22);
			this->textBox1->TabIndex = 13;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(3, 180);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(100, 39);
			this->button4->TabIndex = 12;
			this->button4->Text = L"sk_simplification";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// trackBar2
			// 
			this->trackBar2->Location = System::Drawing::Point(43, 129);
			this->trackBar2->Maximum = 11;
			this->trackBar2->Name = L"trackBar2";
			this->trackBar2->Size = System::Drawing::Size(104, 45);
			this->trackBar2->TabIndex = 11;
			this->trackBar2->Scroll += gcnew System::EventHandler(this, &MyForm::trackBar2_Scroll);
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(43, 439);
			this->trackBar1->Maximum = 15;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(104, 45);
			this->trackBar1->TabIndex = 10;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &MyForm::trackBar1_Scroll);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(17, 561);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(79, 16);
			this->checkBox1->TabIndex = 9;
			this->checkBox1->Text = L"draw_mode";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &MyForm::checkBox1_CheckedChanged);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(9, 490);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(87, 39);
			this->button3->TabIndex = 8;
			this->button3->Text = L"reset";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"標楷體", 26.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->label1->Location = System::Drawing::Point(3, 73);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(34, 35);
			this->label1->TabIndex = 7;
			this->label1->Text = L"s";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(61, 4);
			// 
			// contextMenuStrip2
			// 
			this->contextMenuStrip2->Name = L"contextMenuStrip2";
			this->contextMenuStrip2->Size = System::Drawing::Size(61, 4);
			// 
			// contextMenuStrip3
			// 
			this->contextMenuStrip3->Name = L"contextMenuStrip3";
			this->contextMenuStrip3->Size = System::Drawing::Size(61, 4);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1000, 624);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->hkoglPanelControl1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"OpenMesh_EX";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void hkoglPanelControl1_Load(System::Object^  sender, System::EventArgs^  e)
{
	    GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			std::cout << "GLEW is not initialized!\n";
			return;
		}
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_PROGRAM_POINT_SIZE);

		drawModelShader.Init();
		drawpoints.Init();				
}
//畫
private: System::Void hkoglPanelControl1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
{
	
	aspect = windowWidth * 1.0f / windowHeight;
	glm::mat4 mvMat = meshWindowCam.GetViewMatrix()*meshWindowCam.GetModelMatrix();
	glm::mat4 pMat = meshWindowCam.GetProjectionMatrix(aspect);


	// write faceID+1 to framebuffer


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	model.Render();

	// draw model
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	//glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPushMatrix();
	//glMatrixMode(GL_MODELVIEW);
	//glMultMatrixd((double *)xf);

	//model.Render_Point();
	
	//glPopMatrix();
	drawpoints.Enable();
	glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(mvMat)));
	if (model.model.mode == 2)
		drawpoints.SetFaceColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	else
		drawpoints.SetFaceColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	drawpoints.UseLighting(false);
	drawpoints.SetNormalMat(normalMat);
	drawpoints.SetMVMat(mvMat);
	drawpoints.SetPMat(pMat);
	model.Render_Point();
	drawpoints.Disable();
	drawModelShader.Enable();
	

	//drawModelShader.SetWireColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	if(model.model.mode==2)
		drawModelShader.SetFaceColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	else
		drawModelShader.SetFaceColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	//drawModelShader.SetPointColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	drawModelShader.UseLighting(true);
	//drawModelShader.DrawWireframe(true);
	//drawModelShader.DrawPoint(true);
	drawModelShader.SetNormalMat(normalMat);
	drawModelShader.SetMVMat(mvMat);
	drawModelShader.SetPMat(pMat);
	model.Render();
	drawModelShader.Disable();
	

}
//滑鼠按下去
private: System::Void hkoglPanelControl1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if (e->Button == System::Windows::Forms::MouseButtons::Right ||
		e->Button == System::Windows::Forms::MouseButtons::Middle)
	{
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Left )
	{
		meshWindowCam.mouseEvents(0, 1, e->X, e->Y);
	}
}
//滑鼠轉動
private: System::Void hkoglPanelControl1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if (e->Button == System::Windows::Forms::MouseButtons::Left)
	{
		meshWindowCam.mouseMoveEvent(e->X, e->Y);
		hkoglPanelControl1->Invalidate();
	}

	if (e->Button == System::Windows::Forms::MouseButtons::Right)
	{
		hkoglPanelControl1->Invalidate();
	}
}
//滑鼠滾輪
private: System::Void hkoglPanelControl1_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if (e->Delta < 0)
	{
		meshWindowCam.mouseEvents(3, 3, e->X, e->Y);
		hkoglPanelControl1->Invalidate();
	}
	else
	{
		meshWindowCam.mouseEvents(4, 4, e->X, e->Y);
		hkoglPanelControl1->Invalidate();
	}
}
	private: System::Void hkoglPanelControl1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^ e)
	{
		meshWindowCam.keyEvents(e->KeyChar);
		if (e->KeyChar == 'w' || e->KeyChar == 'W') {
			meshWindowCam.keyEvents(e->KeyChar);

			hkoglPanelControl1->Invalidate();
			//e.Handled = true;
		}
		else if (e->KeyChar == 'a' || e->KeyChar == 'A') {
			meshWindowCam.keyEvents(e->KeyChar);

			hkoglPanelControl1->Invalidate();
			//e.Handled = true;
		}
		else if (e->KeyChar == 's' || e->KeyChar == 'S') {
			meshWindowCam.keyEvents(e->KeyChar);

			hkoglPanelControl1->Invalidate();
			//e.Handled = true;
		}
		else if (e->KeyChar == 'd' || e->KeyChar == 'D') {
			meshWindowCam.keyEvents(e->KeyChar);

			hkoglPanelControl1->Invalidate();
			//e.Handled = true;
		}
		else if (e->KeyChar == '-') {
			meshWindowCam.keyEvents(e->KeyChar);

			hkoglPanelControl1->Invalidate();
			//e.Handled = true;
		}
		else if (e->KeyChar == '+') {
			meshWindowCam.keyEvents(e->KeyChar);

			hkoglPanelControl1->Invalidate();
			//e.Handled = true;
		}
		
		else if (e->KeyChar == '5') {
			model.back();
			hkoglPanelControl1->Invalidate();
		}
		else if (e->KeyChar == '6') {
			model.Skeleton();

			hkoglPanelControl1->Invalidate();
		}
		else if (e->KeyChar == '7') {
			for (int i = 0; i < 1; i++)
			{
			model.SK_simplification();
			}
			hkoglPanelControl1->Invalidate();
		}
		else if (e->KeyChar == '8') {

			model.finetune();

			hkoglPanelControl1->Invalidate();
		}
	}
//按下load model選單
private: System::Void loadModelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
	openModelDialog->Filter = "Model(*.obj)|*obj";
	openModelDialog->Multiselect = false;
	openModelDialog->ShowDialog();
}
//載入檔案
private: System::Void openModelDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
{
	std::string filename;
	
	MarshalString(openModelDialog->FileName, filename);

	//if (mesh != NULL)
		//delete mesh;

	//mesh = new Tri_Mesh;
	if (model.Init(filename))
		{
			std::cout << "Load Model"<<filename << std::endl;
		}
		else
		{
			std::cout << "Load Model Failed" << std::endl;
		}
	hkoglPanelControl1->Invalidate();
}
//按下save model選單
private: System::Void saveModelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
	saveModelDialog->Filter = "Model(*.obj)|*obj";
	saveModelDialog->ShowDialog();
}
//輸出檔案
private: System::Void saveModelDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
{
	std::string filename;
	MarshalString(saveModelDialog->FileName, filename);
	model.End(filename);
	//if (SaveFile(filename, mesh))
		//std::cout << filename << std::endl;
	hkoglPanelControl1->Invalidate();
}

private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
{
	for (int i = 0; i < 11; i++)
	{
		model.Skeleton();		
	}
	hkoglPanelControl1->Invalidate();
}
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) 
{
	for (int i = 0; i < 16; i++)
	{
		if(i<9)
			model.Simplification(5000);
		else if(i>=9&&i<13)
			model.Simplification(1000);
		else if(i==13)
			model.Simplification(500);
		else if(i==14)
			model.Simplification(300);
		else if (i == 15)
			model.Simplification(150);
	}
	//model.Simplification((float)numericUpDown1->Value);
	hkoglPanelControl1->Invalidate();
}
private: System::Void numericUpDown2_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
{
	//numericUpDown2->Value = 50;
	model.s_num((float)numericUpDown2->Value);
}
private: System::Void tableLayoutPanel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
}

private: System::Void numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
{
	model.time_s((float)numericUpDown1->Value);
}
private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) 
{
	model.reset();
	hkoglPanelControl1->Invalidate();
}
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (checkBox1->Checked == true)
		model.model.mode = 2;
	else
		model.model.mode = 1;
	model.model.LoadToShader();
	hkoglPanelControl1->Invalidate();
}
private: System::Void trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e) {

	model.trackbar_simplification(trackBar1->Value);
	int text = model.point_number();
	textBox1->Text = "" + text;
	hkoglPanelControl1->Invalidate();
}
private: System::Void trackBar2_Scroll(System::Object^  sender, System::EventArgs^  e) {

	model.trackbar_skeleton(trackBar2->Value);
	hkoglPanelControl1->Invalidate();	
}
private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {

	
	hkoglPanelControl1->Invalidate();
}
private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
	for (int i = 0; i < 7; i++)
	{
		model.time_s(10000);
		model.SK_simplification();
	}
	for (int i = 0; i < 11; i++)
	{
		model.s_num(4);
		model.Skeleton();
	}
	model.reset();
	for (int i = 0; i < 7; i++)
	{
		model.time_s(00);
		model.SK_simplification();
	}
	model.SK_simplification();
	hkoglPanelControl1->Invalidate();
}
private: System::Void trackBar3_Scroll(System::Object^  sender, System::EventArgs^  e) {

	model.trackbar_sk_skeleton(trackBar3->Value);
	int text = model.point_number();
	textBox2->Text = "" + text;
	hkoglPanelControl1->Invalidate();
}
private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	hkoglPanelControl1->Invalidate();
}
};
}
