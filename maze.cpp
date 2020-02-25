//Matthew Lyan
//CMSC 341 Project 1
//Section 04

#include <stdexcept>
#include "maze.h"
Maze::Maze(int ncell) {
  if (ncell < 1) {
    throw std::invalid_argument("Maze::Maze(): ncell must be >= 1");
  }

  _ncell = ncell;
  _maze = new cell_t[_ncell];
}

Maze::~Maze() {
  // IMPLEMENT DESTRUCTOR
  delete [] _maze; 
  
}

void Maze::readFile(std::string fileName) {
  int numCells;
  int cell, n1, n2, n3, n4; 
  std::ifstream dataFile;
  
  dataFile.open(fileName);
  
  if (dataFile.is_open()) {
    dataFile >> numCells;
    this->reset(numCells);

    for (int i = 0; i < numCells; i++) {
      dataFile >> cell >> n1 >> n2 >> n3 >> n4;
      this->insert(cell_t(cell, n1, n2, n3, n4));
    }
  }
}

int Maze::getNcell() const {
  return _ncell;
}

void Maze::reset(int ncell) {
  if (ncell < 1) {
    throw std::invalid_argument("Maze::reset(): ncell must be >= 1");
  }

  if (_maze != nullptr) {
    delete [] _maze;
  }
  
  _ncell = ncell;
  _maze = new cell_t[_ncell];
}

void Maze::insert(cell_t cell) {
  if (_maze == nullptr) {
    throw std::domain_error("Maze::insert(): attempt to insert into uninitialized Maze object");
  }
  if (cell.cellNum < 0 || cell.cellNum >= _ncell) {
    throw std::invalid_argument("Maze:insert(): invalid cell number");
  }
  _maze[cell.cellNum] = cell;
}
  
cell_t Maze::retrieve(int nc) const {
  if (_maze == nullptr) {
    throw std::domain_error("Maze::retrieve(): attempt to retrieve from an uninitialized Maze object");
  }
  if (nc < 0 || nc >= _ncell) {
    throw std::invalid_argument("Maze:retrieve(): invalid cell index");
  }
  
  return _maze[nc];
}

void Maze::dump() const {

  std::cout << "[*] Dumping the maze cells..." << std::endl;
  
  for (int i = 0; i < _ncell; i++) {
    int nc = _maze[i].cellNum;
    if (nc != i) {
      std::cerr << "Warning: maze cell at index " << i
		<<" has cellNum of " << nc << std::endl;
    }
    std::cout << "Cell " << i << " has neighbor cells: ";
    neighbor_t nbs = _maze[i].neighbors;
    int n0, n1, n2, n3;
    if ( nbs[0] >= 0 ) std::cout << nbs[0] << " ";
    if ( nbs[1] >= 0 ) std::cout << nbs[1] << " ";
    if ( nbs[2] >= 0 ) std::cout << nbs[2] << " ";
    if ( nbs[3] >= 0 ) std::cout << nbs[3] << " ";
    std::cout << std::endl;
  }
}

std::vector<int> Maze::solve() const {
  // IMPLEMENT THE SOLVE METHOD
  //make a copy of _maze to edit, intliaize cell_t
  

  
  cell_t mazeCopy[_ncell]; // make a copy of the list
  for (int i = 0; i < _ncell; i++){
    mazeCopy[i] = _maze[i];
  }

  if (mazeCopy == NULL){ // checks if the stack is empty                                                                                                                                                      
    throw std::domain_error("The maze copy is empty");
  }
  
  bool check = false;
  SolveStack stack;
  src_dest_t start(-1,0);
  stack.push(start); // starts the maze and pushes it onto the stack
  //mark previous square -1
  while (stack.empty() == false and stack.read().second != _ncell - 1){
    for (int i = 0; i < 4; i++){
      check = true; // assumes there are no neighbors if check is true
      if (mazeCopy[stack.read().second].neighbors[i] != VACANT and mazeCopy[stack.read().second].neighbors[i] != stack.read().first){
	src_dest_t entry(stack.read().second, mazeCopy[stack.read().second].neighbors[i]);
	stack.push(entry);
	check = false;
	break;
      }
    }
    if (check){
      src_dest_t number =  stack.pop();
      if (number.first == VACANT){
	std:: cout << "No solution" << std::endl;
	break;
      }
      for( int i = 0; i < 4; i++){ // to go through the previous square neighbor to pop off
	if ( mazeCopy[number.first].neighbors[i] == number.second){
	  mazeCopy[number.first].neighbors[i] = VACANT;
	}
      }
    }
  }
  std::vector <int> solution;
  while ( !stack.empty()){
    solution.insert(solution.begin(),(stack.pop().second));
  }
  
  return solution;
}

Maze::SolveStack::SolveStack() {
  _stack = nullptr;
}

Maze::SolveStack::~SolveStack() { // every node in the linked list. 
  // IMPLEMENT DESTRUCTOR
  entry_t *current = _stack; // sets a current to the head of the linked list
  while (current != nullptr){
    entry_t *next = current->next;
    delete current;
    current = next;
  }
  _stack = nullptr;
  
}

bool Maze::SolveStack::empty() const {
  return _stack == nullptr;
}

void Maze::SolveStack::push(src_dest_t src_dest) { // push at the head
  // IMPLEMENT PUSH METHOD
  //push into a vector or a data structure

    
  StackEntry *temp = new StackEntry(src_dest); // allocate a new structure with the data your trying to push. create a new node
  temp->next = _stack;
  _stack = temp;  
  
}

src_dest_t Maze::SolveStack::pop() { // 
  // IMPLEMENT POP METHOD

  if (_stack == NULL){ // checks if the stack is empty
    throw std::domain_error("The Stack is empty");
  }

  entry_t *temp = _stack;
  src_dest_t save;
  save = temp->src_dest;
  _stack = _stack->next;
  delete temp;
  return save;
}

src_dest_t Maze::SolveStack::read() const {
  // IMPLEMENT READ METHOD
  //return data using peek, and information in entry_t *stack. only needs to read the top of the stack
  if (_stack == NULL){ // checks if the stack is empty                                                                                                                                                     
    throw std::domain_error("The Stack is empty");
  }
  

  return _stack->src_dest;
}
