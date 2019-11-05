/*
MADE 2019 C++ seminar task 04

Нужно написать класс-матрицу, тип элементов int.

В конструкторе задается количество рядов и строк. Поддерживаются оперции: 
- получить количество строк(rows)/столбцов(columns);
- получить конкретный элемент;
- умножить на число(*=);
- сравнение на равенство/неравенство. 

В случае ошибки выхода за границы бросать исключение: throw std::out_of_range("")
*/


#include <iostream>
#include <iomanip>

using namespace std;
    
class Matrix
{
    public:
    class Array1D 
    {
        private:
            size_t dim2;
            int* Array1;
    
        public:
            friend class Matrix;
            
            Array1D()
            : dim2(0), Array1(NULL) {}
            Array1D(size_t d2);
            
            ~Array1D() { delete[] Array1;}
            
            int& operator[](size_t index);
            const int& operator[] (size_t index) const;
            void setDimension(size_t d2);
    }; // a proxy class Array1D
    
    private:
        size_t dim1;
        Array1D* ArrayMatrix;
    public:
        Matrix()
        : dim1(0), ArrayMatrix(NULL) {};
        Matrix(size_t d1, size_t d2);
        
        ~Matrix() { delete[] ArrayMatrix; }
        Array1D& operator[] (size_t index);
        const Array1D& operator[] (size_t index) const;
        
        bool operator==(const Matrix& rhs);
        bool operator!=(const Matrix& rhs) 
        { 
            return !(*this == rhs); 
        }
        
        Matrix& operator *= (int multiplier);
        
        void printMatrix();
        void putValues();
        size_t getNumberOfRows();
        size_t getNumberofCols();
}; // the main class Matrix

Matrix::Array1D::Array1D(size_t d2)
{
    dim2 = d2;
    Array1 = new int [d2];
}

void Matrix::Array1D::setDimension(size_t d2)
{
    dim2 = d2;
    delete [] Array1;
    Array1 = new int [d2];
}

Matrix::Matrix(size_t d1, size_t d2)
{
    dim1 = d1;
    ArrayMatrix = new Array1D [dim1]();
    for (size_t i = 0; i < d1; ++i)
        ArrayMatrix[i].setDimension(d2);
}

Matrix::Array1D& Matrix::operator[] (size_t index)
{
    if (index < dim1 and index >= 0)
        return ArrayMatrix[index];
    throw invalid_argument("Index 1 is out of range!");
}

const Matrix::Array1D& Matrix::operator[] (size_t index) const
{ 
    if (index < dim1 and index >= 0)
        return ArrayMatrix[index];
    throw invalid_argument("Index 1 is out of range!");
}

int& Matrix::Array1D::operator[](size_t index)
{
    if (index < dim2 and index >= 0)
        return Array1[index];
    else
        throw invalid_argument("Index 2 is out of range!");
}

const int& Matrix::Array1D::operator[](size_t index) const
{
    if (index < dim2 and index >= 0)
        return Array1[index];
    else
        throw invalid_argument("Index 2 is out of range!");
}

bool Matrix::operator==(const Matrix& rhs)
{
    if (dim1 == rhs.dim1 and ArrayMatrix[0].dim2 == rhs.ArrayMatrix[0].dim2)
    {
        for(size_t i = 0; i < dim1; ++i)
        {
            for(size_t j = 0; j < ArrayMatrix[i].dim2; ++j)
            {
                if (ArrayMatrix[i][j] != rhs.ArrayMatrix[i][j])
                    return false;
            }
        }
    }
    else
        return false;

    return true;
}


Matrix& Matrix::operator *= (int multiplier)
{
    int currentValue;
    for(size_t i = 0; i < dim1; ++i)
    {
        for(size_t j = 0; j < ArrayMatrix[i].dim2; ++j)
        {
            currentValue = ArrayMatrix[i][j];
            currentValue *= multiplier;
            ArrayMatrix[i][j] = currentValue;
        }
    }
    return *this;
}


void Matrix::printMatrix()
{
    for(size_t i = 0; i < dim1; ++i)
    {
        cout << "| ";
        for(size_t j = 0; j < ArrayMatrix[i].dim2; ++j)
            cout << ArrayMatrix[i][j] << " ";
        cout << "|" << endl;
    }
}

void Matrix::putValues()
{
    int x;
    for(size_t i = 0; i < dim1; ++i)
    {
        for(size_t j = 0; j < ArrayMatrix[i].dim2; ++j)
        {
            cout << "Enter element "<< i << "x" << j <<": ";
            cin >> x;
            ArrayMatrix[i][j]=x;
        }
    }
    cout << endl;
}

size_t Matrix::getNumberOfRows()
{
    return dim1;
}

size_t Matrix::getNumberofCols()
{
    return ArrayMatrix[0].dim2;
}

int main()
{   
    size_t n(0), m(0);

    cout << "Creating the 1st matrix" << endl;
    cout << "Enter matrix dims n и m" << endl;
    
    while(n < 1 or m < 1)
    {
        cin >> n >> m;
        if(n < 1 or m < 1)
            cout << "Both dims must be non-zero positive integers" << endl; 
    }
        

    Matrix mtrx(n,m);

    mtrx.putValues();

    cout << "Printing the matrix: " << endl;
    mtrx.printMatrix();

    cout << "Dimensions of the matrix are: " << mtrx.getNumberOfRows() << "x" << mtrx.getNumberofCols() << endl;
    
    cout << "Trying to access the elememts at " << mtrx.getNumberOfRows()-1 << "x" << mtrx.getNumberofCols()-1 << ": ";
    cout << mtrx[mtrx.getNumberOfRows()-1][mtrx.getNumberofCols()-1] << endl;

    cout << "Enter an integer to replace the 0x0 element of the matrix: ";
    cin >> mtrx[0][0];

    cout << "Printing the updated matrix: " << endl;
    mtrx.printMatrix();

    int mlpl;
    cout << "Enter an integer to multiply the matrix: " << endl;
    cin >> mlpl;

    mtrx *= mlpl;

    cout << "Printing the updated matrix: " << endl;
    mtrx.printMatrix();

    cout << "Enter the 2nd matrix elements" << endl;
    Matrix mtrx2(n, m);
    mtrx2.putValues();
    
    cout << "Printing the 2nd matrix: " << endl;
    mtrx2.printMatrix();

    cout << "Checking if the matrices are equal..." << endl;
    cout << "The matrices are equal: " << std::boolalpha << (mtrx==mtrx2) << endl;
    cout << "The matrices are NOT equal: " << std::boolalpha << (mtrx!=mtrx2) << endl;
    return 0;
}
