//
//  main.cpp
//  p5-ml
//
//  Created by Bends on 8/6/24.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include "csvstream.hpp"

using namespace std;

void errorChecker();

class Classifier {
private:
    // the map that we use to store all unique content per label - words = words from Content
    map<string, int> uniqueWordsPerLabel;
    
    // base number of examples if a label is found
    const int numExample = 1;

    // need a container that maps label to map and numExamples - TUPLE??
    // if label doesn't exist, create a new map and add that map to the container associating it with the label
    // if label does exist, go to container, access the map associated with that label and continue adding words
    vector<map<string,int>> mapVector;
    vector<string> labelVector;
    vector<int> numExamplesVector;

    // EFFECTS: Return a set of unique whitespace delimited words
    set<string> unique_words(const string &str) {
      istringstream source(str);
      set<string> words;
      string word;
      while (source >> word) {
        words.insert(word);
      }
      return words; // set is alphatbetically ordered BST
    }
    
    void separatingCombinedWord(const string combinedWord, string &firstWord, string &label) {
        // separating first word out of the combined word
        string comma = "'";
        size_t lastCommaIndex = combinedWord.find_last_of(comma);
        firstWord = combinedWord.substr(lastCommaIndex+1, string::npos);

        // separating label out of the rest of the combined word
        label = combinedWord.substr(0,lastCommaIndex-1); // setting label to the word before the last comma
        size_t anotherCommaIndex = label.find(comma);           // getting index of another comma if it exists in label
        if (anotherCommaIndex != string::npos) {                // if we find another comma in the label, remove everything before the comma (incl. the comma) and set new label
            label = label.erase(0,anotherCommaIndex);
        }
    }
    
    // EFFECTS: adds words to map
    void addingWordsToMap(map<string,int> &map, const string word) {
        if (map.find(word) != map.end()) {
            ++map.at(word);
        }
        else {
            map[word] = 1;
        }
    }
    
    // EFFECTS: TODO:
  void unique_words_map(const string &str) {
        // defining stringstream and map
        istringstream source(str);
        string firstWord;
        string label;
        // separating out combinedWord : combinedWord = label + first word
        string combinedWord;
        source >> combinedWord;
        
        separatingCombinedWord(combinedWord, firstWord, label);
        
        // check if label already exists or not
        auto result = find(labelVector.begin(), labelVector.end(), label);
        // if label exists
        if (result != labelVector.end()) {
            // calculate index
            auto index = result - labelVector.begin();
            // increment number of examples since we found a new example
            ++numExamplesVector.at(index);
            // find existing map and add words to it
            auto existingMap = mapVector.at(index);
            
            // adding words to existing map - first word has already been read so we need to add this separately
            addingWordsToMap(existingMap, firstWord);
            string word;
            while (source >> word) {
                addingWordsToMap(existingMap, word);
            }
        }
        // if label doesn't exist
        else {
            // add label to label vector
            labelVector.push_back(label);
            // push back numExamples at the same time
            numExamplesVector.push_back(numExample);

            // adding words to new map - first word has already been read so we need to add this separately
            map<string, int> newMap;
            addingWordsToMap(newMap, firstWord);
            string word;
            while (source >> word) {
                addingWordsToMap(newMap, word);
            }
            mapVector.push_back(newMap);
        }
      
    }


public:
    // EFFECTS: reads from filename and sets the playing field, also checks if debug is on to spit out what we read
    void readFromTrainCSV(const string filename, bool debug) {
        ifstream fin;
        fin.open(filename.c_str());
        if (!fin.is_open()) {
            cout << "open failed" << endl;
        }
        
        string throwawayLine;       // place holder for first line
        getline(fin,throwawayLine); // reading first line

        string line;            // real line
        // TODO: let's just use sets for now instead of maps
        while (getline(fin,line)) {
            auto uniqueWords = unique_words(line);
            
        }
        fin.close();
    }
    
};




int main(int argc, const char * argv[]) {
    cout.precision(3);

    if (argc > 4) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;
    }
    // if argv[3] exists and it isn't set to "--debug"
    if (argv[3] && argv[3] != "--debug") {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;
    }
    
    /*
    string train_filename = argv[1];
    ifstream fin(train_filename);
    if (!fin.is_open()) {
        cout << "Error opening file: " << train_filename << endl;
        return 1;
    }
     */
    
    csvstream(train_filename);
    
    // need to check if debug option is ON / OFF
    return 0;
}

map<string, int> unique_words(const string &str) {
  istringstream source(str);
  map<string, int> words_map;
  string word;
  while (source >> word) {
    words_map.insert(word);
  }
  return words_map;
}


    /*
    words_and_appearances[firstWord] = 1;                   // adding first word in content to map and adding a count of 1 to the appearance value
  string word;
  while (source >> word) {
      if (words_and_appearances.find(word) != words_and_appearances.end()) {
          ++words_and_appearances.at(word);
      }
      else {
          words_and_appearances[word] = 1;
      }
  }
  return words_and_appearances; // map is alphatbetically ordered BST
*/
