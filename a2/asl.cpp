#include <iostream>
#include <fstream>
#include <sstream> 
#include <string> 
#include <vector> 
#include <algorithm>

using namespace std;

void makeESTAB(){

}

string makeHeaderRec(vector<string> fileLines, string objFileName){
    string startAddr, progName, progLen;
    bool foundName; 

    ofstream out;
    out.open(objFileName, fstream::app);

    for(auto line : fileLines){

        if(line[0] != '.'){  //makes sure line is not a comment denoted with '.'

            int startPos = line.find("START"); // looking for the line containg "START"
            if(startPos != string::npos){      
                startAddr = line.substr(startPos + 9, 5); // start location, +5 since the mem loc is 5 bits
                startAddr.erase(remove(startAddr.begin(), startAddr.end(), ' '), startAddr.end()); // removes whitespace from startAddr
                int l = startAddr.size(); 
                for(int i = 0; i < 6-l; i++)
                    startAddr.append("0"); // fills the whitespace with zeroes  
            }

        // index 9, first line is where the prog name is located 
            if(!foundName){
                progName = line.substr(8, 6);
                foundName = true; 
            }

            if(line.find("EOF") != string::npos){   // finding prog len, on line containing 'EOF' 
                progLen = line.substr(0,4);
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
    return startAddr;
}


void makeDefRec(vector<string> fileLines, string objFileName ){ // define record 
    ofstream out;
	out.open(objFileName, fstream::app);
    out << "D";

    vector<string> defSymbols;

    for(auto line : fileLines){
        if(line[0] != '.'){
            int extDPos = line.find("EXTDEF");
            if(extDPos != string::npos){ // if EXTREF exists in this line
                stringstream ss(line.substr(extDPos+9)); //string of extrenal symbols, seperated by comma
                
                while(ss.good()){ // filling the vector defSymbols with symbols defined in EXTDREF
                    string substr; 
                    getline(ss, substr, ',');
                    substr.resize(6, ' '); // formats to 6 char length
                    defSymbols.push_back(substr); 
                } 
            }

            for(int j = 0; j < defSymbols.size(); j++ ){
                int symLocIndex = line.substr(8,14).find(defSymbols.at(j));
                if(symLocIndex != string::npos){ 
                    string addr = line.substr(0,6);
                    addr.erase(remove(addr.begin(), addr.end(), ' '), addr.end()); // remove whitespace
                    addr.insert(0, 6-addr.size(), '0'); // formatting 0's to front of addr str
                    out << defSymbols.at(j) << addr; // writing to obj file
                }
            }
        }
    }

    out << endl; 
}


void makeRelRec(vector<string> fileLines, string objFileName ){ // refer record
    ofstream out;
	out.open(objFileName, fstream::app);
    out << "R";

    vector<string> refSymbols;

    for(auto line : fileLines){
        if(line[0] != '.'){
            int extRPos = line.find("EXTREF");
            if(extRPos != string::npos){ // if EXTREF exists in this line
                stringstream ss(line.substr(extRPos+9)); //string of extrenal symbols, seperated by comma
                while(ss.good()){ // filling the vector refSymbols with symbols defined in EXTREF
                    string substr; 
                    getline(ss, substr, ',');
                    substr.resize(6, ' '); // formats to 6 char length
                    refSymbols.push_back(substr); 
                } 
            }
        }
    }

    for(auto sym : refSymbols)
        out << sym; 
    out << endl; 
}


void makeTextRec(){

}


void makeModifRec(vector<string> fileLines, string objFileName ){

}


void makeEndRec(string objFileName, string startAddr){
	ofstream out;
	out.open(objFileName, fstream::app);
	out << "E" << startAddr << endl; 	
}


int main(int argc, char *argv[]) {

    string line;
    string startAddr;
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

        startAddr = makeHeaderRec(lines, objFileName); // calling make header on the empty .obj files 
        makeDefRec(lines, objFileName);
        makeRelRec(lines, objFileName);
        makeEndRec(objFileName, startAddr);
    } 
}



