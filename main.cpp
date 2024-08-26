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
    const int defaultNumExample = 1;
    
    // total number of training posts
    int numTrainingPosts = 0;
    
    
    // EFFECTS: adds words to map
    void addingWordsToTagMap(map<string,pair<int,double>> &map, const string word) {
        if (map.find(word) != map.end()) {
            auto value = map.at(word);
            map[word] = make_pair(++value.first, value.second);
        }
        else {
            auto defaultValue = make_pair(defaultNumExample,0.0);
            map[word] = defaultValue;
        }
    }
    
    // EFFECTS: adds words to map
    void addingWordsToTagContentMap(map<pair<string,string>,pair<int,double>> &map, const pair<string,string> pair) {
        if (map.find(pair) != map.end()) {
            auto value = map.at(pair);
            map[pair] = make_pair(++value.first, value.second);
        }
        else {
            auto defaultValue = make_pair(defaultNumExample,0.0);
            map[pair] = defaultValue;
        }
    }
    
    // EFFECTS: adds words to map
    void addingWordsToContentMap(map<string,int> &map, const string word) {
        if (map.find(word) != map.end()) {
            ++map.at(word);
        }
        else {
            map[word] = defaultNumExample;
        }
    }
    
    void printTagMap(const map<string,pair<int,double>> &map) {
        cout << "classes:" << endl;
        for (auto i : map) {
            cout << "   " << i.first << ", " << i.second.first << " examples, log-prior = " << i.second.second << endl;
        }
    }
    
    void printTagContentMap(const map<pair<string,string>,pair<int,double>> &map) {
        cout << "classifier parameters:" << endl;
        for (auto i : map) {
            cout << "   " << i.first.first << ":" << i.first.second << ", count = " << i.second.first << ", log-likelihood = " << i.second.second << endl;
        }
        // an extra blank line
        cout << endl;
    }
    /*
    void printContentMap(const map<string,int> &map) {
        for (auto i : map) {
            cout << i.first << ", " << i.second << endl;
        }
    }
     */

public:
    
    void printAllMaps() {
        printTagMap(tagMap);
        //printContentMap(contentMap);
        printTagContentMap(tagContentMap);
    }

    void getVocabSize() {
        cout << "vocabulary size = " << contentMap.size() << endl;
    }
    
    void getTrainingPosts() {
        cout << "trained on " << numTrainingPosts << " examples" << endl;
    }
    
    void printData(const string label, const vector<pair<string,string>> &vector_in) {
        cout << "   label = " << label << ", content =";
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
        
        // read row by row
        vector<pair<string, string>> vector_StringPair;
        if (debug) {
            cout << "training data:" << endl;
        }
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
                cout << endl;
            }
        }
    }
    
    void calculateLogLikelihood(const string label, const double numTrainingPostsWithLabelC) {
        cout.precision(3);
        for (auto tagContentMapValue : tagContentMap) {
            auto key = tagContentMapValue.first;
            auto value = tagContentMapValue.second;
            double numTrainingPostsWithLabelCThatContainW = value.first;
            if (key.first == label) {
                double logLikelihood = log(numTrainingPostsWithLabelCThatContainW / numTrainingPostsWithLabelC);
                tagContentMap[key] = make_pair(numTrainingPostsWithLabelCThatContainW, logLikelihood);
            }
        }
    }
    
    void calculateLogPrior() {
        cout.precision(3);
        for (auto tagMapValue : tagMap) {
            string key = tagMapValue.first;
            auto value = tagMapValue.second;
            double numTrainingPostsWithLabelC = value.first;
            // calculating Log Prior and adding to map
            double logPrior = log(numTrainingPostsWithLabelC / numTrainingPosts);
            tagMap[key] = make_pair(numTrainingPostsWithLabelC, logPrior);
            
            //calculating Log Likelihood
            calculateLogLikelihood(key, numTrainingPostsWithLabelC);
        }
    }
    
    void pickMaxProbability(vector<pair<string,double>> &labelProbability, string &labelWinner, double &max) {
        for (auto pair : labelProbability) {
            if (pair.second > max) {
                labelWinner = pair.first;
                max = pair.second;
            }
            // if probability is the same
            else if (pair.second == max) {
                // then use first label in alphabetical order
                if (pair.first < labelWinner) {
                    labelWinner = pair.first;
                    max = pair.second;
                }
            }
        }
    }
    void calculateProbability(vector<pair<string, string>> &vector_StringPair) {
        double logProbability = 0.0;
        vector<pair<string,double>> labelProbability = {};
        for (auto tag: tagMap) {
            // for each tag, add the tag's logPrior
            // we need to do it against every tag in tagMap
            logProbability = tag.second.second;
            double sumOfLogLikelihood = 0.0;
            
            // loop through each word from the row we read in
            for (auto pair : vector_StringPair) {
                string header = pair.first;
                string word = pair.second;
                double logLikelihood = 0.0;
                
                // if header is content, then get the log-likelihood of word
                if (header == "content") {
                    auto key = make_pair(tag.first, word);
                    // if found in map, add the likelihood to sum
                    if (tagContentMap.find(key) != tagContentMap.end()) {
                        auto value = tagContentMap[key];
                        logLikelihood = value.second;
                    }
                    // if not found in map, there's two possibilities:
                    else {
                        // A) check to see if word is in content
                        if (contentMap.find(word) != contentMap.end()) {
                            double numTrainingPostsThatContainW = contentMap[word];
                            logLikelihood = numTrainingPosts / numTrainingPosts;
                        }
                        // B) if word isn't anywhere in classifier maps, then do 1 / numTrainingPosts
                        else {
                            logLikelihood = 1.0 / numTrainingPosts;
                        }
                    }
                }
                
                // sum up all log-likelihoods for each content word
                sumOfLogLikelihood = sumOfLogLikelihood + logLikelihood;
            }
            
            // now summing log-prior and sum(log-likelihood)
            logProbability = logProbability + sumOfLogLikelihood;
            labelProbability.push_back(make_pair(tag.first, logProbability));
        }
        string labelWinner;
        double maxProbability = -INFINITY;
        pickMaxProbability(labelProbability, labelWinner, maxProbability);
        cout << "this is the label: " << labelWinner << "; this is the probability: " << maxProbability << endl;
    }
    
    // EFFECTS: reads from filename and sets the playing field, also checks if debug is on to spit out what we read
    void readFromTestCSV(const string filename) {
        // creating csvstream object
        csvstream file(filename);
        
        // get header
        vector<string> throwawayLine;       // vector of strings for header
        throwawayLine = file.getheader();   // reading header
        
        // read row by row
        vector<pair<string, string>> vector_StringPair;

        while (file >> vector_StringPair) {
            calculateProbability(vector_StringPair);
            cout << endl;
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


