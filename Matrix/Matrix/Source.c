#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <conio.h>
#include <time.h>

//
// task:
//
// Для неориентированного графа, представленного матрицей
// инцидентности построить все цепи, исходящие из заданной вершины.
// Программа должна реализовывать следующие функции :
//
//  • Функцию, позволяющую записывать данные о графе в текстовой файл.
//
//  • Функцию, позволяющую считывать данные о графе из текстового файла.
//
//  • Функцию, позволяющую заполнять граф случайно, запросив у
//          пользователя количество вершин и ребер(данные об инцидентности
//          задаются тоже случайным образом).
//
//  • Функцию, позволяющую по данной матрице инцидентности(данные
//          задаются с консоли) построить матрицу смежности.
//
//  • Функцию добавления ребра. Данные об инцидентных вершинах
//          задаются с консоли.
//
// https://habr.com/ru/post/469967/ : пункт 1
// https://ru.wikipedia.org/wiki/%D0%9C%D0%B0%D1%82%D1%80%D0%B8%D1%86%D0%B0_%D0%B8%D0%BD%D1%86%D0%B8%D0%B4%D0%B5%D0%BD%D1%82%D0%BD%D0%BE%D1%81%D1%82%D0%B8
// https://ru.wikipedia.org/wiki/%D0%9F%D1%83%D1%82%D1%8C_(%D1%82%D0%B5%D0%BE%D1%80%D0%B8%D1%8F_%D0%B3%D1%80%D0%B0%D1%84%D0%BE%D0%B2) 
//

// vertex of graph
typedef struct _Node 
{
  int Id;
} Node;

typedef struct _Edge
{
  int Id;
  Node NodeA; // edge connects two Node : NodeA & NodeB
  Node NodeB;
} Edge;

typedef struct _EdgeList
{
  Edge* Edges;
  size_t Count;
} EdgeList;

typedef struct _EdgeListCollection
{
  EdgeList* EdgeLists;
  size_t Count;
} EdgeListCollection;

typedef struct _IncidenceMatrix
{
  bool* Items; // all items is flatten to 1-dimensional array : arrays follows one by one
  size_t NodesCount;
  size_t EdgesCount;
} IncidenceMatrix;

// items count == NodesCount ^ 2
typedef struct _AdjacencyMatrix
{
  bool* Items;
  size_t NodesCount;
} AdjacencyMatrix;

const int INVALID_ID = -1;
const char FILE_NAME[] = "filename.txt";
const char TRUE_CHAR = '1';
const char FALSE_CHAR = '0';
const char SEPARATOR_CHAR = '\n';
const size_t MAX_MATRIX_SIZE = 30;

// removes single edge from given EdgeList
void RemoveFromEdgeList(EdgeList *list, Edge edgeToRemove)
{
  bool foundRemoved = false;

  for (size_t i = 0; i < list->Count - 1; i++)
  {
    if (list->Edges[i].Id == edgeToRemove.Id)
    {
      foundRemoved = true;
    }

    if (foundRemoved)
    {
      // copy next edge to current one while end of array reached
      list->Edges[i] = list->Edges[i + 1];
    }
  }

  if (list->Edges[list->Count - 1].Id == edgeToRemove.Id)
  {
    foundRemoved = true;
  }

  if (foundRemoved)
  {
    Edge* tmp = (Edge*)realloc(list->Edges, sizeof(Edge) * (list->Count - 1));
    if (tmp)
    {
      list->Edges = tmp;
      list->Count--;
    }
  }
}

// appends newEdge to list with realloc
void ExtendEdgeList(EdgeList *list, Edge newEdge)
{
  if (!list->Edges)
  {
    list->Edges = (Edge*)calloc(1, sizeof(Edge));
  }
  else
  {
    Edge* tmp = (Edge*)realloc(list->Edges, sizeof(Edge) * (list->Count + 1));
    if (tmp)
    {
      list->Edges = tmp;
    }
  }
  list->Edges[list->Count] = newEdge;
  list->Count++;
}

void ExtendEdgesArray(Edge* list, size_t count, Edge newEdge)
{
  Edge* tmp = (Edge*)realloc(list, sizeof(Edge) * (count + 1));
  if (tmp)
  {
    list = tmp;  
    list[count] = newEdge;
  }
}

// clones list of edges by allocating exact copy of original data in heap
EdgeList CloneEdgeList(EdgeList original)
{
  EdgeList result = { .Count = 0, .Edges = NULL, };
  for (size_t i = 0; i < original.Count; i++)
  {
    ExtendEdgeList(&result, original.Edges[i]);
  }
  return result;
}

void ExtendEdgeListCollection(EdgeListCollection *collection, EdgeList newList)
{
  if (!collection->EdgeLists)
  {
    collection->EdgeLists = (EdgeList*)calloc(1, sizeof(EdgeList));
  }
  else
  {
    EdgeList* tmp = (EdgeList*)realloc(collection->EdgeLists, sizeof(EdgeList) * (collection->Count + 1));
    if (tmp)
    {
      collection->EdgeLists = tmp;
    }
  }
  collection->EdgeLists[collection->Count] = newList;
  collection->Count++;
}

// finds all edges that contain targetNode
EdgeList GetEdgesByNode(Node targetNode, EdgeList edgeList)
{
  EdgeList result = (EdgeList){ .Count = 0, .Edges = NULL, };

  if (!edgeList.Edges || edgeList.Count == 0)
  {
    return result;
  }

  for (size_t i = 0; i < edgeList.Count; i++)
  {
    if (edgeList.Edges[i].NodeA.Id == targetNode.Id 
      || edgeList.Edges[i].NodeB.Id == targetNode.Id)
    {
      ExtendEdgeList(&result, edgeList.Edges[i]);
    }
  }
  return result;
}

// recursively finds all pathses of unique edges starting from startNode
void FindAllChainsFromNode(Node startNode, EdgeList left, EdgeList bypassed, EdgeListCollection *chains)
{
  EdgeList outputEdges = GetEdgesByNode(startNode, left);

  if (outputEdges.Count)
  {
    for (size_t i = 0; i < outputEdges.Count; i++)
    {
      Node node;
      // choose next node to follow through
      if (outputEdges.Edges[i].NodeA.Id == startNode.Id)
      {
        node = outputEdges.Edges[i].NodeB;
      }
      else
      {
        node = outputEdges.Edges[i].NodeA;
      }
      EdgeList exteneded = CloneEdgeList(bypassed);
      ExtendEdgeList(&exteneded, outputEdges.Edges[i]);
      EdgeList reduced = CloneEdgeList(left);
      RemoveFromEdgeList(&reduced, outputEdges.Edges[i]);
   
      FindAllChainsFromNode(node, reduced, exteneded, chains);
    }
  }
  else
  {
    ExtendEdgeListCollection(chains, CloneEdgeList(bypassed));
  }

  // release memory
  if (outputEdges.Edges) free(outputEdges.Edges); 
}

void WriteIncidenceMatrixToFile(IncidenceMatrix matrixToWrite)
{
  // output format
  // <DIGIT><DIGIT><DIGIT><\n>
  // <DIGIT><DIGIT><DIGIT><\n>
  // <DIGIT><DIGIT><DIGIT><\n>
  // <DIGIT><DIGIT><DIGIT><\n>
  // <DIGIT><DIGIT><DIGIT><EOF>
  // where DIGIT = {0, 1}
  size_t fullSize = matrixToWrite.NodesCount * (matrixToWrite.EdgesCount + 1);
  char* str = (char*)calloc(fullSize, sizeof(char));

  for (size_t i = 0; i < matrixToWrite.NodesCount; i++)
  {
    for (size_t j = 0; j < matrixToWrite.EdgesCount + 1; j++)
    {
      char next;
      if (matrixToWrite.EdgesCount == j && matrixToWrite.NodesCount - 1 == i)
      {
        next = 0;
      }
      else if (matrixToWrite.EdgesCount == j)
      {
        next = SEPARATOR_CHAR;
      }
      else
      {
        next = matrixToWrite.Items[i * matrixToWrite.EdgesCount + j] ? TRUE_CHAR : FALSE_CHAR;
      }
      str[i * (matrixToWrite.EdgesCount + 1) + j] = next;
    }
  }

  FILE* stream;
  errno_t err = fopen_s(&stream, FILE_NAME, "wt");

  if (err == 0)
  {
    fputs(str, stream);
  }
  else
  {
    puts("The file was not opened");
  }

  if (stream)
  {
    err = fclose(stream);
    if (err != 0)
    {
      puts("The file was not closed");
    }
  }
  free(str);
}

long GetFileSize(FILE* stream)
{
  long pos, length;
  pos = ftell(stream);
  fseek(stream, 0L, SEEK_END);
  length = ftell(stream);
  fseek(stream, pos, SEEK_SET);
  return length;
}

// lengthToCheck < 0 : check while EOF found
// lengthToCheck >= 0 : check first lengthToCheck chars of source
size_t Count(char * source, char c, int lengthToCheck)
{
  size_t count = 0;
  for (size_t i = 0; lengthToCheck < 0 ? source[i] != 0 : lengthToCheck; i++)
  {
    if (source[i] == c)
    {
      count++;
    }
  }
  return count;
}

IncidenceMatrix ReadIncidenceMatrixFromFile()
{
  char *data = NULL;
  FILE *stream;
  errno_t err = fopen_s(&stream, FILE_NAME, "rt");

  if (err == 0)
  {
    data = (char*)calloc(GetFileSize(stream), sizeof(char));
    char currentChar;
    size_t currentCount = 0;
        // count the number of chars in the text
    while ((currentChar = fgetc(stream)) != EOF)
    {
      if (currentChar == TRUE_CHAR || currentChar == FALSE_CHAR || currentChar == SEPARATOR_CHAR)
      {
        data[currentCount] = currentChar;
        currentCount++;
      }
    }       
  }
  else
  {
      puts("The file was not opened");
  }
    
  // close stream
  if (stream)
  {
    err = fclose(stream);
    if (err != 0) 
    { 
      puts("The file was not closed");
    }
  }

  IncidenceMatrix matrix = { .Items = NULL, .NodesCount = 0, .EdgesCount = 0, };
  if (data)
  {
    size_t totalRows = Count(data, SEPARATOR_CHAR, -1) + 1;
    size_t totalItems = Count(data, TRUE_CHAR, -1) + Count(data, FALSE_CHAR, -1);
    if (totalItems % totalRows != 0 || totalRows <= 0 || totalItems <= 0)
    {
      puts("Invalid file data");        
    }
    else
    {
      matrix = (IncidenceMatrix)
      {
        .Items = (bool*)calloc(totalItems, sizeof(bool)),
        .NodesCount = totalRows,
        .EdgesCount = totalItems / totalRows
      };
      for (size_t i = 0, j = 0; data[i] != 0; i++)
      {
        if (data[i] == TRUE_CHAR || data[i] == FALSE_CHAR)
        {
          matrix.Items[j] = data[i] == TRUE_CHAR;
          j++;
        }
      }
    }
  }
  free(data);
  return matrix;
}

void PrintIncidenceMatrix(IncidenceMatrix m)
{
  for (size_t i = 0; i < m.NodesCount * m.EdgesCount; i++)
  {
    if (i % m.EdgesCount == 0)
    {
      puts("");
    }
    putchar(m.Items[i] ? TRUE_CHAR : FALSE_CHAR);
  }
  puts("");
}

IncidenceMatrix CreateRandomIncidenceMatrix(size_t nodesCount, size_t edgesCount)
{
  bool* data = (bool*)calloc(nodesCount * edgesCount, sizeof(bool));

  for (size_t i = 0; i < nodesCount * edgesCount; i++)
  {
    data[i] = false;
  }

  srand((unsigned int)time(NULL));
 
  for (size_t i = 0; i < edgesCount; i++)
  {
    size_t nodeA = (size_t)rand() % nodesCount;
    size_t nodeB = nodeA;
    while (nodeB == nodeA)
    {
      nodeB = (size_t)rand() % nodesCount;
    }

    data[nodeA * edgesCount + i] = true;
    data[nodeB * edgesCount + i] = true;
  }

  return (IncidenceMatrix) { .Items = data, .NodesCount = nodesCount, .EdgesCount = edgesCount };
}

AdjacencyMatrix ConvertToAdjacencyMatrix(IncidenceMatrix matrix)
{
  bool* data = (bool*)calloc(matrix.NodesCount * matrix.NodesCount, sizeof(bool));

  for (size_t i = 0; i < matrix.NodesCount * matrix.NodesCount; i++)
  {
    data[i] = false;
  }

  for (size_t i = 0; i < matrix.EdgesCount; i++)
  {      
    size_t nodeA = INVALID_ID, nodeB = INVALID_ID;

    for (size_t j = 0; j < matrix.NodesCount; j++)
    {
      if (matrix.Items[j * matrix.EdgesCount + i])
      {
        if (nodeA == INVALID_ID)
        {
          nodeA = j;
        }
        else
        {
          nodeB = j;
          break;
        }
      }
    }    
    data[nodeA * matrix.NodesCount + nodeB] = true;
    data[nodeB * matrix.NodesCount + nodeA] = true;
  }
  
  return (AdjacencyMatrix) { .NodesCount = matrix.NodesCount, .Items = data, };
}

void PrintAdjacencyMatrix(AdjacencyMatrix m)
{
  for (size_t i = 0; i < m.NodesCount * m.NodesCount; i++)
  {
    if (i % m.NodesCount == 0)
    {
      puts("");
    }
    putchar(m.Items[i] ? (int)TRUE_CHAR : (int)FALSE_CHAR);
  }
  puts("");
}

void PrintChains(EdgeListCollection collection) 
{
  for (size_t i = 0; i < collection.Count; i++)
  {
    for (size_t j = 0; j < collection.EdgeLists[i].Count; j++)
    {
      printf_s("e%i ", collection.EdgeLists[i].Edges[j].Id);
    }
    puts("");
  }  
  puts("");
}

void PrintEdges(Edge * edges, size_t count)
{
  for (size_t i = 0; i < count; i++)
  {
    printf_s("%i %i %i\n", edges[i].Id, edges[i].NodeA.Id, edges[i].NodeB.Id);
  }
}

void InitializeNodes(Node * nodes, size_t count) 
{
  for (size_t i = 0; i < count; i++)
  {
    nodes[i] = (Node) { .Id = i };
  }
}

void InitializeEdges(Edge * edges, Node * nodes, IncidenceMatrix matrix)
{
  for (size_t i = 0; i < matrix.EdgesCount; i++)
  {
    edges[i] = (Edge){ .Id = i, .NodeA = INVALID_ID, .NodeB = INVALID_ID, };
    for (size_t j = 0; j < matrix.NodesCount; j++)
    {
      if (matrix.Items[j * matrix.EdgesCount + i])
      {
        if (edges[i].NodeA.Id == INVALID_ID)
        {
          edges[i].NodeA = nodes[j];
        }
        else
        {
          edges[i].NodeB = nodes[j];
        }
      }
    }
  }
}

void FreeEdgeLists(EdgeList * edgeLists, size_t count)
{
  if (edgeLists)
  {
    for (size_t i = 0; i < count; i++)
    {
      if (edgeLists[i].Edges) free(edgeLists[i].Edges);
    }
    free(edgeLists);
  }
}

bool ModifyMatrix(IncidenceMatrix * matrix, size_t newEdgeNodeA, size_t newEdgeNodeB)
{
  if (newEdgeNodeA >= matrix->NodesCount || newEdgeNodeB >= matrix->NodesCount)
  {
    return false;
  }  

  bool* oldData = matrix->Items;
  bool* newData = (bool *)calloc((matrix->EdgesCount + 1) * matrix->NodesCount, sizeof(bool));

  for (size_t i = 0; i < matrix->NodesCount; i++)
  {
    for (size_t j = 0; j < matrix->EdgesCount; j++)
    {
      newData[i * (matrix->EdgesCount + 1) + j] = oldData[i * matrix->EdgesCount + j];
      if (j == matrix->EdgesCount - 1)
      {
        newData[i * (matrix->EdgesCount + 1) + j + 1] = i == newEdgeNodeA || i == newEdgeNodeB;        
      }
    }
  }
  matrix->EdgesCount++;
  matrix->Items = newData;
  free(oldData);

  return true;
}

int main()
{
  system("chcp 1251");
  system("cls");

  IncidenceMatrix matrix = { .Items = NULL, .NodesCount = 0, .EdgesCount = 0, };
  Node* nodes = NULL;
  Edge* edges = NULL;

  while (true)
  {
    size_t feature = 0;

    puts("\n\nENTER FEATURE NUMBER:\n"
      "0 FIND CHAINS\n"
      "1 WRITE TO FILE\n"
      "2 READ FROM FILE\n"
      "3 GENERATE RANDOM GRAPH\n"
      "4 GET ADJACENCY MATRIX\n"
      "5 ADD EDGE"
    );    
    scanf_s("%u", &feature);

    switch (feature)
    {
    // FIND CHAINS
    case 0:
    {
      if (nodes && edges && matrix.Items)
      {
        size_t id = 0;
        puts("Enter node id to search");
        scanf_s("%u", &id);
        if (id >= matrix.NodesCount)
        {
          puts("No node of entered id");
        }
        else
        {
          EdgeListCollection collection = { .Count = 0, .EdgeLists = NULL, };
          FindAllChainsFromNode(
            nodes[id],
            (EdgeList) { .Count = matrix.EdgesCount, .Edges = edges, },
            (EdgeList) { .Count = 0, .Edges = NULL, },
            &collection);
          PrintChains(collection);
          FreeEdgeLists(collection.EdgeLists, collection.Count);
        }
      }
      else
      {
        puts("No entered graph");
      }
      break;
    }
    // WRITE TO FILE
    case 1:
    {
      if (nodes && edges && matrix.Items)
      {
        WriteIncidenceMatrixToFile(matrix);
        puts("Ready");
      }
      else
      {
        puts("No entered graph");
      }
      break;
    }
    // READ FROM FILE
    case 2:
    {     
      if (matrix.Items) free(matrix.Items);
      matrix = ReadIncidenceMatrixFromFile();

      if (!matrix.Items)
      {
        puts("Invalid file");
      }
      else
      {
        if (nodes) free(nodes);
        if (edges) free(edges);
        nodes = (Node*)calloc(matrix.NodesCount, sizeof(Node));
        edges = (Edge*)calloc(matrix.EdgesCount, sizeof(Edge));
        InitializeNodes(nodes, matrix.NodesCount);
        InitializeEdges(edges, nodes, matrix);
        PrintIncidenceMatrix(matrix);
      }
      break;
    }
    // GENERATE RANDOM GRAPH
    case 3:
    {
      size_t edgesCount = 0, nodesCount = 0;
      puts("Enter edges count");
      scanf_s("%u", &edgesCount);
      puts("Enter nodes count");
      scanf_s("%u", &nodesCount);
      if (edgesCount <= MAX_MATRIX_SIZE && edgesCount > 0 && nodesCount <= MAX_MATRIX_SIZE && nodesCount > 0)
      {   
        if (matrix.Items) free(matrix.Items);
        matrix = CreateRandomIncidenceMatrix(nodesCount, edgesCount);

        if (!matrix.Items)
        {
          printf("Invalid input");        
        }
        else
        {  
          if (nodes) free(nodes);
          if (edges) free(edges);
          nodes = (Node*)calloc(matrix.NodesCount, sizeof(Node));
          edges = (Edge*)calloc(matrix.EdgesCount, sizeof(Edge));
          InitializeNodes(nodes, matrix.NodesCount);
          InitializeEdges(edges, nodes, matrix);   
          PrintIncidenceMatrix(matrix);
        }
      }
      else
      {
        printf("Input out of acceptable range [1, 30]");
      }

      break;
    }
    // GET ADJACENCY MATRIX
    case 4:
    {
      if (nodes && edges && matrix.Items)
      {
        AdjacencyMatrix a = ConvertToAdjacencyMatrix(matrix);
        PrintAdjacencyMatrix(a);
        if (a.Items)
        {
          free(a.Items);
        }
      }
      else
      {
        puts("No entered graph");
      }
      break;
    }
    // ADD EDGE
    case 5:
    { 
      if (nodes && edges && matrix.Items)
      {
        size_t nodeA , nodeB ;
        puts("Enter node");
        scanf_s("%u", &nodeA);
        puts("Enter another node");
        scanf_s("%u", &nodeB);

        if (ModifyMatrix(&matrix, nodeA, nodeB))
        {
          if (nodes) free(nodes);
          if (edges) free(edges);
          nodes = (Node*)calloc(matrix.NodesCount, sizeof(Node));
          edges = (Edge*)calloc(matrix.EdgesCount, sizeof(Edge));
          InitializeNodes(nodes, matrix.NodesCount);
          InitializeEdges(edges, nodes, matrix);
          PrintIncidenceMatrix(matrix);
        }
        else
        {
          puts("Invalid input");
        }
      }
      else
      {
        puts("No entered graph");
      }
      break;
    }
    default:
      puts("Enter valid number");
      break;
    } 
  }

  puts("PRESS ANY KEY..");
  _getch();
  return 0;
}

