#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <queue>
#include <set>

// ==================================================================
// ==================================================================
// A tiny all-public helper class to record a 2D board position

class Position {
public:
  // the coordinate (-1,-1) is invalid/unitialized
  Position(int r=-1,int c=-1) : row(r),col(c) {}
  int row,col;
};

// convenient functions to print and equality compare Positions
std::ostream& operator<<(std::ostream &ostr, const Position &p);
bool operator==(const Position &a, const Position &b);
bool operator!=(const Position &a, const Position &b);
bool operator<(const Position &a, const Position &b);


// ==================================================================
// ==================================================================
// A tiny all-public helper class to store the position & label of a
// robot.  A robot label must be a capital letter.

class Robot {
public:
  Robot(Position p, char w) : pos(p), which(w) { 
    assert (isalpha(w) && isupper(w)); }
  Position pos;
  char which;
};
inline bool operator==(const Robot &a, const Robot &b) {
	return a.pos == b.pos && a.which == b.which;
}
inline bool operator!=(const Robot &a, const Robot &b) {
	return !(a == b);
}
inline bool operator<(const Robot &a, const Robot &b) {
	return a.pos < b.pos || (a.pos == b.pos && a.which < b.which);
}

// ==================================================================
// ==================================================================
// A tiny all-public helper class to store the position & label of a
// goal.  A goal label must be an upper case letter or '?' indicating
// that any robot can occupy this goal to solve the puzzle.

class Goal {
public:
  Goal(Position p, char w) : pos(p), which(w) { 
    assert (w == '?' || (isalpha(w) && isupper(w))); 
  }
  Position pos;
  char which;
};

// four possible directions to go
enum Directions {TO_EAST = 0, TO_SOUTH, TO_WEST, TO_NORTH, DIR_COUNT};
const int delta_x[] = {1, 0, -1, 0}; // x move to 4 directions' change
const int delta_y[] = {0, -1, 0, 1}; // y move to 4 directions' change

// position of each robot on the board
class BoardState {
public:
	BoardState():depth(0), state_index(-1), parent_index(-1) {}
	int depth; // recursive depth
	int state_index; 
	int parent_index;
	std::vector<Robot> robot_poses;
	std::string move_desc; // something like "robot A moves north"
	BoardState child(char which, const char* direction) const {
		BoardState tmp = *this;
		++ tmp.depth;
		tmp.parent_index = state_index;
		tmp.move_desc = std::string("robot ") + which + std::string(" moves ") + direction;
		return tmp;
	}
	char findPosForPrint(const Position &p) const;
};
inline bool operator==(const BoardState &a, const BoardState &b) {
	return a.robot_poses == b.robot_poses;
}
inline bool operator!=(const BoardState &a, const BoardState &b) {
	return !(a == b);
}
inline bool operator<(const BoardState &a, const BoardState &b) {
	return a.robot_poses < b.robot_poses;
}

class Board;

class TraverseAction {
public:
	TraverseAction(Board& b): board(b) {}
	virtual ~TraverseAction() {}
	virtual bool check(const BoardState& bs) { return false; }
protected:
	Board& board;
};

//Record each goal robots would attain
class VisualizeAction: public TraverseAction {
public:
	VisualizeAction(Board& b, char robot_tag, size_t robot_index, int max_moves,
			BoardState& bs); // bs is the init state
	virtual bool check(const BoardState& bs) ;
	void print();
	char robot_tag;
	int robot_index;
	int max_moves;
	std::vector<std::vector<int> > reachable;  
};

//Find one solution
class ArbitoryPathAction: public TraverseAction {
public:
	ArbitoryPathAction(Board& b, int max_moves) :
		TraverseAction(b), max_moves(max_moves),
		tail_index(-1), used_moves(-1) { assert(max_moves > 0); }
	virtual bool check(const BoardState& bs) ;
	void print(); 
	const int max_moves;
	int tail_index; 
	int used_moves; 
};

//Find all solution
class AllPathAction: public TraverseAction {
public:
	AllPathAction(Board& b, int max_moves) :
		TraverseAction(b), max_moves(max_moves), used_moves(-1) { }
	virtual bool check(const BoardState& bs) ;
	const int max_moves;
	int used_moves;
};

// ==================================================================
// ==================================================================
// A class to hold information about the puzzle board including the
// dimensions, the location of all walls, the current position of all
// robots, the goal location, and the robot (if specified) that must
// reach that position

class Board {
public:

  // CONSTRUCTOR
  Board(int num_rows, int num_cols);

  // ACCESSORS related the board geometry
  int getRows() const { return rows; }
  int getCols() const { return cols; }
  bool getHorizontalWall(double r, int c) const;
  bool getVerticalWall(int r, double c) const;

  // ACCESSORS related to the robots and their current positions
  unsigned int numRobots() const { return robots.size(); }
  char getRobot(int i) const { assert (i >= 0 && i < (int)numRobots()); return robots[i].which; }
  Position getRobotPosition(int i) const { assert (i >= 0 && i < (int)numRobots()); return robots[i].pos; }
  
  // ACCESSORS related to the overall puzzle goals
  unsigned int numGoals() const { return goals.size(); }
  // (if any robot is allowed to reach the goal, this value is '?')
  char getGoalRobot(int i) const { assert (i >= 0 && i < (int)numGoals()); return goals[i].which; }
  Position getGoalPosition(int i) const { assert (i >= 0 && i < (int)numGoals()); return goals[i].pos; }

  // MODIFIERS related to board geometry
  void addHorizontalWall(double r, int c);
  void addVerticalWall(int r, double c);

  // MODIFIERS related to robot position
  // initial placement of a new robot
  void placeRobot(const Position &p, char a);

  // MODIFIER related to puzzle goals
  void addGoal(const std::string &goal_robot, const Position &p);

  // PRINT
  void print(BoardState* bs = nullptr);

//for the game
public:
  void visualize(char robot_tag, int max_moves);
  void findArbitorySolution(int max_moves);
  void findAllSolutions(int max_moves);
  bool checkAllGoals(const BoardState& bs);


  std::vector<BoardState> all_states;
  void printOnePathByTail(int tail_index, bool print_table = true);
private:
  std::vector<std::vector<int> > solutions;

  void saveState(BoardState& bs) {
	  bs.state_index = all_states.size();
	  all_states.push_back(bs);
  }
  // all visited states for bfs
  std::set<BoardState> visited_states;
  // queue for bfs
  std::queue<BoardState> states_queue;
  inline bool stateVisited(const BoardState& s);
  // generate init state of the board before any movements
  BoardState initState();
  // recursively generate movements for visualization
  void traverse(int recurse_depth, TraverseAction& action);
  //Tag for A, B, C robot
  size_t robotIndex(char robot_tag);

  int findEast(const Robot& robot, const BoardState& bs);
  int findSouth(const Robot& robot, const BoardState& bs);
  int findWest(const Robot& robot, const BoardState& bs);
  int findNorth(const Robot& robot, const BoardState& bs);

  // for dfs
  std::vector<std::vector<BoardState> > all_solutions;
  std::vector<BoardState> path; //temporate path
  void dfs(int recurse_depth, const int max_depth);
  bool find_state(const BoardState& state, int recurse_depth);
private:

  // private helper functions
  char getspot(const Position &p) const;
  void setspot(const Position &p, char a);
  char isGoal(const Position &p) const;

  // the board geometry
  const int rows;
  const int cols;
  std::vector<std::vector<char> > board;
  std::vector<std::vector<bool> > vertical_walls;
  std::vector<std::vector<bool> > horizontal_walls;

  // the names and current positions of the robots
  std::vector<Robot> robots;

  // the goal positions & the robots that must reach them
  std::vector<Goal> goals;
};
