#include <iostream>
#include <time.h>
#define size 8000

using namespace std;

void randMatrix(int** matrix);
void printMatrix(int** matrix);
void initMatrix(int*** matrix);
int main()
{
    srand(time(0));
    int* buffer;
    int** matrix1 = NULL, ** matrix2 = NULL;
    initMatrix(&matrix1);
    initMatrix(&matrix2);
    randMatrix(matrix1);
    randMatrix(matrix2);
    int result = 0;
    int mult = 0;
    int** matrix3 = NULL;
    initMatrix(&matrix3);
    unsigned int start_time = clock();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
                matrix3[i][j] = matrix1[i][j] + matrix2[i][j];
            }
    }
    cout << "C/C++ - " << (clock() - start_time) / 1000.0 << endl;
    start_time = clock();
    _asm
    {
        mov esi, matrix1
        mov edi, matrix2
        mov eax, 0
        mov ebx, 0
        mov ecx, 0
        loop1:
            cmp ebx, size
            je loop1Exit

            mov buffer, eax
        loop2 :
            cmp ecx, size
            je loop2Exit

            mov edx, [esi + ebx * 4]
            mov edx, [edx + ecx * 4]
            mov eax, edx
            mov edx, [edi + ebx * 4]
            mov edx, [edx + ecx * 4]
            add eax , edx
            mov esi, matrix3
            mov esi, [esi + ebx * 4]
            mov edx, eax
            mov [esi + ecx * 4], edx
            mov esi, matrix1
            mov eax, buffer        
            add ecx, 1
            jmp loop2
        loop2Exit :
            add ebx, 1
            mov ecx, 0
            jmp loop1
        loop1Exit :
    }
    cout << "Assembler - " << (clock() - start_time) / 1000.0 << endl;
    start_time = clock();
    _asm
    {
        mov esi, matrix1
        mov edi, matrix2
        mov eax, 0
        mov ebx, 0
        mov ecx, 0
        loop3:
            cmp ebx, size
            je loop3Exit

            mov buffer, eax
        loop4 :
            cmp ecx, size
            je loop4Exit

            mov edx, [esi + ebx * 4]
            movq MM0, [edx + ecx * 4]
            mov edx, [edi + ebx * 4]
            movq MM1, [edx + ecx * 4]
            paddb MM0, MM1
            mov esi, matrix3
            mov esi, [esi + ebx * 4]
            movq[esi + ecx * 4], MM0
            mov esi, matrix1
            mov eax, buffer
            add ecx, 2
            jmp loop4
        loop4Exit :
            add ebx, 1
            mov ecx, 0
            jmp loop3
        loop3Exit :
            emms
    }
    cout << "MMX - " << (clock() - start_time) / 1000.0 << endl;
    start_time = clock();
    _asm
    {
        mov esi, matrix1
        mov edi, matrix2
        mov eax, 0
        mov ebx, 0
        mov ecx, 0
        loop5:
        cmp ebx, size
            je loop5Exit

            mov buffer, eax
        loop6 :
            cmp ecx, size
            je loop6Exit

            mov edx, [esi + ebx * 4]
            movss XMM0, [edx + ecx * 4]
            mov edx, [edi + ebx * 4]
            movss XMM1, [edx + ecx * 4]
            addsd XMM0, XMM1
            mov esi, matrix3
            mov esi, [esi + ebx * 4]
            movss [esi + ecx * 4], XMM0
            mov esi, matrix1
            mov eax, buffer
            add ecx, 4
            jmp loop6
        loop6Exit :
            add ebx, 1
            mov ecx, 0
            jmp loop5
        loop5Exit :
    }
    cout << "XMM - " << (clock() - start_time) / 1000.0 << endl;
    return 0;

}
void randMatrix(int** matrix)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 1000 + 1;
        }
    }
}

void printMatrix(int** matrix)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
void initMatrix(int*** matrix)
{
    *matrix = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        (*matrix)[i] = (int*)malloc(size * sizeof(int));
    }
}