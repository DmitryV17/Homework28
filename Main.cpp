// Взял реализацию алгоритма сортировки не из 11 модуля 
// потому что тот код у меня не компилируется

#include <future>
#include <iostream>

using namespace std;

bool make_thread = true; // Флаг для подключения многопоточной работы функции

// Объединяет два подмассива массива[]..
// Первый подмассив - это arr[begin..mid]
// Второй подмассив - это arr[mid+1..end]
void merge(int array[], int const left, int const mid,
    int const right)
{
    int const subArrayOne = mid - left + 1;
    int const subArrayTwo = right - mid;

    // Создание временных массивов
    auto* leftArray = new int[subArrayOne],
        * rightArray = new int[subArrayTwo];

    // Копирование данных во временные массивы leftArray[] и rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;

    // Объедините временных массивов обратно в массив  array[left..right]
    while (indexOfSubArrayOne < subArrayOne
        && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne]
            <= rightArray[indexOfSubArrayTwo]) {
            array[indexOfMergedArray]
                = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            array[indexOfMergedArray]
                = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    // Копируем оставшиеся элементы в
    // left[] если есть
    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray]
            = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    // Копируем оставшиеся элементы в
    // right[] если есть
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray]
            = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
    delete[] leftArray;
    delete[] rightArray;
}

// начало для левого индекса и конец правого индекса
// подмассива arr для сортировки
void mergeSort(int array[], int const begin, int const end)
{
    if (begin >= end)
        return;

    int mid = begin + (end - begin) / 2;
    
    if (make_thread) // Запуск второго параллельного потока 
    {
        // если стоит флаг то запускаем второй поток
        auto f = async(launch::async, [&]() {
            mergeSort(array, begin, mid);;
            });
        mergeSort(array, mid + 1, end);
        merge(array, begin, mid, end);
    }
    else {
        // Запускаем в один поток 
        mergeSort(array, begin, mid);
        mergeSort(array, mid + 1, end);
        merge(array, begin, mid, end);
    }
      
}

// Функция для печати массива
void printArray(int A[], int size)
{
    for (int i = 0; i < size; i++)
        cout << A[i] << " ";
    cout << endl;
}


int main()
{
    // многопоточный запуск
    int arr[] = { 12, 11, 13, 5, 6, 7  };
    int arr_size = sizeof(arr) / sizeof(arr[0]);

    cout << "Running with two threads \n";
    cout << "Given array is: \n";
    printArray(arr, arr_size);
           
    mergeSort(arr, 0, arr_size - 1);
  
    cout << "\nSorted array is: \n";
    printArray(arr, arr_size);
    cout << endl;

    // однопоточный запуск
    make_thread = false;
    int arr2[] = { 144, 1234, 44, 5, 2, 0 };
    int arr_size2 = sizeof(arr2) / sizeof(arr2[0]);

    cout << "Single thread launch \n";
    cout << "Given array is: \n";
    printArray(arr2, arr_size2);
        
    mergeSort(arr2, 0, arr_size2 - 1);
    
    cout << "\nSorted array is: \n";
    printArray(arr2, arr_size2);

    return 0;
}
