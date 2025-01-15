#include <iostream>
#include <time.h>

int** CreateMatrix(int rows, int columns);
void RandomCells(int** matrix, int rows, int columns);
void UserChosenCells(int** matrix, int rows, int columns);
void VisualizeGenerations(int** matrix, int** cellsLives, int** cellsRemainingTime, int rows, int columns, int n);
void SetCellsLives(int** matrix, int** cellsLives, int rows, int columns);
void UpdateRemainingTime(int** matrix, int** cellsRemainingTime, int rows, int columns);
void UpdateMatrix(int** currentMatrix, int** nextMatrix, int rows, int columns);
void DeleteMatrix(int** matrix, int rows);
int CalculateLives(int** matrix, int r, int c, int rows, int columns);
int CalculateSumOfNeighbours(int** matrix, int r, int c, int rows, int columns);
void PrintGeneration(int** matrix, int rows, int columns);

int main()
{
    int rows, columns;

    std::cout << "Select board dimensions (10-30)" << std::endl;

    std::cout << "Rows: ";
    std::cin >> rows;

    std::cout << "Columns: ";
    std::cin >> columns;

    while (rows < 10 || rows > 30 || columns < 10 || columns > 30)
    {
        std::cout << "Invalid dimension size. Select board dimensions (10-30)" << std::endl;

        std::cout << "Rows: ";
        std::cin >> rows;

        std::cout << "Columns: ";
        std::cin >> columns;
    }
    
    int** matrix = CreateMatrix(rows, columns);
    int** cellsLives = CreateMatrix(rows, columns);
    int** cellsRemainingTime = CreateMatrix(rows, columns);

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            if (matrix[r][c] == 1)
            {
                cellsRemainingTime[r][c] = 6;
            }
            else cellsRemainingTime[r][c] = 0;
        }
    }

    RandomCells(matrix, rows, columns);
    UserChosenCells(matrix, rows, columns);

    SetCellsLives(matrix, cellsLives, rows, columns);
    
    std::cout << "How many generations do you want to see: ";
    
    int n;
    std::cin >> n;

    while (n < 0)
    {
        std::cout << "Number must be 0 or higher." << std::endl;
        std::cout << "How many generations do you want to see: ";
        std::cin >> n;
    }

    if (n > 0)
    {
        VisualizeGenerations(matrix, cellsLives, cellsRemainingTime, rows, columns, n);
    }

    DeleteMatrix(matrix, rows);

    return 0;
}

int** CreateMatrix(int rows, int columns)
{
    int** matrix = new int*[rows];

    for (int i = 0; i < rows; i++)
    {
        matrix[i] = new int[columns];
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            matrix[i][j] = 0;
        }
    }

    return matrix;
}

void RandomCells(int** matrix, int rows, int columns)
{
    int min = 0;
    int max = rows * columns;

    srand(time(NULL));

    int randomCellsAmount = min + rand() % (max - min + 1);

    int Xmin = 0, Xmax = rows - 1;
    int Ymin = 0, Ymax = columns - 1;

    for (int i = 0; i < randomCellsAmount; i++)
    {
        int x = Xmin + rand() % (Xmax - Xmin + 1);
        int y = Ymin + rand() % (Ymax - Ymin + 1);
        matrix[x][y] = 1;
    }
}

void UserChosenCells(int** matrix, int rows, int columns)
{
    std::cout << "How many alive cells do you want to manually enter: ";

    int n;
    std::cin >> n;

    while (n < 0)
    {
        std::cout << "Number must be 0 or higher." << std::endl;
        std::cout << "How many alive cells do you want to manually enter: ";
        std::cin >> n;
    }

    if (n == 0) return;

    int x, y;
    int Xmin = 0, Xmax = rows - 1;
    int Ymin = 0, Ymax = columns - 1;

    std::cout << "Board dimensions - " << rows << "x" << columns << ". Enter coordinates of cells below." << std::endl;

    for (int i = 0; i < n; i++)
    {
        std::cout << "Enter x" << i + 1 << ": ";
        std::cin >> x;
        std::cout << "Enter y" << i + 1 << ": ";
        std::cin >> y;

        while (x < Xmin || x > Xmax || y < Ymin || y > Ymax)
        {
            std::cout << "Invalid input! X and Y are out of bound. Board dimensions - " << rows << "x" << columns << std::endl;

            std::cout << "Enter x" << i + 1 << ": ";
            std::cin >> x;
            std::cout << "Enter y" << i + 1 << ": ";
            std::cin >> y;
        }

        matrix[x][y] = 1;
    }
}

void VisualizeGenerations(int** currentMatrix, int** cellsLives, int** cellsRemainingTime, int rows, int columns, int n)
{
    int** nextMatrix = CreateMatrix(rows, columns);
    
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            nextMatrix[r][c] = currentMatrix[r][c];
        }
    }

    for (int i = 0; i < n; i++)
    {
        PrintGeneration(currentMatrix, rows, columns);
        std::cout << std::endl;

        if (i + 1 == n) break;

        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < columns; c++)
            {
                int sumOfNeighbours = CalculateSumOfNeighbours(currentMatrix, r, c, rows, columns);

                if (currentMatrix[r][c] == 1 && (sumOfNeighbours < 2 || sumOfNeighbours > 3))
                {
                    if (cellsLives[r][c] > 1)
                    {
                        cellsLives[r][c]--;
                    }
                    else nextMatrix[r][c] = 0;
                }
                
                else if (currentMatrix[r][c] == 0 && sumOfNeighbours == 3)
                {
                    nextMatrix[r][c] = 1;
                }
            }
        }

        UpdateMatrix(currentMatrix, nextMatrix, rows, columns);
        UpdateRemainingTime(currentMatrix, cellsRemainingTime, rows, columns);
    }

    DeleteMatrix(nextMatrix, rows);
}

void SetCellsLives(int** matrix, int** cellsLives, int rows, int columns)
{
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            int lives = CalculateLives(matrix, r, c, rows, columns);
            cellsLives[r][c] = lives;
        }
    }
}

void UpdateRemainingTime(int** matrix, int** cellsRemainingTime, int rows, int columns)
{
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            if (matrix[r][c] == 0)
            {
                cellsRemainingTime[r][c] = 0;
            }
            if (cellsRemainingTime[r][c] == 1)
            {
                cellsRemainingTime[r][c]--;
                matrix[r][c] = 0;
            }
            else if (cellsRemainingTime[r][c] == 0 && matrix[r][c] == 1)
            {
                cellsRemainingTime[r][c] = 6;
            }
            else cellsRemainingTime[r][c]--;
        }
    }
}

void UpdateMatrix(int** currentMatrix, int** nextMatrix, int rows, int columns)
{
    for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < columns; c++)
            {
                currentMatrix[r][c] = nextMatrix[r][c];
            }
        }
}

void DeleteMatrix(int** matrix, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}

int CalculateLives(int** matrix, int r, int c, int rows, int columns)
{
    int sum = 0;

    for (int i = 0; i < rows; i++)
    {
        if (matrix[i][c] == 1)
        {
            sum++;
        }
    }

    for (int i = 0; i < columns; i++)
    {
        if (matrix[r][i])
        {
            sum++;
        }
    }
    
    return sum - 2;
}

int CalculateSumOfNeighbours(int** matrix, int r, int c, int rows, int columns)
{
    int sum = 0;

    for (int x = r - 1; x <= r + 1; x++)
    {
        for (int y = c - 1; y <= c + 1; y++)
        {
            if (x == r && y == c)
            {
                continue;
            }
            if (x >= 0 && x < rows && y >= 0 && y < columns)
            {
                sum += matrix[x][y];
            }
        }
    }

    return sum;
}

void PrintGeneration(int** matrix, int rows, int columns)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            std::cout << matrix[i][j];
        }
        std::cout << std::endl;
    }
}