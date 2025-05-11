#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include <stdlib.h>

#define TSIZE 128
int table[100][TSIZE];
char terminal[TSIZE];
char nonterminal[26];

struct product {
    char str[100];
    int len;
}pro[20];
int no_pro;
char first[26][TSIZE];
char follow[26][TSIZE];
char first_rhs[100][TSIZE];
int isNT(char c) {
    return c >= 'A' && c <= 'Z';
}

// reading data from the file
void readFromFile() {
    FILE* fptr;
    fptr = fopen("grammer.txt", "r");
    char buffer[255];
    int i;
    int j;
    while (fgets(buffer, sizeof(buffer), fptr)) {
        printf("%s", buffer);
        j = 0;
        nonterminal[buffer[0] - 'A'] = 1;
        for (i = 0; i < strlen(buffer) - 1; ++i) {
            if (buffer[i] == '|') {
                ++no_pro;
                pro[no_pro - 1].str[j] = '\0';
                pro[no_pro - 1].len = j;
                pro[no_pro].str[0] = pro[no_pro - 1].str[0];
                pro[no_pro].str[1] = pro[no_pro - 1].str[1];
                pro[no_pro].str[2] = pro[no_pro - 1].str[2];
                j = 3;
            }
            else {
                pro[no_pro].str[j] = buffer[i];
                ++j;
                if (!isNT(buffer[i]) && buffer[i] != '-' && buffer[i] != '>') {
                    terminal[buffer[i]] = 1;
                }
            }
        }
        pro[no_pro].len = j;
        ++no_pro;
    }
}

void add_FIRST_A_to_FOLLOW_B(char A, char B) {
    int i;
    for (i = 0; i < TSIZE; ++i) {
        if (i != '#')
            follow[B - 'A'][i] = follow[B - 'A'][i] || first[A - 'A'][i];
    }
}
void add_FOLLOW_A_to_FOLLOW_B(char A, char B) {
    int i;
    for (i = 0; i < TSIZE; ++i) {
        if (i != '#')
            follow[B - 'A'][i] = follow[B - 'A'][i] || follow[A - 'A'][i];
    }
}

void FOLLOW() {
    int t = 0;
    int i, j, k, x;
    while (t++ < no_pro) {
        for (k = 0; k < 26; ++k) {
            if (!nonterminal[k])    continue;
            char nt = k + 'A';
            for (i = 0; i < no_pro; ++i) {
                for (j = 3; j < pro[i].len; ++j) {
                    if (nt == pro[i].str[j]) {
                        for (x = j + 1; x < pro[i].len; ++x) {
                            char sc = pro[i].str[x];
                            if (isNT(sc)) {
                                add_FIRST_A_to_FOLLOW_B(sc, nt);
                                if (first[sc - 'A']['#'])
                                    continue;
                            }
                            else {
                                follow[nt - 'A'][sc] = 1;
                            }
                            break;
                        }
                        if (x == pro[i].len)
                            add_FOLLOW_A_to_FOLLOW_B(pro[i].str[0], nt);
                    }
                }
            }
        }
    }
}

void add_FIRST_A_to_FIRST_B(char A, char B) {
    int i;
    for (i = 0; i < TSIZE; ++i) {
        if (i != '#') {
            first[B - 'A'][i] = first[A - 'A'][i] || first[B - 'A'][i];
        }
    }
}

void FIRST() {
    int i, j;
    int t = 0;
    while (t < no_pro) {
        for (i = 0; i < no_pro; ++i) {
            for (j = 3; j < pro[i].len; ++j) {
                char sc = pro[i].str[j];
                if (isNT(sc)) {
                    add_FIRST_A_to_FIRST_B(sc, pro[i].str[0]);
                    if (first[sc - 'A']['#'])
                        continue;
                }
                else {
                    first[pro[i].str[0] - 'A'][sc] = 1;
                }
                break;
            }
            if (j == pro[i].len)
                first[pro[i].str[0] - 'A']['#'] = 1;
        }
        ++t;
    }
}

void add_FIRST_A_to_FIRST_RHS__B(char A, int B) {
    int i;
    for (i = 0; i < TSIZE; ++i) {
        if (i != '#')
            first_rhs[B][i] = first[A - 'A'][i] || first_rhs[B][i];
    }
}
// Calculates FIRST(ß) for each A->ß
void FIRST_RHS() {
    int i, j;
    int t = 0;
    while (t < no_pro) {
        for (i = 0; i < no_pro; ++i) {
            for (j = 3; j < pro[i].len; ++j) {
                char sc = pro[i].str[j];
                if (isNT(sc)) {
                    add_FIRST_A_to_FIRST_RHS__B(sc, i);
                    if (first[sc - 'A']['#'])
                        continue;
                }
                else {
                    first_rhs[i][sc] = 1;
                }
                break;
            }
            if (j == pro[i].len)
                first_rhs[i]['#'] = 1;
        }
        ++t;
    }
}


int main() {
    readFromFile();
    follow[pro[0].str[0] - 'A']['$'] = 1;
    FIRST();
    FOLLOW();
    FIRST_RHS();
    int i, j, k,m,n;
    
    // display first of each variable
    printf("\n");
    for (i = 0; i < no_pro; ++i) {
        if (i == 0 || (pro[i - 1].str[0] != pro[i].str[0])) {
            char c = pro[i].str[0];
            printf("FIRST OF %c: ", c);
            for (j = 0; j < TSIZE; ++j) {
                if (first[c - 'A'][j]) {
                    printf("%c ", j);
                }
            }
            printf("\n");
        }
    }
    
    // display follow of each variable
    printf("\n");
    for (i = 0; i < no_pro; ++i) {
        if (i == 0 || (pro[i - 1].str[0] != pro[i].str[0])) {
            char c = pro[i].str[0];
            printf("FOLLOW OF %c: ", c);
            for (j = 0; j < TSIZE; ++j) {
                if (follow[c - 'A'][j]) {
                    printf("%c ", j);
                }
            }
            printf("\n");
        }
    }
    // display first of each variable ß in form A->ß
    printf("\n");
    for (i = 0; i < no_pro; ++i) {
        printf("FIRST OF %s: ", pro[i].str);
        for (j = 0; j < TSIZE; ++j) {
            if (first_rhs[i][j]) {
                printf("%c ", j);
            }
        }
        printf("\n");
    }
    
    // the parse table contains '$'
    terminal['$'] = 1;
    
    // the parse table do not read '#' to remove '#' from terminals
    terminal['#'] = 0;
    
    // printing parse table
    printf("\n");
    printf("\n\t**************** LL(1) PARSING TABLE *******************\n");
    printf("\t--------------------------------------------------------\n");
    printf("%-10s", "");
    for (i = 0; i < TSIZE; ++i) {		//to print 1st row which shows list of all terminals
       if (terminal[i])    printf("%-10c", i);
    }
    printf("\n");
    int p = 0;
    for (i = 0; i < no_pro; ++i) {
        if (i != 0 && (pro[i].str[0] != pro[i - 1].str[0]))
            p = p + 1;
        for (j = 0; j < TSIZE; ++j) {
            if (first_rhs[i][j] && j != '#') {
                table[p][j] = i + 1;
            }
            else if (first_rhs[i]['#']) {
                for (k = 0; k < TSIZE; ++k) {
                    if (follow[pro[i].str[0] - 'A'][k]) {
                        table[p][k] = i + 1;
                    }
                }
            }
        }
    }
    k = 0;
    for (i = 0; i < no_pro; ++i) {
        if (i == 0 || (pro[i - 1].str[0] != pro[i].str[0])) {
            printf("%-10c", pro[i].str[0]);
            for (j = 0; j < TSIZE; ++j) {
                if (table[k][j]) {
                    printf("%-10s", pro[table[k][j] - 1].str);
                }
                else if (terminal[j]) {
                    printf("%-10s", "");
                }
            }
            ++k;
            printf("\n");
        }
    }

    printf("\n\nPlease enter the desired INPUT STRING = ");
    char input[100];
    scanf("%s",input);
    printf("\n\t\t===================================================================\n");
    printf("\t\t\tStack\t\t\tInput\t\t\tAction");
    printf("\n\t\t===================================================================\n");
    char stack[100];
    int i_ptr = 0,s_ptr = 1;    //input pointer and stack pointer which shows their positions
    stack[0] = '$'; //initial symbol in stack
    stack[1] = pro[0].str[0];

    while(s_ptr != -1){
        printf("\t\t\t"); //-3 tabs;
        int vamp = 0;
        for(vamp=0;vamp<=s_ptr;vamp++){
            printf("%c",stack[vamp]);
        }
        printf("\t\t\t");
        vamp = i_ptr;
        while(input[vamp] != '\0'){
            printf("%c",input[vamp]);
            vamp++;
        }
        printf("\t\t\t");
        char her = input[i_ptr];
        char him = stack[s_ptr];
        s_ptr--;
        int flag=0;
        if(!isupper(him)){
            if(her == him){
                i_ptr++;
                flag++;
                printf("POP\n");
            }
            else{
			    printf("\n\t\t\t================================================\n");
        		printf("\t\t\t\t YOUR STRING HAS BEEN REJECTED !");
			    printf("\n\t\t\t================================================\n");
                exit(0);
            }
        }
        else{
            for (i = 0; i < no_pro; ++i)
            {
                if (pro[i].str[0]==him)
                {
                    for ( j = 0; j < TSIZE; ++j)
                    {
                        char c=j;
                        if (first_rhs[i][j] && c==her)
                        {
                            for (k = strlen(pro[i].str)-1; k >2; k--)
                            {   s_ptr++;
                                stack[s_ptr]=pro[i].str[k];
                            }
                            printf("%s\n",pro[i].str);
                                flag++;
                        }
                        if (flag==0 && first_rhs[i][j] && c=='#')
                        {
                            printf("Epslon production is added\n" );
                            flag++;
                        }
                    }
                }
            }
            if(flag==0)
            {
			    printf("\n\t\t\t================================================\n");
        		printf("\t\t\t\t YOUR STRING HAS BEEN REJECTED !");
			    printf("\n\t\t\t================================================\n");
                exit(0);
            }
        }
    }
    printf("\n\t\t\t================================================\n");
    if (input[i_ptr] == '\0'){
        printf("\t\t\t\t YOUR STRING HAS BEEN ACCEPTED !");
    }
    else						//not needed 
        printf("\t\t\t\t YOUR STRING HAS BEEN REJECTED !");
    printf("\n\t\t\t================================================\n");

}
