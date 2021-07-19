#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(10001, OnButtonClicked)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, 
							wxID_ANY, 
							"Sudoku solver", 
							wxPoint(30, 30), 
							wxSize(465,540), 
							wxMINIMIZE_BOX | wxCLOSE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN)
{

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			m_txt[x][y] = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(50 * x, 50 * y), wxSize(50, 50));
		}
	}

	m_btn_populate = new wxButton(this, 10001, "Populate", wxPoint(0, 450), wxSize(225, 50));
	m_btn_solve = new wxButton(this, 10002, "Solve", wxPoint(225, 450), wxSize(223, 50));
}

cMain::~cMain()
{
}

void cMain::OnButtonClicked(wxCommandEvent &evt)
{
	evt.Skip();
}
