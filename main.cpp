#include <numeric>
#include <map>
#include <tuple>
#include <iostream>
#include <sstream>

template <typename T, auto U
, typename std::enable_if_t<std::is_same_v<T, decltype(U)>, bool> = true
>
/// MatrixRow
/// Type T declares the type of data matrix can contain.
/// Type U declares the default (basic) value for all matrix elements.
struct MatrixRow
{
public:
    MatrixRow() :
    _default_value(U)
    {
    }
    
    MatrixRow(T defaultValue) :
    _default_value(defaultValue)
    {
    }
    
    size_t size()
    {
        auto count_accumulator = [_default_value = _default_value](size_t partial_result, std::pair<const size_t, T> current_element)
        {
            return partial_result + (current_element.second != _default_value ? 1 : 0);
        };
        size_t accumulated_size = std::accumulate(_container.begin(), _container.end(), 0, count_accumulator);
        return accumulated_size;
    }
    
    T& operator[](size_t element_index)
    {
        if (_container.count(element_index) > 0)
            return _container[element_index];
        _container[element_index] = _default_value;
        return _container[element_index];
    }
    
    operator std::tuple<size_t&, T&>() const {
        return std::make_tuple(_container);
    }
    
private:
    T _default_value;
    std::map<size_t, T> _container;
};

template <typename T, auto U
, typename std::enable_if_t<std::is_same_v<T, decltype(U)>, bool> = true
>
/// Matrix
/// Type T declares the type of data matrix can contain.
/// Type U declares the default (basic) value for all matrix elements.
struct Matrix
{
public:
    Matrix()
    {
    }
    
    size_t size()
    {
        auto count_accumulator = [](size_t partial_result, std::pair<const size_t, MatrixRow<T, U>> current_element)
        {
            return partial_result + current_element.second.size();
        };
        size_t accumulated_size = std::accumulate(_matrix.begin(), _matrix.end(), 0, count_accumulator);
        return accumulated_size;
    }
    
    MatrixRow<T, U>& operator[](size_t element_index)
    {
        return _matrix[element_index];
    }
    
    typename std::map<size_t, MatrixRow<T, U>>::iterator begin() {
        return _matrix.begin();
    }

    typename std::map<size_t, MatrixRow<T, U>>::const_iterator begin() const {
        return _matrix.begin();
    }

    typename std::map<size_t, MatrixRow<T, U>>::const_iterator cbegin() const {
        return begin();
    }

    typename std::map<size_t, MatrixRow<T, U>>::iterator end() {
        return _matrix.end();
    }

    typename std::map<size_t, MatrixRow<T, U>>::const_iterator end() const {
        return _matrix.end();
    }

    typename std::map<size_t, MatrixRow<T, U>>::const_iterator cend() const {
        return end();
    }
    
private:
    std::map<size_t, MatrixRow<T, U>> _matrix;
};

void predefined_task()
{
    // бесконечная матрица int заполнена значениями -1
    Matrix<int, -1> matrix;
    assert(matrix.size() == 0); // все ячейки свободны
    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);
    // выведется одна строка
    // 100100314
    for(auto c: matrix)
    {
        // std::remove_const<decltype(c.first)> a;
        decltype(c.second) b;
        std::tie(std::ignore, b) = c;
        
        MatrixRow<int, -1> row;
        std::tie(row) = b;
        // std::cout << x << y << v << std::endl;
    }
}

void homework_task()
{
    Matrix<int, 0> matrix;
    size_t full_matrix_size = 9;
    for (size_t i = 0; i<=full_matrix_size; ++i)
    {
        matrix[i][i] = (int)i;
        matrix[i][full_matrix_size - i] = int(full_matrix_size - i);
    }
    
    size_t offset_matrix_size_begin = 1;
    size_t offset_matrix_size_end = 8;
    
    for (size_t i = offset_matrix_size_begin; i<=offset_matrix_size_end; ++i)
    {
        std::ostringstream output;
        for (size_t j = offset_matrix_size_begin; j<=offset_matrix_size_end; ++j)
        {
            output << matrix[i][j];
            if (j != offset_matrix_size_end)
            {
                output << " ";
            }
        }
        std::cout << output.str() << std::endl;
    }
}

int main()
{
    predefined_task();
    homework_task();
    return 0;
}
