# shogi in c

## usage for starting a new game
player x enter the coordinate if the chess which you want to move:<br />
player x enter the new coordinate of the chess:<br />

player y enter the coordinate if the chess which you want to move:<br />
player y enter the new coordinate of the chess:<br />
(先輸入行再輸入列，輸入0可悔棋，輸入-1可結束遊戲)<br /><br />
if the moving path is against the rules, it will tell you: "error! Please enter again."<br />
如果其中一方的"王"被吃掉，則遊戲結束<br /><br />
自動建立一個文字檔(shogi.txt)儲存移動資料，每移動一步便將資料寫進檔案裡<br/>

## usage for reading an old chessboard
read the file(shogi.txt), then display the chessdboard step by step<br />

## compile
start a new game: gcc main.c<br />
read an old chessboard: gcc main.c -lm

## run
start a new game: ./a.out -n -s sdogi.txt<br />
read an old chessboard: ./a.out -l shogi.txt