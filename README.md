# Quiz Generator

## Overview
This project is a command-line quiz application designed for a **COMP150-AB8 Term Project**. The program takes a `.txt` file containing questions, categorizes them into **Multiple Choice** and **Open-Ended** questions, and quizzes the user. It tracks performance, allowing the user to monitor progress and improvement.

## Features
1. **Question Categorization**:
   - Parses the input file into separate vectors for multiple-choice and open-ended questions.
2. **Interactive Quiz**:
   - Displays questions at random, ensuring varied interaction.
   - For multiple-choice questions, answers are selected and validated automatically.
   - For open-ended questions, users self-assess their responses.
3. **Progress Tracking**:
   - Questions answered correctly are removed from their respective vectors.
   - A log file can be implemented in future iterations to track user performance.
4. **Extensibility**:
   - Potential future GUI integration using SDL.
   - Open for feature expansion (e.g., custom scoring systems, hints).

---

## Getting Started

### Prerequisites
- A C++ compiler supporting C++11 or later (e.g., GCC, Clang, MSVC).
- A `.txt` file formatted for the quiz (see **Input File Format** below).

### Input File Format
The input file should:
- Begin each question with `M` (for Multiple Choice) or `O` (for Open-Ended), followed by the question text.
- For multiple-choice questions:
  - Use `~` to denote options.
  - Include the correct answer immediately after `=` in the format `=A` (e.g., "What is 2+2? ~A. Four ~B. Three =A").
- For open-ended questions:
  - Provide the answer after the punctuation mark ending the question (e.g., "What is the capital of France? Paris").

### Example Input
M What is 2+2? ~A. Four ~B. Three =A 
O What is the capital of France? Paris

---

### Compiling and Running

1. **Compilation**:
   ```bash
   g++ main.cpp -o quiz_app
2. **Running**:
    - Drag and drop your .txt file into the executable.
    - Or run the program with your file as an argument:
    ```bash
    ./quiz_app path/to/test.txt

README.md generated by ChatGPT