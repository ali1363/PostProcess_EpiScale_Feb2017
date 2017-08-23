//main.cpp
//======================
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include "data.h"
#include "stat.h"

using namespace std;
  
bool parse_File(string, vector<Data*>&);

int main()
{
    vector<vector<Data*>> data;
	vector<Data*> cells;
    string Initial ("low/detailedStat_B_") ; 
    string Format (".txt"); 
    string Number;
    string FileName;
    int digits;
    bool Finished = false;

    for (int Ti = 1; !Finished; Ti++)
    {
        digits = ceil(log10(Ti + 1)); 
        if (digits==1 || digits == 0) {
            Number = "0000" + to_string(Ti);	
        }
        else if (digits == 2) {
            Number = "000" + to_string(Ti);
        }
        else if (digits == 3) {
            Number = "00" + to_string(Ti);
        }
        else if (digits == 4) {
            Number = "0" + to_string(Ti);
        }

		FileName = Initial + Number + Format;
        //parse_File returns:
        //  false if it can't read file => Finished should be true
        //  true if it can read file => Finished should be false
        Finished = !parse_File(FileName,cells);

        if (!Finished) {
            //push file's cell data onto main 2d vector
            data.push_back(cells);
            //clear temp vector for next file
            cells.clear();
        } 

    } //finished reading all the files

	//Bounds vectors
	vector<double> low = {0.0, 0.5, 0.9};
	vector<double> high = {0.9, 0.95, 0.98, 1.0};

    // Computing Cell Area statistical analysis
    cout << "Compute Area Stats" << endl;
	cout << "	Non mitotic" << endl;
    Area_Stat area_low("Area_Low",low);
    area_low.add_values(data);
    area_low.calc_Stats();
    area_low.display();
    area_low.print_Graph_Output();

	cout << "	Mitotic" << endl;
	Area_Stat area_high("Area_High",high);
	area_high.add_values(data);
	area_high.calc_Stats();
	area_high.display();
	area_high.print_Graph_Output();
	area_high.print_Raw_Data();

    cout << "Area completed" << endl << endl;
    
    cout << "Compute Perimeter Stats" << endl;
    cout << "	Non mitotic" << endl;
	Perim_Stat perim_low("Perim_Low",low);
    perim_low.add_values(data);
    perim_low.calc_Stats();
    perim_low.display();
    perim_low.print_Graph_Output();
	
	cout << "	Mitotic" << endl;
	Perim_Stat perim_high("Perim_High",high);
    perim_high.add_values(data);
    perim_high.calc_Stats();
    perim_high.display();
    perim_high.print_Graph_Output();
    cout << "Perim completed" << endl;
    
    cout << "Finished with everything" << endl;

	cout << true << ' ' << false << endl;
    return 0;
}

bool parse_File(string FileName, vector<Data*>& cells) {
    
    ifstream ifs(FileName.c_str());

    if (!ifs) {
        cout << FileName << " is not available" << endl;
        //return false because it couldn't read file
        return false;
    }
    //cout << FileName << " is good" << endl;

    stringstream ss; 
    string line;
    string temp;
    int num;
    double val;
    Data* cell;

    // while loop to parse one file
    while (getline(ifs,line)) {
        //cout << "line: " << line << endl;
        ss.str(line);
        getline(ss,temp,':');

        //cout << "temp: " << temp << endl;
            
        if(temp == "CellRank") {
            ss >> num;
            //cout << "CellRank: " << num << endl;
            if (num > 0) {
                //first push_back old cell onto vector
                cells.push_back(cell);
            }
            //creates new current cell to store information
            cell = new Data;
            cell->CellRank = num;
        }
        else if (temp == "    GrowthProgress") {
            ss >> val;
            cell->GrowthProgress = val;
        }
        else if (temp == "    MembrGrowthProgress") {
            ss >> val;
            cell->MembrGrowthProgress = val;
        }
        else if (temp == "    IsBoundrayCell") { //fix later
            ss >> num;
			if (num == 0) {
				cell->IsBoundaryCell = false;
			}
			else if (num == 1) {
				cell->IsBoundaryCell = true;
			}
			else {
				cout << "ISBOUNDARYCELL ERROR -- num = " << num << endl;
			}
        }
        else if (temp == "    NumOfNeighbors") {
            ss >> num;
            cell->NumOfNeighbors = num;
        }
        else if (temp == "    CellArea") {
            ss >> val;
            cell->CellArea = val;
        }
        else if (temp == "    CellPerim") {
            ss >> val;
            cell->CellPerim = val;
        }
        else if (temp == "    NeighborCellsOrdered") {
            vector<int> neigh;
            //throw away the '{' and '}'
            getline(ss,temp,'{');
            getline(ss,temp,'}');
            ss.clear();
            ss.str(temp);
            while(!ss.eof()) {
                ss >> num;
                neigh.push_back(num);
            }
            //at the moment, pushes one too many
            neigh.pop_back();
            cell->NeighborCellsOrdered = neigh;
        }
        else if (temp == "    NumberOfPointsInContactOrdered") {
            vector<int> points;
            //throw away the '{' and '}'
            getline(ss,temp,'{');
            getline(ss,temp,'}');
            ss.clear();
            ss.str(temp);
            while(!ss.eof()) {
                ss >> num;
                points.push_back(num);
            }
            //at the moment, pushes one too many
            points.pop_back();
            cell->NeighborCellsOrdered = points;
        }
        else if (temp == "    CurrentActiveIntnlNode") {
            ss >> num;
            cell->CurrentActiveIntnlNodes = num;
        }
        else if (temp == "    CurrentActiveMembrNodes") {
            ss >> num;
            cell->CurrentActiveMembrNodes = num;
        }
        else if (temp == "    CellCenter") {
            //for later
        }

        //clear stringstream for next input string
        ss.clear();

    } //finished reading one file

    ifs.close();

    return true;
}


