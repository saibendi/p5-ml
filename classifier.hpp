//
//  classifier.hpp
//  p5-ml
//
//  Created by Bends on 8/23/24.
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
            value.first = 2;
        }
        else {
            auto defaultValue = make_pair(numExample,0.0);
            map[word] = defaultValue;
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
            value.first = ++(value.first);
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

    
public:
    
    void getTrainingPosts() {
        cout << numTrainingPosts << endl;
    }
    
    void printTagMap(const map<string,pair<int,double>> &map) {
        cout << "---TAG MAP-----------" << endl;
        for (auto i : map) {
            cout << i.first << ", " << i.second.first << ", " << i.second.second << endl;
        }
    }
    
    void printTagContentMap(const map<pair<string,string>,pair<int,double>> &map) {
        cout << "---TAG CONTENT MAP-----------" << endl;
        for (auto i : map) {
            cout << "{" << i.first.first << ", " << i.first.second << "}"  << " " << i.second.first << " " << i.second.second << endl;
        }
    }

    void printContentMap(const map<string,int> &map) {
        cout << "---CONTENT MAP-----------" << endl;
        for (auto i : map) {
            cout << i.first << ", " << i.second << endl;
        }
    }

    
    void printAllMaps() {
        printTagMap(tagMap);
        printContentMap(contentMap);
        printTagContentMap(tagContentMap);
    }
    
    void testStringPairVectorParsing(vector <pair<string, string>> vector_StringPair) {
        string label;
            for (auto pair : vector_StringPair) {
                string header = pair.first;
                string word = pair.second;
                // if first part of pair is "tag", add to tagMap
                if (header == "tag") {
                    // if tag, word turns into a tag
                    label = word;
                    // then add word to tagMap
                    addingWordsToTagMap(tagMap, label);
                    ++numTrainingPosts;
                }
                else if (header == "content") {
                    // if content, word turns into a content word
                    // then add word to contentMap
                    addingWordsToContentMap(contentMap, word);
                    addingWordsToTagContentMap(tagContentMap, make_pair(label,word));
                }
            }
        cout << "training data:" << endl;
        cout << "label = " << label << ", content =";
        for (auto pair : vector_StringPair) {
            if (pair.first == "content") {
                cout << " " << pair.second;
            }
        }
        cout << endl;

    }
    
    // -------------------------------TEST FUNCTIONS ABOVE-----------------------------------------------------------------------------------------

    
    void printData(const string label, const vector<pair<string,string>> &vector_in) {
        cout << "label = " << label << ", content =";
        for (auto pair : vector_in) {
            if (pair.first == "content") {
                cout << " " << pair.second;
            }
        }
    }
     
    
    // EFFECTS: reads from filename and sets the playing field, also checks if debug is on to spit out what we read
    void readFromTrainCSV(const string filename, bool debug) {
        // creating csvstream object
        csvstream file(filename);
        
        // get header
        vector<string> throwawayLine;       // vector of strings for header
        throwawayLine = file.getheader();   // reading header
        
        vector <pair<string, string>> vector_StringPair;

        cout << "training data:" << endl;
        while (file >> vector_StringPair) {
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
            cout << endl;
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
