#include "classifier.hpp"
#include "unit_test_framework.hpp"
#include <cmath>

/*
TEST(testVectorParsing) {
    classifier object;
    vector<pair<string,string>> testVector;
    // tag, euchre
    // content, I
    // content, really
    // content, like
    // content, this
    // content, project
    testVector.push_back(make_pair("tag","euchre"));
    testVector.push_back(make_pair("content","I"));
    testVector.push_back(make_pair("content","really"));
    testVector.push_back(make_pair("content","like"));
    testVector.push_back(make_pair("content","this"));
    testVector.push_back(make_pair("content","project"));

    object.testStringPairVectorParsing(testVector);
    object.printAllMaps();
    
    vector<pair<string,string>> testVector2;
    testVector2.push_back(make_pair("tag","euchre"));
    testVector2.push_back(make_pair("content","I"));
    testVector2.push_back(make_pair("content","really"));
    testVector2.push_back(make_pair("content","like"));

    object.testStringPairVectorParsing(testVector2);
    object.printAllMaps();

    object.getTrainingPosts();
    
}
 */


TEST(testReadCSV) {
    classifier object;
    string filename = "/Users/saibendi/Desktop/EECS280/p5-ml/train_small.csv";
    bool debug = true;
    object.readFromTrainCSV(filename, debug);
    object.getTrainingPosts();
    object.calculateLogPrior();
    if (debug) {
        object.getVocabSize();
        object.printAllMaps();
    }
    
    string filename2 = "/Users/saibendi/Desktop/EECS280/p5-ml/test_small.csv";
    object.readFromTestCSV(filename2);
}


/*
TEST(testLog) {
    // doing log (3/8), for e.g., is causing 3/8 round down to 0, and then we're getting log(0) which is -inf;
    // to avoid, need to use doubles instead of ints
    cout << log(3.0/8) << endl;
}
*/

TEST_MAIN()
