#pragma once
#include "wx/wx.h"
#include <wx/filepicker.h>

class cMain : public wxFrame
{
private:
	const static int WIDTH = 9;
	const static int HEIGHT = 9;

	wxButton* m_btn_populate = nullptr;
	wxButton* m_btn_solve = nullptr;
	wxFilePickerCtrl* m_fpicker;
	wxTextCtrl* m_txt[HEIGHT][WIDTH];
	wxStaticText* m_lbl[HEIGHT][WIDTH];
	wxStaticText* time_elapsed;
	wxStaticText* time_text;
	wxStopWatch m_time;

public:
	cMain();
	~cMain();

	int board[HEIGHT][WIDTH];

	bool populate_board();
	void solve_board();
	void get_first_move(int& start_y, int& start_x);
	bool check_num(int num, int row_num, int col_num);
	bool check_row(int num, int board[]);
	bool check_col(int num, int col_num);
	bool check_local_grid(int num, int grid_row, int grid_col);
	bool check_solved_board();
	bool check_solved_row(int board[]);
	bool check_solved_col(int col_num);
	bool check_solved_grid(int grid_row, int grid_col);

	void txt_to_matrix();
	void matrix_to_txt();

	void lbl_to_matrix();
	void matrix_to_lbl();

	void OnPopulateClicked(wxCommandEvent &evt);
	void OnSolveClicked(wxCommandEvent &evt);
	wxDECLARE_EVENT_TABLE();
};

