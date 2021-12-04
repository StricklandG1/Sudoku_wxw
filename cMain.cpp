#include "cMain.h"
#include "constants.h"
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/sstream.h>
#include <stack>

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(10001, OnPopulateClicked)
	EVT_BUTTON(10002, OnSolveClicked)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, 
							wxID_ANY, 
							"Sudoku solver", 
							wxPoint(FRAME_X, FRAME_Y),
							wxSize(FRAME_WIDTH, FRAME_HEIGHT),
							wxMINIMIZE_BOX | wxCLOSE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN)
{

	for (int x = 0; x < WIDTH; ++x)
	{
		for (int y = 0; y < HEIGHT; ++y)
		{
			m_lbl[x][y] = new wxStaticText(this, wxID_ANY, "0", wxPoint(TXT_X * x, TXT_Y * y + 10), wxSize(TXT_WIDTH, TXT_HEIGHT),wxALIGN_CENTER);
			board[x][y] = 0;
		}
	}

	m_btn_populate = new wxButton(this, 10001, "Populate", wxPoint(POP_BTN_X, POP_BTN_Y), wxSize(POP_BTN_WIDTH, POP_BTN_HEIGHT));
	m_btn_solve = new wxButton(this, 10002, "Solve", wxPoint(SOLVE_BTN_X, SOLVE_BTN_Y), wxSize(SOLVE_BTN_WIDTH, SOLVE_BTN_HEIGHT));
	m_fpicker = new wxFilePickerCtrl(this, 10003, "", "", "", wxPoint(FILE_CTRL_X, FILE_CTRL_Y), wxSize(FILE_CTRL_WIDTH, FILE_CTRL_HEIGHT));
	time_elapsed = new wxStaticText(this, 10004, "Time Elapsed: ", wxPoint(150, 430), wxSize(150, 15));
	time_text = new wxStaticText(this, 10005, "0.0", wxPoint(225, 430), wxSize(150, 15));

}

cMain::~cMain()
{
}

bool cMain::populate_board()
{
	wxString file = m_fpicker->GetPath();
	wxString temp;
	wxFileInputStream in_file(file);
	wxTextInputStream in_text(in_file, wxT("\x09"), wxConvUTF8);
	
	if (in_file.GetLastError() == wxSTREAM_NO_ERROR)
	{
		for (int i = 0; i < 9; ++i)
		{
			temp = in_text.ReadLine();
			wxStringInputStream sstream(temp);

			for (int j = 0; j < 9; ++j)
			{
				board[j][i] = sstream.GetC() - '0';
				sstream.GetC();
			}
		}
	}

	matrix_to_lbl();
	return false;
}

void cMain::solve_board()
{
	int start_y, start_x;
	get_first_move(start_y, start_x);

	int y = start_y;
	int x = start_x;

	std::stack<std::pair<int, std::pair<int, int>>> moves;

	bool solved = false;

	int num = 1;
	while (!solved && x < 9 && y < 9)
	{
		bool valid = check_num(num, y, x);
		if (valid)
		{
			moves.push({ num, { y, x } });
			board[y][x] = num;

			while (board[y][x] != 0)
			{
				y += (x == 8) ? 1 : 0;
				x = (x + 1) % 9;
			}
			num = 1;
		}
		else
		{
			if (num >= 9)
			{
				while (moves.top().first == 9)
				{
					board[moves.top().second.first][moves.top().second.second] = 0;
					moves.pop();
				}
				y = moves.top().second.first;
				x = moves.top().second.second;
				moves.pop();
				num = board[y][x] + 1;
				board[y][x] = 0;
			}
			else
			{
				++num;
			}
		}

		solved = check_solved_board();
		matrix_to_lbl();
		m_time.Pause();
		time_text->SetLabelText(wxString::Format(wxT("%f"), m_time.Time() * .001));
		m_time.Resume();
	}
	m_time.Pause();
}

void cMain::get_first_move(int& start_y, int& start_x)
{
	bool found = false;
	int i = 0;
	while (!found && i < 9)
	{
		int j = 0;
		while (!found && j < 9)
		{
			if (board[i][j] == 0)
			{
				found = true;
				start_y = i;
				start_x = j;
			}
			++j;
		}
		++i;
	}
}

bool cMain::check_num(int num, int row_num, int col_num)
{
	if (num > 9)
	{
		return false;
	}

	bool row_valid = check_row(num, board[row_num]);
	bool col_valid = check_col(num, col_num);
	bool grid_valid = check_local_grid(num, row_num / 3, col_num / 3);

	return (row_valid && col_valid && grid_valid);
}

bool cMain::check_row(int num, int board[])
{
	int i = 0;
	for (int i = 0; i < 9; ++i)
	{
		if (num == board[i])
		{
			return false;
		}
	}
	return true;
}

bool cMain::check_col(int num, int col_num)
{
	for (int i = 0; i < 9; ++i)
	{
		if (num == (board[i][col_num]))
		{
			return false;
		}
	}
	return true;
}

bool cMain::check_local_grid(int num, int grid_row, int grid_col)
{
	int row_start = grid_row * 3;
	int col_start = grid_col * 3;

	for (int i = row_start; i < row_start + 3; ++i)
	{
		for (int j = col_start; j < col_start + 3; ++j)
		{
			if (num == board[i][j])
			{
				return false;
			}
		}
	}
	return true;
}

bool cMain::check_solved_board()
{
	bool solved = true;

	int i = 0;
	while (solved && i < 9)
	{
		solved = check_solved_row(board[i]);
		solved = check_solved_col(i);
		++i;
	}

	i = 0;
	int j = 0;
	while (solved && i < 3)
	{
		j = 0;
		while (solved && j < 3)
		{
			solved = check_solved_grid(i, j);
			++j;
		}
		++i;
	}

	return solved;
}

bool cMain::check_solved_row(int board[])
{
	int nums[10] = { 0 };
	for (int i = 0; i < 9; ++i)
	{
		++nums[board[i]];
	}

	for (int i = 1; i < 10; ++i)
	{
		if (nums[i] != 1)
		{
			return false;
		}
	}

	return true;
}

bool cMain::check_solved_col(int col_num)
{
	int nums[10] = { 0 };
	for (int i = 0; i < 9; ++i)
	{
		++nums[board[i][col_num]];
	}

	for (int i = 1; i < 10; ++i)
	{
		if (nums[i] != 1)
		{
			return false;
		}
	}

	return true;
}

bool cMain::check_solved_grid(int grid_row, int grid_col)
{
	int row_start = grid_row * 3;
	int col_start = grid_row * 3;
	int nums[10] = { 0 };

	for (int i = row_start; i < row_start + 3; ++i)
	{
		for (int j = col_start; j < col_start + 3; ++j)
		{
			++nums[board[i][j]];
		}
	}

	bool valid = true;
	int i = 1;
	while (valid && i < 10)
	{
		valid = (nums[i] == 1);
		++i;
	}

	return valid;
}

void cMain::txt_to_matrix()
{
	for (int i = 0; i < cMain::HEIGHT; ++i)
	{
		for (int j = 0; j < cMain::WIDTH; ++j)
		{
			board[i][j] = wxAtoi(m_txt[i][j]->GetValue());
		}
	}
}

void cMain::matrix_to_txt()
{
	wxString temp;
	for (int i = 0; i < cMain::HEIGHT; ++i)
	{
		for (int j = 0; j < cMain::WIDTH; ++j)
		{
			temp = "";
			temp << board[i][j];
			m_txt[j][i]->Clear();
			m_txt[j][i]->SetValue(temp);
		}
	}
}

void cMain::lbl_to_matrix()
{
	for (int i = 0; i < cMain::HEIGHT; ++i)
	{
		for (int j = 0; j < cMain::WIDTH; ++j)
		{
			board[i][j] = wxAtoi(m_lbl[i][j]->GetLabelText());
		}
	}
}

void cMain::matrix_to_lbl()
{
	wxString temp;
	for (int i = 0; i < cMain::HEIGHT; ++i)
	{
		for (int j = 0; j < cMain::WIDTH; ++j)
		{
			temp = "";
			temp << board[i][j];
			m_lbl[i][j]->SetLabelText(temp);
		}
	}
}

void cMain::OnPopulateClicked(wxCommandEvent &evt)
{
	populate_board();
	evt.Skip();
}

void cMain::OnSolveClicked(wxCommandEvent &evt)
{
	m_time.Start(0);
	solve_board();
	evt.Skip();
}