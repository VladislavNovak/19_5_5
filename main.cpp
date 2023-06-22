#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream> // stringstream
#include <string>
#include <vector>

using std::string;
using std::vector;

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

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    // Данные будут содержаться в двух файлах
    // Первый будет содержать 13 вопросов
    // Второй будет содержать 13 ответов
    // Каждый вопрос и ответ будет находиться на своей строчке
    // Таким образом можно будет их считать каждый в свой массив

    // Стрелка на барабане будет вращаться случайно. И вопросы будут выпадать соответственно ей
    // Получаем ответ игрока. Сравниваем с ответом. В зависимости от этого будут начисляться баллы

    vector<string> paths = { R"(..\test_questions.txt)", R"(..\test_answers.txt)" };
    vector<string> dataQuestions;
    vector<vector<string>> dataAnswers;

    bool isQuestionsReadSuccessfully = readFileToVector(paths[1].c_str(), dataQuestions);
    bool isAnswersReadSuccessfully = readFileToVectorOfVectors(paths[1].c_str(), dataAnswers);

    if (isQuestionsReadSuccessfully && !dataQuestions.empty()) {
        for (auto const &record : dataQuestions) std::cout << record << std::endl;
    } else {
        std::cout << "Bad questions file" << std::endl;
    }

    std::cout << "===========================" << std::endl;
    if (isAnswersReadSuccessfully && !dataAnswers.empty()) {
        for (auto const &row : dataAnswers) {
            for (auto const &record : row) {
                std::cout << record << " ";
            }

            std::cout << std::endl;
        }
    } else {
        std::cout << "Bad answers file" << std::endl;
    }
}
