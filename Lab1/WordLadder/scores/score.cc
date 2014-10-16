#include "lexicon.h"
#include "strlib.h"

#include <iostream>
#include <vector>
#include <string>

#include <sstream>
#include <fstream>

#include <stdlib.h>
#include <unistd.h>


using namespace std;

const string dir = "./scores/";
const string out =  dir + "yout";
const string c_out = dir + "c_out";
const string input = dir + "traces";

Lexicon dict("../EnglishWords.dat");

/*
 *  @return values
 *  -1 if ladder length is not correct
 *  -2 if ladder's not correct otherwise
 */
int evalLadder(vector<string> ladder,vector<string> base);
vector<string>  split(string s,char c);
int runTest(string input);  //test the correctness of the program
vector<string > lineToLadder(string liyne);
vector<string>  getTestCases(string input);
//remove the temp files
void rmTemp();


int main(int argc,char **argv) {
  chdir("..");
  int res = runTest(input);
  chdir("scores");

  switch (res){
    
  case 0:
    cout<<"Passed!\n";
    break;
  case 1:
    cout<<"Error occurred or timeout while running your program.\n";
    break;
  case 2:
    cout<<"Error occured or timeout while running demo program.\n";
    break;
  default:
    cout<<"There is some mistakes in your evaluation.\n";
    
  }
  return 0;
}

void rmTemp() { system(("rm " + out).c_str());system(("rm " + c_out).c_str());}

/*
 *@ input - the testfile name
 *@ ret   - 0 on success, postive on executing error and negative for failure
 *
 */

vector<string>  getTestCases(string input){
  vector<string > result;
  ifstream fs(input.c_str());
  
  while(!fs.eof()){
    string line;
    getline(fs,line);
    result.push_back(line);
  }
  return result;
}

int runTest(string input) {
  //get test cases;

  vector<string>test_cases = getTestCases(input);
  int test_case = test_cases.size() / 2;

  if(system(("cat " + input +  " | timeout 60 " + "./WordLadder" + " > " + out ).c_str()) != 0)   { rmTemp();return 1;}
  if(system(("cat " + input +  " | timeout 60 " + "./WordLadder-Demo" + " > " + c_out ).c_str()) != 0)  {rmTemp();return 2;}
  
  ifstream fs(out.c_str()) , c_fs(c_out.c_str());
  int count = 0;
  int i = 0;

  while(!fs.eof() && !c_fs.eof() && count < test_case) {
    
    string line,c_line ;
    getline(fs, line);getline(c_fs,c_line);

    vector<string> res = lineToLadder(trim(line)),c_res = lineToLadder(trim(c_line));
    cout<<"Evaluating ladder: "<<test_cases[i++]<<" -> " <<test_cases[i++]<<";Excepted length: "<<((c_res.size())>=2?c_res.size() : 0)<<endl;

    if(evalLadder(res, c_res) < 0){
      fs.close();c_fs.close();
      rmTemp();
      return -1;
    }
    count++;

  }
  fs.close();c_fs.close();
  //  rmTemp();
  return 0;
}

vector<string> lineToLadder(string line){
  //assuming a fixed format
  vector<string> result;
  vector<string>_res  = split(line,':');
  if(_res.size() == 0) 
    return result;
  string end = _res[_res.size()-1];
  return split(trim(end), ' ');
}


vector<string> split(string s,char c){
  
  vector<string > result;
  string field;
  istringstream ss( s );

  while (!ss.eof())         
    {
      getline( ss, field, c );  
      result.push_back(field);
    }  
  return result;
}


int evalLadder(vector<string> ladder,vector<string> base) {
  
  
  if (ladder.size() != base.size())
    {
      return -1;
      
    }
  int size = base.size();

  if(ladder[0] != base[0] || ladder[size-1] != base[size-1]){
    return -2;
  }
  for(int i = 1;i < ladder.size();++i){
    //make sure that each transition is valid
    
    if (ladder[i].size() != ladder[i-1].size() )
      return -3;
    //make sure that the word is valid
    if(!dict.contains(ladder[i])){
      return -4;
    }
    int count = 0;//count the different number
      
    for(int j = 0;j < ladder[i].size();++j){
      if(ladder[i][j] != ladder[i-1][j])
	count ++;
    }
    if(count != 1 ){
      return -5;
    }
  }
  //done
  return 0;
}









