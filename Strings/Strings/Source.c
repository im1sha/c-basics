#include <stdio.h>
#include <stdbool.h>

// task:
// Из строки, состоящей из букв, цифр, запятых, точек, знаков + и –,
// выделить подстроку, задающую вещественное число с плавающей точкой

#define NUMBERS "0123456789"
#define SIGNS "+-"
#define POINTS ".,"
#define E_LETTERS "Ee"

#define TEST_LENGTH 100

// +1.528535047E-25
//                   +                1                .             528535047         E             -               25
// BeforeNumber MantissaSign MantissaIntegerPart MantissaPoint MantissaRealPart ExponentLetter ExponentSign ExponentIntegerPart AfterNumber
typedef enum _State
{
  BeforeNumberState,
  MantissaSignState,
  MantissaIntegerPartState,
  MantissaPointState, 
  MantissaRealPartState,
  ExponentLetterState,
  ExponentSignState,
  ExponentIntegerPartState,
  AfterNumberState,
} State;

// describes array of char
typedef struct _StringInfo 
{
  char * Begin; // char array begin pointer 
  size_t Length; // total chars in array
} StringInfo;

// Checks whether stringToTest[stringLength] contains any char from charsArray[totalChars]
bool Contains(char * stringToTest, size_t stringLength, char * charsArray, size_t totalChars) 
{
  for (size_t i = 0; i < stringLength; i++)
  {
    for (size_t j = 0; j < totalChars; j++)
    {
      if (stringToTest[i] == charsArray[j])
      {
        return true;
      }
    }
  }
  return false;
}

// gets float number from given string str[len]
StringInfo ExtractRealNumber(char * str, size_t len)
{
  State currentState = BeforeNumberState;
  StringInfo result = { .Length = 0, .Begin = NULL };

  for (size_t i = 0; i < len; i++)
  {
    if (currentState == AfterNumberState)
    {
      // check whether resulting number is float but not integer
      if (Contains(result.Begin, result.Length, POINTS, strlen(POINTS))
        || Contains(result.Begin, result.Length, E_LETTERS, strlen(E_LETTERS)))
      {
        break;
      }
      else
      {
        // go to initial state and continue search 'cause number is not float
        currentState = BeforeNumberState;
        result = (StringInfo) { .Length = 0, .Begin = NULL };
      }
    }
    // pay attention to current State &
    // analyze next char of string 
    //      (it may be of 5 types: sign chars,
    //      number chars,
    //      exponenta chars,
    //      point chars
    //      other chars)
    // and move to next state of enumeration State 
    switch (currentState)
    {
    case BeforeNumberState: 
    {      
      // we at i-th character so we should pass reference to (str + i)
      if (Contains(str + i, 1, SIGNS, strlen(SIGNS)))
      {
        currentState = MantissaSignState;
        result.Begin = str + i;
        result.Length = 1;
      }
      else if (Contains(str + i, 1, NUMBERS, strlen(NUMBERS)))
      {
        currentState = MantissaIntegerPartState;
        result.Begin = str + i;
        result.Length = 1;
      }
      else
      {
        //currentState = BeforeNumberState
      }
      break; 
    }
    case MantissaSignState:
    { 
      if (Contains(str + i, 1, SIGNS, strlen(SIGNS)))
      {
        currentState = MantissaSignState;
        result.Begin = str + i;
        result.Length = 1;
      }
      else if (Contains(str + i, 1, NUMBERS, strlen(NUMBERS)))
      {
        currentState = MantissaIntegerPartState;
        ++result.Length;
      }
      else
      {
        currentState = BeforeNumberState;
        result.Length = 0;
      }
      break; 
    }
    case MantissaIntegerPartState:
    {
      if (Contains(str + i, 1, POINTS, strlen(POINTS)))
      {
        currentState = MantissaPointState;
        ++result.Length;
      }
      else if (Contains(str + i, 1, NUMBERS, strlen(NUMBERS)))
      {
        ++result.Length;
      }
      else if (Contains(str + i, 1, E_LETTERS, strlen(E_LETTERS)))
      {
        currentState = ExponentLetterState;
        ++result.Length;
      }
      else
      {
        currentState = AfterNumberState;
      }
      break;
    }
    case MantissaPointState:
    {
      if (Contains(str + i, 1, NUMBERS, strlen(NUMBERS)))
      {
        currentState = MantissaRealPartState;
        ++result.Length;
      }
      else
      {
        currentState = BeforeNumberState;
        result.Length = 0;
      }
      break;
    }
    case MantissaRealPartState:
    {
      if (Contains(str + i, 1, NUMBERS, strlen(NUMBERS)))
      {
        result.Length++;
      }
      else if (Contains(str + i, 1, E_LETTERS, strlen(E_LETTERS)))
      {
        currentState = ExponentLetterState;
        result.Length++;
      }
      else if (Contains(str + i, 1, SIGNS, strlen(SIGNS)))
      {
        currentState = BeforeNumberState;
        result.Length = 0;
      }
      else
      {
        currentState = AfterNumberState;
      }
      break;
    }
    case ExponentLetterState:
    {
      if (Contains(str + i, 1, NUMBERS, strlen(NUMBERS)))
      {
        currentState = ExponentIntegerPartState;
        result.Length++;
      }
      else if (Contains(str + i, 1, SIGNS, strlen(SIGNS)))
      {
        currentState = ExponentSignState;
        result.Length++;
      }
      else
      {
        currentState = AfterNumberState;
        result.Length--;
      }
      break;
    }
    case ExponentSignState:
    {
      if (Contains(str + i, 1, NUMBERS, strlen(NUMBERS)))
      {
        currentState = ExponentIntegerPartState;
        result.Length++;
      }
      else
      {
        currentState = AfterNumberState;
        result.Length -= 2; // subtract "E∀" string length
      }
      break;
    }
    case ExponentIntegerPartState:
    {
      if (Contains(str + i, 1, NUMBERS, strlen(NUMBERS)))
      {
        result.Length++;
      }   
      else
      {
        currentState = AfterNumberState;
      }
      break;
    }
    default:
      break;
    }   
  }

  if (AfterNumberState != currentState)
  {
    return (StringInfo) { .Length = 0, .Begin = NULL };
  }

  return result;
}


#define TOTAL_STRINGS 28
int main()
{
  system("chcp 1251");
  system("cls");
  
 /* char tests[TOTAL_STRINGS][TEST_LENGTH] =
  {
    "+1.528535047E-25",
    "1.5",
    "+1.5",
    "1.5E25",
    "+1.5E-25",
    "+1.5E25",
    "1.5E+25",
    "dsvrewg+1.528535047E-25svdsgvdsfdsgvs",
    "dsvds1.5svsdfd",
    "dsgvsgves+1.5svfbvds",
    "sdfdsfsd1.5E25svdsvdsvds",
    "vsdvds+1.5E-25svs",
    "dsvsvdes+1.5E25sdvdsvds",
    "vdsvds1.5E+25vsdsvdsv",
    "vgewgergbwg",
    "e",
    "e12",
    "1.",
    "+1.",
    "+1.e+54",
    "1.0e00e+",
    "1",
    "2425452",
    "2ee5",
    "2ee++55",
    "2+e55",
    "e6",
    "e55.e45",
  };*/

  char input[TEST_LENGTH];
  while (true)
  {
    
    puts("ENTER ANY STRING...");

    gets_s(input, TEST_LENGTH);
    StringInfo result = ExtractRealNumber(input, TEST_LENGTH);

    if (result.Begin != NULL)
    {
      for (size_t i = 0; i < result.Length; i++)
      {
        putchar(result.Begin[i]);
      }
      puts("");
    }
    else
    {
      puts("NO FLOAT NUMBER");
    }
  }

  return 0;
}


