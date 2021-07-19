#pragma once

#include "wx/wx.h"
class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

	wxButton* m_btn_populate = nullptr;
	wxButton* m_btn_solve = nullptr;
	wxTextCtrl* m_txt[9][9];

	int width = 9;
	int height = 9;

	void OnButtonClicked(wxCommandEvent &evt);

	wxDECLARE_EVENT_TABLE();
};

