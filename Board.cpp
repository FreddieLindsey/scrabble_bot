#include "Board.hpp"

Board::Board(string& name_, int& width_, int& height_, rapidjson::Value& mods)
  : name(name_), width(width_), height(height_) {
  this->board.resize(width);
  for (size_t i = 0; i < this->board.size(); i++) {
    this->board[i].resize(height);
    for (int j = 0; j < width; j++) {
      this->board[i][j] = new BoardMarker();
    }
  }

  if (!mods["double_l"].IsNull()) {
    Modifier m = DOUBLE_L;
    apply_modifier_array(m, mods["double_l"]);
  }

  if (!mods["triple_l"].IsNull()) {
    Modifier m = TRIPLE_L;
    apply_modifier_array(m, mods["triple_l"]);
  }

  if (!mods["double_w"].IsNull()) {
    Modifier m = DOUBLE_W;
    apply_modifier_array(m, mods["double_w"]);
  }

  if (!mods["triple_w"].IsNull()) {
    Modifier m = TRIPLE_W;
    apply_modifier_array(m, mods["triple_w"]);
  }

  print_board(*this);
}

void Board::apply_modifier_array(Modifier& m, rapidjson::Value& mod_array) {
  assert(mod_array.IsArray());
  for (rapidjson::SizeType i = 0; i < mod_array.Size(); i++) {
    int x = mod_array[i]["x"].GetInt();
    int y = mod_array[i]["y"].GetInt();
    this->board[x - 1][y - 1]->apply_modifier(m);
  }
}

bool Board::set_char(char c, int w, int h) {
  if ( w < this->width && h < this->height
    && w >= 0 && h >= 0) {
    return this->board[w][h]->set_char(c);
  }
  return false;
}

void Board::print_board(Board& b) {
  for (int j = (int) this->board[0].size() - 1; j > -1 ; j--) {
    for (size_t i = 0; i < this->board.size(); i++) {
      this->board[i][j]->print_marker();
    }
    cout << endl;
  }
}