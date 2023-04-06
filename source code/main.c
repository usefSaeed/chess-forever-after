#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

char A[8] = {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};
char c[8][8];
char D[600][8][8];
int brk2 = 0;
int restart = 0;
int player = 2;
int sm, dp, chck;
int prev_c = -1, curr_c = -1, prev_r = -1, curr_r = -1;
int endGame = 0,undo = 0;
int moveIndex = 0;
int brk = 0;
int invalid = 0;
int pieceIndex[2];
int promotion = 0;
int detectPromotion = 0;
int promotionAlert = 0;
int threeFold = 0;
int fiveFold = 0;
int fd = 0;
char input[7];
char whiteTaken[16];
char saveWhiteTaken[600][16];
char saveBlackTaken[600][16];
char blackTaken[16];
char player1[50], player2[50];

//function that pauses program for seconds seconds
int sleep(int seconds);

//fills array with spaces
void fillWithSpace(char A[], int n){
    int i;
    for (i=0;i<n;i++){
        A[i] = ' ';
    }
}
//SAVING CURRENT BOARD
void saveBoard(){
    int i,j;
    // TO OUR DATABASE ARRAY OF BOARD
    for(i=0;i<8;i++){
        for (j=0;j<8;j++){
            D[moveIndex][i][j] = c[i][j];
        }
    }
    //SAVING PIECES CAPTURED
    for (i=0;i<16;i++){
        saveBlackTaken[moveIndex][i] = blackTaken[i];
        saveWhiteTaken[moveIndex][i] = whiteTaken[i];
    }
    moveIndex++;
}

//RETURNS STRING LENGTH
int strLength(char str[]){
    int i = 0;
    char *q;
    q = str;
    for (q;*q!='\0';q++){
        i++;
    }
    return i;
}

//RETURNS 1 WHEN DEAD POSITION TAKES PLACE
int deadPosition(){
    int nb=0,NB=0,colourb,colourB,b=0,B=0,i,j,specialCase=0;
    for (i=0;i<8;i++){
        for (j=0;j<8;j++){
            if (c[i][j]!= 'k' && c[i][j]!= 'K' && c[i][j]!= 'b' && c[i][j]!= 'B' && c[i][j]!= 'n' && c[i][j]!= 'N' && c[i][j] != ' '){
                //RETURNS 0 IF THERE'S ANYTHING RATHER THAN PREVIOUS CONDITIONS
                return 0;
            }else if (c[i][j] == 'b' || c[i][j] == 'n'){
                nb++;
                //FOR 2 BISHOPS SPECIAL CASE
                if (c[i][j] == 'b'){
                    colourb = i+j;
                    b++;
                }
            }else if (c[i][j] == 'N' || c[i][j] == 'B'){
                NB++;
                //FOR 2 BISHOP SPECIAL CASE
                if (c[i][j] == 'B'){
                    colourB = i+j;
                    B++;
                }
            }
        }
    }
    //COVERING 2 BISHOP SPECIAL CASE
    if (b==1 && B==1 && (colourB%2 == colourb%2)){
        specialCase = 1;
    }
    //COVERING ALL DEAD POSITION POSSIBILITIES
    if ((nb==1 && NB==0) || (nb==0 && NB==1)  || specialCase){
        return 1;
    }else{
        return 0;
    }
}

//MAKES A MOVE ,SHOWS NEW BOARD AND SHIFTS PLAYER TURN
void move(){
    int i,j,k,count=0;
    //NAMING EACH COLUMN
    printf("\n\n\n\t       White Pieces Captured\t\t\t\t\t\t\t\t\t\t     Black Pieces Captured\n\n\t\t\t\t\t    A        B        C        D        E        F        G        H");
    //LOOPS 8 TIMES TO FILL ALL BLOCK ROWS
    for (i=0;i<8;i++){
        (i!=0) ? count+=2 : 0;
        //LOOPS 5 TIMES AS EACH BLOCK HAS 5 ROWS
        for (k=0;k<5;k++){
            //DISPLAYING ROW NAMES
            if (k==2){
                printf("\n\t\t     %c      %c\t     %d  ",whiteTaken[count],whiteTaken[count+1],(player==2) ? 8-i : i+1);
            }else{
                printf("\n\t\t\t\t\t");
            }
            //LOOPS 8 TIMES TO FILL ALL COLUMNS OF SUB-ROW k IN ROW i
            for (j=0;j<8;j++){
                switch(k){
                    case 0:     if ((i+j)%2==0){//WHITE BLOCK
                                    printf("---------");
                                }else{//BLACK BLOCK
                                    printf("#########");
                                }
                                break;
                    case 4:     if ((i+j)%2==0){
                                    printf("---------");
                                }else{
                                    printf("#########");
                                }
                                break;
                    //WHERE WE PLACE THE PIECE
                    case 2:     if ((i+j)%2==0){
                                    printf("|   %c   |",(player==2) ? c[i][j] : c[7-i][j]);
                                }else{
                                    printf("#   %c   #",(player==2) ? c[i][j] : c[7-i][j]);
                                }
                                //DOUBLE NAMING COLUMNS
                                if(j==7){
                                    printf("   %d       %c      %c",(player==2) ? 8-i : i+1, blackTaken[count], blackTaken[count+1]);
                                }
                                break;
                    default:    if ((i+j)%2==0){
                                    printf("|       |");
                                }else{
                                    printf("#       #");
                                }

                }
            }
        }
    }
    //DOUBLE NAMING COLUMNS
    printf("\n\t\t\t\t\t    A        B        C        D        E        F        G        H\n");
    saveBoard();
    switch (player){
        case 1: player = 2;
                break;
        case 2: player = 1;
                break;
    }
    //DECLARING VARIABLES TO REPETITIVE FUNCTIONS EVERY MOVE
    sm = stalemate();
    dp = deadPosition();
    chck = check();
    fd = fold();
    //STATING FIVEFOLD AND THREEFOLD IN THIS MOVE
    if (fd==5){
        fiveFold = 1;
        threeFold = 1;
    }else if (fd==3){
        threeFold = 1;
    }else{
        fiveFold = threeFold = 0;
    }
    //PRINTS WHEN PROMOTION TAKES PLACE
    if (promotion){
            printf("\n%s promoted his pawn to '%c' !\n",(player == 1) ? player2 : player1, c[curr_r][curr_c]);
        }
    //ENDING GAME AND IF NOT SHIFTS TURN
    if (!sm && !dp && !fiveFold){
        printf("\n%s ! Its your turn.\t\t\t\t\t\t\t\t\t  (RES --> restart / R --> Resign / U --> Undo / D --> Draw / RE --> Redo / S --> Save / L --> Load)\n",(player == 1) ? player1 : player2);
        if (chck){
            printf("\n\a\t\t\t\t\tYOUR KING IS IN CHECK !!!\n");
        }else if (threeFold){
            printf("\nYou have the right to end the game in draw by threefold repetition\n");
        }
    }else if (sm && !dp && !chck && !fiveFold){
        printf("\n\t\t\t\t\tTHE GAME ENDED IN A DRAW BY STALEMATE !!!\n ");
        endGame = 1;
    }else if (dp){
        printf("\n\t\t\t\t\tTHE GAME ENDED IN A DRAW BY DEAD POSITION !!!\n ");
        endGame = 1;
    }else if (fiveFold){
        printf("\n\t\t\t\t\tTHE GAME ENDED IN A DRAW BY FIVEFOLD REPETITION !!!\n ");
        endGame = 1;
    }else if (chck && sm){
        printf("\n\t\t\tCHECKMATE !!! ");
        congrats((player == 1) ? player2 : player1);
        endGame = 1;
    }
}

//RETURNS HOW MANY TIMES CURRENT BOARD HAS BEEN REPEATED
int fold(){
    int i,j,k,count,foldCount=0;
    for (k=moveIndex-1;k>-1;k-=2){
        count = 0;
        for (i=0;i<8;i++){
            for (j=0;j<8;j++){
                if (c[i][j] == D[k][i][j])
                    count++;
            }
        }
        (count == 64) ? foldCount++ : 0;
    }
    return foldCount;
}

//CHECKS IF THE DESIRED MOVE WILL RESULT A CHECK TO CURRENT KING
int possibleCheck(int p_r,int p_c,int c_r,int c_c){
    int a,i,j;
    //MAKING MOVE
    shiftArray(p_r, p_c, c_r, c_c);
    if (check()){
        a = 1;
    }else{
        a = 0;
    }
    //PUTTING EVERYTHING BAC TO NORMAL
    for(i=0;i<8;i++){
        for (j=0;j<8;j++){
            c[i][j] = D[moveIndex-1][i][j];
        }
    }
    for (i=0;i<16;i++){
            whiteTaken[i] = saveWhiteTaken[moveIndex-1][i];
            blackTaken[i] = saveBlackTaken[moveIndex-1][i];
    }
    if (a){
        return 1;
    }else{
        return 0;
    }
}

//TAKES INPUT OF USER AND CHECK IF IT IS VALID
//RECURSE TILL USER ENTERS A VALID ONE
void validate_inp(){
    int a=0, b=0, k,i,j;
    int x = strLength(input);
    char A[]={'A','B','C','D','E','F','G','H'};
    char B[]={'1','2','3','4','5','6','7','8'};
    char *p;
    p = input;
    //IF USED MADE A NON-MOVE BREAK OUT OF FUNCTION TO CONTINUE EXECUTION IN MAIN
    if((input[0]=='U' && input[1]=='\0')||(input[0]=='L' && input[1]=='\0')||(input[0]=='S' && input[1]=='\0') ||(input[0] == 'R' && input[1] == 'E' && input[2] =='\0')||(input[0]=='R' && input[1]=='\0')||(input[0]=='D' && input[1]=='\0')){
        brk = 1;
    }else{
        brk = 0;
        for (j=0;j<8;j++){
            if (*p==A[j]){
                a++;
                break;
            }
        }
        for (j=0;j<8;j++){
            if (*(p+1)==B[j]){
                a++;
                break;
            }
        }
        for (j=0;j<8;j++){
            if (*(p+2)==A[j]){
                a++;
                break;
            }
        }
        for (j=0;j<8;j++){
            if (*(p+3)==B[j]){
                a++;
                break;
            }
        }
        if (*p != *(p+2) || *(p+1) != *(p+3)){
            b=1;
        }
        //CHECKING THAT INPUT IS 'A1A2' SIMILAR AND HAS 4 OR 5 CHARACTORS AND NOT 'A1A1'
        if ((x!=4 && x!=5) || a!=4 || !b){
            printf("\nPlease Enter valid input\n");
            invalid = 1;
            gets(input);
            validate_inp();
            return;
        }else{
        //CONVERT 'A1A2' TO INDICES OF c
            switch(*p) {

                case 'A': prev_c = 0;
                          break;
                case 'B': prev_c = 1;
                          break;
                case 'C': prev_c = 2;
                          break;
                case 'D': prev_c = 3;
                          break;
                case 'E': prev_c = 4;
                          break;
                case 'F': prev_c = 5;
                          break;
                case 'G': prev_c = 6;
                          break;
                case 'H': prev_c = 7;
                          break;
            }

            switch(*(p+1)) {

                case '1': prev_r = 7;
                          break;
                case '2': prev_r = 6;
                          break;
                case '3': prev_r = 5;
                          break;
                case '4': prev_r = 4;
                          break;
                case '5': prev_r = 3;
                          break;
                case '6': prev_r = 2;
                          break;
                case '7': prev_r = 1;
                          break;
                case '8': prev_r = 0;
                          break;
            }

            switch(*(p+2)) {

                case 'A': curr_c = 0;
                          break;
                case 'B': curr_c = 1;
                          break;
                case 'C': curr_c = 2;
                          break;
                case 'D': curr_c = 3;
                          break;
                case 'E': curr_c = 4;
                          break;
                case 'F': curr_c = 5;
                          break;
                case 'G': curr_c = 6;
                          break;
                case 'H': curr_c = 7;
                          break;
            }

            switch(*(p+3)) {

                case '1': curr_r = 7;
                          break;
                case '2': curr_r = 6;
                          break;
                case '3': curr_r = 5;
                          break;
                case '4': curr_r = 4;
                          break;
                case '5': curr_r = 3;
                          break;
                case '6': curr_r = 2;
                          break;
                case '7': curr_r = 1;
                          break;
                case '8': curr_r = 0;
                          break;
            }
            //CHECK IF 5 CHARACTOR INPUT IS A VALID PROMOTION
            promotion = checkPromotion();
            //CASE X: SET PROMOTION = 1 IF PROMOTION INPUT BECAME VALID IN SECOND TO LAST INPUT
            (brk2) ? promotion = 1 : 0;
            //IF INPUT IS NOT A NON-MOVE AND NOT CASE X
            if (!brk && !brk2){
                if (c[prev_r][prev_c] == ' '){
                    printf("\nDuh. You haven't chosen any piece. Please enter another input.\n");
                    invalid = 1;
                    gets(input);
                    validate_inp();
                    return;
                }
                if (player==1){
                    if ( isupper(c[prev_r][prev_c]) ){
                        printf("\n'%c' is not your piece. Please enter another input.\n",c[prev_r][prev_c]);
                        invalid = 1;
                        gets(input);
                        validate_inp();
                    }
                    else if ( islower(c[curr_r][curr_c])) {
                        printf("\nPlease Enter valid input.\n");
                        invalid = 1;
                        gets(input);
                        validate_inp();
                        return;
                    }
                }
                if (player==2){
                    if ( islower(c[prev_r][prev_c]) ){
                        printf("\n'%c' is not your piece. Please enter another input.\n",c[prev_r][prev_c]);
                        invalid = 1;
                        gets(input);
                        validate_inp();
                        return;
                    }
                    else if ( isupper(c[curr_r][curr_c]) ){
                        printf("\nPlease Enter valid input\n");
                        invalid = 1;
                        gets(input);
                        validate_inp();
                        return;
                    }
                }
                //CHECK IF MOVE WILL RESULT CHECK TO CURRENT KING
                if (possibleCheck(prev_r, prev_c, curr_r, curr_c)){
                    printf("\n'%c' cannot move here. Enter another input.\n",c[prev_r][prev_c]);
                    invalid = 1;
                    gets(input);
                    validate_inp();
                    return;
                //CHECK IF MOVE IS VALID FOR PIECE
                }else if (!check_movement(prev_r,prev_c,curr_r,curr_c) && !promotionAlert){
                    printf("\n'%c' cannot move here. Enter another input.\n",c[prev_r][prev_c]);
                    invalid = 1;
                    gets(input);
                    validate_inp();
                    return;
                //CHECK IF MOVE WAS SUPPOSED TO BE 5 CHARACTORS
                }else if (promotionAlert){
                    printf("\nInvalid input for promotion of pawn. Enter another input.\n",c[prev_r][prev_c]);
                    invalid = 1;
                    gets(input);
                    validate_inp();
                    promotionAlert = 0;
                    return;
                }
            }
        }
    }
}


//CHECK IF USER WANTS TO PROMOTE AND WHETHER IF HIS INPUT IS VALID
int checkPromotion(){
    int i = 0;
    int x = strLength(input);
    if (x == 5){
        if (c[prev_r][prev_c] == 'p' || c[prev_r][prev_c] == 'P'){
            detectPromotion = 1;
            if (check_movement(prev_r,prev_c,curr_r,curr_c)){
                //MUST BE IN THE BEFORE LAST ROW
                if ((player==1) ? (prev_r == 1) : (prev_r == 6)){
                    //CHECK IF HIS PROMOTION OPTION IS VALID
                    for (i = 0; i < 4; i++ ){
                        if ((player==1) ? input[4] == tolower(A[i]) : input[4] == A[i]){
                            brk2 = 0;
                            detectPromotion = 0;
                            return 1;
                        }
                    }
                    printf("\nPlease Enter a valid input for promotion\n");
                    invalid = 1;
                    gets(input);
                    validate_inp();
                    //CASE X
                    (promotion) ? brk2 = 1 : 0;
                    detectPromotion = 0;
                    return 0;
                }else{
                    printf("\nPlease Enter valid input\n");
                    invalid = 1;
                    gets(input);
                    validate_inp();
                    //CASE X
                    (promotion) ? brk2 = 1 : 0;
                    detectPromotion = 0;
                    return 0;
                }
            }else{
                printf("\nPlease Enter valid input\n");
                invalid = 1;
                gets(input);
                validate_inp();
                //CASE X
                (promotion) ? brk2 = 1 : 0;
                detectPromotion = 0;
                return 0;
            }
        }else{
            printf("\nPlease Enter valid input\n");
            invalid = 1;
            gets(input);
            validate_inp();
            //CASE X
            (promotion) ? brk2 = 1 : 0;
            detectPromotion = 0;
            return 0;
        }
    }
    detectPromotion = 0;
    brk2 = 0;
    return 0;
}

//CHECK IF MOVE IS VALID FOR PIECE
int check_movement(int prev_r,int prev_c,int curr_r,int curr_c){
    //safeside conditions
    if (player ==1 && islower(c[curr_r][curr_c]) || (player==2 && isupper(c[curr_r][curr_c]))){
        return 0;
    }
    if (curr_r<0 || curr_r>7 || curr_c<0 || curr_c>7 || prev_r<0 || prev_r>8 || prev_r<0 || prev_r>8){
        return 0;
    }
    int i=0,j=0,k=0;
    //KNIGHT COVERS 8 FORMS OF L SHAPE
    if (c[prev_r][prev_c] == 'n' || c[prev_r][prev_c] == 'N'){
        if ( abs(curr_c - prev_c) == 1 ){
            if (abs(curr_r - prev_r ) == 2){
                return 1;
            }else{
                return 0;
            }
        }else if( abs(curr_c - prev_c) == 2){
            if (abs(curr_r - prev_r) == 1){
                return 1;
            }else{
                return 0;
            }
        }else{
            return 0;
        }
    //KING MOVES ONE STEP AT ANY DIRECTION AND 2 IN CASE OF CASTLING
    }else if (c[prev_r][prev_c] == 'k' || c[prev_r][prev_c] == 'K'){
        if( abs(curr_c - prev_c) == 1 ){
            if (abs(curr_r - prev_r) == 1 || curr_r - prev_r == 0){
                return 1;
            }
        }else if(curr_c - prev_c == 0){
            if (abs(curr_r - prev_r) == 1){
                return 1;
            }
        //LEFT CASTLING
        //KING MUST NOT BE IN CHECK
        }else if(curr_c - prev_c == 2 && curr_r == prev_r && !check()){
            if (c[curr_r][5] == ' ' && c[curr_r][6] == ' ') {
                //CHECKING IF KING AND ROOK HAVEN'T MOVED SINCE THE BEGINNING OF THE GAME
                for (i=0;i<moveIndex;i++){
                    ( D[i][prev_r][4] != ' ' ) ? j++ : 0;
                    ( D[i][prev_r][7] != ' ' ) ? k++ : 0;
                }
                if (j == moveIndex && k == moveIndex){
                    c[prev_r][curr_c-1] = c[prev_r][7];
                    c[prev_r][7] = ' ';
                    return 1;
                }
            }
        //RIGHT CASTLING
        }else if(curr_c - prev_c == -2 && curr_r == prev_r && !check(c)){
            if (c[curr_r][1] == ' ' && c[curr_r][2] == ' ' && c[curr_r][3] == ' '){
                for (i=0;i<moveIndex;i++){
                    ( D[i][prev_r][4] != ' ' ) ? j++ : 0;
                    ( D[i][prev_r][0] != ' ' ) ? k++ : 0;
                }
                if (j == moveIndex && k == moveIndex){
                    c[prev_r][curr_c+1] = c[prev_r][0];
                    c[prev_r][0] = ' ';
                    return 1;
                }
            }
        }
        return 0;
    }
    //ROOK AND QUEEN CAN MOVE UP,DOWN,LEFT AND RIGHT TIL THEY FIND ANOTHER PIECE OR REACH THE END OF THE BOARD
    else if (c[prev_r][prev_c] == 'r' || c[prev_r][prev_c] == 'R' || c[prev_r][prev_c] == 'q' || c[prev_r][prev_c] == 'Q'){
        //MOVING VERTICALLY
        if(prev_c == curr_c) {
                //DOWNWARDS
            if(curr_r > prev_r) {
                for (i=prev_r+1;i<curr_r;i++){
                    if (c[i][curr_c] != ' '){
                        return 0;
                    }
                }
                return 1;
            }
                //UPWARDS
            else{
                for (i=prev_r-1;i>curr_r;i--){
                    if (c[i][curr_c] != ' '){
                        return 0;
                    }
                }
                return 1;
            }
        }
        //MOVING HORIZONTALLY
        else if(prev_r == curr_r){
            //LEFT
            if(curr_c > prev_c) {
                for (i=prev_c+1;i<curr_c;i++){
                    if (c[curr_r][i] != ' '){
                        return 0;
                    }
                }
                return 1;
            }
            //RIGHT
            else{
                for (i=prev_c-1;i>curr_c;i--){
                    if (c[curr_r][i] != ' '){
                        return 0;
                    }
                }
                return 1;
            }
        }
        //DONT RETURN 0 TO COVER THE POSSIBILITY THAT ITS A QUEEN MOVING LIKE A BISHOP
    }
    //BISHOP OR QUEEN CAN MOVE DIAGONALLY IN THE 4 DIRECTIONS TILL THEY FIND ANOTHER PIECE OR REACH THE END OF THE BOARD
    if (c[prev_r][prev_c] == 'B' || c[prev_r][prev_c] == 'b' || c[prev_r][prev_c] == 'q' || c[prev_r][prev_c] == 'Q'){
        //CHECK IF MOVE IS IN DIAGONAL
        if (abs(curr_c - prev_c) == abs(curr_r - prev_r)){
            //DOWN LEFT
            if(curr_r > prev_r && curr_c > prev_c) {
                i=prev_r+1;
                j=prev_c+1;
                while ((i != curr_r) && (j != curr_c)) {
                     if (c[i][j] != ' '){
                        return 0;
                    }
                    i++;
                    j++;
                }
                return 1;
            //DOWN RUGHT
            }else if(curr_r > prev_r && curr_c < prev_c) {
                i=prev_r+1;
                j=prev_c-1;
                 while ((i != curr_r) && (j != curr_c)) {
                     if (c[i][j] != ' '){
                        return 0;
                    }
                    i++;
                    j--;
                }
                return 1;
            //UP RIGHT
            }else if(curr_r < prev_r && curr_c > prev_c) {
                i=prev_r-1;
                j=prev_c+1;
                 while ((i != curr_r) && (j != curr_c)) {
                     if (c[i][j] != ' '){
                        return 0;
                    }
                    i--;
                    j++;
                }
                return 1;
            //DOWN RIGHT
            }else if(curr_r < prev_r && curr_c < prev_c) {
                i=prev_r-1;
                j=prev_c-1;
                 while ((i != curr_r) && (j != curr_c)) {
                     if (c[i][j] != ' '){
                        return 0;
                    }
                    i--;
                    j--;
                }
                return 1;
            }
        }
        return 0;
    }
    //RETURN 0 IF IT'S A ROOK INVALID MOVE WHICH HAVENT BEEN SHUT OUT YET
    if (c[prev_r][prev_c] == 'r' || c[prev_r][prev_c] == 'R'){
        return 0;
    }
    else if (c[prev_r][prev_c] == 'p'){
        //PROMOTE PAWN TO ENTERED 5TH CHARACTOR
        if (promotion){
            c[prev_r][prev_c] = input[4];
            return 1;
        //DO THAT IF FUNCTION IS CALLED FROM CHECK PROMOTION OR WHEN PAWN IS NOT IN THE BEFORE LAST ROW
        }else if (prev_r != 1 || detectPromotion){
            //FORWARD ONE BLOCK MOVE
            if (prev_c == curr_c && curr_r - prev_r == -1 && c[curr_r][curr_c] == ' '){
                return 1;
            //FORWARD 2 BLOCKS MOVE
            }else if (prev_c == curr_c && curr_r - prev_r == -2 && prev_r == 6 && c[curr_r][curr_c] == ' ' && c[curr_r+1][curr_c] == ' '){
                return 1;
            }else if (abs(prev_c - curr_c) == 1 && curr_r - prev_r == -1){
                //PAWN CAPTURES
                if (c[curr_r][curr_c] != ' '){
                    return 1;
                //EN PASSANT
                }else if (prev_r == 3 && c[prev_r][curr_c] == 'P' && D[moveIndex-2][1][curr_c] == 'P'){
                    char *p;
                    for (p=blackTaken;p<blackTaken+16;p++){
                        if (*p == ' '){
                            *p = c[prev_r][curr_c];
                            break;
                        }
                    }
                    c[prev_r][curr_c] = ' ';
                    return 1;
                }
            }
        }else{
            promotionAlert = 1;
        }
        return 0;
    }else if (c[prev_r][prev_c] == 'P'){
        if (promotion){
            c[prev_r][prev_c] = input[4];
            return 1;
        }else if (prev_r != 6 || detectPromotion){
            if (prev_c == curr_c && curr_r - prev_r == 1 && c[curr_r][curr_c] == ' '){
                return 1;
            }else if (prev_c == curr_c && curr_r - prev_r == 2 && prev_r == 1 && c[curr_r][curr_c] == ' ' && c[curr_r-1][curr_c] == ' '){
                return 1;
            }else if (abs(prev_c - curr_c) == 1 && curr_r - prev_r == 1){
                if (c[curr_r][curr_c] != ' '){
                    return 1;
                }else if (prev_r == 4 && c[prev_r][curr_c] == 'p' && D[moveIndex-2][6][curr_c] == 'p'){
                    char *q;
                    for (q=whiteTaken;q<whiteTaken+16;q++){
                        if (*q == ' '){
                            *q = c[prev_r][curr_c];
                            break;
                        }
                    }
                    c[prev_r][curr_c] = ' ';
                    return 1;
                }
            }
        }
    }else{
        promotionAlert = 1;
    }
    return 0;
}

//MAKES THE MOVE AND ADDS THE CAPTURES TO CORRESPONDING ARRAYS
void shiftArray(int prev_r, int prev_c, int curr_r, int curr_c){
    char temp = ' ';
    if (c[curr_r][curr_c]== ' '){
        c[curr_r][curr_c] = c[prev_r][prev_c];
        c[prev_r][prev_c] = ' ';
    }else{
        temp = c[curr_r][curr_c];
        c[curr_r][curr_c] = c[prev_r][prev_c];
        c[prev_r][prev_c] = ' ';
    }

    char *p;
    p = blackTaken;
    char *q;
    q = whiteTaken;
    if (temp != ' '){
        if (player == 1){
            for (p=blackTaken-1;p<blackTaken+16;p++){
                if (*p == ' '){
                    *p = temp;
                    break;
                }
            }
        }
        if (player == 2){
            for (q=whiteTaken-1;q<whiteTaken+16;q++){
                if (*q == ' '){
                    *q = temp;
                    break;
                }
            }
        }
    }
    temp = ' ';
}

//SEARCH FOR UNIQUE PIECE IN BOARD AND RETURN ITS INDICES
void getPieceIndex (char piece){
    int i,j;
    for (i=0;i<8;i++){
        for (j=0;j<8;j++){
            if (c[i][j] == piece){
                pieceIndex[0] = i;
                pieceIndex[1] = j;
            }
        }
    }
}

//CHECK IF CURRENT KING IS IN CHECK
int check(){
    //GETTING COL AND ROW OF KING CURRENT
    getPieceIndex((player == 1) ? 'k' : 'K');
    int kingRow = pieceIndex[0],kingCol = pieceIndex[1],i,j;
    char shifter,piece,piece2 = 'q';
    if (player == 1){
        shifter = 'k';
    }else{
        shifter = 'K';
    }
    //GETS ENEMY PIECES
    stringInverter(shifter,&piece2);
    //CHECKS IF THERE'S A THREATENING ENEMY PAWN
    if (player == 1){
        if (c[kingRow-1][kingCol+1] == 'P' || c[kingRow-1][kingCol-1] == 'P'){
            return 1;
        }
    }else{
        if (c[kingRow+1][kingCol+1] == 'p' || c[kingRow+1][kingCol-1] == 'p'){
            return 1;
        }
    }
    //CHECKS IF THERE'S A THREATENING ENEMY KNIGHT
    piece = 'n';
    stringInverter(shifter,&piece);
    for (i=-2;i<3;i++){
        if (abs(i) == 1){
            if (c[kingRow+i][kingCol+2] ==  piece || c[kingRow+i][kingCol-2] == piece){
                return 1;
            }
        }else if (abs(i) == 2){
            if (c[kingRow+i][kingCol+1] == piece || c[kingRow+i][kingCol-1] == piece){
                return 1;
            }
        }
    }
    //CHECKS IF THERE'S A THREATENING ENEMY BISHOP
    piece = 'b';
    stringInverter(shifter, &piece);
    j = kingCol+1;
    for (i = kingRow+1; i < 8 && j < 8 ; i++) {
        if (c[i][j] == piece || c[i][j] == piece2){
            return 1;
        }else if(c[i][j] == ' '){
            j++;
            continue;
        }else{
            break;
        }
    }
    j = kingCol-1;
    for (i = kingRow+1; i < 8 && j >= 0 ; i++) {
        if (c[i][j] == piece || c[i][j] == piece2){
            return 1;
        }else if(c[i][j] == ' '){
            j--;
            continue;
        }else{
            break;
        }
    }
    j = kingCol+1;
    for (i = kingRow-1; i >= 0 && j < 8 ; i--) {
        if (c[i][j] == piece || c[i][j] == piece2){
            return 1;
        }else if(c[i][j] == ' '){
            j++;
            continue;
        }else{
            break;
        }
    }
    j = kingCol-1;
    for (i = kingRow-1; i >= 0 && j >= 0 ; i--) {
        if (c[i][j] == piece || c[i][j] == piece2){
            return 1;
        }else if(c[i][j] == ' '){
            j--;
            continue;
        }else{
            break;
        }
    }
    //CHECKS IF THERE'S A THREATENING ENEMY rOOK
    piece = 'r';
    stringInverter(shifter, &piece);
    for (i = kingRow+1;i<8;i++){
        if (c[i][kingCol] == piece || c[i][kingCol] == piece2){
            return 1;
        }else if(c[i][kingCol] == ' '){
            continue;
        }else{
            break;
        }
    }
    for (i = kingRow-1;i>-1;i--){
        if (c[i][kingCol] == piece || c[i][kingCol] == piece2){
            return 1;
        }else if(c[i][kingCol] == ' '){
            continue;
        }else{
            break;
        }
    }
    for (i = kingCol+1;i<8;i++){
        if (c[kingRow][i] == piece || c[kingRow][i] == piece2){
            return 1;
        }else if(c[kingRow][i] == ' '){
            continue;
        }else{
            break;
        }
    }
    for (i = kingCol-1;i>-1;i--){
        if (c[kingRow][i] == piece || c[kingRow][i] == piece2){
            return 1;
        }else if(c[kingRow][i] == ' '){
            continue;
        }else{
            break;
        }
    }
    //CHECKS IF THERE'S A THREATENING ENEMY KING
    piece = 'k';
    stringInverter(shifter,&piece);
    for (i=kingRow-1;i<kingRow+2;i++){
        for (j=kingCol-1;j<kingCol+2;j++){
            if (c[i][j] == piece){
                return 1;
            }
        }
    }
    //IF NO THREAT AT ALL RETURN 0
    return 0;
}

//GETS ENEMIES
void stringInverter(char identifier, char *p){
    if (islower(identifier)){
        *p = toupper(*p);
    }
}

//RETURN 1 WHEN CHARCTOR IS SPACE OR LOWERCASE
int lsp(char s){
    if (s==' ' || islower(s)){
        return 1;
    }else{
        return 0;
    }
}

//RETURN 1 WHEN CHARCTOR IS SPACE OR UPPERCASE
int usp(char s){
    if (s==' ' || isupper(s)){
        return 1;
    }else{
        return 0;
    }
}

//RETURNS 0 IF PAWN HAS ANY VALID MOVE
int checkVMPawn(int i,int j){
    if (c[i][j] == 'p'){
        if ((c[i-1][j] == ' ' && !possibleCheck(i,j,i-1,j)) ||  ((j>0) ? (!lsp(c[i-1][j-1]) && !possibleCheck(i,j,i-1,j-1)) : 0) || ((j<7) ? (!lsp(c[i-1][j+1]) && !possibleCheck(i,j,i-1,j+1)): 0)){
            return 0;
        }else if (i==3 && ((c[i-1][j-1] == 'P' && D[moveIndex-2][1][j-1] == 'P' && !possibleCheck(i,j,i-1,j-1))  || (c[i-1][j+1] == 'P' && D[moveIndex-2][1][j+1] == 'P' && !possibleCheck(i,j,i-1,j+1)))){
            return 0;
        }
    }else if (c[i][j] == 'P'){
        if ((c[i+1][j] == ' ' && !possibleCheck(i,j,i+1,j)) ||  ((j>0) ? (!usp(c[i+1][j-1]) && !possibleCheck(i,j,i+1,j-1)) : 0) || ((j<7) ? (!usp(c[i+1][j+1]) && !possibleCheck(i,j,i+1,j+1)): 0)){
            return 0;
        }else if (i==4 && ((c[i+1][j-1] == 'p' && D[moveIndex-2][6][j-1] == 'p' && !possibleCheck(i,j,i+1,j-1))  || (c[i+1][j+1] == 'p' && D[moveIndex-2][6][j+1] == 'p' && !possibleCheck(i,j,i+1,j+1)))){
            return 0;
        }
    }
    return 1;
}

//RETURNS 0 IF KNIGHT HAS ANY VALID MOVE
int checkVMKnight(int i,int j){
    int k;
    if (c[i][j] == 'n' || c[i][j] == 'N'){
        for (k=i-2;k<i+3 && k<8;k++){
            (k<0) ? k=0 : 0;
            if (abs(k-i) == 2){
                if (((player==1) ? ((j>0) ? (usp(c[k][j-1]) && !possibleCheck(i,j,k,j-1)) : 0) || ((j<7) ? (usp(c[k][j+1]) && !possibleCheck(i,j,k,j+1)) : 0)  :  ((j>0) ? (lsp(c[k][j-1]) && !possibleCheck(i,j,k,j-1)) : 0) || ((j<7) ? (lsp(c[k][j+1]) && !possibleCheck(i,j,k,j+1)) : 0))){
                    return 0;
                }
            }else if (abs(k-i) == 1){
                if (((player==1) ? ((j>1) ? (usp(c[k][j-2]) && !possibleCheck(i,j,k,j-2)) : 0) || ((j<6) ? (usp(c[k][j+2]) && !possibleCheck(i,j,k,j+2)) : 0)  :  ((j>1) ? (lsp(c[k][j-2]) && !possibleCheck(i,j,k,j-2)) : 0) || ((j<6) ? (lsp(c[k][j+2]) && !possibleCheck(i,j,k,j+2)) : 0))){
                    return 0;
                }
            }
        }
    }
    return 1;
}

//RETURNS 0 IF ROOK HAS ANY VALID MOVE OR QUEEN HAS ANY VALID ROOK-LIKE MOVE
int checkVMRookQ1(int i,int j){
    int k;
    if (c[i][j] == 'r' || c[i][j] == 'q' || c[i][j] == 'R' || c[i][j] == 'Q'){
        for (k=0;k<8;k++){
            if (!possibleCheck(i,j,k,j) && k != i){
                if (check_movement(i,j,k,j)){
                    return 0;
                }
            }
            if (!possibleCheck(i,j,i,k) && k != j){
                if (check_movement(i,j,i,k)){
                    return 0;
                }
            }
        }
    }
    return 1;

}
//RETURNS 0 IF BISHOP HAS ANY VALID MOVE OR QUEEN HAS ANY VALID BISHOP-LIKE MOVE
int checkVMBishopQ2 (int i,int j){
    int k, u;
    if (c[i][j] == 'b' || c[i][j] == 'B' || (c[i][j] == 'q' || c[i][j] == 'Q')){
        for (k=0;k<8;k++){
            for (u=0;u<8;u++){
                if (!possibleCheck(i,j,k,u) && k != i && u != j){
                    if (check_movement(i,j,k,u)){
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

//RETURNS 0 IF KING HAS ANY VALID MOVE
int checkVMKing (int i,int j){
    int k,u;
    if (c[i][j] == 'k' || c[i][j] == 'K'){
        for (k=i-1;k<i+2;k++){
            (k==-1) ? k=0 : 0;
            if (k==8){
                break;
            }
            for (u=j-1;u<j+2;u++){
                (u==-1) ? u=0 : 0;
                if (u==8){
                    break;
                }
                if ((player==1) ? usp(c[k][u]) : lsp(c[k][u])){
                    if(!possibleCheck(i,j,k,u)){
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

//interchanges 2 arrays
interchange(char s[], char d[]){
    int x = strLength(s);
    int y = strLength(d);
    int temp[50];
    int i;
    for (i=0;i<x || i<y;i++){
        temp[i] = s[i];
        s[i] = d[i];
        d[i] = temp[i];
    }
}

//RETURNS 1 WHEN THERE'S A STALEMATE
int stalemate(){
    int i,j;
    for (i=0;i<8;i++){
        for (j=0;j<8;j++){
            //CONTINUES IF PIECE IS NOT CURRENT
            if (c[i][j] == ' ' || ((player==1) ? isupper(c[i][j]) : islower(c[i][j])) ){
                continue;
            }else{
                if (!checkVMPawn(i,j)){
                    return 0;
                }else if (!checkVMKnight(i,j)){
                    return 0;
                }else if (!checkVMKing(i,j)){
                    return 0;
                }else if (!checkVMRookQ1(i,j)){
                    return 0;
                }else if (!checkVMBishopQ2(i,j)){
                    return 0;
                }
            }
        }
    }
    return 1;
}

//UPPER CASES THE CHAR ARRAY AND PRINTS WHO WON.
void congrats(char s[]){
    int i;
    int x = strLength(s);
    char k[50];
    for (i=0;i<x;i++){
        if (islower(s[i])){
            k[i] = toupper(s[i]);
        }else{
            k[i] = s[i];
        }
    }
    k[x] = '\0';
    printf("CONGRATULATIONS %s ! YOU WON.",k);
}



int main()
{
    char *p;
    char s[50],index[2];
    int i,j,x,y,k,r,a=0;
    printf("\n\t\t\t\t\t\t\t WELCOME TO CHESS FOREVER AFTER. \n\t\t\t\t\t\t\t We hope you have a nice time :)\n");
    printf("\nEnter both your names having a new line between them (To go ahead with player 1 and player 2, type 0 --> Preferred if you have a saved game).\n");
    //TAKING PLAYERS NAMES AND FIGURING OUT WHO IS PLAYER 1
    gets(player1);
    if (player1[0] != '0' || player1[1] != '\0'){
        gets(player2);
    }else{
        strcpy(player1,"Player 1");
        strcpy(player2,"Player 2");
        a = 1;
    }
    while (1){
        brk2 = 0;
        player = 2;
        prev_c = -1;
        curr_c = -1;
        prev_r = -1;
        curr_r = -1;
        endGame = 0;
        undo = 0;
        moveIndex = 0;
        brk = 0;
        restart = 0;
        invalid = 0;
        promotion = 0;
        detectPromotion = 0;
        promotionAlert = 0;
        threeFold = 0;
        fiveFold = 0;
        fd = 0;
        fillWithSpace(whiteTaken,16);
        fillWithSpace(blackTaken,16);
        //INITIAL BOARD
        for (i=0;i<8;i++){
            for(j=0;j<8;j++){
                switch(i){
                    case 0: c[i][j]=A[j];
                            break;
                    case 1: c[i][j]= 'P';
                            break;
                    case 6: c[i][j]= 'p';
                            break;
                    case 7: c[i][j]= tolower(A[j]);
                            break;
                    default:c[i][j] = ' ';
                }
            }
        }
        if (!a){
            printf("\nType 1 to take %s as player 1, 2 to take %s as player 1 and type anything else for choosing player 1 randomly.\n",player1,player2);
            gets(index);
            if (index[0] == '1' && index[1] == '\0'){
                0;
            }else if (index[0] == '2' && index[1] == '\0'){
                interchange(player1, player2);
            }else{
                r = (rand()%2);
                if (!r){
                    interchange(player1, player2);
                }
            }
        }
        printf("\nInput Instructions:\n1.Enter the column name of the first block followed by its row name.\n2.Enter the column name of the second block followed by its row name.\nEx: A7A5 \n$. When promotion, follow your input by the piece you want to promote your pawn to (uppercase or lowercase according to which player you are 1 or 2).\nEx: A7A8q\nNB: No spaces allowed in input.\n");
        move();
        //GAME LOOP
        while (!endGame){
            gets(input);
            FILE *fp;
            //USER SAVING GAME
            if (input[0]=='S' && input[1]=='\0'){
                fp = fopen("SaveMeSaveMe.txt","w");
                //SAVING CURRENT BOARD
                for (i=0;i<8;i++){
                    for (j=0;j<8;j++){
                        fprintf(fp,"%c",c[i][j]);
                    }
                    fprintf(fp,"\n");
                }
                for (i=0;i<16;i++){
                    fprintf(fp,"%c",whiteTaken[i]);
                }
                fprintf(fp,"\n");
                for (i=0;i<16;i++){
                    fprintf(fp,"%c",blackTaken[i]);
                }
                //SAVING NAMES AND THEIR LENGTHS
                fprintf(fp,"\n%s\n%d\n%s\n%d",player1,strLength(player1),player2,strLength(player2));
                fprintf(fp,"\n%d\n",(player==1) ? 2 : 1);
                //SAVING THE MOVEINDEX DECREASED ONE AS CURRENT BOARD WILL BE RESAVED
                fprintf(fp,"%d\n",moveIndex-1);
                //SAVING DATABASE SINCE BEGINNING OF GAME
                for (k=0;k<moveIndex-1;k++){
                    fprintf(fp,"\n");
                    for (i=0;i<8;i++){
                        for (j=0;j<8;j++){
                            fprintf(fp,"%c",D[k][i][j]);
                        }
                        fprintf(fp,"\n");
                    }
                }
                for (k=0;k<moveIndex-1;k++){
                    fprintf(fp,"\n");
                    for (i=0;i<16;i++){
                        fprintf(fp,"%c",saveWhiteTaken[k][i]);
                    }
                }
                fprintf(fp,"\n");
                for (k=0;k<moveIndex-1;k++){
                    fprintf(fp,"\n");
                    for (i=0;i<16;i++){
                        fprintf(fp,"%c",saveBlackTaken[k][i]);
                    }
                }
                fclose(fp);
                printf("\nGame is saved\n");
                printf("\n%s ! Its your turn.\t\t\t\t\t\t\t\t\t  (RES --> restart / R --> Resign / U --> Undo / D --> Draw / RE --> Redo / S --> Save / L --> Load)\n",(player == 1) ? player1 : player2);
                continue;
            }
            //LOADING LAST GAME SAVED
            if (input[0]=='L' && input[1]=='\0'){
                printf("\n%s would like to load the last game saved in this application. If %s agrees, please type L. If not, type anything else\n",(player == 1) ? player1 : player2,(player == 1) ? player2 : player1);
                gets(input);
                if (input[0]=='L' && input[1]=='\0'){
                    fp = fopen("SaveMeSaveMe.txt","r");
                    for (i=0;i<8;i++){
                        fgets(s,10,fp);
                        for (j=0;j<8;j++){
                            c[i][j] = s[j];
                        }
                    }
                    fgets(s,20,fp);
                    for (i=0;i<16;i++){
                        whiteTaken[i] = s[i];
                    }
                    fgets(s,20,fp);
                    for (i=0;i<16;i++){
                        blackTaken[i] = s[i];
                    }
                    fgets(s,50,fp);
                    fscanf(fp," %d",&x);
                    strncpy(player1,s,x);
                    player1[x] = '\0';
                    fgets(s,50,fp);
                    fgets(s,50,fp);
                    fscanf(fp," %d",&y);
                    strncpy(player2,s,y);
                    player2[y] = '\0';
                    fscanf(fp," %d",&player);
                    fgets(s,50,fp);
                    fscanf(fp," %d",&moveIndex);
                    fgets(s,50,fp);
                    for (k=0;k<moveIndex;k++){
                        fgets(s,50,fp);
                        for (i=0;i<8;i++){
                            fgets(s,50,fp);
                            for (j=0;j<8;j++){
                                D[k][i][j] = s[j];
                            }
                        }
                    }
                    fgets(s,50,fp);
                    for (k=0;k<moveIndex;k++){
                        fgets(s,50,fp);
                        for (i=0;i<16;i++){
                            saveWhiteTaken[k][i] = s[i];
                        }
                    }
                    fgets(s,50,fp);
                    for (k=0;k<moveIndex;k++){
                        fgets(s,50,fp);
                        for (i=0;i<16;i++){
                            saveBlackTaken[k][i] = s[i];
                        }
                    }
                    fclose(fp);
                    promotion = 0;
                    move();
                    continue;
                }else{
                    printf("\n%s didn't accept your request for load.\n",(player == 1) ? player2 : player1);
                    printf("\n%s ! Its your turn.\t\t\t\t\t\t\t\t\t  (RES --> restart / R --> Resign / U --> Undo / D --> Draw / RE --> Redo / S --> Save / L --> Load)\n",(player == 1) ? player1 : player2);
                    continue;
                }

            }
            if (input[0] == 'R' && input[1] == 'E' && input[2] =='S' && input[3] == '\0'){
                printf("\n%s would like to restart the game. If %s agrees, please type RES. If not, type anything else.\n",(player == 1) ? player1 : player2,(player == 1) ? player2 : player1);
                gets(input);
                if (input[0] == 'R' && input[1] == 'E' && input[2] =='S' && input[3] == '\0'){
                    restart = 1;
                    break;
                }else{
                    printf("\n%s didn't accept your request for restarting the game.\n",(player == 1) ? player2 : player1);
                    printf("\n%s ! Its your turn.\t\t\t\t\t\t\t\t\t  (RES --> restart / R --> Resign / U --> Undo / D --> Draw / RE --> Redo / S --> Save / L --> Load)\n",(player == 1) ? player1 : player2);
                    continue;
                }
            }
            //DRAW FOR THREEFOLD
            if (input[0]=='D' && input[1]=='\0' && threeFold==1){
                printf("\n\t\t\t\tGAME ENDED IN DRAW BY THREEFOLD REPETITION!!!\n");
                endGame = 1;
                break;
            //DRAW REQUEST
            }else if (input[0]=='D' && input[1]=='\0'){
                printf("\n%s would like to end the game with draw. If %s agrees, please type D. If not, type anything else.\n",(player == 1) ? player1 : player2,(player == 1) ? player2 : player1);
                gets(input);
                if (input[0]=='D' && input[1]=='\0'){
                    endGame = 1;
                    printf("\n\t\t\t\t\tGAME ENDED WITH A DRAW!!!");
                    break;
                }else{
                    printf("\n%s didn't accept your request for draw.\n",(player == 1) ? player2 : player1);
                    printf("\n%s ! Its your turn.\t\t\t\t\t\t\t\t\t  (RES --> restart / R --> Resign / U --> Undo / D --> Draw / RE --> Redo / S --> Save / L --> Load)\n",(player == 1) ? player1 : player2);
                    continue;
                }
            }
            //RESIGNATION
            if (input[0]=='R' && input[1]=='\0'){
                endGame = 1;
                congrats((player == 1) ? player2 : player1);
                break;
            }
            //REDO
            if (input[0] == 'R' && input[1] == 'E' && input[2] =='\0'){
                //CHECK IF THERE HAS BEEN AN UNDO
                if(undo){
                    printf("\n\n%s would like to redo the last pair of moves. If %s agrees, please type RE. If not, type anything else.\n",(player == 1) ? player1 : player2, (player == 1) ? player2 : player1);
                    gets(input);
                    if (input[0] == 'R' && input[1] == 'E' && input[2]=='\0'){
                        for(i=0;i<8;i++){
                            for (j=0;j<8;j++){
                                c[i][j] = D[moveIndex+1][i][j];
                            }
                        }
                        for (i=0;i<16;i++){
                            whiteTaken[i] = saveWhiteTaken[moveIndex+1][i];
                            blackTaken[i] = saveBlackTaken[moveIndex+1][i];
                        }
                        moveIndex +=1;
                        promotion = 0;
                        switch (player){
                            case 1: player = 2;
                                    break;
                            case 2: player = 1;
                                    break;
                        }
                        move();
                    }
                    else {
                        printf("\n%s didn't accept your request for redo\n",(player == 1) ? player2 : player1);
                        printf("\n%s ! Its your turn.\t\t\t\t\t\t\t\t\t  (RES --> restart / R --> Resign / U --> Undo / D --> Draw / RE --> Redo / S --> Save / L --> Load)\n",(player == 1) ? player1 : player2);
                    }
                }else{
                    printf("\nYou cannot redo. You haven't undone any moves.\n");
                    printf("\n%s ! Its your turn.\t\t\t\t\t\t\t\t\t  (RES --> restart / R --> Resign / U --> Undo / D --> Draw / RE --> Redo / S --> Save / L --> Load)\n",(player == 1) ? player1 : player2);
                }
                continue;
            }

            //UNDO
            if (input[0]=='U' && input[1]=='\0'){
                //CHECKIF THERE HAS BEEN AT LEAST ONE MOVE
                if (moveIndex > 2){
                    printf("\n%s would like to undo the last pair of moves. If %s agrees, please type U. If not, type anything else.\n",(player == 1) ? player1 : player2, (player == 1) ? player2 : player1);
                    gets(input);
                    if (input[0]=='U' && input[1]=='\0'){
                        for(i=0;i<8;i++){
                            for (j=0;j<8;j++){
                                c[i][j] = D[moveIndex-3][i][j];
                            }
                        }
                        for (i=0;i<16;i++){
                            whiteTaken[i] = saveWhiteTaken[moveIndex-3][i];
                            blackTaken[i] = saveBlackTaken[moveIndex-3][i];
                        }
                        moveIndex -=3;
                        undo = 1;
                        promotion = 0;
                        switch (player){
                            case 1: player = 2;
                                    break;
                            case 2: player = 1;
                                    break;
                        }
                        move();
                    }else{
                        printf("\n%s didn't accept your request for undo.\n",(player == 1) ? player2 : player1);
                        printf("\n%s ! Its your turn.\t\t\t\t\t\t\t\t\t  (RES --> restart / R --> Resign / U --> Undo / D --> Draw / RE --> Redo / S --> Save / L --> Load)\n",(player == 1) ? player1 : player2);
                    }
                }
                else{
                    printf("\nYou cannot undo. You haven't made any moves yet.\n");
                    printf("\n%s ! Its your turn.\t\t\t\t\t\t\t\t\t  (RES --> restart / R --> Resign / U --> Undo / D --> Draw / RE --> Redo / S --> Save / L --> Load)\n",(player == 1) ? player1 : player2);
                }
                continue;
            //COME HERE IF FIRST ENTERED INPUT IS NOT A NON-MOVE
            }else {
                validate_inp();
                //COMEHERE IF THE SECOND TO LAST INPUT AFTER FIRST WAS INVALID IS A NON-MOVE
                if (brk){
                    printf("\nPlease enter your input again\n");
                    continue;
                }
                //IF USER ENTERED A VALID INPUT REDO OPTION IS FALSE
                (!invalid) ? undo = 0 : 0;
            }
            //EVERYTHING WILL GET HERE IS A VALID INPUT
            //MAKE MOVE
            shiftArray(prev_r, prev_c, curr_r, curr_c);
            //SHOW BOARD AND DETECT STALEMATE,FIVEFOLD,THREEFOLD,CHECK, DEAD POSITION & CHECKMATE
            move();
        }
        if (!restart){
            printf("\n\n\nIf %s would like to play again, type 1. If not, type anything.\n", player1);
            gets(input);
            if (!(input[0] == '1' && input[1] == '\0')){
                break;
            }else{
                printf("\n%s would like to play again! %s, are you ready for another challenge? Type 1 if yes. If not, type anything.\n", player1, player2);
                gets(input);
                if (!(input[0] == '1' && input[1] == '\0')){
                    break;
                }
            }
        }
    }
    printf("\nProgram terminates after 5 seconds\n");
    int time_to_sleep = 5; // sleep 5 seconds
    while(time_to_sleep){
        time_to_sleep = sleep(time_to_sleep);
    }
    return 0;
}
