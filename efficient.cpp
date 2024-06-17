#include <iostream>
#include <bits/stdc++.h>
#include <sys/resource.h>
#include <errno.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>  
#include<algorithm>
#include<string>
#include <cctype>

#define _BSD_SOURCE
using namespace std;
extern int errno;

map<string, int> mp;
vector<pair<char, char>> res;
vector<int> laligncost, raligncost;

int alpha;

int delta=30;
long getTotalMemory()
{
    struct rusage usage;
    int returnCode = getrusage(RUSAGE_SELF, &usage);
    if (returnCode == 0)
    {
        return usage.ru_maxrss;
    }
    else
    {
        // It should never occur. Check man getrusage for more info to
        // debug.
            // printf("error %d", errno);
            return -1;
    }
}
vector<int> costDP2(const string& x, const string& y)
{
    int m = x.size() + 1; // OPT table's row
    int n = y.size() + 1; // OPT table's col
    vector<vector<int>> dpopt(2, vector<int>(n, INT32_MAX));

    for (int i = 0; i < n; i++) {
        dpopt[0][i] = i * delta;
    }

    for (int i = 1; i < m; i++) {
        dpopt[1][0] = i * delta;
        for (int j = 1; j < n; j++) {
            string ch="";
            ch.push_back(x[i-1]);
            ch.push_back(y[j-1]);

            dpopt[1][j] =
                min(dpopt[0][j - 1] + mp[ch],
                    min(dpopt[0][j] + delta, dpopt[1][j - 1] + delta));
        }
        dpopt[0] = dpopt[1];
    }

    return dpopt[1];
}
vector<vector<int>> costDP(const string& x, const string& y, int flag)
{
    int m= x.size()+1; 
    int n= y.size()+1; 
    
    vector<vector<int>> Mat(m, vector<int>(n, INT32_MAX));

    Mat[0][0] = 0;
    for (int i=1; i<m; i++) 
    {
        Mat[i][0]= i*delta;
    }

    for (int j=1; j<n; j++) 
    {
        Mat[0][j]= j*delta;
    }

    for (int i=1; i<m; i++) 
    {
        for (int j=1; j<n; j++) 
        {
            string ch="";
            ch.push_back(x[i-1]);
            ch.push_back(y[j-1]);

            int alignment = mp[ch];

            Mat[i][j] =min(Mat[i-1][j-1] + alignment, min(delta+Mat[i-1][j], delta+Mat[i][j-1]));
        }
    }

    if(flag==0)
    {
        vector<vector<int>>an(1);
        
        for(int i=0; i<n; i++)
            an[0].push_back(Mat[m-1][i]);

        return an;
    }

    return Mat;
}


vector<pair<char, char>> mergePairs(vector<vector<int>>& table, const string& x, const string& y)
{
    int i=table.size()-1;
    int j=table[0].size()- 1;
    string ch="";

    vector<pair<char, char>> alignment;
    
    while (i>=1||j>=1) 
    {
        ch="";
        ch.push_back(x[i-1]);
        ch.push_back(y[j-1]);

        if(j>=1 && table[i][j]==(table[i][j-1]+delta) )
        {
            alignment.push_back({'_', y[j-1]});
            j--;
        } 

        else if(i>=1&&j>=1 && table[i][j]==(table[i-1][j-1]+mp[ch])) 
        {
            alignment.push_back({x[i-1], y[j-1]});
            i--;
            j--;
        } 
        
        else if(i>=1 && table[i][j] == (table[i-1][j]+delta)) 
        {
            alignment.push_back({x[i-1], '_'});
            i--;
        }
    }

    return alignment;
}


string reverseStrfn(string str)
{
    int i;
    string ans="";
    for(i=str.length()-1;i>=0;i--)
    {
        ans+=str[i];
    }

    return ans;
}

int findmin(vector<int> cost)
{
    int i;
    int minim = cost[0];
    for(i=1;i<cost.size();i++)
    {
        if(cost[i]<minim)
            minim=cost[i];
    }
    return minim;
}

void transferStr(string stra, int starta, int enda, string* strb, int posb)
{
    int i;
    int count=0;
    for(i=starta; i<=enda;i++)
    {
        strb[posb+count]=stra[i];
    }
}


vector<pair<char, char>> costDnC(const string& x, const string& y, int& Mat_value)
{
    int x1len= x.size();
    int ylen = y.size();
    int x1end= x1len-x1len/2;
    string x1_first, x1_sec, xrev_sec, yrev;

    if(x1len<=2 || ylen<=2) 
    {
        vector<vector<int>> Mat_table = costDP(x, y, 1);
        return mergePairs(Mat_table, x, y);
    }
    
    x1_first = x.substr(0, x.size()/2);
    x1_sec = x.substr(x.size()/2, x1end);

    xrev_sec= reverseStrfn(x1_sec);
    yrev= reverseStrfn(y);

    laligncost = costDP2(x1_first, y);
    raligncost = costDP2(xrev_sec, yrev);


    vector<int> cost(laligncost.size(), 0);

    for(int i=0; i<laligncost.size(); i++) 
    {
        cost[i]=raligncost[raligncost.size()-1- i]+laligncost[i];
    }

    int y_cut =  std::min_element(cost.begin(), cost.end()) - cost.begin(); 
    Mat_value = cost[y_cut];

    string t1="", t2="";
    t1= y.substr(0, y_cut);
    t2= y.substr(y_cut, y.size()-y_cut);


    auto cost_first = costDnC(x1_first, t1, alpha);
    auto cost_sec = costDnC(x1_sec, t2, alpha);

    res.resize(cost_sec.size() + cost_first.size());
    
    move(cost_sec.begin(), cost_sec.end(), res.begin());
    move(cost_first.begin(), cost_first.end(), res.begin() + cost_sec.size());

    return res;
}

vector<string> get_inputs(string filename)
{
    string line;
    string input_str1;
    string input_str2;
    string temp_str;   
    int count=0;
    ifstream finy;

    finy.open(filename);
    
    while (getline(finy, line))
    {
            line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

        if(isdigit(line[0])==false)
        {
            if(count==1)
            { 
                // read second input string
                //cout<<line.length()<<endl;
                line = line.substr(0,line.length());
                        
                input_str1=temp_str;
                temp_str = line;
            }
            
            else
            {
                line = line.substr(0,line.length());
                temp_str = line;
                count++;
            }
        }
        else
        {
            temp_str.insert(stoi(line)+1, temp_str);
        } 
    }

    input_str2=temp_str;
    finy.close();
    

    vector<string>input_strings;
    input_strings.push_back(input_str1);
    input_strings.push_back(input_str2);

    return input_strings;
}

int main(int argc, char** argv)
{
    mp["AA"]=0;
    mp["AC"]=110;
    mp["AG"]=48;
    mp["AT"]=94;

    mp["CA"]=110;
    mp["CC"]=0;
    mp["CG"]=118;
    mp["CT"]=48;

    mp["GA"]=48;
    mp["GC"]=118;
    mp["GG"]=0;
    mp["GT"]=110;

    mp["TA"]=94;
    mp["TC"]=48;
    mp["TG"]=110;
    mp["TT"]=0;

    //cout<< "You have entered " << argc<< " arguments:" << "\n";
  
    //for (int i = 1; i < argc; ++i)
       // cout << argv[i] << "\n";

    vector<string>ans = get_inputs(argv[1]);

    int Mat_val=0;

   
     
    string s1 = ans[0];
    string s2 = ans[1];
    int m=ans[0].length();
    int n= ans[1].length();

    struct timeval begin, end;
    gettimeofday(&begin, 0);

    vector<pair<char, char>> res = costDnC(s1,s2,Mat_val);   //Time Complexity: O(mn) and Space Complexity: O(mn)
        
    gettimeofday(&end, 0); 
    
    long seconds = end.tv_sec - begin.tv_sec; 
    long microseconds = end.tv_usec - begin.tv_usec; 
    double totaltime = seconds*1000 + microseconds*1e-3; 
    double totalmemory = getTotalMemory(); 

    ofstream file;
    file.open(argv[2]);

    file<<Mat_val<< endl;

    for(int i=0; i<res.size(); i++)
    {
        //cout<<res[i].first;
        file<<res[i].first;
    }

    //cout<<endl;
    file<<endl;

    for(int i=0; i<res.size(); i++)
    {
        
        
        //cout<<res[i].second;
        file<<res[i].second;
    }

    file<<endl;

    
    file<<totaltime<< endl;
    file<<totalmemory<< endl;;
    file.close();
}
