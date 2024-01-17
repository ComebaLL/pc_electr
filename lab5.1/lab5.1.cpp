#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;

const int PAGE_SIZE = 4;           // Количество элементов в странице
const int MAIN_MEMORY_SIZE = 10;   // Размер основной памяти (количество страниц)
const int CACHE_SIZE = 10;          // Размер кэша (количество строк)

// Структура для представления строки в памяти
struct MemoryRow {
    int elements[PAGE_SIZE];
};

// Тип для представления кэша
typedef vector<MemoryRow> Cache;

// Функция для генерации случайного четырехзначного числа
int generateRandomNumber() {
    return rand() % 9000 + 1000;
}

// Функция для создания текстового файла с основной памятью
void createMainMemoryFile(const string& fileName) {
    ofstream file(fileName);

    if (!file.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        return;
    }

    for (int i = 0; i < MAIN_MEMORY_SIZE; ++i) {
        for (int j = 0; j < PAGE_SIZE; ++j) {
            file << generateRandomNumber() << " ";
        }
        file << endl;
    }

    file.close();
    cout << "Main memory file created successfully: " << fileName << endl;
}

// Функция для чтения основной памяти из текстового файла
void readMainMemoryFromFile(const string& fileName, vector<MemoryRow>& mainMemory) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        return;
    }

    for (int i = 0; i < MAIN_MEMORY_SIZE; ++i) {
        for (int j = 0; j < PAGE_SIZE; ++j) {
            file >> mainMemory[i].elements[j];
        }
    }

    file.close();
}

// Функция для записи основной памяти в текстовый файл
void writeMainMemoryToFile(const string& fileName, const vector<MemoryRow>& mainMemory) {
    ofstream file(fileName);

    if (!file.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        return;
    }

    for (int i = 0; i < MAIN_MEMORY_SIZE; ++i) {
        for (int j = 0; j < PAGE_SIZE; ++j) {
            file << mainMemory[i].elements[j] << " ";
        }
        file << endl;
    }

    file.close();
}

// Функция для вывода строки памяти на экран
void printMemoryRow(const MemoryRow& row) {
    for (int i = 0; i < PAGE_SIZE; ++i) {
        cout << row.elements[i] << " ";
    }
    cout << endl;
}

// Функция для вывода кэша на экран
void printCache(const Cache& cache) {
    for (const auto& row : cache) {
        for (int element : row.elements) {
            cout << element << " ";
        }
        cout << endl;
    }
}

// Функция для поиска элемента в кэше
bool searchCache(const Cache& cache, int address, MemoryRow& resultRow) {
    int rowIndex = address / PAGE_SIZE;
    if (rowIndex < 0 || rowIndex >= CACHE_SIZE) {
        return false;
    }

    int offset = address % PAGE_SIZE;
    resultRow = cache[rowIndex];

    cout << "Element at address " << address << ": " << resultRow.elements[offset] << " (loaded from Cache)" << endl;
    // Вывод всей строки, где был найден элемент
    cout << "Row at address " << address << ": ";
    printMemoryRow(resultRow);

    return true;
}

// Функция для поиска элемента в основной памяти
bool searchMainMemory(const vector<MemoryRow>& mainMemory, int address, MemoryRow& resultRow) {
    int rowIndex = address / PAGE_SIZE;
    if (rowIndex < 0 || rowIndex >= MAIN_MEMORY_SIZE) {
        return false;
    }

    int offset = address % PAGE_SIZE;
    resultRow = mainMemory[rowIndex];

    cout << "Element at address " << address << ": " << resultRow.elements[offset] << " (loaded from Main Memory)" << endl;
    // Вывод всей строки, где был найден элемент
    cout << "Row at address " << address << ": ";
    printMemoryRow(resultRow);

    return true;
}

// Функция для вывода элемента по заданному адресу
void printElementAtAddress(const vector<MemoryRow>& mainMemory, int address) {
    int rowIndex = address / PAGE_SIZE;
    int offset = address % PAGE_SIZE;
    cout << "Element at address " << address << ": " << mainMemory[rowIndex].elements[offset] << endl;
}

// Функция для вывода всей строки, где был найден элемент
void printRowContainingElement(const vector<MemoryRow>& mainMemory, int address) {
    int rowIndex = address / PAGE_SIZE;
    cout << "Row at address " << address << ": ";
    printMemoryRow(mainMemory[rowIndex]);
}

// Функция для обновления кэша и основной памяти по принципу обратной записи
void updateCacheAndMemory(Cache& cache, vector<MemoryRow>& mainMemory, int address, int newValue) {
    int rowIndex = address / PAGE_SIZE;
    int offset = address % PAGE_SIZE;

    // Обновляем кэш
    cache[rowIndex].elements[offset] = newValue;

    // Обновляем основную память
    mainMemory[rowIndex].elements[offset] = newValue;

    // Вывод обновленного значения
    cout << "Value at address " << address << " updated: " << newValue << endl;

    // Вывод всей строки, где был найден элемент
    cout << "Updated row at address " << address << ": ";
    printMemoryRow(mainMemory[rowIndex]);

    // Записываем изменения в файл основной памяти
    writeMainMemoryToFile("main_memory.txt", mainMemory);
}

int main() {
    srand(time(0));

    // Создание текстового файла с основной памятью (если его нет)
    string mainMemoryFileName = "main_memory.txt";
    ifstream mainMemoryFile(mainMemoryFileName);
    if (!mainMemoryFile) {
        mainMemoryFile.close();
        createMainMemoryFile(mainMemoryFileName);
    }
    else {
        mainMemoryFile.close();
    }

    // Чтение данных из текстового файла в основную память
    vector<MemoryRow> mainMemory(MAIN_MEMORY_SIZE);
    readMainMemoryFromFile(mainMemoryFileName, mainMemory);

    // Инициализация кэша
    Cache cache(CACHE_SIZE);
    for (int i = 0; i < CACHE_SIZE; ++i) {
        cache[i] = mainMemory[i];
    }

    while (true) {
        int choice;
        cout << "Choose an option:" << endl;
        cout << "1. Read from memory" << endl;
        cout << "2. Write to memory" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";

        // Добавлена проверка на ввод числового значения
        string choiceInput;
        cin >> choiceInput;

        if (isdigit(choiceInput[0])) {
            // Преобразуем строку в число
            choice = atoi(choiceInput.c_str());
        }
        else {
            // Если введено не число, завершаем программу
            cout << "Invalid input. Exiting program." << endl;
            break;
        }

        if (choice == 3) {
            break;
        }

        int address;
        cout << "Enter the memory address (0-99): ";

        // Добавлена проверка на ввод числового значения
        string addressInput;
        cin >> addressInput;

        if (isdigit(addressInput[0])) {
            // Преобразуем строку в число
            address = atoi(addressInput.c_str());
        }
        else {
            // Если введено не число, завершаем программу
            cout << "Invalid input. Exiting program." << endl;
            break;
        }

        if (address < 0 || address >= MAIN_MEMORY_SIZE * PAGE_SIZE) {
            cout << "Invalid memory address. Please enter a valid address." << endl;
            continue;
        }

        if (choice == 1) {
            // Чтение из памяти
            MemoryRow resultRow;
            if (searchCache(cache, address, resultRow)) {
                // Элемент найден в кэше
            }
            else {
                // Элемент не найден в кэше, поиск в основной памяти
                searchMainMemory(mainMemory, address, resultRow);

                // Копирование строки из основной памяти в кэш
                cache[address / PAGE_SIZE] = resultRow;
            }
        }
        else if (choice == 2) {
            // Запись в память
            int newValue;
            cout << "Enter the new value: ";
            cin >> newValue;

            updateCacheAndMemory(cache, mainMemory, address, newValue);

            cout << "Value updated successfully." << endl;
        }
    }

    return 0;
}
