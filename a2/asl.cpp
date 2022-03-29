#include <iostream>
#include <fstream>
#include <sstream> 
#include <string> 
#include <vector> 
#include <algorithm>

using namespace std;


void makeESTAB(){

}

void makeHeaderRec(vector<string> fileLines, string objFileName){
    string startAddr, progName, progLen;
    bool foundName; 

    ofstream out;
    out.open(objFileName);

    for(int i = 0; i < fileLines.size(); i++){

        if(fileLines.at(i)[0] != '.'){  //makes sure line is not a comment denoted with '.'

            int startPos = fileLines.at(i).find("START"); // looking for the line containg "START"
            if(startPos != string::npos){      
                startAddr = fileLines.at(i).substr(startPos + 9, 5); // start location, +5 since the mem loc is 5 bits
                startAddr.erase(remove(startAddr.begin(), startAddr.end(), ' '), startAddr.end()); // removes whitespace from startAddr
                int l = startAddr.size(); 
                for(int i = 0; i < 6-l; i++)
                    startAddr.append("0"); // fills the whitespace with zeroes  
            }

        // index 9, first line is where the prog name is located 
            if(!foundName){
                progName = fileLines.at(i).substr(8, 6);
                foundName = true; 
            }

            if(fileLines.at(i).find("EOF") != string::npos){   // finding prog len, on line containing 'EOF' 
                progLen = fileLines.at(i).substr(0,4);
                int hex_to_int = stoi(progLen, 0, 16) + 3; // adding hex +3 for format 3/4 progs
                stringstream stream; 
                stream << std::hex << hex_to_int; 
                string res(stream.str()); 
                progLen = res;   
                for(int i = 0; i <= 6 -progLen.size(); i++ ){ // formattig with zeroes 
                    progLen.insert(0, "0");
                } 
            }
        }
    }
    out << "H" << progName << startAddr << progLen << endl; 
}

void makeTextRec(vector<string> fileLines, string objFileName){

}



void makeEndRec(vector<string> fileLines, string objFileName){

    //open obj file

    for(int i = 0; i < fileLines.size(); i++){
        if(fileLines.at(i)[0] != '.'){  
            //add the addr of first instruction to obj file
        }
    }
}


int main(int argc, char *argv[]) {

    string line; 
    vector<string> lines; // vector containing each line of the argv files

    if(argc == 1){
        cout<<"NO FILE PROVIDED, TERMINATING"; 
        return 0; 
    }   

    for(int i = 1; i < argc; i++){
        ifstream f(argv[i]);
        if(!f.is_open()){
            cout << "ERROR OPENING '" << argv[i] << "'";
            return 0;
        }

        while(getline(f, line)){
            lines.push_back(line); // filling vector
        }

        string objFileName = argv[i];
        objFileName = objFileName.substr(0,objFileName.size() - 4).append(".obj"); // creating obj files, filename of format "argv[i].obj"
        ofstream outfile(objFileName);

        makeHeaderRec(lines, objFileName); // calling make header on the empty .obj files 
        makeEndRec(lines,objFileName );
    }

    
}


