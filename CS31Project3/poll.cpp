// Code for Project 3
// poll

#include <iostream>
#include <string>
#include <cctype>
#include <cassert>
using namespace std;

bool isValidUppercaseStateCode(string stateCode);

bool hasCorrectSyntax(string pollData);

bool zeroElectoralVotes (string pollData);

int countVotes(string pollData, char party, int& voteCount);

int main()
{
    assert(hasCorrectSyntax("TX38RCA55D"));
    assert(hasCorrectSyntax("Tx38RCA55d"));
    assert(!hasCorrectSyntax("MX38RCA55D"));
    int votes;
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("TX38RCA55DMs6rnY29dUT06L", 'd', votes) == 0  &&  votes == 84);
    votes = -999;
    assert(countVotes("TX38RCA55DMs6rnY29dUT06L", 'D', votes) == 0  &&  votes == 84);
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("@TX38RCA55D", 'D', votes) == 1 && votes == -999);
    assert(countVotes(" ", 'D', votes) == 1 && votes == -999);
    assert(countVotes("TX38R CA55D", 'D', votes) == 1 && votes == -999);
    assert(countVotes("Shreya", 's', votes) == 1 && votes == -999);
    assert(countVotes("9989359100", 'k', votes) == 1 && votes == -999);
    assert(countVotes("TX38RCA55DCa", 'r', votes) == 1 && votes == -999);
    assert(countVotes("", 'd', votes) == 0 && votes == 0);
    votes = -999;
    assert(countVotes("TX38RCA55D", '%', votes) == 3  &&  votes == -999);
    assert(countVotes("TX38RCA55D", '3', votes) == 3 && votes == -999);
    assert(countVotes("TX38RCA55D", ' ', votes) == 3 && votes == -999);
    assert(countVotes("TX38RCA55DCa", 'r', votes) == 1 && votes == -999);
    assert(countVotes("TX0RCA55D", 'm', votes) == 2 && votes == -999);
    assert(countVotes("TX0RCA00D", 'm', votes) == 2 && votes == -999);
    assert(countVotes("TX4RCA00D", 'm', votes) == 2 && votes == -999);
    assert(!hasCorrectSyntax("ny29dtx38+hi4d"));
    int v = -888;
    assert(countVotes("ny29dtx38+hi4d", 'd', v) == 1 && v == -888);
    assert(countVotes("ny29dtx383hi4d", 'd', v) == 1 && v == -888);
    cerr << "Got it." ;
}


bool isValidUppercaseStateCode(string stateCode)
{
    const string codes =
    "AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
    "KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC."
    "ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
    return (stateCode.size() == 2  &&
            stateCode.find('.') == string::npos  &&  // no '.' in stateCode
            codes.find(stateCode) != string::npos);  // match found
}

bool hasCorrectSyntax(string pollData)
{
    //This function returns true if its parameter is a poll data string (i.e., it meets the definition above), or false otherwise.
    if (pollData == "")
        return true;
    for (int i = 0; i < pollData.size(); i++)
    {
        pollData[i] = toupper(pollData[i]);
    }
    int i = 0;
    while (i < pollData.size())
    {
        string stateCode;
        if (isalpha(pollData[i]) && isalpha(pollData[i+1]) && isValidUppercaseStateCode(pollData.substr(i,2)))
            {
                if ((isdigit(pollData[i+2])) && (isdigit(pollData[i+3])))
                {
                    {
                        if (isalpha(pollData[i+4]))
                            i +=5;
                        else
                            return false;
                    }
                }
                else if (isdigit(pollData[i+2]))
                {
                    if(isalpha(pollData[i+3]))
                        i += 4;
                    else
                        return false;
                }
                else
                    return false;
            }
        else
            return false;
            
    }
    return true;
}

bool zeroElectoralVotes (string pollData)
{
    //This function returns true if pollData is a poll data string in which at least one state forecast predicts zero electoral votes for that state, or false otherwise.
    int i = 0;
    while(i < pollData.size())
    {
        if ((isdigit(pollData[i+2])) && (isdigit(pollData[i+3])))
        {
            if ((pollData[i+2] == '0') && (pollData[i+3] == '0'))
                return true;
            else
                i +=5;
        }
        else if (isdigit(pollData[i+2]))
        {
            if(pollData[i+2] == '0')
                return true;
            else
                i += 4;
        }
    }
    return false;
}

int changeVoteCount(string pollData, char party, int& voteCount)
{
    //Sets and returns voteCount to the total number of electoral votes that pollData predicts the candidate of the party indicated by party will get TX38RCA55DMs6rnY29dUT06L
    int i = 0, temp1 = 0, temp2 = 0, temp = 0;
    while (i < pollData.size())
    {
        
        if (toupper(pollData[i]) == toupper(party) && (isdigit(pollData[i-1])))
        {
            if ((isdigit(pollData[i-2])))
            {
                temp1 = 10*(pollData[i-2] - '0'); //To convert characters to int
                temp2 = pollData[i-1] - '0';
                temp = temp + temp1 + temp2;
                i +=5;
            }
            else
            {
                temp = temp + pollData[i-1] - '0';
                i += 4;
            }
        }
        else
        {
            i++;
        }
    }
    voteCount = temp;
    return voteCount;
}

int countVotes(string pollData, char party, int& voteCount)
{
    if (!hasCorrectSyntax(pollData))
        return 1;
    else if (zeroElectoralVotes(pollData))
        return 2;
    else if (!isalpha(party))
        return 3;
    else
        changeVoteCount(pollData, party, voteCount);
    return 0;
}



