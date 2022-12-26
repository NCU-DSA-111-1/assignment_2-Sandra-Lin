#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stddef.h>
#include<unistd.h>
void newboard();//產生一個全新棋盤
void printboard();//印出棋盤
void movechess();//移動棋子
void regret();//悔棋
void push(int start1,int start2,int end1,int end2);// push stack
void read_board();//讀舊棋譜
int pop();//pop stack
FILE *new_game;//儲存新棋盤的檔案
FILE *old_game;//載入舊棋盤

struct node{
    char start_chess;
    char end_chess;
    int start[2];//起點座標
    int end[2];//終點座標
    struct node *next;
}*stack=NULL;

typedef struct node NODE;
int i, j, end=0, king=0;
int turn=1;//計算回合數，奇數時玩家x移動，偶數時玩家y移動
int origin_row,origin_column,new_row,new_column;//coordinates of the chess
char board[10][10]={0};//棋盤
NODE *inverse(NODE *list);//inverse of linked list

int main(int argc,char **argv)
{
    int opt=0;

    while((opt=getopt(argc,argv,"ns:l:"))!=-1)
    {
        switch(opt)
        {
            case'n':
                break;
            case's':
                if((new_game=fopen("shogi.txt","w+"))!=NULL)
                    fprintf(new_game,"%-6s%-6s%-4s%-6s\n","turns","from","to","chess");
    
                newboard();
                printboard();

                while(1)
                {
                    movechess();

                    if(end==-1)
                        break;
                    
                    if(king==1)
                    {
                        printboard();

                        if(96 < stack->start_chess && stack->start_chess < 105)
                            printf("Player y win!\n");
                        if(104 < stack->start_chess && stack->start_chess < 113)
                            printf("Player x win!\n");

                        fclose(new_game);
                        break;
                    }
                    printboard();
                }
                break;

            case'l':
                if((old_game=fopen("shogi.txt","r"))!=NULL)
                    read_board();
                break;
        }
    }
    
    return 0;
}

void read_board()
{
    NODE *read=NULL,*temp,*front=NULL,*fnext=NULL,*list;
    list=(NODE*)malloc(sizeof(NODE));
    read=(NODE*)malloc(sizeof(NODE));

    fseek(old_game,22,SEEK_SET);//游標移到檔案第一列最後
    while(!feof(old_game))
    {
        temp=(NODE*)malloc(sizeof(NODE));

        fseek(old_game,7,SEEK_CUR);//游標移到from
        fscanf(old_game,"%d %d %d %d %c",&temp->start[1],&temp->start[0],&temp->end[1],&temp->end[0],&temp->end_chess);
        temp->next=NULL;
        temp->next=read;
        read=temp;
    }
    fclose(old_game);

    list=read;
    while(list)//inverse of the list
    {
        fnext=front;
        front=list;
        list=list->next;
        front->next=fnext;
    }

    stack=front;
    newboard();
    while(stack->next!=NULL)//重播下棋過程
    {
        pop();
        printboard();
        printf("\n");
    }
}

void push(int start1,int start2,int end1,int end2)
{
    NODE *temp;
    temp=(NODE*)malloc(sizeof(NODE));

    temp->start[0]=start1;
    temp->start[1]=start2;
    temp->end[0]=end1;
    temp->end[1]=end2;
    temp->start_chess=board[start1][(start2)-1];
    temp->end_chess=board[end1][(end2)-1];
    temp->next=stack;
    stack=temp;
}

int pop()
{
    if(stack==NULL){
        printf("Cannot go back anymore.\n");
        return 0;
    }
    else
    {
        NODE *temp;
        temp=(NODE*)malloc(sizeof(NODE));

        temp=stack;
        board[temp->start[0]][(temp->start[1])-1]=temp->start_chess;
        board[temp->end[0]][(temp->end[1])-1]=temp->end_chess;
        stack=stack->next;
        free(temp);
        turn++;
        return 1;
    }
}

void newboard()
{
    for(i=0;i<9;i++)
    {
        board[0][i]=i+49;
        board[i+1][9]=i+49;
        board[3][i]='a';//a represents red步
        board[7][i]='i';//i represents blue步
    }

    board[1][0]='b';//b represents red香
    board[1][8]='b';
    board[9][0]='j';//j represents blue香
    board[9][8]='j';
    board[1][1]='c';//c represents red桂
    board[1][7]='c';
    board[9][1]='k';//k representd blue桂
    board[9][7]='k';
    board[1][2]='d';//d represents red銀
    board[1][6]='d';
    board[9][2]='l';//l representd blue銀
    board[9][6]='l';
    board[1][3]='e';//e represents red金
    board[1][5]='e';
    board[9][3]='m';//m represents blue金
    board[9][5]='m';
    board[1][4]='f';//f represents red王
    board[9][4]='n';//n represents blue王
    board[2][1]='g';//g represents red飛
    board[8][7]='o';//o represents blue飛
    board[2][7]='h';//h represents red角
    board[8][1]='p';//p represents blue角
}

void printboard()
{
    for(i=0;i<10;i++)
    {
        for(j=0;j<10;j++)
        {
            if(board[i][j]=='a')
                printf("\033[31m步\033[m");
            if(board[i][j]=='i')
                printf("\033[34m步\033[m");
            if(board[i][j]=='b')
                printf("\033[31m香\033[m");
            if(board[i][j]=='j')
                printf("\033[34m香\033[m");
            if(board[i][j]=='c')
                printf("\033[31m桂\033[m");
            if(board[i][j]=='k')
                printf("\033[34m桂\033[m");
            if(board[i][j]=='d')
                printf("\033[31m銀\033[m");
            if(board[i][j]=='l')
                printf("\033[34m銀\033[m");
            if(board[i][j]=='e')
                printf("\033[31m金\033[m");
            if(board[i][j]=='m')
                printf("\033[34m金\033[m");
            if(board[i][j]=='f')
                printf("\033[31m王\033[m");
            if(board[i][j]=='n')
                printf("\033[34m王\033[m");
            if(board[i][j]=='g')
                printf("\033[31m飛\033[m");
            if(board[i][j]=='o')
                printf("\033[34m飛\033[m");
            if(board[i][j]=='h')
                printf("\033[31m角\033[m");
            if(board[i][j]=='p')
                printf("\033[34m角\033[m");
            if(i==0 || j==9)
                printf("%c ",board[i][j]);
            if(board[i][j]==0)
                printf("  ");
        }
        printf("\n");
    }
}

void movechess()
{
    int a,b;//暫時儲存座標
    
    if(turn%2==1)//藍色移動
    {
        printf("player x enter the coordinate of the chess which you want to move:\n");
        scanf("%d",&a);

        if(a==0)//輸入0悔棋
        {
            regret();
            if(pop)
                fseek(new_game,20,SEEK_CUR);
        }
        else if(a==-1)//輸入-1結束遊戲
        {
            printf("End\n");
            fclose(new_game);
            end=-1;
        }
        else if(a!=0 && a!=-1)
        {
            scanf("%d",&origin_row);
            origin_column=a-1;
            printf("player x enter the new coordinate of the chess:\n");
            scanf("%d %d",&b,&new_row);
            new_column=b-1;
        }
    }
    else if(turn%2==0)//紅色移動
    {
        printf("player y enter the coordinate of the chess which you want to move:\n");
        scanf("%d",&a);

        if(a==0)//輸入0悔棋
        {
            regret();
            if(pop)
                fseek(new_game,20,SEEK_CUR);
        }
        else if(a==-1)//輸入-1結束遊戲
        {
            printf("End\n");
            fclose(new_game);
            end=-1;
        }
        else if(a!=0 && a!=-1)
        {
            scanf("%d",&origin_row);
            origin_column=a-1;
            printf("player y enter the new coordinate of the chess:\n");
            scanf("%d %d",&b,&new_row);
            new_column=b-1;
        }
    }

    if(board[origin_row][origin_column]=='a' && turn%2==0)//red步
    {   
        if(new_row != (origin_row+1))//判斷走位正確
        {
            printf("error!Please enter again.\n");
            movechess();
        }

        if(board[new_row][new_column]!=0)//if board[new_row][new_column] 有棋子
        {
            if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//吃
            {
                if(board[new_row][new_column]=='n')//吃到王
                    king=1;
                
                push(origin_row,origin_column+1,new_row,new_column+1);
                fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                board[new_row][new_column]=board[origin_row][origin_column];
                board[origin_row][origin_column]=0;
                turn++;
            }
            else if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//遇到同色棋子
            {
                printf("error!Please enter again.\n");
                movechess();
            }
        }

        else if(board[new_row][new_column]==0)//move chess
        {
            push(origin_row,origin_column+1,new_row,new_column+1);
            fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
            board[new_row][new_column]=board[origin_row][origin_column];
            board[origin_row][origin_column]=0;
            turn++;
        }
    }

    else if(board[origin_row][origin_column]=='i' && turn%2==1)//blue步
    {
        if(new_row != (origin_row-1))//判斷走位正確
        {
            printf("error!Please enter again.\n");
            movechess();
        }

        if(board[new_row][new_column]!=0)//if board[new_row][new_column] 有棋子
        {
            if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//吃
            {
                if(board[new_row][new_column]=='f')//吃到王
                    king=1;
                
                push(origin_row,origin_column+1,new_row,new_column+1);
                fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                board[new_row][new_column]=board[origin_row][origin_column];
                board[origin_row][origin_column]=0;
                turn++;
            }
            else if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//遇到同色棋子
            {
                printf("error!Please enter again.\n");
                movechess();
            }
        }

         if(board[new_row][new_column]==0)//move chess
        {
            push(origin_row,origin_column+1,new_row,new_column+1);
            fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
            board[new_row][new_column]=board[origin_row][origin_column];
            board[origin_row][origin_column]=0;
            turn++;
        }
    }

    else if(board[origin_row][origin_column]=='b' && turn%2==0)//red香
    {
        if(new_column!=origin_column || (new_row-origin_row <= 0))//判斷規則
        {
            printf("error!Please enter again.\n");
            movechess();
        }
        
        for(i=origin_row+1; i<new_row ;i++)//判斷是否跨越別的棋子
        {
            if(board[i][origin_column]!=0)
            {
                printf("error?Please enter again.\n");
                movechess();
            }
        }

        if(board[new_row][new_column]!=0)//if board[new_row][new_column] 有棋子
        {
            if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//吃
            {
                if(board[new_row][new_column]=='n')//吃到王
                    king=1;
                
                push(origin_row,origin_column+1,new_row,new_column+1);
                fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                board[new_row][new_column]=board[origin_row][origin_column];
                board[origin_row][origin_column]=0;
                turn++;
            }
            else if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//遇到同色棋子
            {
                printf("error!Please enter again.\n");
                movechess();
            }
        }

        if(board[new_row][new_column]==0)//move chess
        {
            push(origin_row,origin_column+1,new_row,new_column+1);
            fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
            board[new_row][new_column]=board[origin_row][origin_column];
            board[origin_row][origin_column]=0;
            turn++;
        }
    }
    
    else if(board[origin_row][origin_column]=='j' && turn%2==1)//blue香
    {           
        if(new_column!=origin_column || (new_row-origin_row >= 0))//判斷規則
        {
            printf("error!Please enter again.\n");
            movechess();
        }

        for(i=origin_row-1 ;i>new_row ;i--)//判斷是否跨越別的棋子
        {
            if(board[i][origin_column]!=0)
            {
                printf("error?Please enter again.\n");
                movechess();
            }
        }

        if(board[new_row][new_column]!=0)//if board[new_row][new_column] 有棋子
        {
            if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//吃
            {
                if(board[new_row][new_column]=='f')//吃到王
                    king=1;

                push(origin_row,origin_column+1,new_row,new_column+1);
                fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                board[new_row][new_column]=board[origin_row][origin_column];
                board[origin_row][origin_column]=0;
                turn++;
            }
            else if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//遇到同色棋子
            {
                printf("error!Please enter again.\n");
                movechess();
            }
        }

        if(board[new_row][new_column]==0)//move chess
        {
            push(origin_row,origin_column+1,new_row,new_column+1);
            fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
            board[new_row][new_column]=board[origin_row][origin_column];
            board[origin_row][origin_column]=0;
            turn++;
        }
    }

     else if(board[origin_row][origin_column]=='c' && turn%2==0)//red桂
     {     
        if(new_row-origin_row!=2 || (new_row-origin_row==2 && (abs(new_column-origin_column)!=1)))//判斷移動正確
        {
            printf("error!Please enter again.\n");
            movechess();
        }

         if(board[new_row][new_column]!=0)//if board[new_row][new_column] 有棋子
        
        {
            if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//吃
            {
                if(board[new_row][new_column]=='n')//吃到王
                    king=1;
                
                push(origin_row,origin_column+1,new_row,new_column+1);
                fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                board[new_row][new_column]=board[origin_row][origin_column];
                board[origin_row][origin_column]=0;
                turn++;
            }
            else if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//遇到同色棋子
            {
                printf("error!Please enter again.\n");
                movechess();
            }
        }

        if(board[new_row][new_column]==0)//move chess
        {
            push(origin_row,origin_column+1,new_row,new_column+1);
            fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
            board[new_row][new_column]=board[origin_row][origin_column];
            board[origin_row][origin_column]=0;
            turn++;
        }
     }

    else if(board[origin_row][origin_column]=='k' && turn%2==1)//blue桂
     {     
        if(new_row-origin_row!=-2 || (new_row-origin_row==-2 && (abs(new_column-origin_column)!=1)))//判斷移動正確
        {
            printf("error!Please enter again.\n");
            movechess();
        }

         if(board[new_row][new_column]!=0)//if board[new_row][new_column] 有棋子
        {
            if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//吃
            {
                if(board[new_row][new_column]=='f')//吃到王
                    king=1;
                
                push(origin_row,origin_column+1,new_row,new_column+1);
                fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                board[new_row][new_column]=board[origin_row][origin_column];
                board[origin_row][origin_column]=0;
                turn++;
            }
            else if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//遇到同色棋子
            {
                printf("error!Please enter again.\n");
                movechess();
            }
        }

        if(board[new_row][new_column]==0)//move chess
        {
            push(origin_row,origin_column+1,new_row,new_column+1);
            fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
            board[new_row][new_column]=board[origin_row][origin_column];
            board[origin_row][origin_column]=0;
            turn++;
        }
     }

    else if(board[origin_row][origin_column]=='d' && turn%2==0)//red銀
    {      
        if((abs(new_column-origin_column))>1 || (abs(new_row-origin_row))>1 || ((new_row==origin_row) && (abs(new_column-origin_column))==1) || ((new_row==origin_row-1) && new_column==origin_column))//判斷移動正確
        {
            printf("error!Please enter again.\n");
            movechess();
        }

        if(board[new_row][new_column]!=0)//if board[new_row][new_column]有棋子
        {
            if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//吃
            {
                if(board[new_row][new_column]=='n')//吃到王
                    king=1;
                
                push(origin_row,origin_column+1,new_row,new_column+1);
                fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                board[new_row][new_column]=board[origin_row][origin_column];
                board[origin_row][origin_column]=0;
                turn++;
            }
            else if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//遇到同色棋子
            {
                printf("error!Please enter again.\n");
                movechess();
            }
        }
        
        if(board[new_row][new_column]==0)//move chess
        {
            push(origin_row,origin_column+1,new_row,new_column+1);
            fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
            board[new_row][new_column]=board[origin_row][origin_column];
            board[origin_row][origin_column]=0;
            turn++;
        }
    }

    else if(board[origin_row][origin_column]=='l' && turn%2==1)//blue銀
    {
        if((abs(new_column-origin_column))>1 || (abs(new_row-origin_row))>1 || ((new_row==origin_row) && (abs(new_column-origin_column))==1) || ((new_row==origin_row+1) && new_column==origin_column))//判斷移動正確
        {
            printf("error!Please enter again.\n");
            movechess();
        }

        if(board[new_row][new_column]!=0)//if board[new_row][new_column]有棋子
        {
            if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//吃
            {
                if(board[new_row][new_column]=='f')//吃到王
                    king=1;
                
                push(origin_row,origin_column+1,new_row,new_column+1);
                fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                board[new_row][new_column]=board[origin_row][origin_column];
                board[origin_row][origin_column]=0;
                turn++;
            }
            if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//遇到同色棋子
            {
                printf("error!Please enter again.\n");
                movechess();
            }
        }
        
        if(board[new_row][new_column]==0)//move chess
        {
            push(origin_row,origin_column+1,new_row,new_column+1);
            fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
            board[new_row][new_column]=board[origin_row][origin_column];
            board[origin_row][origin_column]=0;
            turn++;
        }
    }

    else if(board[origin_row][origin_column]=='e' && turn%2==0)//red金
    {  
        if((new_column-origin_column==-1) && (abs(new_row-origin_row)==1))//判斷移動正確
        {
            printf("error!Please enter again.\n");
            movechess();
        }

        if(board[new_row][new_column]!=0)//if board[new_row][new_column]有棋子
        {
            if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//吃
            {
                if(board[new_row][new_column]=='n')//吃到王
                    king=1;
                
                push(origin_row,origin_column+1,new_row,new_column+1);
                fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                board[new_row][new_column]=board[origin_row][origin_column];
                board[origin_row][origin_column]=0;
                turn++;
            }
            if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//遇到同色棋子
            {
                printf("error!Please enter again.\n");
                movechess();
            }
        }

        if(board[new_row][new_column]==0)//move chess
        {
            push(origin_row,origin_column+1,new_row,new_column+1);
            fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
            board[new_row][new_column]=board[origin_row][origin_column];
            board[origin_row][origin_column]=0;
            turn++;
        }
    }

    else if(board[origin_row][origin_column]=='m' && turn%2==1)//blue金
    {
        if((new_column-origin_column==1) && (abs(new_row-origin_row)==1)){
            printf("error!Please enter again.\n");
            movechess();
        }//判斷移動正確

        if(board[new_row][new_column]!=0)//if board[new_row][new_column]有棋子
        {   
            if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//吃
            {
                if(board[new_row][new_column]=='f')//吃到王
                    king=1;
                
                push(origin_row,origin_column+1,new_row,new_column+1);
                fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                board[new_row][new_column]=board[origin_row][origin_column];
                board[origin_row][origin_column]=0;
                turn++;
            }
            if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//遇到同色棋子
            {
                printf("error!Please enter again.\n");
                movechess();
            }
        }

        if(board[new_row][new_column]==0)//move chess
        {
            push(origin_row,origin_column+1,new_row,new_column+1);
            fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
            board[new_row][new_column]=board[origin_row][origin_column];
            board[origin_row][origin_column]=0;
            turn++;
        }
    }
    
    else if((board[origin_row][origin_column]=='f' && turn%2==0) || (board[origin_row][origin_column]=='n' && turn%2==1))//王
    {
        if((abs(new_column-origin_column)>1) || (abs(new_row-origin_row)>1))//判斷移動正確
        {
            printf("error!Please enter again.\n");
            movechess();
        }
        
        if(board[new_row][new_column]!=0)//if board[new_row][new_column]有棋子
        {
            if(board[origin_row][origin_column]=='f')//red王
            {
                if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//吃
                {
                    if(board[new_row][new_column]=='n')//吃到王
                        king=1;
                    
                    push(origin_row,origin_column+1,new_row,new_column+1);
                    fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                    board[new_row][new_column]=board[origin_row][origin_column];
                    board[origin_row][origin_column]=0;
                    turn++;
                }
                if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//遇到同色棋子
                {
                    printf("error!Please enter again.\n");
                    movechess();
                }
            }
            
            if(board[origin_row][origin_column]=='n')//blue王
            {
                if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//吃
                {
                    if(board[new_row][new_column]=='f')//吃到王
                        king=1;
                    
                    push(origin_row,origin_column+1,new_row,new_column+1);
                    fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                    board[new_row][new_column]=board[origin_row][origin_column];
                    board[origin_row][origin_column]=0;
                    turn++;
                }
                if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//遇到同色棋子
                {
                    printf("error!Please enter again.\n");
                    movechess();
                }
            }
        }
        
        if(board[new_row][new_column]==0)//move chess
        {
            push(origin_row,origin_column+1,new_row,new_column+1);
            fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
            board[new_row][new_column]=board[origin_row][origin_column];
            board[origin_row][origin_column]=0;
            turn++;
        }
    }

    else if((board[origin_row][origin_column]=='g' && turn%2==0) || (board[origin_row][origin_column]=='o' && turn%2==1))//飛
    {
        if(new_column-origin_column != 0 && new_row-origin_row != 0)//判斷移動正確
        {
            printf("error!Please enter again.\n");
            movechess();
        }

        if(new_row-origin_row<0) //上的走
        {
            for(i=origin_row-1; i>new_row; i--)
            {
                if(board[i][origin_column]!=0)
                {
                    printf("error!Please enter again.\n");
                    movechess();
                }
            }
        }

        else if(new_row-origin_row>0) //下的走
        {
            for(i=origin_row+1; i<new_row; i++)
            {
                if(board[i][origin_column]!=0)
                {
                    printf("error!Please enter again.\n");
                    movechess();
                }
            }
        }

        else if(new_column-origin_column<0) //左的走
        {
            for(i=new_column-1; i>origin_column; i--)
            {
                if(board[origin_row][i]!=0)
                {
                    printf("error!Please enter again.\n");
                    movechess();
                }
            }
        }

        else if(new_column-origin_column>0) //右的走
        {
            for(i=new_column+1; i<origin_column; i++)
            {
                if(board[origin_row][i]!=0)
                {
                    printf("error!Please enter again.\n");
                    movechess();
                }
            }
        }
        
        if(board[new_row][new_column]!=0)//if board[new_row][new_column]有棋子
        {
            if(board[origin_row][origin_column]=='g')//red飛
            {
                if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//吃
                {
                    if(board[new_row][new_column]=='n')//吃到王
                        king=1;
                    
                    push(origin_row,origin_column+1,new_row,new_column+1);
                    fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                    board[new_row][new_column]=board[origin_row][origin_column];
                    board[origin_row][origin_column]=0;
                    turn++;
                }
                if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//遇到同色棋子
                {
                    printf("error!Please enter again.\n");
                    movechess();
                }
            }
            
            if(board[origin_row][origin_column]=='o')//blue飛
            {
                if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//吃
                {
                    if(board[new_row][new_column]=='f')//吃到王
                        king=1;
                    
                    push(origin_row,origin_column+1,new_row,new_column+1);
                    fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                    board[new_row][new_column]=board[origin_row][origin_column];
                    board[origin_row][origin_column]=0;
                    turn++;
                }
                else if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//遇到同色棋子
                {
                    printf("error!Please enter again.\n");
                    movechess();
                }
            }
        }
        
        if(board[new_row][new_column]==0)//move chess
        {
            push(origin_row,origin_column+1,new_row,new_column+1);
            fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
            board[new_row][new_column]=board[origin_row][origin_column];
            board[origin_row][origin_column]=0;
            turn++;
        }
    }

    else if((board[origin_row][origin_column]=='h' && turn%2==0) || (board[origin_row][origin_column]=='p' && turn%2==1))//角
    {
        if(abs(new_column-origin_column) != abs(new_row-origin_row))
        {
            printf("error!Please enter again.\n");
            movechess();
        }

        if(new_row-origin_row<0 && new_column-origin_column>0) //右上的走
        {
            j=origin_row-1;
            for(i=origin_column+1; i<new_column; i++)
            {
                if(board[j][i]!=0)
                {
                    printf("error!Please enter again.\n");
                    movechess();
                }
                j--;
            }
        }

        else if(new_row-origin_row>0 && new_column-origin_column>0) //右下的走
        {
            j=origin_row+1;
            for(i=origin_column+1; i<new_column; i++)
            {
                if(board[j][i]!=0)
                {
                    printf("error!Please enter again.\n");
                    movechess();
                }
                j++;
            }
        }

        else if(new_row-origin_row>0 && new_column-origin_column<0) //左下的走
        {
            j=origin_row+1;
            for(i=origin_column-1; i>new_column; i--)
            {
                if(board[j][i]!=0)
                {
                    printf("error!Please enter again.\n");
                    movechess();
                }
                j++;
            }
        }

        else if(new_row-origin_row<0 && new_column-origin_column<0) //左上的走
        {
            j=origin_row-1;
            for(i=origin_column-1; i>new_column; i--)
            {
                if(board[j][i]!=0)
                {
                    printf("error!Please enter again.\n");
                    movechess();
                }
                j--;
            }
        }
        
        if(board[new_row][new_column]!=0)//if board[new_row][new_column]有棋子
        {
            if(board[origin_row][origin_column]=='h')//red角
            {
                if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//吃
                {
                    if(board[new_row][new_column]=='n')//吃到王
                        king=1;
                    
                    push(origin_row,origin_column+1,new_row,new_column+1);
                    fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                    board[new_row][new_column]=board[origin_row][origin_column];
                    board[origin_row][origin_column]=0;
                    turn++;
                }
                else if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//遇到同色棋子
                {
                    printf("error!Please enter again.\n");
                    movechess();
                }
            }
            
            else if(board[origin_row][origin_column]=='p')//blue角
            {
                if((96<board[new_row][new_column]) && (board[new_row][new_column]<105))//吃
                {
                    if(board[new_row][new_column]=='f')//吃到王
                        king=1;
                    
                    push(origin_row,origin_column+1,new_row,new_column+1);
                    fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
                    board[new_row][new_column]=board[origin_row][origin_column];
                    board[origin_row][origin_column]=0;
                    turn++;
                }
                else if((104<board[new_row][new_column]) && (board[new_row][new_column]<113))//遇到同色棋子
                {
                    printf("error!Please enter again.\n");
                    movechess();
                }
            }
        }
        
        if(board[new_row][new_column]==0)//move chess
        {
            push(origin_row,origin_column+1,new_row,new_column+1);
            fprintf(new_game,"%-6d%-2d%-2d%3d%2d%4c\n",turn,origin_column+1,origin_row,new_column+1,new_row,board[origin_row][origin_column]);
            board[new_row][new_column]=board[origin_row][origin_column];
            board[origin_row][origin_column]=0;
            turn++;
        }
    }
}

void regret()
{
    pop();
    printboard();
    movechess();
}