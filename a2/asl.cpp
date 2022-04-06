#include <iostream>
#include <fstream>
#include <sstream> 
#include <string> 
#include <vector> 
#include <algorithm>
#include <math.h>
#include <iomanip>

using namespace std;

void makeESTAB(){

}

string makeHeaderRec(vector<string> fileLines, string objFileName){
    string startAddr, progName, progLen;
    bool foundName = false; 

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


vector<string> makeRelRec(vector<string> fileLines, string objFileName ){ // refer record
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

    return refSymbols;
}

string helper(string lines) {
	string temp = "";
	vector<char> result;
	for (char ch : lines)
    		{
        // Copy only alphabetical characters and numeric digits
            		result.push_back(ch);
    		}
    	if(result.size() > 50) {
    		for(int i = 51; i < result.size(); i++) {
    			temp += result.at(i);
    		}
    		return temp;
    	} else {
    		return "";
    	}
}

void makeTextRec(vector<string> fileLines, string objFileName){
    ofstream out;
    out.open(objFileName, fstream::app);
    vector<string> objectCode;
    vector<string> address;
    int counter = 0;
    int startPos = 0;
    int endPos = 0;
    string objectcode = "";
    unsigned int length = 0;
    string temp;
        for(int i = 0; i < fileLines.size(); i++){
        if(fileLines.at(i)[0] != '.') {
            if (std::find(address.begin(), address.end(), fileLines.at(i).substr(0,4)) == address.end() and fileLines.at(i).substr(0,4) != "")
            {
              address.push_back(fileLines.at(i).substr(0,4));
            }
            temp = fileLines.at(i);
            temp = helper(temp);
            if(temp != "") {
                objectCode.push_back(temp);
            }
        }
    }
    int j = 0;
    int total = 0;
    for(int i = 0; i < objectCode.size(); i++) {
        cout << objectCode.at(i) << endl;
    }
    for(int i = 0; i < objectCode.size(); i++) {
        j = floor(objectCode.at(i).size()/2);
        length += j;
        total += j;
        endPos = i;
        if(i == (objectCode.size()-1) or ((j == 4) and (length >= (59-3))) or ((j==5) and (length >= (59-4)))) {
            for(int f = startPos; f <= endPos; f++) {
                objectcode += objectCode.at(f);
            }
            out << "T" << address.at(startPos) << std::hex << length << objectcode;
            out << endl;
            startPos = endPos+1;
            endPos++;
            length = 0;
        } else {
            continue;
        }
    }
}


void makeModifRec(vector<string> fileLines, vector<string> refSymbols, string objFileName ){
    ofstream out;
	out.open(objFileName, fstream::app);
    vector<string> m_recLines; 
	
    string progName; 
    bool foundName = false; 

    for(auto line : fileLines){
        if(line[0] != '.'){

            string addr = line.substr(0,4);

            if(!foundName){
                progName = line.substr(8, 6);
                foundName = true; 
            }

            for( auto symbol : refSymbols){
                 
                symbol.erase(remove(symbol.begin(), symbol.end(), ' '), symbol.end());
              
                int has_symbol = line.find(symbol);
                int has_base = line.find("BASE"); 
                if(has_symbol != string::npos && has_base == string::npos && addr != "0000"){ // make sure its not a BASE declaration
                     
                    string addr = line.substr(0,4);
                    int hex_to_int = stoi(addr, 0, 16) + 1; // adding hex +1
                    stringstream stream; 
                    stream << std::hex << hex_to_int; 
                    string res(stream.str()); 
                    transform(res.begin(), res.end(), res.begin(), ::toupper);
                    addr = res;   
                    addr.insert(0, 6-addr.size(), '0');
                   // out << "M" << addr << "05+" << symbol <<endl; 
                    m_recLines.insert(m_recLines.end(), "M" + addr + "05+" + symbol);
                }
            }

            int has_format4 = line.find('+');
            int has_s; 

            for (auto s : refSymbols){
                has_s = line.find(s);
            }

            bool skip_line = (has_s != string::npos); // skips line if contains symbol, avoids duplicates in format 4

            if(has_format4 != string::npos && !skip_line && addr != "0000"){
                
                int hex_to_int = stoi(addr, 0, 16) + 1; // adding hex +1
                stringstream stream; 
                stream << std::hex << hex_to_int; 
                string res(stream.str()); 
                transform(res.begin(), res.end(), res.begin(), ::toupper);
                addr = res;   
                addr.insert(0, 6-addr.size(), '0');
               // out << "M" << addr << "05+" << progName <<endl; 
                m_recLines.insert(m_recLines.end(), "M"+ addr+ "05+"+ progName);
            }
        }
    }




    std::vector<string>::reverse_iterator it = m_recLines.rbegin(); // need to make this delete duplicates
    while (it != m_recLines.rend())
    {
        std::cout<<*it<<", ";
        it++;
    }

    for( auto recLine : m_recLines){
     //   cout << recLine << endl; 
      continue;
    }




}


void makeEndRec(string objFileName, string startAddr){
	ofstream out;
	out.open(objFileName, fstream::app);
	out << "E" << startAddr << endl; 	
}


int main(int argc, char *argv[]) {

    string line;
    string startAddr;
     // vector containing each line of the argv files

    if(argc == 1){
        cout<<"NO FILE PROVIDED, TERMINATING"; 
        return 0; 
    }   

    for(int i = 1; i < argc; i++){ // runs thru all user inputted files
        ifstream f(argv[i]);
        vector<string> lines; //  this must be in the for loop, or all files will be written to lines vector
        string line;
        string startAddr; 
       
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
        vector<string> ext_references = makeRelRec(lines, objFileName);

    
        makeTextRec(lines, objFileName);
        makeModifRec(lines, ext_references, objFileName);
        makeEndRec(objFileName, startAddr);
        
    } 
}



