# shogi in c

## usage
player x enter the coordinate if the chess which you want to move:<br />
player x enter the new coordinate of the chess:<br />

player y enter the coordinate if the chess which you want to move:<br />
player y enter the new coordinate of the chess:<br />
(先輸入行再輸入列，輸入0可悔棋，輸入-1可結束遊戲)<br />
if the moving path is against the rules, it will tell you: "error! Please enter again."<br />

自動建立一個文字檔(shogi.txt)儲存移動資料，每移動一步便將資料寫進檔案裡<br/>

## compile
gcc main.c

## run
./a.out