#include <bits/stdc++.h>
#define n 9
using namespace std;

int trigger_update = 0;
int flag_debug = 0;

int sudoku[n][n];
int presolved[n][n];
int possible[n][n][n+1];

pair<int,int> boxNotation(int i, int j){
    int x = i-i%3;
    int y = j-j%3;
    int boxId = 3*(x/3)+(y/3)+1;
    int squareInBox = 3*(i%3)+(j%3)+1;
    return make_pair(boxId, squareInBox);
}
void printBoxNotation(int i, int j){
    pair<int,int> temp = boxNotation(i,j);
    cout << sudoku[i][j] << "=" << temp.first << "," << temp.second << "\n";
}
//---------------------------------------------------------------
void initialize(){
    for(int i = 0; i < n; i++){
        for(int j =0 ; j < n; j++){
            for(int k = 1; k <= n; k++){
                possible[i][j][k] = 1;
            }
        }
    }
}
//---------------------------------------------------------------
void singleUpdate(int i, int j){
    if(!sudoku[i][j]) return;
    
    //DEBUG
    if(flag_debug){
        printBoxNotation(i, j);
        //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    for(int tempval = 1; tempval <= n; tempval++) possible[i][j][tempval] = 0;

    int val = sudoku[i][j];
    for(int r = 0; r < n; l++) possible[r][j][val] = 0;
    for(int c = 0; c < n; c++) possible[i][c][val] = 0;

    int x = i-i%3;
    int y = j-j%3;
    for(int dx = 0; dx < 3; dx++){
        for(int dy = 0; dy < 3; dy++){
            possible[x+dx][y+dy][val] = 0;
        }
    }
}
void updateFilledSquares(){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(sudoku[i][j])
                singleUpdate(i, j);
        }
    }  
}
//---------------------------------------------------------------
void checkPossible(){
    cout << "blob0\n";
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int cnt = 0, newValue = 0;
            for(int k = 1; k <= n; k++){
                cnt += possible[i][j][k];
                if(possible[i][j][k]) newValue = k;
            }
            if(cnt == 1){
                sudoku[i][j] = newValue;
                trigger_update = 1;
                singleUpdate(i, j);
            }
        }
    }
}
//---------------------------------------------------------------
void checkRows(){
    cout << "blob1\n";
    for(int i = 0; i < n; i++){
        for(int val = 1; val <= n; val++){
            int cnt = 0, last = 0;
            for(int j = 0; j < n; j++){
                if(possible[i][j][val]){
                    last = j;
                    cnt++;
                }
            }
            if(cnt == 1){
                sudoku[i][last] = val;
                trigger_update = 1;
                singleUpdate(i, last);
            }
        }
    }
}
void checkColumns(){
    cout << "blob2\n";
    for(int i = 0; i < n; i++){
        for(int val = 1; val <= n; val++){
            int cnt = 0, last = 0;
            for(int j = 0; j < n; j++){
                if(possible[j][i][val]){
                    last = j;
                    cnt++;
                }
            }
            if(cnt == 1){
                sudoku[last][i] = val;
                trigger_update = 1;
                singleUpdate(last, i);
            }
        }
    }
}
void checkBoxes(){
    cout << "blob3\n";
    for(int i = 0; i < n; i++){ //There are 9 boxes in total
        int x = 3*(i/3);
        int y = 3*(i%3);
        for(int val = 1; val <= n; val++){
            int cnt = 0, lastx = 0, lasty = 0;
            for(int dx = 0; dx < 3; dx++){
                for(int dy = 0; dy < 3; dy++){
                    if(possible[x+dx][y+dy][val]){
                        lastx = x+dx;
                        lasty = y+dy;
                        cnt++; 
                    }
                }
            }
            if(cnt == 1){
                sudoku[lastx][lasty] = val;
                trigger_update = 1;
                singleUpdate(lastx, lasty);
            }
        }
    }
}
//---------------------------------------------------------------
void printTab(int tab[n][n]){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(!tab[i][j]) cout << "  ";
            else cout << tab[i][j] << " ";
            
            if(j == 2 || j == 5) cout << "| ";
        }
        cout << "\n";
        if(i == 2 || i == 5) cout << "- - - x - - - x - - -\n";
    }
}
void printReport(){
    cout << "\n--- UNSOLVED SUDO ---\n";
    printTab(presolved);
    cout << "\n---- SOLVED SUDO ----\n";
    printTab(sudoku);
}

int main(){
    initialize();

    //Here we use 0 to represent blank cells
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> sudoku[i][j];
            presolved[i][j] = sudoku[i][j];
        }
    }

    updateFilledSquares();
    flag_debug = 0;

    bool finish = 0;
    int cnt = 0;
    while(!finish && cnt < 200){
        if(cnt == 0 || trigger_update){
            trigger_update = 0;
            checkPossible();
            checkRows();
            checkColumns();
            checkBoxes();
        }
        cnt++;
    }
    printReport();
    return 0;
}