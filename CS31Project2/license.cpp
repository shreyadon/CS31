// Code for Project 2
// Report License Fee


#include <iostream>
#include <cstring>

using namespace std;
int main()
{
    string identification, country;
    double expectedRevenue, licenseFee;
    cout << "Identification: ";
    getline(cin, identification);
    
    if (identification == "")
    {
        cout << "---" << endl;
        cout << "You must enter a property identification." << endl;
        return 1;
    }
    else
    {
        cout << "Expected revenue (in millions): ";
        cin >> expectedRevenue;
        
        if (expectedRevenue < 0)
        {
            cout << "---" << endl;
            cout << "The expected revenue must be nonnegative." << endl;
            return 1;
        }
        
        else
        {
            cin.ignore(10000, '\n'); // causes characters up to and including the next newline character          to be consumed and discarded
            cout << "Country: ";
            getline(cin, country);
            
            if (country == "")
            {
                cout << "---" << endl;
                cout << "You must enter a country." << endl;
                return 1;
            }
            
            else
            {
                cout.setf(ios::fixed); //by default means write the number out in scientific form
                cout.precision(3);
                
                if (country == "Turkey" || country == "UAE")
                {
                    if (expectedRevenue <= 20)
                    {
                        licenseFee = 0.181*expectedRevenue;
                        
                    }
                    else if (expectedRevenue <=50)
                    {
                        licenseFee = (((expectedRevenue-20) * 0.217) + (20 * 0.181));
                        
                    }
                    else
                    {
                        licenseFee = (((expectedRevenue-50) * 0.230) + (30 * 0.217) + (20 * 0.181));
                        
                    }
                }
                
                else
                {
                    if (expectedRevenue <= 20)
                    {
                        licenseFee = 0.181*expectedRevenue;
                        
                    }
                    else if (expectedRevenue <=50)
                    {
                        licenseFee = (((expectedRevenue-20) * 0.203) + (20 * 0.181));
                        
                    }
                    else
                    {
                        licenseFee = (((expectedRevenue-50) * 0.230) + (30 * 0.203) + (20 * 0.181));
                        
                    }
                }
                
                cout << "---" << endl;
                cout << "The license fee for " << identification << " is $" << licenseFee << " million." << endl;
            }
        }
    }
    return 0;
}
