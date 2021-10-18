#include<fstream>
#include<math.h>
#include<string>
using namespace std;

//3 times the cost of the logic gate
#define NOT 2
#define XOR 9
#define XNOR 9
#define AND 4
#define NAND 3
#define OR 4
#define NOR 3
#define NAND3 4
#define AND3
#define NOR3 4
#define OR3 
#define XNOR3 14
#define XOR3 14
#define MOAI1 6 
#define MAOI1 8

// PROST : 0, 4, 8, 15, 1, 5, 14, 9, 2, 7, 10, 12, 11, 13, 6, 3
// PRESENT : 12, 5, 6, 11, 9, 0, 10, 13, 3, 14, 15, 8, 4, 7, 1, 2 
// SKINNY : 12, 6, 9, 0, 1, 10, 2, 11, 3, 8, 5, 13, 4, 14, 7, 15
// PICCOLO : 14, 4, 11, 2, 3, 8, 0, 9, 1, 10, 7, 15, 6, 12, 5, 13 
// RECTANGLE : 6, 5, 12, 10, 1, 14, 7, 9, 11, 0, 3, 13, 8, 15, 4, 2
// LBLOCK : 14, 9, 15, 0, 13, 4, 10, 11, 1, 2, 8, 3, 7, 6, 12, 5
// TWINE : 12, 0, 15, 10, 2, 11, 9, 5, 8, 3, 13, 7, 1, 14, 6, 4
string Cipher="RECTANGLE";
int bitnum = 4;
int GateNum = 10;
int Size = pow(2, bitnum);
int Sbox[128] = {6, 5, 12, 10, 1, 14, 7, 9, 11, 0, 3, 13, 8, 15, 4, 2};
// int LowerBound = 8;  // minimum number of gates
int UpperBound = 18; // area when minimum
int MinGEC = 55;
ofstream fout(Cipher + "_bs4_Stp_K" + to_string(GateNum) + "_GEC" + to_string(MinGEC) + ".cvc");

int QNum = 4 * GateNum;
int aNum = 4 * (2 * bitnum + GateNum - 1) * GateNum / 2 + bitnum * bitnum + GateNum * bitnum;
//4*(bitnum + bitnum + GateNum - 1 ) * GatenNum / 2
int bNum = 8 * GateNum;   //t = b0q1q2q3 + b1q1q2 + b1q2q3 + b1q1q3 + b1q1 + b1q2 + b1q3 + b2q1 + b2q2 + b2q3 
                          //    + b3q1q2 + b4q1 + b4q2 + b5q1 +b6

int A[8][256]={0};


string tobits(int num, int bit_len)
{ //to binary
    string res = "";

    for (int pos = 0; pos < bit_len; pos++)
    {
        res = to_string(num % 2) + res;
        num /= 2;
    }

    return res;
}


void Decompose(int flag){
    for (int i = 0; i < Size; i++){
    	int tem;
        if(flag == 0){
            tem = i;
        }
        else{
            tem = Sbox[i];
        }
        for (int j = bitnum -1; j >= 0;j--){
            A[j][i] = tem % 2;
            tem /= 2;
        }
    }
    return;
}


void State_Variate(){
    //State Variate
    //x
    for (int i = 0; i < bitnum; i++)
    {
        fout << "X_" << i;
        if (i == bitnum - 1)
        {
            fout << " : BITVECTOR( " << Size << " );" << endl;
        }
        else
        {
            fout << " , ";
        }
    }
    //y
    for (int i = 0; i < bitnum; i++)
    {
        fout << "Y_" << i;
        if (i == bitnum - 1)
        {
            fout << " : BITVECTOR( " << Size << " );" << endl;
        }
        else
        {
            fout << " , ";
        }
    }
    //t
    for (int i = 0; i < GateNum; i++)
    {
        fout << "T_" << i;
        if (i == GateNum - 1)
        {
            fout << " : BITVECTOR( " << Size << " );" << endl;
        }
        else
        {
            fout << " , ";
        }
    }
    //q
    for (int i = 0; i < QNum; i++)
    {
        fout << "Q_" << i;
        if (i == QNum - 1)
        {
            fout << " : BITVECTOR( " << Size << " );" << endl;
        }
        else
        {
            fout << " , ";
        }
    }
    //a
    for (int i = 0; i < aNum; i++)
    {
        fout << "a_" << i;
        if (i == aNum - 1)
        {
            fout << " : BITVECTOR( " << Size << " );" << endl;
        }
        else
        {
            fout << " , ";
        }
    }
    //b
    for (int i = 0; i < bNum; i++)
    {
        fout << "b_" << i;
        if (i == bNum - 1)
        {
            fout << " : BITVECTOR( " << Size << " );" << endl;
        }
        else
        {
            fout << " , ";
        }
    }
    //B
    for (int i = 0; i < GateNum; i++)
    {
        fout << "B_" << i;
        if (i == GateNum - 1)
        {
            fout << " : BITVECTOR( 8 );" << endl;
        }
        else
        {
            fout << " , ";
        }
    }
    //Logic cost
    fout << "Cost : ARRAY BITVECTOR(8) OF BITVECTOR(8);" << endl;
    //GEC
    fout<< "GEC : BITVECTOR( 8 );" << endl;

    fout << "ASSERT( Cost[0bin00000000] = 0bin00000000 );" << endl;//0
    fout << "ASSERT( Cost[0bin00000001] = 0bin00000000 );" << endl;//1
    fout << "ASSERT( Cost[0bin00000011] = 0bin00000010 );" << endl;//NOT
    fout << "ASSERT( Cost[0bin00000100] = 0bin00001000 );" << endl;//XOR 
    fout << "ASSERT( Cost[0bin00000101] = 0bin00000110 );" << endl;//XNOR
    fout << "ASSERT( Cost[0bin00001000] = 0bin00000100 );" << endl;//AND
    fout << "ASSERT( Cost[0bin00001001] = 0bin00000011 );" << endl;//NAND
    fout << "ASSERT( Cost[0bin00001100] = 0bin00000100 );" << endl;//OR
    fout << "ASSERT( Cost[0bin00001101] = 0bin00000011 );" << endl;//NOR
    fout << "ASSERT( Cost[0bin01000000] = 0bin10000000 );" << endl; //AND3
    fout << "ASSERT( Cost[0bin01000001] = 0bin10000010 );" << endl; //NAND3
    fout << "ASSERT( Cost[0bin01100000] = 0bin00000011 );" << endl; //OR3
    fout << "ASSERT( Cost[0bin01100001] = 0bin00000011 );" << endl; //NOR3
    fout << "ASSERT( Cost[0bin00010000] = 0bin00000011 );" << endl; //XOR3
    fout << "ASSERT( Cost[0bin00010001] = 0bin00000011 );" << endl; //XNOR3
    fout << "ASSERT( Cost[0bin10000000] = 0bin00001000 );" << endl; //MAOI1
    fout << "ASSERT( Cost[0bin10000001] = 0bin00000110 );" << endl; //MOAI1
    return;
}

void Trival_Constraint(){
    //Trival Constraints

    //X
    Decompose(0);
    for (int i = 0; i < bitnum;i++){
        fout << "ASSERT( X_" << i << " = 0bin";
        for (int j = 0; j < Size;j++){
            fout << A[i][j];
        }
        fout << " );" << endl;
    }
    //Y
    Decompose(1);
    for (int i = 0; i < bitnum; i++)
    {
        fout << "ASSERT( Y_" << i << " = 0bin";
        for (int j = 0; j < Size; j++)
        {
            fout << A[i][j];
        }
        fout << " );" << endl;
    }
    //Q_a
    int a_Start = 0, a_counter = bitnum;
    for (int k = 0; k < GateNum; k++){
        for (int q = 0; q < 4;q++){
            for (int i = a_Start; i < a_Start + a_counter - 1; i++)
            {
                for (int j = i + 1; j < a_Start + a_counter; j++)
                {
                    fout << "ASSERT( a_" << i << " & a_" << j << " = 0bin" << tobits(0, Size) << " );" << endl;
                }
            }
            a_Start += a_counter;
        }
        a_counter++;
    }
    //Y_a
    for (int k = 0; k < bitnum; k++){
        for (int i = a_Start; i < a_Start + a_counter - 1; i++)
        {
            for (int j = i + 1; j < a_Start + a_counter; j++)
            {
                fout << "ASSERT( a_" << i << " & a_" << j << " = 0bin" << tobits(0, Size) << " );" << endl;
            }
        }
        a_Start += a_counter;
    }
    //all_a
    for (int i = 0; i < aNum; i++){
        fout << "ASSERT( a_" << i << " = 0bin";
        for (int j = 0; j < Size; j++)
        {
            fout << 1;
        }
        fout<< " OR a_" << i << " =0bin";
        for (int j = 0; j < Size; j++)
        {
            fout << 0;
        }
        fout << " );" << endl;
    }
    //all_b
    for (int i = 0; i < bNum; i++)
    {
        fout << "ASSERT( b_" << i << " = 0bin";
        for (int j = 0; j < Size; j++)
        {
            fout << 1;
        }
        fout << " OR b_" << i << " = 0bin";
        for (int j = 0; j < Size; j++)
        {
            fout << 0;
        }
        fout << " );" << endl;
    }
    for (int i = 0; i < bNum; i += 7)
    {
        fout << "ASSERT( IF b_" << i + 6 << " = 0bin1111111111111111 => (b_" << i + 4 << " = 0bin0000000000000000 AND b_" << i + 5 << " = 0bin0000000000000000 AND b_" << i + 7 << " = 0bin111111111111 ) ENDIF );" << endl;
        fout << "ASSERT( IF b_" << i + 1 << " = 0bin1111111111111111 => (b_" << i + 3 << " = 0bin0000000000000000 AND b_" << i + 4 << " = 0bin0000000000000000 AND b_" << i + 5 << " = 0bin0000000000000000 AND b_" << i + 6 << " = 0bin0000000000000000 ) ENDIF );" << endl;
        fout << "ASSERT( IF b_" << i + 3 << " = 0bin1111111111111111 => (b_" << i + 1 << " = 0bin0000000000000000 AND b_" << i + 2 << " = 0bin0000000000000000 AND b_" << i + 4 << " = 0bin0000000000000000 AND b_" << i + 5 << " = 0bin0000000000000000AND b_" << i + 6 << " = 0bin0000000000000000 ) ENDIF );" << endl;

        fout << "ASSERT( IF b_" << i  << " = 0bin1111111111111111 => (b_" << i + 1 << " = 0bin0000000000000000 AND b_" << i + 2 << " = 0bin0000000000000000 AND b_" << i + 3 << " = 0bin0000000000000000 AND b_" << i + 4 << " = 0bin0000000000000000 AND b_" << i + 5 << " = 0bin0000000000000000 AND b_" << i + 6 << " = 0bin0000000000000000) ENDIF );" << endl;
    }
    return;
}

void Logic_Constraint(){
    int countA = 0, countB = 0;
    int countQ = 0, countT = 0;
    int countX = 0, countY = 0;

    for (int k = 0; k < GateNum; k++)
    {   
        //Q
        for (int q = 0; q < 4; q++;){
            fout << "ASSERT( Q_" << countQ++ << " = ";
            for (int i = 0; i < bitnum; i++)
            {
                if (k == 0 && i == bitnum - 1)
                {
                    fout << "a_" << countA++ << " & X_" << i;
                }
                else
                {
                    fout << "BVXOR( a_" << countA++ << " & X_" << i << " , ";
                }
            }
            for (int i = 0; i < countT; i++)
            {
                if (i == countT - 1)
                {
                    fout << "a_" << countA++ << " & T_" << i;
                }
                else
                {
                    fout << "BVXOR( a_" << countA++ << " & T_" << i << " , ";
                }
            }
            //bracket
            for (int i = 0; i < bitnum + countT - 1 + 1; i++)
            {
                if (i == bitnum + countT - 1)
                {
                    fout << " );" << endl;
                }
                else
                {
                    fout << " )";
                }
            }
        }
        //b0 ·q0 ·q1 ·q2 ·q3 +b0 ·q0 ·q1 ·q2 + b0 ·q0 ·q1 ·q3 +b0 ·q2 ·q3 +b0 ·q2 +b0 ·q3+b1 ·q0 ·q1 ·q2 +b2 ·q0 ·q1 +b2 ·q0 ·q2 +b2 ·q1 ·q2+ b2 ·q0 +b2 ·q1 +b2 ·q2 +b3 ·q0 +b3 ·q1 +b3 ·q2+b4 ·q0 ·q1 +b5 ·q0 +b5 ·q1 +b6 ·q0 +b7.
        //T
        fout << "ASSERT( T_" << countT++ 
        << " = BVXOR( b_" << countB << " & Q_" << countQ - 4 << " & Q_" << countQ - 3 << " & Q_" << countQ - 2 << " & Q_" << countQ - 1 
        << " , BVXOR( b_" << countB << " & Q_" << countQ - 4 << " & Q_" << countQ - 3 << " & Q_" << countQ - 2 
        << " , BVXOR( b_" << countB << " & Q_" << countQ - 4 << " & Q_" << countQ - 3 << " & Q_" << countQ - 1 
        << " , BVXOR( b_" << countB << " & Q_" << countQ - 2 << " & Q_" << countQ - 1 
        << " , BVXOR( b_" << countB << " & Q_" << countQ - 2 
        << " , BVXOR( b_" << countB << " & Q_" << countQ - 1
        << " , BVXOR( b_" << countB + 1 << " & Q_" << countQ - 4 << " & Q_" << countQ - 3 << " & Q_" << countQ - 2 
        << " , BVXOR( b_" << countB + 2 << " & Q_" << countQ - 4 << " & Q_" << countQ - 3 
        << " , BVXOR( b_" << countB + 2 << " & Q_" << countQ - 4 << " & Q_" << countQ - 2 
        << " , BVXOR( b_" << countB + 2 << " & Q_" << countQ - 3 << " & Q_" << countQ - 2 
        << " , BVXOR( b_" << countB + 2 << " & Q_" << countQ - 4
        << " , BVXOR( b_" << countB + 2 << " & Q_" << countQ - 3 
        << " , BVXOR( b_" << countB + 2 << " & Q_" << countQ - 2 
        << " , BVXOR( b_" << countB + 3 << " & Q_" << countQ - 4 
        << " , BVXOR( b_" << countB + 3 << " & Q_" << countQ - 3 
        << " , BVXOR( b_" << countB + 3 << " & Q_" << countQ - 2 
        << " , BVXOR( b_" << countB + 4 << " & Q_" << countQ - 4 << " & Q_" << countQ - 3 
        << " , BVXOR( b_" << countB + 5 << " & Q_" << countQ - 4 
        << " , BVXOR( b_" << countB + 5 << " & Q_" << countQ - 3 
        << " , BVXOR( b_" << countB + 6 << " & Q_" << countQ - 4 
        << " , b_" << countB + 7 
        << "）) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ; " << endl;

        countB += 8;
    }
    //Y
    for (int y = 0; y < bitnum; y++)
    {
        fout << "ASSERT( Y_" << y<<" = ";
        for (int i = 0; i < bitnum;i++){
            if(i==bitnum-1 && GateNum == 0){
                fout << "a_" << countA++ << " & X_" << i;
            }
            fout << "BVXOR( a_" << countA++ << " & X_" << i << " , ";
        }
        for (int i = 0; i < GateNum;i++){
            if (i == GateNum - 1){
                fout << "a_" << countA++ << "& T_" << i;
            }
            else{
                fout << "BVXOR( a_" << countA++ << " & T_" << i << " , ";
            }
        }
        //bracket
        for (int i = 0; i < bitnum + GateNum - 1 + 1; i++)
        {
            if (i == bitnum + GateNum - 1)
            {
                fout << " );" << endl;
            }
            else
            {
                fout << " )";
            }
        }
        //end
    }

    return;
}

void Objective(){
    for (int i = 0; i < GateNum; i++){
        fout << "ASSERT( B_" << i << " = Cost[b_" << 8 * i << "[0:0]@b_" << 8 * i + 1 << "[0:0]@b_" << 8 * i + 2 << "[0:0]@b_" << 8 * i + 3 < "[0:0]@b_" << 8 * i + 4 < "[0:0]@b_" << 8 * i + 5 < "[0:0]@b_"<< 8 * i + 6 < "[0:0]@b_" << 8 * i + 7 << "[0:0]] );" << endl;
    }
    for (int i = 0; i < GateNum; i++){
        if (i==0){
            fout << "ASSERT( GEC = BVPLUS( 8 , ";
        }
        fout << "B_" << i;
        if (i == GateNum - 1)
        {
            fout << " ) );" << endl;
        }
        else{
            fout << " , ";
        }
    }
    fout << "ASSERT( BVLT(GEC , 0bin" << tobits(MinGEC, 8) << ") );"<<endl;
    fout << "QUERY(FALSE);\nCOUNTEREXAMPLE;\n";
    
    return;
}

int main()
{
    State_Variate();
    Trival_Constraint();
    Logic_Constraint();
    Objective();
    return 0;
}
