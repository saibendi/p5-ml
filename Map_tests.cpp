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
    cout.precision(3);
    classifier object;
    string filename = "/Users/saibendi/Desktop/EECS280/p5-ml/w16_projects_exam.csv";
    //string filename = "/Users/saibendi/Desktop/EECS280/p5-ml/train_small.csv";
    bool debug = false;
    bool test = true;
    object.readFromTrainCSV(filename, debug);
    object.getTrainingPosts();
    object.calculateLogPrior(test);
    if (debug) {
        object.getVocabSize();
        object.printAllMaps();
    }
    
    string filename2 = "/Users/saibendi/Desktop/EECS280/p5-ml/sp16_projects_exam.csv";
    //string filename2 = "/Users/saibendi/Desktop/EECS280/p5-ml/test_small.csv";
    object.readFromTestCSV(filename2);
    
    
    
    cout << "-----------------------------DEBUGGING-----------------------" << endl;
    string correct_string = "im a bit confused about linux is that something we need to download";
    object.testCalculateProbability(correct_string);
    //RESULTS:
    // Log-Likelihood
        // Set had:
        // 1) Found in Map
        // 2) Found in ContentMap
    
        // Winner (image) had:
        // 1) Found in Map
    
    // Log-Prior: (I think its correct)
    
    string correct_string2 = "in the statsh file it says stdvectorstdvectordouble summarizestdvectordouble v is that also a function we have to implement thank you";
    object.testCalculateProbability(correct_string2);
    //RESULTS:
    // Log-Likelihood
        // Set had:
        // 1) Found in Map
        // 2) Found in ContentMap
    
        // Winner (statistics) had:
        // 1) Found in Map
    
    // Log-Prior: (I think its correct)

    string incorrect_string = "i have a problem when i log into autograder it says i am not enrolled into any class do i need to worry about that or its just because its not open yet";
    object.testCalculateProbability(incorrect_string);
    
    
    string incorrect_string2 = "the statistics pdf references two files called statscpp and maincpp but neither of those files are in the starter files folder or in the zip will they be posted later or do we have to build them from scratch thanks";
    object.testCalculateProbability(incorrect_string2);
    
    
    string correct_string3 = "for comparison assertion can we use fabs";
    object.testCalculateProbability(correct_string3);


    
    
    
    
    
    
}


/*
TEST(testLog) {
    // doing log (3/8), for e.g., is causing 3/8 round down to 0, and then we're getting log(0) which is -inf;
    // to avoid, need to use doubles instead of ints
    cout << log(3.0/8) << endl;
}
*/

TEST(testCalculateProbability) {
    string test = "weve posted the final exam questions and solutions to the google drive please look over them before submitting a regrade request pin";
    
}

TEST_MAIN()
