#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <cstdlib> 
#include <algorithm> 
#include <sstream>

enum class State {
    kEmpty,
    kObstacle,
    kClosed,
    kPath,
    kFinish,
    kStart
};


// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

std::string CellString(State cell){

    if(cell == State::kObstacle) 
        return "⛰️";
    if(cell == State::kPath) 
        return "🚗";
    if(cell == State::kStart) 
        return "🚦";
    if(cell == State::kFinish) 
        return "🏁";

    return "0"; 

}

void PrintBoard(std::vector<std::vector<State>> board) {

    for(int i=0;i<board.size();i++)
    {
        for(int j=0;j<board[0].size();j++)
        {
            std::cout<<CellString(board[i][j])<<", ";
        }
        std::cout<<""<<std::endl;
    }
}

void AddToOpen(int x, int y, int g, int h, std::vector<std::vector<int>>& openNodes, std::vector<std::vector<State>>& grid){


    openNodes.push_back(std::vector<int>{x, y, g, h});
    grid[x][y] = State::kClosed;
}

int Heuristic(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}


bool CheckValidCell(int x, int y, std::vector<std::vector<State>>& grid){

    if((x < grid.size())&&(y < grid[0].size())&&(x >= 0)&&(y >= 0)){

        if(grid[x][y] == State::kEmpty){
            return true;
        }

    }
    return false;
}
void ExpandNeighbors(std::vector<int>& current_node, std::vector<std::vector<int>>& openNodes, std::vector<std::vector<State>>& grid, int goal_point[2]){

    int g = current_node[2];
    int h, g_;
    std::vector<int> node{0,0};  

    for(int e=0;e<4;e++){
        
        node[0] = current_node[0] + delta[e][0];
        node[1] = current_node[1] + delta[e][1];
        if(CheckValidCell(node[0], node[1],grid)){

            g_ = g + 1;
            h = Heuristic(node[0], node[1], goal_point[0], goal_point[1]);
            AddToOpen(node[0], node[1], g_, h, openNodes, grid);
        }
    }
}

bool Compare(const std::vector<int>& node1, const std::vector<int>& node2){

    int f1 = node1[2] + node1[3];
    int f2 = node2[2] + node2[3];

    if(f1 > f2){
      return true;
    }
    return false;
}


void CellSort(std::vector<std::vector<int>>& v) {
    std::sort(v.begin(), v.end(), Compare);

}


std::vector<std::vector<State>> ReadBoardFile(){
          
    std::ifstream inputFile("readboard.txt");
    std::string read;
    std::vector<std::vector<State>> board;

    if(!inputFile){
        std::cerr << "Error opening file" << std::endl;
    }
    while(std::getline(inputFile,read))
    {
        std::stringstream ss(read);
        std::string token;
        std::vector<State> row;
        while(std::getline(ss, token, ',')) {
            if (!token.empty()) {
                row.push_back(static_cast<State>(std::stoi(token)));       
            }
        }
        board.push_back(row);
    }
    inputFile.close();
    return board;
}

std::ostream& operator<<(std::ostream& os, const std::vector<int>& node) {
    
    for(int i=0;i<2;i++){
        os<<node[i];
    }
    return os;
}

std::vector<std::vector<State>> Search(std::vector<std::vector<State>> grid, int initial_point[2], int goal_point[2]){

    std::vector<std::vector<int>> openNodes;
    int h = Heuristic(initial_point[0], initial_point[1], goal_point[0], goal_point[1]);
    AddToOpen(initial_point[0],initial_point[1],0,h,openNodes,grid);
    
    while (!openNodes.empty()){

        CellSort(openNodes);
        std::vector<int> current_cell = openNodes.back();
        openNodes.pop_back();
        grid[current_cell[0]][current_cell[1]] = State::kPath;
        if((current_cell[0]==goal_point[0])&&(current_cell[1]==goal_point[1])){
            grid[initial_point[0]][initial_point[1]] = State::kStart;
            grid[goal_point[0]][goal_point[1]] = State::kFinish;
            return grid;
        }
        ExpandNeighbors(current_cell, openNodes, grid, goal_point);
        
    }

    std::cout << "List of open nodes is empty, you have run out of new nodes to explore and haven't found a path." << std::endl;
    return {};

}


int main() {

    int initial_point[2] = {0,0};
    int goal_point[2] = {4,5};
    std::vector<std::vector<State>> board = ReadBoardFile();
    auto solution = Search(board,initial_point,goal_point);
    PrintBoard(solution);
    return 0;
}