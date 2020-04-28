#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// task:
// Дан массив целых чисел X = (x1, x2, .., xn).
// Записать элементы заданного массива Х в массив Z следующим образом :
// в начальной части расположить отрицательные элементы в порядке возрастания,
// затем в порядке убывания положительные элементы, 
// все нулевые элементы записать в конце массива.

#define TEST_LENGTH 100

// simplest sorting algorithm
void InsertionSort(int* arrayToSort, int len)
{
  for (size_t i = 1; i < len; ++i)
  {
    int tmp = arrayToSort[i];
    size_t j = i;
    while (j > 0 && tmp < arrayToSort[j - 1])
    {
      arrayToSort[j] = arrayToSort[j - 1];
      --j;
    }
    arrayToSort[j] = tmp;
  }
}


int main()
{
  char stringToInput[TEST_LENGTH];
  puts("ENTER ARRAY SIZE: ");
  size_t size = atoi(gets_s(stringToInput, TEST_LENGTH));
  
  //
  // malloc() allocates memory with random data inside
  // it's ok if we overwrite all data of this array later with another data
  // https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/malloc?view=vs-2019
  //
  // calloc() initializes with zeroes all the allocated memory
  // https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/calloc?view=vs-2019
  //
  // should call free() for variables allocated with malloc(), calloc(), realloc() 
  // when allocated variables used no more
  // https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/free?view=vs-2019
  //

  int *inputNumbers = (int *)malloc(size * sizeof(int));
  puts("ENTER ITEMS:");
  for (size_t i = 0; i < size; i++)
  {
    inputNumbers[i] = atoi(gets_s(stringToInput, TEST_LENGTH));
  }

  size_t positiveCount = 0, negativeCount = 0;

  // count positive and negative numbers

  for (size_t i = 0; i < size; i++)
  {
    if (inputNumbers[i] < 0)
    {
      negativeCount++;
    }
    else if (inputNumbers[i] > 0)
    {
      positiveCount++;
    }
  }

  // allocate memory for signed values to sort
  int* positiveNumbers = (int*)malloc(positiveCount * sizeof(int));
  int* negativeNumbers = (int*)malloc(negativeCount * sizeof(int));

  int* resultArray = (int*)calloc(size, sizeof(int));

  size_t positiveAdded = 0, negativeAdded = 0;

  // get all signed values to appropriate arrays
  for (size_t i = 0; i < size; i++)
  {
    if (inputNumbers[i] < 0)
    {
      negativeNumbers[negativeAdded] = inputNumbers[i];
      negativeAdded++;
    }
    else if (inputNumbers[i] > 0)
    {
      positiveNumbers[positiveAdded] = inputNumbers[i];
      positiveAdded++;
    }
  }

  InsertionSort(positiveNumbers, positiveCount);
  InsertionSort(negativeNumbers, negativeCount);

  // fill result array up
  for (size_t i = 0; i < size; i++)
  {
    if (i < negativeCount)
    {
      resultArray[i] = negativeNumbers[i];
    }
    else if (i >= negativeCount && i < negativeCount + positiveCount)
    {
      // reverse positive values
      resultArray[i] = positiveNumbers[negativeCount + positiveCount - i - 1];
    }
  }

  puts("RESULT:");
  for (size_t i = 0; i < size; i++)
  {
    printf_s("%i\n", resultArray[i]);
  }

  free(negativeNumbers);
  free(positiveNumbers);
  free(inputNumbers);
  free(resultArray);

  _getch();

  return 0;
}

