// code for Project 5: satisfy

#include <iostream>
#include <cctype>
#include <cassert>
#include <cstring>
using namespace std;

const int MAX_DOC_LENGTH = 200;
const int MAX_WORD_LENGTH = 20;

int normalizeRules(char word1[][MAX_WORD_LENGTH+1], char word2[][MAX_WORD_LENGTH+1], int distance[], int nRules);
int calculateSatisfaction(const char word1[][MAX_WORD_LENGTH+1], const char word2[][MAX_WORD_LENGTH+1],const int distance[], int nRules, const char document[]);
int main()
{
    const int TEST1_NRULES = 4;
    char test1w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
        "mad",       "deranged", "nefarious", "have"
    };
    char test1w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
        "scientist", "robot",    "plot",      "mad"
    };
    int test1dist[TEST1_NRULES] = {
        2,           4,          1,           13
    };

    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "The mad UCLA scientist unleashed a deranged evil giant robot.") == 2);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "The mad UCLA scientist unleashed    a deranged robot.") == 2);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "**** 2016 ****") == 0);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "  That plot: NEFARIOUS!") == 1);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "deranged deranged robot deranged robot robot") == 1);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "That scientist said two mad scientists suffer from deranged-robot fever.") == 0);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "That mad kid child scientist is a deranged robot") == 1);	//checks basic
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "The ma4986d UCLA scien&&&^tist unleashed a deranged evil giant robot.") == 2);	//makes sure satisfaction is still correct when special characters are used
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "have scientist") == 0);	//checks basic with no satisfaction works properly
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "scientist mad deranged robot nefarious plot have mad") == 4);	//checks basic with all rules satisfied works properly
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, 0,
                                 "The mad UCLA scientist unleashed a deranged evil giant robot") == 0);	//checks that 0 for nRules returns 0
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, -3,
                                 "The mad UCLA scientist unleashed a deranged evil giant robot.") == 0);	//checks that negative nRules works the same as 0
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "How mad is that scientist robot who is %$%^ guy deranged, he probably has a nefarious plot") == 2);	//checks basic
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "I am prof. nefarious") == 0);	//checks that nothing out of range is checked when one word is found at the end
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES, "") == 0);	//checks that empty string works properly
    int testdid[5] = { 1, 2, 3, 4, 5};
    char word1[5][MAX_WORD_LENGTH+1] = {"cat", "dog", "cow", "pig", "chicken"};
    char word2[5][MAX_WORD_LENGTH+1] = {"dog", "cat", "cow", "horse", "sheep"};
    assert(normalizeRules(word1, word2, testdid, 5) == 4);
    
    int test2dist[5] = {
        2,           4,          1,           13,12
    };
    char word3[5][MAX_WORD_LENGTH+1] = {"cat", "dog", "cow", "pig", "chicken"};
    char word4[5][MAX_WORD_LENGTH+1] = {"dog", "cat", "cow", "horse", "sheep"};
    assert(normalizeRules(word3, word4, test2dist, 0) == 0);
    assert(normalizeRules(word3, word4, test2dist, -1) == 0);
 
    int test3dist[5] = {
        2,           4,          1,           13,12
    };
    char word5[5][MAX_WORD_LENGTH+1] = {"c-at", "dog", "co!w", "pig", "chicken"};
    char word6[5][MAX_WORD_LENGTH+1] = {"dog", "cat", "cow", "horse", "sheep"};
    assert(normalizeRules(word5, word6, test3dist, 5) == 3);
  
    char word7[10][MAX_WORD_LENGTH+1] = {"mad", "deranged", "NEFARIOUS", "half-witted", "robot", "plot", "have"};
    char word8[10][MAX_WORD_LENGTH+1] = { "scientist", "robot", "PLOT", "assistant", "deranged","Nefarious", "mad"};
    int test4dist[10] = { 2, 4, 1, 3, 2, 1, 13};
    
    
    
    assert(normalizeRules(word7, word8, test4dist, 7) == 4);
    for ( int i = 0; i < 4; i++)
        cerr << word7[i] << " " << word8[i] << " " << test4dist[i];
    int testdis[10] = {1, 3, -1, 9, 8};
    char word9[5][MAX_WORD_LENGTH+1] = {"cat", "dog", "cow", "pig", "chicken"};
    char word10[5][MAX_WORD_LENGTH+1] = {"dog", "cat", "cow", "horse", "sheep"};
    assert(normalizeRules(word9, word10, testdis, 5) == 3);
    
    int testdis1[10] = {1, 3, -1, 9, 8};
    char word11[5][MAX_WORD_LENGTH+1] = {"", "dog", "cow", "pig", "chicken"};
    char word12[5][MAX_WORD_LENGTH+1] = {"dog", "cat", "cow", "horse", "sheep"};
    assert(normalizeRules(word11, word12, testdis1, 5) == 3);

    
    cerr << "All tests succeeded" << endl;
}

int normalizeRules(char word1[][MAX_WORD_LENGTH+1], char word2[][MAX_WORD_LENGTH+1], int distance[], int nRules)
{
    if (nRules <= 0)
        return 0;// what you would return if nRules was 0)
    for (int i = 0; i < nRules; i++)// check for empty strings in word1
    {
        if(strcmp(word1[i], "") == 0)
        {
            for (int k = i; k < (nRules - 1); k++) // use function rotateLeft
            {
                strcpy(word1[k],word1[k + 1]);
                strcpy(word2[k],word2[k + 1]);
                distance[k] = distance [k + 1];
                cerr << "1empty" << endl;
            }
            nRules--; cerr << nRules << endl;
            i--;
        }
        
        
    }
    for (int i = 0; i < nRules; i++)// check for empty strings in word2
    {
        if(strcmp(word2[i], "") == 0)
        {
            for (int k = i; k < (nRules - 1); k++) // use function rotateLeft
            {
                strcpy(word2[k],word2[k + 1]);
                strcpy(word1[k],word1[k + 1]);
                distance[k] = distance [k + 1];
                cerr << "2empty" << endl;
            }
            nRules--; cerr << nRules << endl;
            i--;
        }
        
    }
    for (int i = 0; i < nRules; i++)// convert all chars in word1 to lower case;
    {
        for(int j = 0; j < strlen(word1[i]); j++)
        {
            word1[i][j] = tolower(word1[i][j]);
            if(!isalpha(word1[i][j])) //remove words that have chars that are not letters
            {
                for (int k = i; k < (nRules - 1); k++) // use function rotateLeft
                {
                    strcpy(word1[k],word1[k + 1]);
                    strcpy(word2[k],word2[k + 1]);
                    distance[k] = distance [k + 1];
                }
                nRules--; cerr << nRules << endl;
                i--;
                break;
            }
        }
        
    }
    for (int i = 0; i < nRules; i++)// convert all chars in word2 to lower case;
    {
        for(int j = 0; j < strlen(word2[i]); j++)
        {
            word2[i][j] = tolower(word2[i][j]);
            if(!isalpha(word2[i][j])) //remove words that have chars that are not letters
            {
                for (int k = i; k < (nRules - 1); k++) // use function rotateLeft
                {
                    strcpy(word2[k],word2[k + 1]);
                    strcpy(word1[k],word1[k + 1]);
                    distance[k] = distance [k + 1];
                }
                nRules--; cerr << nRules << endl;
                i--;
                break;
            }
            
        }
        
    }
    for (int i = 0; i < nRules; i++)
    {
        if(distance[i] <= 0) // distance is not positive
        {
            for (int k = i; k < (nRules - 1); k++) // use function rotateLeft
            {
                strcpy(word2[k],word2[k + 1]);
                strcpy(word1[k],word1[k + 1]);
                distance[k] = distance [k + 1];
            }
            nRules--; cerr << nRules << endl;
            i--;
        }
        
    }
    for(int i = 0; i < nRules; i++)
    {
        for (int j = 0; i >= 0 && i < nRules && j < nRules; j++)
        {
            if (strcmp(word1[i], word1[j]) == 0) // check if two w1 are the same
            {
                cout << "w1 =" << endl;
                if (i != j && strcmp(word2[i], word2[j]) == 0) // check if corresponding w2 are the same
                {
                    cout << "w2 =" << endl;
                    if (distance[i] < distance [j])
                    {
                        for (int k = i; k < (nRules - 1); k++) // use function rotateLeft
                        {
                            strcpy(word2[k],word2[k + 1]);
                            strcpy(word1[k],word1[k + 1]);
                            distance[k] = distance [k + 1];
                        }
                        nRules--;
                        i--;
                    }
                    else
                    {
                        for (int k = j; k < (nRules - 1); k++) // use function rotateLeft
                        {
                            strcpy(word2[k],word2[k + 1]);
                            strcpy(word1[k],word1[k + 1]);
                            distance[k] = distance [k + 1];
                        }
                        nRules--; cerr << nRules << endl;
                        j--;
                    }
                }
            }
        }
    }
    for(int i = 0; i < nRules; i++)
    {
        for (int j = i + 1;  i >= 0 && i < nRules && j < nRules; j++)
        {
            if (i != j && strcmp(word1[i], word2[j]) == 0) // check if w1 the is the same as any w2
            {
                cout << "w1 = w2" << endl;
                if (strcmp(word2[i], word1[j]) == 0) // check if corresponding w2 is the same as w1
                {
                    cout << "w2 = w1" << endl;
                    if (distance[i] < distance [j])
                    {
                        for (int k = i; k < (nRules - 1); k++) // use function rotateLeft
                        {
                            strcpy(word2[k],word2[k + 1]);
                            strcpy(word1[k],word1[k + 1]);
                            distance[k] = distance [k + 1];
                        }
                        nRules--;
                        i--;
                    }
                    else
                    {
                        for (int k = j; k < (nRules - 1); k++) // use function rotateLeft
                        {
                            strcpy(word2[k],word2[k + 1]);
                            strcpy(word1[k],word1[k + 1]);
                            distance[k] = distance [k + 1];
                        }
                        nRules--; cerr << nRules << endl;
                        j--;
                    }
                }
            }
        }
    }
    cerr << nRules << endl;
    return nRules;
}

int calculateSatisfaction(const char word1[][MAX_WORD_LENGTH+1], const char word2[][MAX_WORD_LENGTH+1],const int distance[], int nRules, const char document[])
{
    if (nRules < 0)
    {
        nRules = 0;
    }
    char doc[100][MAX_DOC_LENGTH] = {"\0"}; // HUNDRED is the max number of words a doc can have
    int satisfactionscore = 0;
    char newDocument[MAX_DOC_LENGTH];
    for (int i = 0, docmax = 0; i < strlen(document); i++) // create a new doc without non alphabetic characters
    {
        if(isalpha(document[i]) || document[i] == ' ')
        {
               if(document[i] == ' ')
               {
                   if( i > 0 && document[i - 1] == ' ')
                   {
                       continue;
                   }
               }
    
            newDocument[docmax++] = tolower(document[i]);
            newDocument[docmax] = '\0';
           }
        
    }
    cerr << newDocument << endl;
    int wcount = 0;
    for (int i = 0, j = 0; i < strlen(newDocument); i++)
    {
        if (newDocument[i] == ' ')
        {
            if( i > 0 && newDocument[i - 1] == ' ')
            {
                continue;
            }
            wcount++;
            j = 0;
        }
        else
        {
            doc[wcount][j++] = newDocument[i];
            doc[wcount][j] = '\0';
        }
        
    }
    wcount++;
    cout << endl;
    for (int i = 0; i < nRules; i++)
    {
        for (int j = 0; j < wcount && i < nRules; j++)
        {
            cerr << word1[i] << " " << doc[j] << endl;
            if(strcmp(word1[i], doc[j]) == 0)// check if word1 exists in word
            {
                cerr << "w1 is equal" << endl;
                for(int k = 0; k < wcount; k++)
                {
                    cerr << word2[i] << " " << doc[k] << endl;
                    if(strcmp(word2[i], doc[k]) == 0)
                    {
                        cerr << "w2 is equal" << endl;
                        cerr << abs(j - k) << endl;
                        if(abs(j - k) <= distance[i])
                        {
                            cerr << satisfactionscore << endl;
                            satisfactionscore++;
                            cerr << satisfactionscore << endl;
                            i++; j = - 1;
                            cerr << i << " " << j << endl;
                            break;
                        }
                    }
                }
            }
            
        }
    }
    
    return satisfactionscore;
}
