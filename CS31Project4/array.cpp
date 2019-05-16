// Code for Project 4 : array

#include <iostream>
#include <string>
#include <cassert>

using namespace std;

int appendToAll(string a[], int n, string value);

int lookup(const string a[], int n, string target);

int positionOfMax(const string a[], int n);

int rotateLeft(string a[], int n, int pos);

int countRuns(const string a[], int n);

int flip(string a[], int n);

int differ(const string a1[], int n1, const string a2[], int n2);

int subsequence(const string a1[], int n1, const string a2[], int n2);

int lookupAny(const string a1[], int n1, const string a2[], int n2);

int separate(string a[], int n, string separator);

int main()
{
    string h[7] = { "jill", "hillary", "donald", "tim", "", "evan", "gary" };
    assert(lookup(h, 7, "evan") == 5);
    assert(lookup(h, 7, "donald") == 2);
    assert(lookup(h, 2, "donald") == -1);
    assert(positionOfMax(h, 7) == 3);
    
    string g[4] = { "jill", "hillary", "gary", "mindy" };
    assert(differ(h, 4, g, 4) == 2);
    assert(appendToAll(g, 4, "?") == 4 && g[0] == "jill?" && g[3] == "mindy?");
    assert(rotateLeft(g, 4, 1) == 1 && g[1] == "gary?" && g[3] == "hillary?");
    
    string e[4] = { "donald", "tim", "", "evan" };
    assert(subsequence(h, 7, e, 4) == 2);
    
    string d[5] = { "hillary", "hillary", "hillary", "tim", "tim" };
    assert(countRuns(d, 5) == 2);
    
    string f[3] = { "gary", "donald", "mike" };
    assert(lookupAny(h, 7, f, 3) == 2);
    assert(flip(f, 3) == 3 && f[0] == "mike" && f[2] == "gary");
    
    assert(separate(h, 7, "gary") == 3);
    
    cerr << "All tests succeeded" << endl;
}
int appendToAll(string a[], int n, string value)
{
    if (n < 0)
        return -1;
    else
    {
        for (int i = 0; i < n; i++)
        {
            a[i] += value;
        }
        return n;
    }
}

int lookup(const string a[], int n, string target)
{
    if (n < 0)
        return -1;
    for (int i = 0; i < n; i++)
    {
        if (a[i] == target) // if target is found returns i immediately
            return i;
    }
    return -1;
}

int positionOfMax(const string a[], int n)
{
    /*if (...arrray is empty...)
        return -1;*/
    string max = "";
    if (n == 0 || n < 0)
    {
        return -1;
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (a[i] >= max)
            {
                if (a[i] > max) // changes the value of max only if it is greater
                {
                    max = a[i];
                }
            }
        }
        int temp = 0;
        for(int i = 0; i < n; i++)
        {
            if (a[i] == max)
            {
                temp = i; break; // breaks when max is found
            }
        }
        return temp;
    }
}

int rotateLeft(string a[], int n, int pos)
{
    if (n <= 0)
        return -1;
    if (pos < 0 || pos >= n)
        return -1;
    string temp = a[pos];
    for (int i = pos; i < (n - 1); i++)
    {
        a[i] = a[i + 1]; // changes the value of the string at pos
    }
    a[n - 1] = temp; // changes the value of the last string to that of the one initially at pos
    return pos;

}

int countRuns(const string a[], int n)
{
    if (n < 0)
        return -1;
    int temp = 0;
    for (int i = 0; i < n; i ++)
    {
        if (a[i] != a [i + 1])
        {
            temp++; // increments temp everytime the string is not the same
        }
    }
    return temp;
}

int flip(string a[], int n)
{
    if (n < 0)
        return -1;
    string temp;
    for (int i = 0; i < (n/2); i++) // swaps the values from the start and n
    {
        temp = a[i];
        a[i] = a[n - i - 1];
        a[n - i - 1] = temp;
    }
    return n;
}

int differ(const string a1[], int n1, const string a2[], int n2)
{
    if (n1 < 0 || n2 < 0)
        return -1;
    int temp = 0;
    if (n1 > n2)
    {
        for (int i = 0; i < n2; i++) // runs till a2 runs out of elements
        {
            if (a1[i] == a2[i])
            {
                temp++;
            }
                
        }
    }
    else // n1 <= n2
    {
        for (int i = 0; i < n1; i++) // runs till a1 or both arrays run out of elements
        {
            if (a1[i] == a2[i])
            {
                temp++;
            }
        }
    }
    return temp;
}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
    if (n2 == 0)
        return 0; // 0
    if (n2 > n1)
        return -1; // this is not a valid scenario
    int pos = -1;
    int x = -1 ;
    
    int i, j ;
    for (i = 0; i <= (n1 -n2); i++)
    {
        pos = i; // needed to mark the position
        x = i; // temp var needed to avoid re-iterating from the start of the bigger string
        
        for (j = 0; j < n2; j++)
        {
            if (a2[j] == a1[x])
            {
                x++;
            }
            else
            {
                break;
            }
        }
        
        // If we reached all elements of the smaller string, then return position as success
        if (j == n2)
        {
            return pos;
        }
    }
    
    // reached end and could not find match, return -1
    return -1;
}

int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
    if (n1 < 0 || n2 < 0)
        return -1;
    int i;
    for (i = 0; i < n1; i++)
    {
        for (int j = 0; j < n2; j++)
        {
            if (a1[i] == a2[j]) // checks every element of a2 with one element of a1
                return i;
        }
    }
    return -1;
}

int separate(string a[], int n, string separator)
{
    if (n < 0)
        return -1;
    for(int i = n - 1; i > 0; i--) // using bubble sort
    {
        for (int j = 0; j < i; j++)
        {
            if (a[j] > a[j + 1])
            {
                string temp;
                temp = a[j];
                a[j] = a[j+1];
                a[j + 1] = temp;
            }
        }
    }
    int i;
    for(i = 0; i < n; i++) // finding the first element that is greater than or equal to separator
    {
        if(a[i] >= separator)
            return i;
    }
    return n; // if no element is greater returns n
}


