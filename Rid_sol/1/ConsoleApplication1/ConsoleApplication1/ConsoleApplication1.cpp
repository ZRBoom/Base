// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdint.h>
#include <vector>


#define m  8      // степень RS-полинома     (согласно Стандарта ECMA-130 - восемь)
#define n  255    // n=2**m-1         (длина кодового слова)
#define t  64	  // Кол-во исправляемых ошибок
//uint8_t GenPoly[t * 2 + 1];
int alpha_to[]{
		1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234, 201, 143,
		3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35,
		70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95, 190, 97, 194, 153, 47, 94,
		188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163,
		91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206, 129, 31,
		62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204, 133, 23, 46, 92, 184, 109, 218, 169,
		79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183,
		115, 230, 209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171,
		75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112,
		224, 221, 167, 83, 166, 81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18,
		36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207,
		131, 27, 54, 108, 216, 173, 71, 142, 1};

int primitiv_of[]{
	-1, 0, 1, 25, 2, 50, 26, 198, 3, 223, 51, 238, 27, 104, 199, 75, 4, 100, 224, 14, 52, 141, 239, 129, 28,
		193, 105, 248, 200, 8, 76, 113, 5, 138, 101, 47, 225, 36, 15, 33, 53, 147, 142, 218, 240, 18, 130, 69,
		29, 181, 194, 125, 106, 39, 249, 185, 201, 154, 9, 120, 77, 228, 114, 166, 6, 191, 139, 98, 102, 221,
		48, 253, 226, 152, 37, 179, 16, 145, 34, 136, 54, 208, 148, 206, 143, 150, 219, 189, 241, 210, 19, 92,
		131, 56, 70, 64, 30, 66, 182, 163, 195, 72, 126, 110, 107, 58, 40, 84, 250, 133, 186, 61, 202, 94, 155,
		159, 10, 21, 121, 43, 78, 212, 229, 172, 115, 243, 167, 87, 7, 112, 192, 247, 140, 128, 99, 13, 103,
		74, 222, 237, 49, 197, 254, 24, 227, 165, 153, 119, 38, 184, 180, 124, 17, 68, 146, 217, 35, 32, 137,
		46, 55, 63, 209, 91, 149, 188, 207, 205, 144, 135, 151, 178, 220, 252, 190, 97, 242, 86, 211, 171, 20,
		42, 93, 158, 132, 60, 57, 83, 71, 109, 65, 162, 31, 45, 67, 216, 183, 123, 164, 118, 196, 23, 73, 236,
		127, 12, 111, 246, 108, 161, 59, 82, 41, 157, 85, 170, 251, 96, 134, 177, 187, 204, 62, 90, 203, 89,
		95, 176, 156, 169, 160, 81, 11, 245, 22, 235, 122, 117, 44, 215, 79, 174, 213, 233, 230, 231, 173, 232,
		116, 214, 244, 234, 168, 80, 88, 175};
uint8_t GenPoly[]{
	1, 248, 253, 36, 153, 124, 31, 51, 245, 23, 76, 56, 81, 231, 62, 128, 162, 145, 87, 176, 37, 81, 40, 241,
		130, 125, 77, 78, 179, 201, 1, 51, 186, 188, 77, 226, 95, 200, 16, 142, 183, 155, 170, 185, 210, 31, 240,
		107, 53, 210, 6, 10, 174, 162, 142, 113, 1, 229, 122, 38, 202, 172, 146, 65, 91, 190, 228, 175,
		240, 85, 11, 20, 205, 235, 19, 67, 81, 7, 35, 162, 161, 139, 52, 19, 225, 86, 149, 151, 50, 198, 119, 189,
		52, 232, 174, 233, 26, 35, 10, 224, 36, 155, 112, 241, 173, 208, 248, 77, 68, 98, 8, 208, 6, 81, 87, 96,
		192, 146, 175, 107, 185, 187, 28, 30, 183, 87, 246, 187, 217};

int p[m + 1] = { 1, 0, 1, 1, 1, 0, 0, 0, 1 };

//int alpha_to[n + 1];                    // таблица степеней примитивного члена

//int primitiv_of[n + 1];

void generate_gf()

{

	int i, mask;

	mask = 1; alpha_to[m] = 0;

	for (i = 0; i < m; i++)

	{

		alpha_to[i] = mask;

		//index_of[alpha_to[i]] = i;

		if (p[i] != 0) alpha_to[m] ^= mask;

		mask <<= 1;

	} /*index_of[alpha_to[m]] = m;*/ mask >>= 1;

	for (i = m + 1; i < n; i++)

	{

		if (alpha_to[i - 1] >= mask)

			alpha_to[i] = alpha_to[m] ^ ((alpha_to[i - 1] ^ mask) << 1);

		else

			alpha_to[i] = alpha_to[i - 1] << 1;

	}
	alpha_to[255] = 1;

	/*for (int i = 0; i <= n; i++){
		printf("Index: %d, Alfa: %d, Alfa_manual: %d \r\n", i, alpha_to[i], alpha_toi[i]);
		if (alpha_toi[i] != alpha_to[i]){
			printf("\n\n\n\n\n%d\n\n\n\n", i);
		}
	}*/

	for (int i = 0; i <= n; i++)
		primitiv_of[alpha_to[i]] = i;
	primitiv_of[0] = -1;
	primitiv_of[1] = 0;
	/*for (int i = 0; i <= n; i++){
		printf("i: %d, Primitiv: %d, Primitiv_manual: %d \r\n", i, primitiv_of[i], primitivi[i]);
		if (primitiv_of[i] != primitivi[i])
			printf("\n\n\n\n\n%d\n\n\n\n", i);
	}*/
}

int multy_galua(int a, int b)

{

	int sum;

	if (a == 0 || b == 0) return 0;   // немного оптимизации не повредит

	sum = primitiv_of[a] + primitiv_of[b];  // вычисляем сумму индексов полиномов

	sum = sum % 255;
	//if (sum >= n ) sum -= n;     // приводим сумму к модулю GF

	return alpha_to[sum];             // переводим результат в полиномиальную…

	// …форму и возвращаем результат

}

int division_galua(int a, int b)

{

	int diff;

	if (a == 0) return 0;             // немного оптимизации не повредит

	if (b == 0) return -1;            // на ноль делить нельзя!

	diff = primitiv_of[a] + (255 - primitiv_of[b]); // вычисляем разность индексов

	//if (diff < 0) diff += n;       // приводим разность к модулю GF
	diff = diff % 255;

	return alpha_to[diff];            // переводим результат в полиномиальную…

	// …форму и возвращаем результат

}

uint8_t sum_galua(uint8_t leftValue, uint8_t rightValue){
	return leftValue ^ rightValue;
}

void create_block(uint8_t input_data[], uint8_t input_size, uint8_t output_data[]){
	for (int i = 0; i < input_size; i++)
		output_data[i] = input_data[i];
	for (int i = input_size; i <= n - 1; i++)
		output_data[i] = 0;
}

int pow_galua(int number, int power){
	
	if (power == 0) return 1;
	if (power == 1) return number;
	return alpha_to[(power * primitiv_of[number]) % 255];

}

int inverse_multy_galua(int number){
	if (number == 0) return -1;
	return alpha_to[255 - primitiv_of[number]];
}
//DECODEING Functions
//Check NULL syndrom
bool CheckSyndrom(uint8_t Syndrom[], int size){
	for (int i = 0; i < size; i++){
		if (Syndrom[i] != 0) return false;
	}
	return true;

}

//Truncated linear convolution "Line9na9 svertka"
void LinearConvolution(uint8_t * vectorA, int lengthA, uint8_t *vectorB, int lengthB, int SizeModule, uint8_t *result, int &resultLength){
	int k = lengthA;
	int l = lengthB;
	uint8_t *preResult;
	uint8_t preLength;

	preLength = k + l - 1;
	preResult = new uint8_t[preLength];

	for (int q = 0; q <= k + l - 2; q++){
		int sum = 0;
		for (int i = 0; i <= q; i++){
			if ((i > k - 1) || (q - i > l - 1)) sum += 0;
			else sum = sum_galua(sum, multy_galua(vectorA[i], vectorB[q - i]));
		}
		preResult[q] = sum;
	}

	resultLength = SizeModule;
	for (int i = 0; i < SizeModule; i++){
		result[i] = preResult[preLength - SizeModule + i];
	}
	delete[]preResult;
}

//Cyclic convolution "Cykli4eska9 svertka"
void CyclicConvlution(uint8_t * vectorA, int lengthA, uint8_t *vectorB, int lengthB, int powOfPoli, uint8_t *result, int &resultLength){
	int k = lengthA;
	int l = lengthB;

	uint8_t *preResult = new uint8_t[powOfPoli];

	resultLength = powOfPoli;

	uint8_t * MyA = new uint8_t[lengthA];
	uint8_t * MyB = new uint8_t[lengthB];

	for (int i = 0; i < lengthA; i++)
		MyA[i] = vectorA[lengthA - 1 - i];

	for (int i = 0; i < lengthB; i++)
		MyB[i] = vectorB[lengthB - 1 - i];

	for (int q = 0; q <= powOfPoli-1; q++){
		int sum = 0;
		for (int i = 0; i <= powOfPoli - 1; i++){
			if ((i > k - 1) || (q - i > l - 1)) sum += 0;
			else sum = sum_galua(sum, multy_galua(MyA[i], MyB[(powOfPoli +  q - i)%powOfPoli]));
		}
		preResult[q] = sum;
	}

	for (int i = 0; i < powOfPoli; i++){
		printf(" %d", preResult[i]);
	}
	for (int i = 0; i < powOfPoli; i++){
		result[i] = preResult[powOfPoli - i -1];
	}
	for (int i = 0; i < powOfPoli; i++){
		printf(" %d", result[i]);
	}

	delete[]MyA;
	delete[]MyB;
	delete[]preResult;
}

//Find R(x) in F(x) =Q(x)*g(x)+R(x); R(x) = F(x)mod(g(x)) 
void DivisionVectVect(uint8_t *vector, int vectLength, uint8_t *normPolinom, int polinomLength, uint8_t *result){
	int r = polinomLength - 1;
	int k = vectLength;
	uint8_t *Fi;
	Fi = new uint8_t[k];

	uint8_t *Rold, *Rcur;
	Rold = new uint8_t[r];
	Rcur = new uint8_t[r];

	for (int i = 0; i < k; i++)
		Fi[i] = vector[k - 1 - i];//x0 == Fi[0] == vector[k-1]

	for (int i = 0; i < r; i++)
	{
		Rold[i] = Fi[k - r + i];
	}

	for (int s = 1; s <= k - r; s++){
		Rcur[0] = sum_galua(Fi[k - r - s], multy_galua(normPolinom[polinomLength - 1], Rold[r - 1]));
		for (int i = 1; i < r; i++)
			Rcur[i] = sum_galua(Rold[i - 1], multy_galua(normPolinom[polinomLength - 1 - i], Rold[r - 1]));
		for (int i = 0; i < r; i++)
			Rold[i] = Rcur[i];
	}

	for (int i = 0; i < r; i++)
		result[i] = Rcur[r - 1 - i];

	delete[]Fi;
	delete[]Rold;
	delete[]Rcur;

}

//Formal derivative "Proizvodna9"
void Derivate(uint8_t *vector, int lengthOfVector, uint8_t *result){
	for (int i = 0; i < lengthOfVector - 1; i++){
		result[i] = multy_galua(vector[i], (lengthOfVector - 1 - i) % 2);
	}
}

//Computing a value of the polynomial at the given argument(Gorner`s method)
int CalculateValue(uint8_t * vector, int length, uint8_t value){
	int result = 0;
	for (int s = 1; s <= length; s++){
		result = sum_galua(multy_galua(value, result), vector[s - 1]);
		//printf("preresult: %d\n", result);
	}
	return result;
}

//Multi of vector and vector
void MultiVectVect(uint8_t * vectorA, int lengthA, uint8_t *vectorB, int lengthB, uint8_t *result, int &resultLength){
	int k = lengthA;
	int l = lengthB;

	resultLength = k + l - 1;

	for (int q = 0; q <= k + l - 2; q++){
		int sum = 0;
		for (int i = 0; i <= q; i++){
			if ((i > k - 1) || (q - i > l - 1)) sum += 0;
				else sum = sum_galua(sum, multy_galua(vectorA[i], vectorB[q - i]));
		}
		result[q] = sum;
	}
}
//Multi vector on x (1, 0)
void MultiVectOnX(uint8_t * vectorA, int lengthA, uint8_t *result){
	for (int i = 0; i < lengthA-1; i++){
		result[i] = vectorA[i + 1];
	}
	result[lengthA - 1] = 0;
}

//Multi of vector and matrix
void MultiVectMatrix(uint8_t *vector, uint8_t **matrix, int length, uint8_t *resultVect){
	for (int i = 0; i < length; i++){
		resultVect[i] = 0;
		for (int j = 0; j < length; j++){
			resultVect[i] = sum_galua(resultVect[i], multy_galua(vector[j], matrix[j][i]));
		}
	}
}

//Multi number and matrix
void MultiNumVector(uint8_t *vector, int length, int number){
	for (int i = 0; i < length; i++)
		vector[i] = multy_galua(vector[i], number);
}

//Genetator polynomial "Obrazuushii polinom"
void generatorPoly(int numberOfCorrections, int parametrB, uint8_t *result){
	int length = 0;
	int B = 0;
	int curLength = 2;
	int r = numberOfCorrections * 2;
	uint8_t A[2];
	uint8_t *curResult;
	uint8_t *oldResult;

	A[0] = 1;
	length = r + 1;
	curResult = new uint8_t[length];
	oldResult = new uint8_t[length];

	if (parametrB == 0) B = inverse_multy_galua(2);
	else B = pow_galua(2, parametrB - 1);

	oldResult[0] = 1;
	oldResult[1] = multy_galua(B, 2);
	for (int s = 2; s <= r; s++)
	{
		A[1] = multy_galua(B, pow_galua(2, s));
		MultiVectVect(oldResult, curLength, A, 2, curResult, curLength);
		for (int i = 0; i < curLength; i++){
			oldResult[i] = curResult[i];
		}
	}

	for (int i = 0; i < length; i++){
		result[i] = curResult[i];
	}

	delete[]curResult;
	delete[]oldResult;
}

//Difference(sum) of two Vectors with same length
void DiffVectors(uint8_t *vector1, uint8_t *vector2, int length, uint8_t *result){
	for (int i = 0; i < length; i++){
		result[i] = sum_galua(vector1[i], vector2[i]);
	}
}

//Sum(differrence) of two Vectors without same length
void SumAnyVectors(uint8_t * vectorA, int lengthA, uint8_t *vectorB, int lengthB, uint8_t *result, int &length){
	bool A = false;
	if (lengthA >= lengthB) {
		length = lengthA;
		A = true;
	}
	else {
		length = lengthB;
		A = false;
	}
	if (A){
		for (int i = 0; i < lengthB; i++)
		{
			result[lengthA - 1 - i] = sum_galua(vectorA[lengthA - 1 - i], vectorB[lengthB - 1 - i]);
		}
		for (int i = 0; i < lengthA - lengthB; i++)
			result[i] = vectorA[i];
	}
	else{
		printf("im here B\n");
		for (int i = 0; i < length; i++)
			result[lengthB - 1 - i] = sum_galua(vectorB[lengthB - 1 - i], vectorA[lengthA - 1 - i]);
		for (int i = 0; i < lengthB - lengthA; i++)
			result[i] = vectorB[i];
	}
}

//Coder and Decoder
void RS_codec(uint8_t *input_data, uint8_t input_size, uint8_t *output_data){
	uint8_t *Rcur;
	Rcur = new uint8_t[t * 2];
	int length = 0;
	for (int i = 0; i < input_size; i++){
		output_data[i] = input_data[i];
	}
	for (int i = input_size; i < input_size + t * 2; i++)
	{
		output_data[i] = 0;
	}

	DivisionVectVect(output_data, input_size + t * 2, GenPoly, t * 2 + 1, Rcur);
	SumAnyVectors(output_data, input_size + t * 2, Rcur, t * 2, output_data, length);

	delete Rcur;

}

//Calculate the Syndrom from receiving data
void CalcSyndrom(uint8_t *recievedData, int lengthOfData, uint8_t *Syndrom){
	for (int i = 1; i <= t * 2; i++)
	{
		Syndrom[i] = CalculateValue(recievedData, lengthOfData, pow_galua(2, i));
	}
	Syndrom[0] = 0;
}

//Get position of errors(Berlekamp - Messi)
void GetPosError(uint8_t *Syndrom, std::vector<uint8_t> *PosErrors, uint8_t *ErrLokator, int &LDegree){
	uint8_t r = t * 2;						//<--start move 1
	uint8_t q = 0;
	int M = -1;
	uint8_t L = 0;
	uint8_t Lx;
	uint8_t delta;
	int tresh;
	bool success = false;

	uint8_t Lambda[t * 2 + 1];
	uint8_t LambdaX[t * 2 + 1];
	uint8_t B[t * 2 + 1];
	uint8_t Bx[t * 2 + 1];

	MultiNumVector(Lambda, t * 2 + 1, 0);
	MultiNumVector(LambdaX, t * 2 + 1, 0);
	MultiNumVector(B, t * 2 + 1, 0);
	MultiNumVector(Bx, t * 2 + 1, 0);

	Lambda[t * 2] = 1;
	B[t * 2 - 1] = 1;						//<--stop move 1

	for (;;){
		memcpy(Bx, B, r + 1);
		delta = 0;
		for (int i = 0; i <= q; i++)//<--start move 2
			delta = sum_galua(delta, multy_galua(Lambda[r - i], Syndrom[q - i + 1]));//<--stop move 2
		/*printf("q: %d; L: %d; delta: %d\n", q, L, delta);
		printf("\n");*/

		if (delta != 0){//<--start and stop move 3
			MultiNumVector(Bx, r + 1, delta);//<--start move 4.1
			SumAnyVectors(Lambda, r + 1, Bx, r + 1, LambdaX, tresh);//<--stop move 4.1

			if (L < q - M){//<--start and stop move 4.2
				Lx = q - M; //<--start move 4.2.1
				M = q - L;
				L = Lx;
				MultiNumVector(Lambda, r + 1, inverse_multy_galua(delta));
				memcpy(B, Lambda, r + 1);//<--stop 4.2.1
			}

			memcpy(Lambda, LambdaX, r + 1);//<-- move 4.3
		}

		MultiVectOnX(B, r + 1, B); //<--move 5
		q++;//<--move 6

		if (q == 2 * t){//<--move 7
			for (int i = 0; i < r + 1; i++){
				if (Lambda[i] != 0) {
					LDegree = r - i;
					break;
				}
			}

			if (LDegree == L){
				printf("\nL(x) success\n");
				success = true;
			}
			else{
				printf("\nL(x) no success\n");
				success = false;
			}

			break;
		}
	}
	if (!success){ PosErrors->push_back(0); }
	else{
		int u = 0;
		for (int i = 1; i <= 255; i++){
			u = CalculateValue(Lambda, r + 1, i);
			if (u == 0){
				printf("Koren: %d\n", i);
				PosErrors->push_back(primitiv_of[inverse_multy_galua(i)]);
				printf("Position: %d\n", primitiv_of[inverse_multy_galua(i)]);
			}
		}
	}
	/*for (int i = 0; i < t * 2 + 1; i++){
		printf("L[%d]: %d; Lx[%d]: %d; B[%d]: %d,Bx[%d]: %d\n", i, Lambda[i], i, LambdaX[i], i, B[i], i, Bx[i]);
	}*/
	memcpy(ErrLokator, Lambda, r + 1);
}

//Get error polinom
void GetErrPoli(uint8_t *Syndrom, std::vector<uint8_t> *PosErrors,std::vector<uint8_t> *errPoli, uint8_t *Lambda, int LDegree){
	uint8_t r = t * 2;

	uint8_t *Q;
	Q = new uint8_t[r + 1];
	uint8_t *LDer;
	LDer = new uint8_t[r];

	MultiNumVector(Q, r + 1, 0);
	MultiNumVector(LDer, r, 0);

	for (int q = 0; q <= LDegree - 1; q++){
		int sum = 0;
		for (int i = 0; i <= q; i++){
			sum = sum_galua(sum, multy_galua(Lambda[r - i], Syndrom[q - i + 1]));
		}
		Q[r - q] = sum;
	}
	
	/*for (int i = 0; i < r + 1; i++){
		printf("Q[%d]: %d\n", i, Q[i]);
	}*/

	Derivate(Lambda, r + 1, LDer);
	/*for (int i = 0; i < r; i++){
		printf("L`[%d]: %d\n", i, LDer[i]);
	}*/

	uint8_t Q_u = 0;
	uint8_t LDer_u = 0;
	for (std::vector<uint8_t>::iterator it = PosErrors->begin(); it < PosErrors->end(); ++it){
		Q_u = CalculateValue(Q, r + 1, inverse_multy_galua(pow_galua(2, *it)));
		LDer_u = CalculateValue(LDer, r, inverse_multy_galua(pow_galua(2, *it)));
		errPoli->push_back(division_galua(Q_u, LDer_u));
		//err = division_galua(CalculateValue(Q, r + 1, inverse_multy_galua(pow_galua(2, (int)PosErrors[i])))
	}
}


void RS_decode(uint8_t input_data[], int input_size, uint8_t output_data[]){
	
	uint8_t Syndrom[t * 2 + 1];
	CalcSyndrom(input_data, input_size, Syndrom);
	if (CheckSyndrom(Syndrom, t * 2 + 1)){
		printf("The input Data haven`t erros\n");
		for (int i = 0; i < input_size - t * 2; i++){
			output_data[i] = input_data[i];
		}
		return;
	}
	else {
		printf("The input Data have some erros\n");
		/*for (int i = 0; i < t * 2 + 1; i++){
			printf("S[%d]: %d\n", i, Syndrom[i]);
		}*/
	}
	printf("\n");

	std::vector<uint8_t> PosErrors(0);
	uint8_t *Lambda;
	Lambda = new uint8_t[2 * t + 1];
	int degreeOfLambda;

	GetPosError(Syndrom, &PosErrors, Lambda, degreeOfLambda);
	printf("Size: %d\n", PosErrors.size());
	for (uint8_t i = 0; i < PosErrors.size(); i++){
		printf("Postion_%d: %d\n", i, PosErrors[i]);
	}

	std::vector<uint8_t> ErrorPolinom(0);

	GetErrPoli(Syndrom, &PosErrors, &ErrorPolinom, Lambda, degreeOfLambda);
	printf("Size: %d\n", ErrorPolinom.size());
	for (uint8_t i = 0; i < ErrorPolinom.size(); i++){
		printf("Error_%d: %d\n", i, ErrorPolinom[i]);
	}
	std::vector<uint8_t>::iterator itP;//= PosErrors.begin();
	std::vector<uint8_t>::iterator itE;// = ErrorPolinom.begin();
	for (itP = PosErrors.begin(), itE = ErrorPolinom.begin(); itP != PosErrors.end(); ++itP, ++itE)
	{
		input_data[input_size - *itP -1] = sum_galua(input_data[input_size - *itP-1], *itE);
	}

	for (int i = 0; i < input_size - t * 2; i++){
		output_data[i] = input_data[i];
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//generate_gf();

	//generatorPoly(t, 1, GenPoly);


	uint8_t data[127];
	uint8_t dataRS[127];
	uint8_t F[255];

	for (int i = 0; i < 127; i++){
		data[i] = i;
	}

	RS_codec(data, 127, F);

	for (int i = 0; i < 64; i++){
		F[i] = sum_galua(F[i], 255 - i);
	}
	RS_decode(F, 255, dataRS);
	for (int i = 0; i < 127; i++){
		printf("%d ", dataRS[i]);
	}

	/*int a = 0;
	int b = 0;*/

	/*uint8_t data[]{213, 224, 234, 229, 240};
	uint8_t dataRS[5];
	uint8_t F[133];

	printf(" \n");
	RS_codec(data, 5, F);
	for (int i = 0; i < 133; i++){
		printf("F[%d]: %d\n", i, F[i]);
	}
	printf(" \n");

	F[1] = 203;
	F[3] = 236;
	F[2] = 3;
	F[0] = 0;
	RS_decode(F, 5 + t * 2, dataRS);

	printf(" \n");
	for (int i = 0; i < 5; i++){
		printf("%d ", dataRS[i]);
	}*/

	/*printf(" \n");
	uint8_t Syndrom[5];
	CalcSyndrom(F, 9, Syndrom);
	for (int i = 0; i < 5; i++){
		printf("S[%d]: %d\n", i, Syndrom[i]);
	}*/


	/*uint8_t aa[]{ 49, 50, 51, 0, 0 };
	uint8_t gg[]{ 1, 6, 8};
	uint8_t R[2];

	DivisionVectVect(aa, 5, gg, 3, R);
	printf("R[1]: %d\n", R[1]);
	printf("R[0]: %d\n", R[0]);*/

	/*uint8_t *R = new uint8_t[5];
	int length;
	generatorPoly(2, 1, R, length);

	printf("Result length: %d\n", length);
	for (int i = 0; i < length; i++)
		printf("R[%d]: %d\n", i, R[i]);*/
	
	/*uint8_t A[]{1, 7, 14, 8};
	uint8_t B[]{1, multy_galua(142, 16)};
	uint8_t *R = new uint8_t[5];
	int length;
	MultiVectVect(A, 4, B, 2, R, length);

	printf("Result length: %d\n", length);
	for (int i = 0; i < length; i++)
		printf("R[%d]: %d\n", i, R[i]);*/

/*	uint8_t B[]{1, 2, 3, 4};
	uint8_t A[]{1, 0, 2};
	uint8_t *R = new uint8_t[5];
	int length;
	SumAnyVectors(A, 3, B, 4, R, length);

	printf("Result length: %d\n", length);
	for (int i = 0; i < length; i++)
	printf("R[%d]: %d\n", i, R[i]);*/

	/*printf("\nInput data: \n");
	for (int i = 0; i < 245; i++)
	{
		printf(" %X", data[i]);
	}*/

	/*uint8_t A[]{49, 50, 51};
	uint8_t B[]{19, 93, 1};
	uint8_t *R = new uint8_t[5];
	int length;
	LinearConvolution(A, 3, B, 3, 2, R, length);

	printf("Result length: %d\n", length);
	for (int i = 0; i < length; i++)
	printf("R[%d]: %d\n", i, R[i]);*/


	/*uint8_t A[]{49, 50, 51};
	uint8_t B[]{19, 93, 1};
	uint8_t *R = new uint8_t[5];
	int length;
	CyclicConvlution(A, 3, B, 3, 3, R, length);

	printf("Result length: %d\n", length);
	for (int i = 0; i < length; i++)
	printf("R[%d]: %d\n", i, R[i]);*/

	/*uint8_t A[]{100, 218, 31, 3, 51};
	uint8_t R[5];

	Derivate(A, 5, R);
	for (int i = 0; i < 5; i++)
		printf("R[%d]: %d\n", i, R[i]);*/
		
	/*uint8_t A[]{1, 30, 216, 231, 116};
	
	uint8_t result = CalculateValue(A, 5, 77);
	printf("Result: %d\n", result);*/

	/*printf("\nInput a:\n");
	std::cin >> a;
	printf("Input b:\n");
	std::cin >> b;
	printf("\n Result of sum: %d\n", sum_galua(a, b));*/


	/*printf("Input a:\n");
	std::cin >> a;
	printf("Input b:\n");
	std::cin >> b;
	printf("\n Result of diff: %d\n", division_galua(a, b));*/

	/*printf("\nInput a:\n");
	std::cin >> a;
	printf("Input b:\n");
	std::cin >> b;
	printf("\n Result of multy: %d\n", multy_galua(a, b));*/


	/*printf("\nInput number:\n");
	std::cin >> a;
	printf("Input power:\n");
	std::cin >> b;
	printf("\n Result of power: %d\n", pow_galua(a, b));*/

	/*printf("\nInput number:\n");
	std::cin >> a;
	printf("\n Result of inverse_multy: %d\n", inverse_multy_galua(a));*/



	return 0;
}

