#include "classifier.hpp"
#include "unit_test_framework.hpp"


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

TEST(testReadCSV) {
    classifier object;
    string filename = "/Users/saibendi/Desktop/EECS280/p5-ml/train_small.csv";
    object.readFromTrainCSV(filename, true);
    
    
    
}
TEST_MAIN()
