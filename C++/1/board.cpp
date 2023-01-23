#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <limits>

#include "board.h"

// ==================================================================
// ==================================================================
// Implementation of the Position class


// allows a position to be output to a stream
std::ostream& operator<<(std::ostream &ostr, const Position &p) {
  ostr << '(' << p.row << "," << p.col << ')';
  return ostr;
}


// equality and inequality comparision of positions
bool operator==(const Position &a, const Position &b) {
  return (a.row == b.row && a.col == b.col);
}
bool operator!=(const Position &a, const Position &b) {
  return !(a==b);
}
bool operator<(const Position &a, const Position &b) {
	return a.row < b.row || (a.row == b.row && a.col < b.col);
}


// ==================================================================
// ==================================================================
// Implementation of the Board class


// ===================
// CONSTRUCTOR
// ===================

Board::Board(int r, int c) : rows(r), cols(c) {
  // initialize the dimensions
  //rows = r;
  //cols = c;

  // allocate space for the contents of each grid cell
  board = std::vector<std::vector<char> >(rows,std::vector<char>(cols,' '));

  // allocate space for booleans indicating the presense of each wall
  // by default, these are false == no wall
  // (note that there must be an extra column of vertical walls
  //  and an extra row of horizontal walls)
  vertical_walls = std::vector<std::vector<bool> >(rows,std::vector<bool>(cols+1,false));
  horizontal_walls = std::vector<std::vector<bool> >(rows+1,std::vector<bool>(cols,false));

  // initialize the outermost edges of the grid to have walls
  for (int i = 0; i < rows; i++) {
    vertical_walls[i][0] = vertical_walls[i][cols] = true;
  }
  for (int i = 0; i < cols; i++) {
    horizontal_walls[0][i] = horizontal_walls[rows][i] = true;
  }
}


// ===================
// ACCESSORS related to board geometry
// ===================

// Query the existance of a horizontal wall
bool Board::getHorizontalWall(double r, int c) const {
  // verify that the requested wall is valid
  // the row coordinate must be a half unit
  assert (fabs((r - floor(r))-0.5) < 0.005);
  assert (r >= 0.4 && r <= rows+0.6);
  assert (c >= 1 && c <= cols);
  // subtract one and round down because the corner is (0,0) not (1,1)
  return horizontal_walls[floor(r)][c-1];
}

// Query the existance of a vertical wall
bool Board::getVerticalWall(int r, double c) const {
  // verify that the requested wall is valid
  // the column coordinate must be a half unit
  assert (fabs((c - floor(c))-0.5) < 0.005);
  assert (r >= 1 && r <= rows);
  assert (c >= 0.4 && c <= cols+0.6);
  // subtract one and round down because the corner is (0,0) not (1,1)
  return vertical_walls[r-1][floor(c)];
}


// ===================
// MODIFIERS related to board geometry
// ===================

// Add an interior horizontal wall
void Board::addHorizontalWall(double r, int c) {
  // verify that the requested wall is valid
  // the row coordinate must be a half unit
  assert (fabs((r - floor(r))-0.5) < 0.005);
  assert (r >= 0 && r <= rows);
  assert (c >= 1 && c <= cols);
  // verify that the wall does not already exist
  assert (horizontal_walls[floor(r)][c-1] == false);
  // subtract one and round down because the corner is (0,0) not (1,1)
  horizontal_walls[floor(r)][c-1] = true;
}

// Add an interior vertical wall
void Board::addVerticalWall(int r, double c) {
  // verify that the requested wall is valid
  // the column coordinate must be a half unit
  assert (fabs((c - floor(c))-0.5) < 0.005);
  assert (r >= 1 && r <= rows);
  assert (c >= 0 && c <= cols);
  // verify that the wall does not already exist
  assert (vertical_walls[r-1][floor(c)] == false);
  // subtract one and round down because the corner is (0,0) not (1,1)
  vertical_walls[r-1][floor(c)] = true;
}


// ===================
// PRIVATE HELPER FUNCTIONS related to board geometry
// ===================

char Board::getspot(const Position &p) const {
  // verify that the requested coordinate is valid
  assert (p.row >= 1 && p.row <= rows);
  assert (p.col >= 1 && p.col <= cols);
  // subtract one from each coordinate because the corner is (0,0) not (1,1)
  return board[p.row-1][p.col-1];
}


void Board::setspot(const Position &p, char a) {
  // verify that the requested coordinate is valid
  assert (p.row >= 1 && p.row <=  rows);
  assert (p.col >= 1 && p.col <= cols);
  // subtract one from each coordinate because the corner is (0,0) not (1,1)
  board[p.row-1][p.col-1] = a;
}

char Board::isGoal(const Position &p) const {
  // verify that the requested coordinate is valid
  assert (p.row >= 1 && p.row <= rows);
  assert (p.col >= 1 && p.col <= cols);
  // loop over the goals, see if any match this spot
  for (unsigned int i = 0; i < goals.size(); i++) {
    if (p == goals[i].pos) { return goals[i].which; }
  }
  // else return space indicating that no goal is at this location
  return ' ';
}


// ===================
// MODIFIERS related to robots
// ===================

// for initial placement of a new robot
void Board::placeRobot(const Position &p, char a) {

  // check that input data is reasonable
  assert (p.row >= 1 && p.row <= rows);
  assert (p.col >= 1 && p.col <= cols);
  assert (getspot(p) == ' ');
  
  // robots must be represented by a capital letter
  assert (isalpha(a) && isupper(a));

  // make sure we don't already have a robot with the same name
  for (unsigned int i = 0; i < robots.size(); i++) {
    assert (robots[i].which != a);
  }

  // add the robot and its position to the vector of robots
  robots.push_back(Robot(p,a));

  // mark the robot on the board
  setspot(p,a);
}


// ===================
// MODIFIER related to the puzzle goal
// ===================

void Board::addGoal(const std::string &gr, const Position &p) {

  // check that input data is reasonable
  assert (p.row >= 1 && p.row <= rows);
  assert (p.col >= 1 && p.col <= cols);

  char goal_robot;
  if (gr == "any") {
    goal_robot = '?';
  } else {
    assert (gr.size() == 1);
    goal_robot = gr[0];
    assert (isalpha(goal_robot) && isupper(goal_robot));
  }

  // verify that a robot of this name exists for this puzzle
  if (goal_robot != '?') {
    int robot_exists = false;
    for (unsigned int i = 0; i < robots.size(); i++) {
      if (getRobot(i) == goal_robot) 
        robot_exists = true;
    }
    assert (robot_exists);
  }
  
  // make sure we don't already have a robot at that location
  assert (isGoal(p) == ' ');

  // add this goal label and position to the vector of goals
  goals.push_back(Goal(p,goal_robot));
}


// ==================================================================
// PRINT THE BOARD
// ==================================================================

void Board::print(BoardState* bs) {

  // print the column headings
  std::cout << " ";
  for (int j = 1; j <= cols; j++) {
    std::cout << std::setw(5) << j;
  }
  std::cout << "\n";
  
  // for each row
  for (int i = 0; i <= rows; i++) {

    // don't print row 0 (it doesnt exist, the first real row is row 1)
    if (i > 0) {
      
      // Note that each grid rows is printed as 3 rows of text, plus
      // the separator.  The first and third rows are blank except for
      // vertical walls.  The middle row has the row heading, the
      // robot positions, and the goals.  Robots are always uppercase,
      // goals are always lowercase (or '?' for any).
      std::string first = "  ";
      std::string middle;
      for (int j = 0; j <= cols; j++) {

        if (j > 0) { 
          // determine if a robot is current located in this cell
          // and/or if this is the goal
          Position p(i,j);
          char c = bs ? bs->findPosForPrint(p) : getspot(p);
          char g = isGoal(p);
          if (g != '?') g = tolower(g);
          first += "    ";
          middle += " "; 
          middle += c; 
          middle += g; 
          middle += " ";
        }

        // the vertical walls
        if (getVerticalWall(i,j+0.5)) {
          first += "|";
          middle += "|";
        } else {
          first += " ";
          middle += " ";
        }
      }

      // output the three rows
      std::cout << first << std::endl;
      std::cout << std::setw(2) << i << middle << std::endl;
      std::cout << first << std::endl;
    }

    // print the horizontal walls between rows
    std::cout << "  +";
    for (double j = 1; j <= cols; j++) {
      (getHorizontalWall(i+0.5,j)) ? std::cout << "----" : std::cout << "    ";
      std::cout << "+";
    }
    std::cout << "\n";
  }
}

bool Board::stateVisited(const BoardState &s) {
	return visited_states.find(s) != visited_states.end();
}

//This function is used to get an arbitory solution of path
void Board::findArbitorySolution(int max_moves) {
	assert(max_moves > 0);

	BoardState bs = initState();
	saveState(bs);
	visited_states.insert(bs);
	states_queue.push(bs);

	ArbitoryPathAction action(*this, max_moves);
	traverse(0, action);

	//go over and print
	action.print();
	std::cout<<"\n";
}

void Board::findAllSolutions(int max_moves) {
	BoardState bs = initState();
	saveState(bs);
	visited_states.insert(bs);
	states_queue.push(bs);

	// First calculate the lenth of the path
	AllPathAction action(*this, max_moves);
	traverse(0, action);

	if(action.used_moves < 0) {
		std::cout<<"no solutions with "<<max_moves<<" or fewer moves\n";
		return;
	}

	//Using dfs to find all the path equaled to action.used_moves
	path.resize(action.used_moves+1);
	path[0] = bs;
	dfs(1, action.used_moves);

	//go over and print
	print();
	std::cout<<all_solutions.size()<<" different "<<action.used_moves<<" move solutions:\n";
	for(size_t i = 0; i < all_solutions.size(); ++i) {
		std::vector<BoardState>& va = all_solutions[i];
		for(size_t j = 0; j < va.size(); ++j) {
			BoardState& a = va[j];
			std::cout<<a.move_desc<<"\n";
		}
		std::cout<<"All goals are satisfied after "<<action.used_moves<<" moves\n";
	}
	std::cout<<"\n";
}

void Board::visualize(char robot_tag, int max_moves) {
	BoardState bs = initState();
	saveState(bs);
	visited_states.insert(bs);
	states_queue.push(bs);

	size_t robot_index = robotIndex(robot_tag);
	if(robot_index >= robots.size()) {
		std::cout<<"index: "<<robot_index<<", size: "<<robots.size()<<"\n";
	}
	assert(robot_index < robots.size());
	VisualizeAction action(*this, robot_tag, robot_index, max_moves, bs);

	traverse(0, action);

	//go over and print
	action.print();
}

BoardState Board::initState() {
	BoardState bs;
	bs.robot_poses = robots;
	return bs;
}

void Board::dfs(int recurse_depth, const int max_depth) {
	if(recurse_depth > max_depth) {
		if(checkAllGoals(path[max_depth])) {
			all_solutions.push_back(path); //find a suitable way
		}
		return;
	}

	assert(recurse_depth <= max_depth+1);

	assert(path.size() == max_depth + 1);
	const BoardState& bs = path[recurse_depth-1];

	//Trying to move to 4 directions
	//Testing the next step
	for(size_t current_index = 0; current_index < bs.robot_poses.size(); ++ current_index) {
		const Robot& robot = bs.robot_poses[current_index];

		//Check if it has a wall in the East
		{
			int x = findEast(robot, bs);
			//Check if it would touch other robot
			for(size_t ri = 0; ri < bs.robot_poses.size(); ++ri) {
				const Robot& r = bs.robot_poses[ri];
				if(r != robot && r.pos.row == robot.pos.row && r.pos.col > robot.pos.col) {
					x = std::min(x, r.pos.col - 1);
				}
			}
			if(x > robot.pos.col) //Could move to the East
			{
				BoardState new_state = bs.child(robot.which, "east");
				new_state.robot_poses[current_index].pos.col = x;
				if(! find_state(new_state, recurse_depth)) {
					path[recurse_depth] = new_state;
					dfs(recurse_depth+1, max_depth);
				}
			}
		}

		//Check if it has a wall in the South
		{
			// Find the first wall
			int y = findSouth(robot, bs);
			//Check if it would touch other robot
			for(size_t ri = 0; ri < bs.robot_poses.size(); ++ri) {
				const Robot& r = bs.robot_poses[ri];
				if(r != robot && r.pos.col == robot.pos.col && r.pos.row > robot.pos.row) {
					y = std::min(y, r.pos.row - 1);
				}
			}
			if(y > robot.pos.row) { //Could move to the South
				BoardState new_state = bs.child(robot.which, "south");
				new_state.robot_poses[current_index].pos.row = y;
				if(! find_state(new_state, recurse_depth)) {
					path[recurse_depth] = new_state;
					dfs(recurse_depth+1, max_depth);
				}
			}
		}

		//Check if it has a wall in the West
		{
			int x = findWest(robot, bs);
			//Check if it would touch other robot
			for(size_t ri = 0; ri < bs.robot_poses.size(); ++ri) {
				const Robot& r = bs.robot_poses[ri];
				if(r != robot && r.pos.row == robot.pos.row && r.pos.col < robot.pos.col) {
					x = std::max(x, r.pos.col + 1);
				}
			}
			if(x < robot.pos.col) //Could move to the West
			{
				BoardState new_state = bs.child(robot.which, "west");
				new_state.robot_poses[current_index].pos.col = x;
				if(! find_state(new_state, recurse_depth)) {
					path[recurse_depth] = new_state;
					dfs(recurse_depth+1, max_depth);
				}
			}
		}
		//Check if it has a wall in the North
		{
			// Find the first wall
			int y = findNorth(robot, bs);
			//Check if it would touch other robot
			for(size_t ri = 0; ri < bs.robot_poses.size(); ++ri) {
				const Robot& r = bs.robot_poses[ri];
				if(r != robot && r.pos.col == robot.pos.col && r.pos.row < robot.pos.row) {
					y = std::max(y, r.pos.row + 1);
				}
			}
			if(y < robot.pos.row) { //Could move to the North
				BoardState new_state = bs.child(robot.which, "north");
				new_state.robot_poses[current_index].pos.row = y;
				if(! find_state(new_state, recurse_depth)) {
					path[recurse_depth] = new_state;
					dfs(recurse_depth+1, max_depth);
				}
			}
		}
	}
}

int Board::findEast(const Robot& robot, const BoardState& bs){
	for(int x = robot.pos.col; x <= cols; ++x) {
		if(vertical_walls[robot.pos.row-1][x]) { //Encountered wall and stop moving
			return x;
		}
	}
	return cols;
};

int Board::findSouth(const Robot& robot, const BoardState& bs) {
	for(int y = robot.pos.row; y <= rows; ++y) {
		if(horizontal_walls[y][robot.pos.col-1]) { //Encountered wall and stop moving
			return y;
		}
	}
	return rows;
}

int Board::findWest(const Robot& robot, const BoardState& bs) {
	for(int x = robot.pos.col; x > 0; --x) {
		if(vertical_walls[robot.pos.row-1][x-1]) { //Encountered wall and stop moving
			return x;
		}
	}
	return 1;
}


int Board::findNorth(const Robot& robot, const BoardState& bs) {
	for(int y = robot.pos.row; y > 0; --y) {
		if(horizontal_walls[y-1][robot.pos.col-1]) { //Encountered wall and stop moving
			return y;
		}
	}
	return 0;
}

void Board::traverse(int recurse_depth, TraverseAction& action) {
	while(! states_queue.empty() && states_queue.front().depth == recurse_depth) {
		const BoardState bs = states_queue.front();
		states_queue.pop();

		if(! action.check(bs)) {
			return;
		}

		//Test each robot's nect step
		for(size_t current_index = 0; current_index < bs.robot_poses.size(); ++ current_index) {
			const Robot& robot = bs.robot_poses[current_index];

			//Check if it has a wall in the East
			{
				int x = findEast(robot, bs);
				//Check if it would touch other robot
				for(size_t ri = 0; ri < bs.robot_poses.size(); ++ri) {
					const Robot& r = bs.robot_poses[ri];
					if(r != robot && r.pos.row == robot.pos.row && r.pos.col > robot.pos.col) {
						x = std::min(x, r.pos.col - 1);
					}
				}
				if(x > robot.pos.col) //Could move to the East
				{
					BoardState new_state = bs.child(robot.which, "east");
					new_state.robot_poses[current_index].pos.col = x;
					if(! stateVisited(new_state)) {
						saveState(new_state);
						visited_states.insert(new_state);
						states_queue.push(new_state);
					}
				}
			}

			//Check if it has a wall in the South
			{
				int y = findSouth(robot, bs);
				//Check if it would touch other robot
				for(size_t ri = 0; ri < bs.robot_poses.size(); ++ri) {
					const Robot& r = bs.robot_poses[ri];
					if(r != robot && r.pos.col == robot.pos.col && r.pos.row > robot.pos.row) {
						y = std::min(y, r.pos.row - 1);
					}
				}
				if(y > robot.pos.row) { //Could move to the South
					BoardState new_state = bs.child(robot.which, "south");
					new_state.robot_poses[current_index].pos.row = y;
					if(! stateVisited(new_state)) {
						saveState(new_state);
						visited_states.insert(new_state);
						states_queue.push(new_state);
					}
				}
			}

			//Check if it has a wall in the West
			{
				int x = findWest(robot, bs);
				//Check if it would touch other robot
				for(size_t ri = 0; ri < bs.robot_poses.size(); ++ri) {
					const Robot& r = bs.robot_poses[ri];
					if(r != robot && r.pos.row == robot.pos.row && r.pos.col < robot.pos.col) {
						x = std::max(x, r.pos.col + 1);
					}
				}
				if(x < robot.pos.col) //Could move to the West
				{
					BoardState new_state = bs.child(robot.which, "west");
					new_state.robot_poses[current_index].pos.col = x;
					if(! stateVisited(new_state)) {
						saveState(new_state);
						visited_states.insert(new_state);
						states_queue.push(new_state);
					}
				}
			}
			//Check if it has a wall in the North
			{
				int y = findNorth(robot, bs);
				//Check if it would touch other robot
				for(size_t ri = 0; ri < bs.robot_poses.size(); ++ri) {
					const Robot& r = bs.robot_poses[ri];
					if(r != robot && r.pos.col == robot.pos.col && r.pos.row < robot.pos.row) {
						y = std::max(y, r.pos.row + 1);
					}
				}
				if(y < robot.pos.row) { //Could move to the North
					BoardState new_state = bs.child(robot.which, "north");
					new_state.robot_poses[current_index].pos.row = y;
					if(! stateVisited(new_state)) {
						saveState(new_state);
						visited_states.insert(new_state);
						states_queue.push(new_state);
						//set_reachable(new_state);
					}
				}
			}
		}
	}

	if(! states_queue.empty()) {
		traverse(recurse_depth + 1, action);
	}
}

size_t Board::robotIndex(char robot_tag) {
	for(size_t i = 0; i < robots.size(); ++i) {
		if(robots[i].which == robot_tag) {
			return i;
		}
	}

	return robots.size();
}

VisualizeAction::VisualizeAction(Board &b, char robot_tag, size_t robot_index, int max_moves,
		BoardState& bs) :
		TraverseAction(b), robot_tag(robot_tag), robot_index(robot_index), max_moves(max_moves) {
	// set init value for the output matrix
	reachable.resize(b.getRows());
	for(size_t i = 0; i < reachable.size(); ++i) {
		reachable[i].resize(b.getCols(), std::numeric_limits<int>::max());
	}

	const Position& pos = bs.robot_poses[robot_index].pos;
	if(reachable[pos.row-1][pos.col-1] > bs.depth) { //Find the minimum
		reachable[pos.row-1][pos.col-1] = bs.depth;
	}
}

void VisualizeAction::print() {
	std::cout<<"Reachable by robot "<<robot_tag<<":\n";
	for(size_t y = 0; y < reachable.size(); ++y) {
		for(size_t x = 0; x < reachable[y].size(); ++x) {
			if(reachable[y][x] == std::numeric_limits<int>::max()) {
				std::cout<<std::setw(3)<<".";
			} else {
				std::cout<<std::setw(3)<<reachable[y][x];
			}
		}
		std::cout<<"\n";
	}
}

bool VisualizeAction::check(const BoardState &bs) {
	if(max_moves > 0 && bs.depth > max_moves) {
		return false;
	}

	const Position& pos = bs.robot_poses[robot_index].pos;
	if(reachable[pos.row-1][pos.col-1] > bs.depth) { //Find the minimum
		reachable[pos.row-1][pos.col-1] = bs.depth;
	}

	return true;
}

bool ArbitoryPathAction::check(const BoardState &bs) {
	if(bs.depth > max_moves) {
		return false;
	}

	if(board.checkAllGoals(bs)) { //Find a path
		tail_index = bs.state_index;
		used_moves = bs.depth;
		return false;
	}

	//Check if it needs to continue
	return true;
}

bool Board::checkAllGoals(const BoardState &bs) {
	for(size_t gi = 0; gi < goals.size(); ++gi) {
		const Goal& g = goals[gi];
		bool found = false; //Find by position
		for(size_t ri = 0; ri < bs.robot_poses.size(); ++ri) {
			const Robot& r = bs.robot_poses[ri];
			if(r.pos == g.pos && (g.which == '?' || g.which == r.which)) {
				found = true;
				break;
			}
		}
		if(!found) {
			return false;
		}
	}
	return true;
}

void ArbitoryPathAction::print() {
	board.print();
	if(tail_index < 0) { //No way to achieve in the max_move
		std::cout<<"no solutions with "<<max_moves<<" or fewer moves\n";
	} else {
		board.printOnePathByTail(tail_index);
		std::cout<<"All goals are satisfied after "<<used_moves<<" moves\n";
	}
}

void Board::printOnePathByTail(int tail_index, bool print_table) {
	assert(all_states.size() > tail_index);
	BoardState& bs = all_states[tail_index];
	if(bs.parent_index > 0) {
	    //Recursion to print the parent's index
	    printOnePathByTail(bs.parent_index, print_table);
	}
	//Recursion to print self's information
	if(! bs.move_desc.empty()) {
		std::cout<<bs.move_desc<<"\n";
	}
	if(print_table) {
		print(&bs);
	}
}

char BoardState::findPosForPrint(const Position &p) const {
	for(size_t ri = 0; ri < robot_poses.size(); ++ri) {
		const Robot& r = robot_poses[ri];
		if(r.pos == p) {
			return r.which;
		}
	}
	return ' ';
}

bool AllPathAction::check(const BoardState &bs) {
	if(max_moves > 0 && bs.depth > max_moves) {
		return false;
	}

	if(used_moves > 0 && bs.depth > used_moves) {
		return false;
	}

	if(board.checkAllGoals(bs)) { //Find and save a way
		used_moves = bs.depth;
		return false;
	}

	return true;
}

bool Board::find_state(const BoardState &state, int recurse_depth)  {
	for(int i = 0; i < recurse_depth; ++i) {
		if(path[i] == state) {
			return true;
		}
	}
	return false;
}
