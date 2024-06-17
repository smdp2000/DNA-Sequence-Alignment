#include <iostream>
#include <bits/stdc++.h>
#include <sys/resource.h>
#include <errno.h>
#include <stdio.h>
#include <sys/time.h>
#include<unistd.h>
#include<algorithm>
#include<string>
#include <cctype>


#include<chrono>
#define _BSD_SOURCE
using namespace std;

int m,n;

vector<vector<int>> Mat; //mat size - Change it to: [20001][20001]
unordered_map<char, unordered_map<char, int>>mismatch_penalty_table_;
map<string, int> mp;
int delta = 30;
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





//dpcost
int dpcost(string x, string y, int count)
{
    int m=x.length();
    int n=y.length();
    int i,j;
    for(i=0;i<=m;i++)
    {
        for(j=0;j<=n;j++)
        {
            Mat[i][j]=0;
        }
    }
    
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
 
    for(i=0;i<=m;i++)
    {
        Mat[i][0]=delta*i;
    }
    for(j=0;j<=n;j++)
    {
        Mat[0][j]=delta*j;
    }
    for(i=1;i<=m;i++)
    {
        for(j=1;j<=n;j++)
        {
            string ch="";
            ch.push_back(x[i-1]);
            ch.push_back(y[j-1]);

            int alignment = mp[ch];
            
            if(x[i-1]==y[j-1])
            {
                Mat[i][j]= Mat[i-1][j-1];
            }
            else
            {
                Mat[i][j]=min({Mat[i-1][j-1]+alignment, delta+Mat[i-1][j], delta+Mat[i][j-1]});
            }
        }
    }
    
     //"Min cost: "
    return Mat[m][n];
    //constructing strings
   
}






//dp alignment
vector<string> alignment(string x, string y, vector<vector<int>>&Mat){
    int i=x.length();
    int j=y.length();
    int totallen = x.length()+y.length();
    
    int ptr1 = totallen;
    int ptr2 = totallen;
    
    string finalstr1(totallen, '\0'), finalstr2(totallen, '\0');
    while(i>0 && j>0)
    {
        string ch="";
        ch.push_back(x[i-1]);
        ch.push_back(y[j-1]);

        int alignment = mp[ch];
                
        if((x[i-1]==y[j-1]) || (Mat[i][j] == Mat[i-1][j-1] + alignment))
        {
            finalstr1[ptr1]=x[i-1];
            finalstr2[ptr2]=y[j-1];
            ptr1--;
            ptr2--;
            i--;
            j--;
        }
        else if(Mat[i][j] == Mat[i-1][j] + delta)
        {
            finalstr1[ptr1] =x[i-1];
            finalstr2[ptr2] = '_';
            ptr1--;
            ptr2--;
            i--;
        
        }
        else if(Mat[i][j] == Mat[i][j-1]+delta)
        {
            finalstr1[ptr1]='_';
            finalstr2[ptr2]=y[j-1];
            ptr1--;
            ptr2--;
            j--;
        }
    }
    while(ptr1>0)
    {
        if (i>0)
        {
            i=i-1;
            finalstr1[ptr1] = x[i];
        }
        else 
        {
            finalstr1[ptr1] = '_';
        }
        ptr1=ptr1-1;
    }
    while (ptr2>0)
    {
        if (j > 0) 
        {
            j=j-1;
            finalstr2[ptr2] = y[j];
        }
        else 
        {
            finalstr2[ptr2] = '_';
        }
        
        ptr2=ptr2-1;
    }
    
    int start = 1;
    for(i=totallen; i>= 1; i--)
    {
        if(finalstr1[i]=='_' && finalstr2[i]=='_')
        {
            start= i+1;
            break;
        }
    }
    
    string ans1= finalstr1.substr(start, totallen-start);
    string ans2 = finalstr2.substr(start, totallen-start);

    return {ans1,ans2};
    
}

vector<string> get_inputs(string filename){

    string line;
    string input_str1;
    string input_str2;
    string temp_str;   
    int count=0;
    ifstream finy;

    finy.open(filename);
            while (getline(finy, line)) {
                line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

                if(isdigit(line[0])==false)
                {
                    if(count==1){ // read second input string
                        
                        
                        line = line.substr(0,line.length());
                        
                        input_str1=temp_str;
                        temp_str = line;
                    }
                    else{
                        
                        line = line.substr(0,line.length());
                        temp_str = line;
                        count++;
                    }
                }
                else{
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

 //cout << "You have entered " << argc
         //<< " arguments:" << "\n";
  
    //for (int i = 1; i < argc; ++i)
        //cout << argv[i] << "\n";

vector<string>ans = get_inputs(argv[1]);


    string x = ans[0];
    string y = ans[1];
     int m=ans[0].length();
    int n= ans[1].length();
    Mat.resize(m+1, vector<int>(n+1));

    struct timeval begin, end;
    gettimeofday(&begin, 0);

    int Matcost = dpcost(x,y,0);    //Time Complexity: O(mn) and Space Complexity: O(mn)
     vector<string>alans = alignment(x,y, Mat);

    
        gettimeofday(&end, 0); 

    double totalmemory = getTotalMemory(); 
    
    long seconds = end.tv_sec - begin.tv_sec; 
    long microseconds = end.tv_usec - begin.tv_usec; 
    double totaltime = seconds*1000 + microseconds*1e-3; 
    //printf("%f\n", totaltime); 
    //printf("%f\n", totalmemory);

    ofstream file;
    file.open(argv[2]);
   
    file<<Matcost<< endl;
    file<<alans[0]<< endl;
    file<<alans[1]<< endl;
    file<<totaltime<< endl;
    file<<totalmemory<< endl;;
    file.close();


}
