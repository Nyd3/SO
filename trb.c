#include <stdio.h>


#define LIMITE 300

int Mem[LIMITE] = {0};

int main()
{
    FILE *ficheiro;
    ficheiro = fopen("file.dat", "r+");
    if(ficheiro == NULL) //if file does not exist, create it
        ficheiro = fopen("file.dat", "w+");
    
    short instruction;
    short v1;
    short v2;
    int process = 1;
    while(scanf("%hd", instruction) != EOF)
    {
        if(instruction == "\n")
            process++;

        scanf(" %hd %hd ", v1, v2);

        

        switch (instruction)
        {
            case 1 :
                break;
        
            case 8 :
                break;
            
            case 11 :
                break;

            default:
                break;
        }
    }
}