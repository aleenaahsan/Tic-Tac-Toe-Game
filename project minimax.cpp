#include <iostream>
#include <climits>
#include <cstdlib>
using namespace std;

const int BOARD_SIZE = 3;
const int EMPTY = 0;
const int PLAYER_X = 1;
const int PLAYER_O = 2;


struct Node{
    int move;
    int row;
    int column;
    Node *next;
    Node *previous;
    Node(int m,int r,int w){
        move=m;
        row=r;
        column=w;
        next=NULL;
        previous=NULL;
    }
};

class Stack{
    Node *head;
    Node *top;
    public:
        Stack(){
            head=NULL;
            top=NULL;
        }
        void push(int m,int r, int c){
            Node *newnode= new Node(m,r,c);
            if(head==NULL){
                head=newnode;
                top=newnode;
            }
            else{
                top->next=newnode;
                newnode->previous=top;
                top=newnode;
            }
        }
        Node *pop(){
            if(top==NULL){
                cout<<"Board empty cant popup more\n";
                head=NULL;
                return NULL;
            }
            else{
                 Node *temp=top;
            top=top->previous;
            return temp;
            }
           
        }
};

Stack st;

bool isBoardFull(int board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == EMPTY) {
                return false; 
            }
        }
    }
    return true; 
    }

int evaluateBoard(int board[BOARD_SIZE][BOARD_SIZE]) 
{
    for (int row = 0; row < BOARD_SIZE; ++row) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == PLAYER_X)
                return 10;
            else if (board[row][0] == PLAYER_O)
                return -10;
        }
    }

    for (int col = 0; col < BOARD_SIZE; ++col) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == PLAYER_X)
                return 10;
            else if (board[0][col] == PLAYER_O)
                return -10;
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == PLAYER_X)
            return 10;
        else if (board[0][0] == PLAYER_O)
            return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == PLAYER_X)
            return 10;
        else if (board[0][2] == PLAYER_O)
            return -10;
    }

    return 0; 
}

int minimax(int board[BOARD_SIZE][BOARD_SIZE], int depth, bool isMaximizing) {
    int score = evaluateBoard(board);

    if (score == 10 || score == -10 || isBoardFull(board)) {
        return score;
    }

    if (isMaximizing) {
        int bestScore = INT_MIN;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_X;
                    bestScore = max(bestScore, minimax(board, depth + 1, !isMaximizing));
                    board[i][j] = EMPTY; 
                }
            }
        }
        return bestScore;
    }
    else {
        int bestScore = INT_MAX;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_O;
                    bestScore = min(bestScore, minimax(board, depth + 1, !isMaximizing));
                    board[i][j] = EMPTY;
                }
            }
        }
        return bestScore;
    }
}

void findBestMove(int board[BOARD_SIZE][BOARD_SIZE]) {
    int bestScore = INT_MIN;
    int bestMoveRow = -1;
    int bestMoveCol = -1;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == EMPTY) {
                board[i][j] = PLAYER_X;
                int moveScore = minimax(board, 0, false);
                board[i][j] = EMPTY; 

                if (moveScore > bestScore) {
                    bestScore = moveScore;
                    bestMoveRow = i;
                    bestMoveCol = j;
                }
            }
        }
    }

    board[bestMoveRow][bestMoveCol] = PLAYER_X;
    st.push(PLAYER_X, bestMoveRow, bestMoveCol);

}

void printBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == EMPTY)
                std::cout << "- ";
            else if (board[i][j] == PLAYER_X)
                cout << "X ";
            else
                cout << "O ";
        }
        cout << endl;
    }
}

int main() 
{
    int board[BOARD_SIZE][BOARD_SIZE] = {{EMPTY, EMPTY, EMPTY},
                                         {EMPTY, EMPTY, EMPTY},
                                         {EMPTY, EMPTY, EMPTY}};

    bool gameEnd = false;
    int currentPlayer;
    cout<<"Lets have a toss!\n";
    int turn = 1+(rand()%2);
starting:
    if(turn == 1){
         currentPlayer = PLAYER_X;
         cout<<"Its player X's turn\n";
    }
    else if(turn==2){
        cout<<"its player O's turn\n";
        currentPlayer=PLAYER_O;
    }
	printBoard(board);

    while (!gameEnd) {
        
        
        cout<<"\n----------------\n";

        if (currentPlayer == PLAYER_X) {
            cout<<"PLAYER_X's Turn\n";
            findBestMove(board);
           	printBoard(board);
            currentPlayer = PLAYER_O;
        } 
        else {
            int row, col;
            again:
            cout<<"\nPLAYER_O's Turn\n";
            cout << "Enter row and column (0-2) for your move: \n";
            cin >> row >> col;

            if (board[row][col] == EMPTY) {
                board[row][col] = PLAYER_O;
                currentPlayer = PLAYER_X;
                st.push(PLAYER_O, row, col);
            } 
            else{
                cout << "Invalid move! Try again." << endl;
                goto again;
                
            }
            printBoard(board);
            char undo;
    	undo:
        cout<<"\nFor undo press y or n for no undo\n";
        cin>>undo;
        if(undo=='y'){
            Node *popup= st.pop();
            if(popup==NULL){
            	cout<<"board is empty cant popup more\n";
                goto starting;
			}
			else{
			 board[popup->row][popup->column]=EMPTY;
            cout<<"Undo done\n";
        	printBoard(board);
         	goto undo;
			}           
        }

        }
		
        int gameResult = evaluateBoard(board);
        if (gameResult == 10) {
            printBoard(board);
            cout << "Player X wins!" <<endl;
            gameEnd = true;
        } else if (gameResult == -10) {
            printBoard(board);
            cout << "Player O wins!" << endl;
            gameEnd = true;
        } else if (isBoardFull(board)) {
            printBoard(board);
            cout << "It's a tie!" << endl;
            gameEnd = true;
        }
    }

    return 0;
}
