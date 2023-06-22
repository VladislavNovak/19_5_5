#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream> // stringstream
#include <string>
#include <vector>
#include <cstdlib> // rand, srand
#include <ctime> // time
#include <algorithm> // find

using std::string;
using std::vector;

bool isIncludes(const char &item, const std::string &range) {
    return std::any_of(range.begin(),
                       range.end(),
                       [&item](const char &c) { return c == item; });
}

std::string getJoinRange(const std::string &range) {
    std::string joinRange;
    for (int i = 0; i < range.size(); ++i) {
        joinRange += range[i];
        joinRange += (i != range.size() - 1) ? ',' : '.';
    }

    return joinRange;
}

template<typename T> T getUserChar(std::string const &range) {
    T input;
    bool proceed = false;
    const char* warning = "Error. Symbol %c not in range: %s Repeat: ";

    while (!proceed) {
        std::cin >> input;

        proceed = isIncludes(input, range);
        if (!proceed) printf(warning, input, getJoinRange(range).c_str());

        // Сбрасываем коматозное состояние cin и очищаем потом ввода
        std::cin.clear();
        fflush(stdin);
    }

    return input;
}

int getUserInput() {
    std::cout << "==========\nВведите свой ответ: ";
    return (getUserChar<char>("1234") - '0');
}

void printSuggestion(int questionNumber, const vector<string> &dataQuestions, const vector<vector<string>> &dataAnswers) {
    printf("Вопрос: %s\n==========\nВыберите один из ответов:\n", dataQuestions[questionNumber].c_str());

    for (int i = 1; i <= dataAnswers[questionNumber].size() - 1; ++i) {
        std::cout << i << ": " << dataAnswers[questionNumber][i] << std::endl;
    }
}

int getRandomIntInRange(int from, int to) {
    return (from + std::rand() % (to - from + 1)); // NOLINT(cert-msc50-cpp)
}

// Обрезаем с начала и с конца все НЕ символы
std::string trim(std::string const &str, std::string const &whitespace=" \r\n\t\v\f") {
    auto start = str.find_first_not_of(whitespace);
    auto end = str.find_last_not_of(whitespace);

    return str.substr(start, end - start + 1);
}

// Записывает в data данные из файла по принципу: массив строк.
// Возвращает true, если чтение прошло успешно
bool readFileToVector(const char* pathName, vector<string> &data) {
    bool isFileReadSuccessfully = false;
    std::ifstream in(pathName);

    if (in.is_open() && !in.bad()) {
        string textLine;

        while (std::getline(in, textLine)) {
            data.push_back(textLine);
        }

        in.clear();
        in.seekg(0, std::ios_base::beg);
        isFileReadSuccessfully = true;
    }

    in.close();

    return isFileReadSuccessfully;
}

// Записывает в data данные из файла по принципу: массив строк <- массив слов.
bool readFileToVectorOfVectors(const char* pathName, vector<vector<string>> &data) {
    vector<string> rows;
    bool isFileReadSuccessfully = readFileToVector(pathName, rows);

    if (isFileReadSuccessfully && !rows.empty()) {
        for (const auto &row : rows) {
            vector<string> words;
            string word;
            std::stringstream ss(row);

            // Делим строки на токены по запятой
            while (std::getline(ss, word, ',')) {
                string retStr = trim(word);
                words.push_back(retStr);
            }

            data.push_back(words);
        }
    }

    return isFileReadSuccessfully;
}

void printWarning(const char* currentPath) {
    printf("%s не обнаружен.\nОн должен находиться в директории с исполняемым файлом!\n", currentPath);
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    const int NUMBER_OF_ROUNDS = 13;
    vector<int> numbersOfPrevQuestions;
    vector<string> paths = { R"(..\test_questions.txt)", R"(..\test_answers.txt)" };
    vector<string> dataQuestions;
    vector<vector<string>> dataAnswers;

    bool isQuestionsReadSuccessfully = readFileToVector(paths[0].c_str(), dataQuestions);
    bool isAnswersReadSuccessfully = readFileToVectorOfVectors(paths[1].c_str(), dataAnswers);

    if (!isQuestionsReadSuccessfully) {
        printWarning(paths[0].c_str());
        return 1;
    }

    if (!isAnswersReadSuccessfully) {
        printWarning(paths[1].c_str());
        return 1;
    }

    while (numbersOfPrevQuestions.size() < NUMBER_OF_ROUNDS) {
        int questionNumber = getRandomIntInRange(0, (int)dataQuestions.size() - 1);
        auto it = std::find(numbersOfPrevQuestions.begin(), numbersOfPrevQuestions.end(), questionNumber);
        if (it != numbersOfPrevQuestions.cend()) continue;

        printSuggestion(questionNumber, dataQuestions, dataAnswers);

        // Верный ответ находится в массиве [questionNumber][string positionOfCorrectAnswer]
        string correctAnswer = dataAnswers[questionNumber][stoi(dataAnswers[questionNumber][0])];

        int userAnswer = getUserInput();
        bool isChoiceRight = dataAnswers[questionNumber][userAnswer] == correctAnswer;

        if (isChoiceRight) {
            std::cout << "Вы сделали верный выбор! Вам добавляется балл" << std::endl;
        } else {
            std::cout << "Вы сделали неверный выбор! Балл уходит зрителям" << std::endl;
            std::cout << "Верный ответ: (" << dataAnswers[questionNumber][0] << ") " << correctAnswer << std::endl;
        }

        numbersOfPrevQuestions.push_back(questionNumber);

        system("pause");
        system("cls");
    }

    std::cout << "Игра закончена!" << std::endl;
}
