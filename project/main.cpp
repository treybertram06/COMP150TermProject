/*
main.cpp

COMP150-AB8 Term Project

 This program will take a .txt file full of questions as input
 then parse each line and put the multiple choice questions
 in one vector and the open ended ones in the other.
 It will then quiz the user and track their performance and
 improvement in a log file. For open ended questions the user
 will compare their answer to the provided one and decide if
 they got it correct.
 
 1) Input text file
 2) create a vector for both open and multiple chice questions
 3) Parse each question at random and output certain parts of the question to the user in a readable format
 4) If user gets question correct then remove it from its vector
 5) Create a log file for the user to track thier performance
 
 
 To-do:
    numbers 3.5, 4, 5, and think of new features
    maybe GUI using SDL?

Created by Trey Bertram on 2024-11-11.
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
using namespace std;

bool checkAnswer(string question, char userAnswer);
bool checkAnswer(string question);

void displayOEQuestion(string question);
bool displayMCQuestion(string question);

int randNum(int start, int end) {
    return start + (rand() % end);
}

int main(int argc, char *argv[]) {

    //seed random number function with current time
    srand((unsigned) time(0));
    
    //import test file
    ifstream testFile;
    string line;

    //for testing on macbook
    //testFile.open("/Users/treybertram/Desktop/testingTest.txt");
    //for testing on pc
    testFile.open("test.txt");
    
    if (!testFile) {
        cout << "Error opening test file.\n";
        return 1;
    }
    
    vector<string> multipleChoiceQuestions;
    vector<string> openEndedQuestions;
    //int openQuestionsSize = 0;
    //int multipleQuestionsSize = 0;
    
    //parse file and put each question in its vector
    while ( getline(testFile, line) ) {
        if ( line[0] == 'M' ) {
            //store in mc vector
            //multipleQuestionsSize++;
            multipleChoiceQuestions.push_back(line);
            //displayMCQuestion(line);
        } else if ( line[0] == 'O' ) {
            //store in oc vector
            //openQuestionsSize++;
            openEndedQuestions.push_back(line);
            //displayOEQuestion(line);
        }
    }
    
    int currentIndex;
    while ( !openEndedQuestions.empty() || !multipleChoiceQuestions.empty() ) {
        if ( randNum(0, 2) && !multipleChoiceQuestions.empty()) {
            currentIndex = randNum(0, multipleChoiceQuestions.size());
            if (displayMCQuestion(multipleChoiceQuestions[currentIndex])) {
                multipleChoiceQuestions.erase(multipleChoiceQuestions.begin() + currentIndex);
            }
            
        } else if ( !openEndedQuestions.empty() ) {
            displayOEQuestion(openEndedQuestions[randNum(0, openEndedQuestions.size())]);
        }
    }
    
    cout << "Congratulations on completing the quiz!\n";
    
    
    return 0;
}

bool checkAnswer(string question, char userAnswer) {
    size_t answerPos = 0;
    answerPos = question.find('=');
    char answer = question[answerPos + 1];
    return userAnswer == answer;
}

bool displayMCQuestion(string question) {
    
    size_t questionPos = 0;
    if (question.find('?') != string::npos) {
        questionPos = question.find('?');
    } else if (question.find('.') != string::npos) {
        questionPos = question.find('.');
    } else {
        std::cout << "Error parsing question string.\n";
    }
    string questionText = question.substr(2, questionPos - 1);
    cout << questionText << endl << endl;
    
    vector<string> answers;
    size_t start = question.find('~');
    
    while (start != string::npos) {
            size_t end = question.find('~', start + 1);
            if (end == string::npos) {
                answers.push_back(question.substr(start + 1));
                break;
            } else {
                answers.push_back(question.substr(start + 1, end - start - 1));
            }
            start = end;
        }
    
    for (int i = 0; i < answers.size() - 1; i++) {
        cout << answers[i] << endl;
    }
    
    char userAnswer = 'A';
    cout << "Enter answer: ";
    cin >> userAnswer;
    
    if ( checkAnswer(question, userAnswer) ) {
        cout << "Correct!\n\n";
        return true;
    } else {
        cout << "Wrong.\n\n";
        return false;
    }
}

bool checkAnswer(string question) {
    size_t questionPos = 0;
    if (question.find('?') != string::npos) {
        questionPos = question.find('?');
    } else if (question.find('.') != string::npos) {
        questionPos = question.find('.');
    } else {
        std::cout << "Error parsing question string.\n";
    }
    
    string answerText = question.substr(questionPos + 1);
    cout << answerText << endl;
    
    char userAnswer = 'N';
    cout << "Does your answer match the one provided? (Y/N): ";
    cin >> userAnswer;
    return userAnswer == 'Y';
}

void displayOEQuestion(string question) {
    size_t questionPos = 0;
    if (question.find('?') != string::npos) {
        questionPos = question.find('?');
    } else if (question.find('.') != string::npos) {
        questionPos = question.find('.');
    } else {
        std::cout << "Error parsing question string.\n";
    }
    string questionText = question.substr(2, questionPos - 1);
    cout << questionText << endl << endl;
    
    string userAnswer = "";
    cout << "Enter answer: ";
    cin >> userAnswer;
    
    if ( checkAnswer(question) ) {
        cout << "Correct!\n\n";
    } else {
        cout << "Wrong.\n\n";
    }
    
}

