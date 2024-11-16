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
    number 5, and think of new features
    maybe GUI using SDL?

Created by Trey Bertram on 2024-11-11.

Sources:
    https://stackoverflow.com/questions/2075898/good-input-validation-loop-using-cin-c
    https://stackoverflow.com/questions/25475384/when-and-why-do-i-need-to-use-cin-ignore-in-c
    https://www.geeksforgeeks.org/passing-vector-function-cpp/
    https://www.dcs.bbk.ac.uk/~roger/cpp/week13.htm#:~:text=A%20two%2Ddimensional%20vector%20in,int%3E%20and%20the%20second%20%3E.
*/


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <limits>
using namespace std;

//function prototypes because I like putting all my functions below main()
bool checkAnswer(string question, char userAnswer);
bool checkAnswer(string question);

bool displayOEQuestion(string question);
bool displayMCQuestion(string question);

void rollQuestions(vector<string> openEndedQuestions, vector<string> multipleChoiceQuestions);

string findTest(int argc, char *argv[]);

vector< vector<string> > parseTest(string testPath);

//generates a random integer within a specified range
int randNum(int start, int end) {
    return start + (rand() % end);
}

int main(int argc, char *argv[]) {

    //seed random number function with current time
    srand((unsigned) time(0));

    //find the path to test.txt file
    string testPath = findTest(argc, argv);

    //parse text.txt file and put its contents in a 2d vector
    vector< vector<string> > questions = parseTest(testPath);

    //display questions at random until the user gets them all correct
    rollQuestions(questions[0], questions[1]);
    
    //nice congratulations message
    cout << "Congratulations on completing the quiz!\n";
    
    //keeps terminal open until user presses enter (only an issue on windows)
    cout << "Press Enter to exit..."; 
    cin.ignore(); 
    cin.get(); 
    
    return 0;
}

vector< vector<string> > parseTest(string testPath) {
    //import test file
    ifstream testFile;
    string line;
    
    testFile.open(testPath);
    
    while (!testFile) {
        cout << "There was an error finding / opening your test file, please copy the path of your test here or close the program and reopen by dragging test file onto the executable: ";
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
    //initalize question position 
    size_t questionPos = 0;
    //the string::find method returns npos if the character is not found in the string
    if (question.find('?') != string::npos) {
        //if a question mark is found then the question end position is equal to the position of it
        questionPos = question.find('?');
        //else check for a perios and do same as before
    } else if (question.find('.') != string::npos) {
        questionPos = question.find('.');
    } else {
        //else there is an issue
        std::cout << "Error parsing question string.\n";
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
        cout << "Correct!\n\n";
        return true;
    } else {
        cout << "Wrong.\n\n";
        return false;
    }
}

//check opeb ended answer
bool checkAnswer(string question) {
    //finds position of question end
    size_t questionPos = 0;
    if (question.find('?') != string::npos) {
        questionPos = question.find('?');
    } else if (question.find('.') != string::npos) {
        questionPos = question.find('.');
    } else {
        std::cout << "Error parsing question string.\n";
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
    //find where question ends
    size_t questionPos = 0;
    if (question.find('?') != string::npos) {
        questionPos = question.find('?');
    } else if (question.find('.') != string::npos) {
        questionPos = question.find('.');
    } else {
        std::cout << "Error parsing question string.\n";
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
        cout << "Correct!\n\n";
        return true;
    } else {
        cout << "Wrong.\n\n";
        return false;
    }
    
}
