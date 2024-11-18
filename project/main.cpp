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
    score logging and performance tracking
    difficulty levels
    hint system
    timer for multiple choice only quizzes
    better error handling for malformed quizzes
    t/f questions or fill in the blank
    
    audio feedback

    refactor!!!
    use ptrs to avoid passing around so much data
    imporve error handling even more because users are stupid

Created by Trey Bertram on 2024-11-11.

Sources:
    https://stackoverflow.com/questions/2075898/good-input-validation-loop-using-cin-c
    https://stackoverflow.com/questions/25475384/when-and-why-do-i-need-to-use-cin-ignore-in-c
    https://www.geeksforgeeks.org/passing-vector-function-cpp/
    https://www.dcs.bbk.ac.uk/~roger/cpp/week13.htm#:~:text=A%20two%2Ddimensional%20vector%20in,int%3E%20and%20the%20second%20%3E.
    https://www.geeksforgeeks.org/how-to-change-console-color-in-cpp/
    https://github.com/adamstark/AudioFile
*/


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <limits>
//#include "dependancies/AudioFile.h"
#include "dependancies/portaudio.h"
using namespace std;

//constant values for text colors
const int RED = 31;
const int GREEN = 32;
const int YELLOW = 33;


//function prototypes because I like putting all my functions below main()
bool checkAnswer(string question, char userAnswer);
bool checkAnswer(string question);

bool displayOEQuestion(string question);
bool displayMCQuestion(string question);

void rollQuestions(vector<string> openEndedQuestions, vector<string> multipleChoiceQuestions);

string findTest(int argc, char *argv[]);

vector< vector<string> > parseTest(string testPath);

void outputColored(string text, int color);
void setColor(int textColor) { cout << "\033[" << textColor << "m"; }
void resetColor() { cout << "\033[0m"; }

void cleanUp();

//generates a random integer within a specified range
int randNum(int start, int end) {
    return start + (rand() % end);
}
/*
//should pass in a vector full of every audio file and loop through them, if anything doesnt load return false else finish the loop and return true
bool loadAudio(string path, AudioFile<double>* correctSound, AudioFile<double>* incorrectSound) {
    if ( correctSound->load(path + "assets/sounds/correct.wav") && incorrectSound->load(path + "assets/sounds.correct.wav") ) {
        return true;
    } else {
        return false;
    }
}
 */

int main(int argc, char *argv[]) {
    string currentPath = argv[0];
    size_t pathPos = currentPath.find_last_of("/\\");
    string path = currentPath.substr(0, pathPos);
    /*
    //load audio files
    AudioFile<double> correctSound;
    AudioFile<double> incorrectSound;
    if ( !loadAudio(path, &correctSound, &incorrectSound) ) {
        outputColored("Error loading audio files.", YELLOW);
    }
     */
    

    //seed random number function with current time
    srand((unsigned) time(0));

    //find the path to test.txt file
    string testPath = findTest(argc, argv);
    
    //create

    //parse text.txt file and put its contents in a 2d vector
    vector< vector<string> > questions = parseTest(testPath);

    //display questions at random until the user gets them all correct
    rollQuestions(questions[0], questions[1]);
    
    cleanUp();
    
    return 0;
}

vector< vector<string> > parseTest(string testPath) {
    //import test file
    ifstream testFile;
    string line;
    
    testFile.open(testPath);
    
    while (!testFile) {
        outputColored("There was an error finding / opening your test file, please copy the path of your test here or close the program and reopen by dragging test file onto the executable: ", YELLOW);
        cin >> testPath;
        testFile.open(testPath);
    }

    //2d vector, compiler gets angry if youd dont put the spaces in there
    vector< vector<string> > questions;
    //vectors to store questions in
    vector<string> multipleChoiceQuestions;
    vector<string> openEndedQuestions;
    
    //parse file and put each question in its vector
    while ( getline(testFile, line) ) {
        if ( line[0] == 'M' ) {
            //store in mc vector
            multipleChoiceQuestions.push_back(line);
            //displayMCQuestion(line);
        } else if ( line[0] == 'O' ) {
            //store in oc vector
            openEndedQuestions.push_back(line);
            //displayOEQuestion(line);
        }
    }

    //add each question vector to the 2d question vector
    questions.push_back(openEndedQuestions);
    questions.push_back(multipleChoiceQuestions);

    return questions;
}

void cleanUp() {
    //nice congratulations message
    cout << "Congratulations on completing the quiz!\n";
    
    //keeps terminal open until user presses enter (only an issue on windows)
    cout << "Press Enter to exit...";
    cin.ignore();
    cin.get();
}

void outputColored(string text, int color) {
    setColor(color);
    cout << text;
    resetColor();
}

string findTest(int argc, char *argv[]) {
    //path to .exe is always argv[0]
    string currentPath = argv[0];
    //find the last \\ or /
    size_t pathPos = currentPath.find_last_of("/\\");
    //cut off everythong after the \\ to leave just the current directory
    string testPath = currentPath.substr(0, pathPos);
    cout << "Current working directory is: " << testPath << endl << endl;
    //add test.txt
    testPath.append("\\test.txt");

    //if theres 2 arguments then that means the user specified the test they want to use
    if ( argc == 2 ) {
        testPath = argv[1];
        cout << testPath << endl << endl;
    }
    
    return testPath;
}

void rollQuestions(vector<string> openEndedQuestions, vector<string> multipleChoiceQuestions) {
    //define current index
    int currentIndex;
    //while either the open ended or multiple chioce vectors are not empty...
    while ( !openEndedQuestions.empty() || !multipleChoiceQuestions.empty() ) {
        // picks between the multiple choice and open ended (66% MC and 33% OE)
        //also checks that the selected vector is not empty and re-rolls if it is
        if ( randNum(0, 2) && !multipleChoiceQuestions.empty()) {
            //picks a random question
            currentIndex = randNum(0, (int)multipleChoiceQuestions.size());
            //if the user gets the question correct the function returns true
            if (displayMCQuestion(multipleChoiceQuestions[currentIndex])) {
                //erase question that user got correct
                multipleChoiceQuestions.erase(multipleChoiceQuestions.begin() + currentIndex);
            }
            //checks that oe vector is not empty
        } else if ( !openEndedQuestions.empty() ) {
            //same as before
            currentIndex = randNum(0, (int)openEndedQuestions.size());
            if ( displayOEQuestion(openEndedQuestions[currentIndex]) ) {
                openEndedQuestions.erase(openEndedQuestions.begin() + currentIndex);
            }
        }
    }
}

size_t findPunc(string question) {
    //the string::find method returns npos if the character is not found in the string
    if (question.find('?') != string::npos) {
        //if a question mark is found then the question end position is equal to the position of it
        return question.find('?');
        //else check for a perios and do same as before
    } else if (question.find('.') != string::npos) {
        return question.find('.');
    } else {
        //else there is an issue
        outputColored("There was an error parsing the question string.\n", RED);
        return string::npos;
    }
}

bool checkAnswer(string question, char userAnswer) {
    //initialize answer position
    size_t answerPos = 0;
    //the psoition of the answer character is immediately follwoing the '='
    answerPos = question.find('=');
    char answer = question[answerPos + 1];
    //return true if the users answer matches the provided one
    return userAnswer == answer;
}

bool displayMCQuestion(string question) {
    
    size_t questionPos = findPunc(question);
    if (questionPos == string::npos) {
        return false;
    }
    //question substring is from position 2 (after the M or O question identifier and the ' ') to the position of the first punctuation mark
    string questionText = question.substr(2, questionPos - 1);
    //output the question
    cout << questionText << endl << endl;
    
    //create a vector for every possible answer
    vector<string> answers;
    //teh start of a potential answer is where the '~' is
    //finds the first potential answer
    size_t start = question.find('~');
    
    //while we have not reached the end of the string
    while (start != string::npos) {
            //finds the next '~' starting from the position immediately after the last one, indicating the end of the answer
            size_t end = question.find('~', start + 1);
            //if we have reached the end of the string
            if (end == string::npos) {
                //append the remaining text to the vector then break out of the loop
                answers.push_back(question.substr(start + 1));
                break;
            } else {
                //append potential answer
                //answer starts after the position of the '~' and the length of the answer is the end position - start - 1
                answers.push_back(question.substr(start + 1, end - start - 1));
            }
            //set the start position of the next answer equal to the end of the current one
            start = end;
        }
    
    //loop through answers vector outputting every answer
    for (int i = 0; i < answers.size() - 1; i++) {
        cout << answers[i] << endl;
    }
    
    //prompt user for answer
    char userAnswer;
    cout << "Enter answer then press enter again to confirm: ";
    cin >> userAnswer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');


    
    //check answer
    if ( checkAnswer(question, userAnswer) ) {
        outputColored("Correct!\n\n", GREEN);
        return true;
    } else {
        outputColored("Wrong.\n\n", RED);
        return false;
    }
}

//check opeb ended answer
bool checkAnswer(string question) {
    
    size_t questionPos = findPunc(question);
    if (questionPos == string::npos) {
        return false;
    }
    
    //answer is all the text following the question
    string answerText = question.substr(questionPos + 1);
    cout << answerText << endl;
    
    
    //prompt user to self check if their answer matches the provided one
    char userAnswer;
    cout << "Does your answer match the one provided? (Y/N): ";
    cin >> userAnswer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');


    //return resulkt
    return userAnswer == 'Y';
}

bool displayOEQuestion(string question) {
    
    size_t questionPos = findPunc(question);
    if (questionPos == string::npos) {
        return false;
    }
    
    //the question is all the text before the punctuation mark position
    string questionText = question.substr(2, questionPos - 1);
    cout << questionText << endl << endl;
    
    
    string userAnswer; 
    cout << "Enter answer then press enter again to confirm: "; 
    cin.ignore(); 
    getline(cin, userAnswer);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');


    
    //if they think their answer is adequate then return true
    if ( checkAnswer(question) ) {
        outputColored("Correct!\n\n", GREEN);
        return true;
    } else {
        outputColored("Wrong.\n\n", RED);
        return false;
    }
    
}
