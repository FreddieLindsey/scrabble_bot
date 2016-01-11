#include "Game.hpp"

Game::Game(options o) : players(o.players), this_player_go(o.this_player_go), no_tiles_each(7) { // TODO: NO_TILES_EACH
  // Generate board and start playing
  rapidjson::Document d = get_config_from_file(o.board_config);
  string board_name(d["board_name"].GetString());
  int width = d["board_width"].GetInt();
  int height = d["board_height"].GetInt();
  rapidjson::Value& mods(d["modifiers"]);
  rapidjson::Value& scores(d["scores"]);
  this->b = new Board(board_name, width, height, mods, scores);

  assert(o.players > 0 && o.this_player_go > 0 && o.this_player_go <= o.players);

  cout << "Players:\t" << this->players << endl;
  cout << "Your Go:\t" << this->this_player_go << endl;
  cout << endl;
  cout << "Board:\t" << endl;
  this->b->set_word("I WILL BEAT YOU", 0, 7, EAST);
  this->b->print_board();

  int go = 0;
  while (!this->is_end()) {
    int player_turn = go % o.players;
    if (player_turn == o.this_player_go - 1) {
      cout << "Your go!" << endl;
      // this->player_go();
    } else {
      cout << "Opponent's go:\tPlayer" << (player_turn + 1) << endl;
      this->opponent_go();
      this->b->print_board();
    }
    go++;
  }
}

rapidjson::Document get_config_from_file(string& config) {
  ifstream config_file(config);

  // get length of file:
  config_file.seekg (0, config_file.end);
  int length = config_file.tellg();
  config_file.seekg (0, config_file.beg);
  char* config_file_raw = new char[length];

  config_file.read(config_file_raw, length);

  rapidjson::Document d;
  d.Parse(config_file_raw);

  return d;
}

bool Game::is_end(void) {
  return false;
}

void Game::opponent_go(void) {
  while (true) {
    string input;

    // For putting on the Board
    string word;
    int x, y;
    Direction d;

    while (true) {
      string word_regex("[A-Za-z]{1,");
      word_regex.append(to_string(this->no_tiles_each));
      word_regex.append("}");
      regex validator(word_regex);

      // Get the word
      cout << "Please enter the word (if you type more than 1 word, the second will be ignored):\t";
      cin >> input;
      cin.clear();
      cin.ignore(INT_MAX, '\n');
      if (regex_match(input, validator) && valid_word_for_game(input)) {
        word = input;
        break;
      }
      cout << "The word you entered was invalid. Please try again." << endl;
    }

    while (true) {
      regex y_("^[A-Za-z]$");

      // Get the position
      cout << "Please enter the position on the board (e.g. 1 A):\t";
      cin >> input;
      try {
        x = stoi(input);
        if (x <= 0 || x > this->b->get_width()) {
          throw invalid_argument("");
        }
        x -= 1; // Convert from user to program
      } catch (invalid_argument e) {
        cout << "The value " << input << " is not in the range {1,"
             << this->b->get_width() << "}."<< endl;
        continue;
      }

      cin >> input;
      if (!regex_match(input, y_)) {
        continue;
      }
      y = (int) toupper(input[0]) - 'A';

      cin.clear();
      cin.ignore(INT_MAX, '\n');

      if (valid_position_for_game(x, y)) {
        break;
      }
    }

    while (true) {
      // Get the word
      cout << "Please enter the direction (NORTH|EAST|SOUTH|WEST):\t";
      cin >> input;
      cin.clear();
      cin.ignore(INT_MAX, '\n');

      for (auto& c : input) {
        c = toupper(c);
      }

      if (input.compare("NORTH") == 0) {
        d = NORTH; break;
      } else if (input.compare("EAST") == 0) {
        d = EAST; break;
      } else if (input.compare("SOUTH") == 0) {
        d = SOUTH; break;
      } else if (input.compare("WEST") == 0) {
        d = WEST; break;
      }

      cout << "Incorrect direction. Please try again!" << endl;
    }

    if (can_put_word_on_board(word, x, y, d)) {
      this->b->set_word(word, x, y, d);
      break;
    }

    cout << "Incorrect parameters specified. Please try again!" << endl;
  }
}

void Game::player_go(void) {

}

bool Game::valid_word_for_game(string& input) {
  return true;
}

bool Game::valid_position_for_game(int& x, int& y) {
  return true;
}

bool Game::can_put_word_on_board(string& word, int& w, int& h, Direction& d) {
  return true;
}
