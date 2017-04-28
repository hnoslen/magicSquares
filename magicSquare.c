#include <stdio.h>
#include <stdlib.h>

// n is dimensions of square
// arr is square read across rows
int is_magic(unsigned int n, unsigned int * arr){
	unsigned int sum;
	int i;
	for (i = 0; i < n; ++i){
		sum = sum + arr[i];
	}
	unsigned int testsum;
	// check rows
	for (unsigned int row = 1; row < n; row++){
		testsum = 0;
		for (i = 0; i < n; ++i){
			testsum = testsum + arr[(row*n)+i];
		}
		if (testsum != sum){
			return 0;
		}
	}
	// check columns
	for (unsigned int row = 0; row < n; row++){
		testsum = 0;
		for (i = 0; i < n; ++i){
			testsum = testsum + arr[row+(i*n)];
		}
		if (testsum != sum){
			return 0;
		}
	}
	// diagonals
	testsum = 0;
	for (i = 0; i < n; i++){
		testsum = testsum + arr[i*(n+1)];
	}
	if (testsum != sum){
		return 0;
	}
	testsum = 0;
	for (i = 1; i <= n; i++){
		testsum = testsum + arr[i*(n-1)];
	}
	if (testsum != sum){
		return 0;
	}else{
		return 1;
	}
}

void printSquare(unsigned int n, unsigned int * arr){
	unsigned int row, entry;
	for (row = 0; row < n; row++){
		for (entry = 0; entry < n; entry++){
			printf("%u", arr[(row*n)+entry]);
		}
		printf("\n");
	}
}

unsigned int factorial(unsigned int x){
	unsigned int out = 1;
	for (unsigned int i = x; i > 1; i--){
		out = out*i;
	}
	return out;
}

typedef struct{
	unsigned int ** array;
	unsigned int n;
	unsigned int k;
	unsigned int combs;
        unsigned int numFilled;
} CombArray;

typedef struct{
	unsigned int ** array;
        unsigned int numPerm;
        unsigned int size_of_each;
        unsigned int numFilled;
} PermArray;

PermArray * newPermArray(unsigned int n){
	unsigned int num = factorial(n);
	unsigned int i;
	PermArray * out = malloc(sizeof(PermArray)*n);
	out->numPerm = num;
	out->size_of_each = n;
	out->numFilled = 0;
	out->array = malloc(sizeof(unsigned int *)*num);
	for (i = 0; i < num; i++){
		out->array[i] = malloc(sizeof(unsigned int)*n);
	}
	return out;
}

void freePermArray(PermArray * arr){
	for (unsigned int i = 0; i < arr->numPerm; i++){
		free(arr->array[i]);
	}
        free(arr->array);
	free(arr);
	return;
}

void printPermArray(PermArray * arr){
	for (unsigned int i = 0; i < arr->numFilled; i++){
		for (unsigned int entry = 0; entry < arr->size_of_each; entry++){
			printf("%u\t", arr->array[i][entry]);
		}
		printf("\n");
	}
	return;
}

void printCombArray(CombArray * arr){
	for (unsigned int i = 0; i < arr->numFilled; i++){
		for (unsigned int entry = 0; entry < arr->k; entry++){
			printf("%u\t", arr->array[i][entry]);
		}
		printf("\n");
	}
	return;
}

CombArray * newCombArray(unsigned int n, unsigned int k){
	CombArray * out = malloc(sizeof(CombArray));
	out->n = n;
	out->k = k;
        out->numFilled = 0;
	out->combs = factorial(n)/(factorial(k)*factorial(n-k));
	out->array = malloc(sizeof(unsigned int *) * out->combs);
	for (unsigned int i = 0; i < out->combs; i++){
		out->array[i] = malloc(sizeof(unsigned int)*k);
	}
	return out;
}

void freeCombArray(CombArray * x){
	for (unsigned int i = 0; i < x->combs; i++){
		free(x->array[i]);
	}
	free(x->array);
	free(x);
}

void recursivePermute(unsigned int numdone, unsigned int * done, unsigned int numleft, unsigned int * left, PermArray * arr){
	unsigned int i;
	if (numleft <= 1){
		for (i = 0; i < numdone; i++){
			arr->array[arr->numFilled][i] = done[i];
		}
		arr->array[arr->numFilled][numdone] = left[0];
		arr->numFilled++;
	}else{
		unsigned int newleft[numleft-1];
		for (i = 0; i < numleft; i++){
			done[numdone] = left[i];
			unsigned int place = 0;
			for (unsigned int j = 0; j < numleft; j++){
				if (j == i){
					;
				}else{
					newleft[place] = left[j];
					place++;
				}
			}
			recursivePermute(numdone+1, done, numleft-1, newleft,arr);
		}
	}
}

PermArray * permuteArray(unsigned int n, unsigned int * array){
	PermArray * out = newPermArray(n);
	unsigned int done[n];
	recursivePermute(0, done, n, array, out);
	return out;
}

void recursiveCombinate(unsigned int numFixed, unsigned int * fixed, unsigned int numFree, unsigned int * free, CombArray * arr){
  unsigned int i;
  if (numFixed < arr->k){
    for (i = 0; i < numFree; i++){
      fixed[numFixed] = free[i];
      recursiveCombinate(numFixed+1,fixed,numFree-i-1,free+i+1,arr);
    }
  }else{
    for (i = 0; i < arr->k; i++){
      arr->array[arr->numFilled][i] = fixed[i];
    }
    arr->numFilled++;
  }
}

CombArray * combinateArray(unsigned int n, unsigned int k, unsigned int * a){
  CombArray * out = newCombArray(n,k);
  unsigned int fixed[k];
  recursiveCombinate(0,fixed,n,a,out);
  return out;
}

void check_magic_withMax(unsigned int squareDim, unsigned int N){
  unsigned int numbs[N];
  unsigned int i;
  for (i = 0; i < N; i++){
    numbs[i] = i;
  }
  unsigned int checknums[squareDim*squareDim-1];
  CombArray * ca = combinateArray(N,squareDim*squareDim-1,numbs);
  for (i = 0; i < ca->combs; i++){
    for (unsigned int j = 0; j < ca->k; j++){
      checknums[j] = ca->array[i][j];
    }
    checknums[ca->k] = N;
    PermArray * pa = permuteArray(ca->k,checknums);
    for (unsigned int k = 0; k < pa->numPerm; k++){
      if (is_magic(squareDim, pa->array[k])){
        printf("Found magic:\n");
        printSquare(squareDim,pa->array[k]);
      }
    }
    freePermArray(pa);
  }
  freeCombArray(ca);
}

int main(){
  unsigned int square_edge = 3;
  unsigned int start = 8;
  unsigned int max = 8;
  for (unsigned int i = start; i < max+1; i++){
    printf("Checking size %u with max %u\n",square_edge,i);
    check_magic_withMax(square_edge, i);
  }
  return 0;
}
