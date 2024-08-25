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
#include <cmath>

using namespace std;

void errorChecker();

class classifier {
private:
    
    map<string, pair<int,double>> tagMap;
    map<pair<string,string>, pair<int, double>> tagContentMap;
    map<string, int> contentMap;
    
    // base number of examples if a label is found
    const int numExample = 1;

    // total number of training posts
    int numTrainingPosts = 0;
    /*
    // EFFECTS: Return a set of unique whitespace delimited words
    map<string, int> unique_words(const string &str) {
      istringstream source(str);
      map<string, int> words_map;
      string word;
      while (source >> word) {
        words_map.insert(word);
      }
      return words_map;
    }
     */
    
    // EFFECTS: adds words to map
    void addingWordsToTagMap(map<string,pair<int,double>> &map, const string word) {
        if (map.find(word) != map.end()) {
            auto value = map.at(word);
            ++value.first;
        }
        else {
            auto defaultValue = make_pair(numExample,0.0);
            map[word] = defaultValue;
            ++numTrainingPosts;
        }
    }

        
    // EFFECTS: adds words to map
    void addingWordsToTagContentMap(map<pair<string,string>,pair<int,double>> &map, const pair<string,string> pair) {
        if (map.find(pair) != map.end()) {
            /*
            using pairType = std::pair<int,double>;
            pairType value = map.at(pair);
             */
            auto value = map.at(pair);
            ++value.first;
        }
        else {
            auto defaultValue = make_pair(numExample,0.0);
            map[pair] = defaultValue;
        }
    }
    
    // EFFECTS: adds words to map
    void addingWordsToContentMap(map<string,int> &map, const string word) {
        if (map.find(word) != map.end()) {
            ++map.at(word);
        }
        else {
            map[word] = numExample;
        }
    }
    
    void printData(const string label, const vector<pair<string,string>> &vector_in) {
        cout << "training data:" << endl;
        cout << "label = " << label << ", content =";
        for (auto pair : vector_in) {
            if (pair.first == "content") {
                cout << " " << pair.second;
            }
        }
        cout << endl;
    }

public:
    // default ctor;
    classifier() {
        
    }
    
    // EFFECTS: reads from filename and sets the playing field, also checks if debug is on to spit out what we read
    void readFromTrainCSV(const string filename, bool debug) {
        // creating csvstream object
        csvstream file(filename);
        
        // get header
        vector<string> throwawayLine;       // vector of strings for header
        throwawayLine = file.getheader();   // reading header
        
        // Reading in from CSV row by row
        vector <pair<string, string>> vector_StringPair;
        while (true) {
            // vector_StringPair gets cleared each time and reads in a new row
            file >> vector_StringPair;
            
            // if vector is empty after reading in from csv, break from loop
            if (vector_StringPair.empty()) {
                break;
            }

            // if not empty, parse data and add to individual maps
            string label;
            for (auto pair : vector_StringPair) {
                string header = pair.first;
                string word = pair.second;
                                
                // if first part of pair is "tag", add to tagMap
                if (header == "tag") {
                    // if tag, word turns into a label
                    label = word;
                    // then add label to tagMap
                    addingWordsToTagMap(tagMap, label);
                    // increase the number of training posts everytime we encounter a tag
                    ++numTrainingPosts;
                }
                else if (header == "content") {
                    // if content, word turns into a content word
                    // then add word to contentMap
                    string sentence = word;
                    // EFFECTS: Return a set of unique whitespace delimited words
                      istringstream source(sentence);
                      string contentWord;
                      while (source >> contentWord) {
                          addingWordsToContentMap(contentMap, contentWord);
                          addingWordsToTagContentMap(tagContentMap, make_pair(label,contentWord));
                      }
                }
            }
            
            // Step A (debug). Print out data
            if (debug == true) {
                printData(label, vector_StringPair);
            }
        }
    }

    void calculateLogLikelihood(const string label, const int numTrainingPostsWithLabelC) {
        for (auto tagContentMapValue : tagContentMap) {
            auto tagContent = tagContentMapValue.first;
            auto intDouble = tagContentMapValue.second;

            if (tagContent.first == label) {
                double logLikelihood = log(intDouble.first / numTrainingPostsWithLabelC);
                intDouble.second = logLikelihood;
            }
        }
    }
    
    void calculateLogPrior() {
        for (auto tagMapValue : tagMap) {
            string label = tagMapValue.first;
            auto labelIntDouble = tagMapValue.second;
            int numTrainingPostsWithLabelC = labelIntDouble.first;
            
            // calculating Log Prior and replacing map occurrence value with log-prior value
            double logPrior = log(numTrainingPostsWithLabelC / numTrainingPosts);
            labelIntDouble.second = logPrior;
            
            //calculating Log Likelihood
            calculateLogLikelihood(label, numTrainingPostsWithLabelC);
        }
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
    
    
    // need to check if debug option is ON / OFF
    return 0;
}


