#include "MyForm.h"
//#include <iostream>
using namespace System;
using namespace System::Windows::Forms;
//using namespace std;
[STAThread]
void main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	OpenMesh_EX::MyForm myForm;
	Application::Run(%myForm);
}