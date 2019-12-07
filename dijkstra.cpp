#include <iostream>
#include <conio.h>
#include <sstream>
#include <fstream>
#include <regex>
#include <ctype.h>
using namespace std;

class Digraph
{
private:
	int** finalArray;
public:
	ifstream inputFile;
	string line;
	int number_of_vertices, starting_row;
	ofstream ofile;

	void appendPath(int* path, int i)
	{
		if (path[i] == -1)
		{
			return;
		}
		appendPath(path, path[i]);
		ofile << "  " << i;

	}

	void printOutput(int* dist, int V, int* path, int src, string folderName)
	{
		ofile << endl << endl << endl;
		ofile << "Start vertex is: " << src;
		ofile << endl << endl;
		ofile << "\n\nTotal" << endl;
		ofile << "Dest  | Weight  | Path" << endl;
		ofile << "------------------------------" << endl;
		for (int i = 0; i < V; i++)
		{
			if (dist[i] == 99999999)
			{
				if(i < 10)
					ofile << i << "         inf";
				else
					ofile << i << "        inf";
			}
			else
			{
				ofile << i << "         " << dist[i] << "        ";
				appendPath(path, i);

			}
			ofile << endl << endl;
		}
		ofile.close();
		cout << "\nOutput appended in the output file, check the output file: "+ folderName + "_Result.txt\n";
	}

	int  minimum_dist(int dist[], bool shrtvert[], int nov)
	{
		int min = 99999999, min_index;

		for (int i = 0; i < nov; i++)
		{
			if (shrtvert[i] == false && dist[i] <= min)
			{
				min = dist[i];
				min_index = i;
			}
		}

		return min_index;

	}

	void dijkstra(int** array, int nov, int sr, string folderName)
	{
		int* distance;
		distance = new int[nov];
		bool* shrtvert;
		shrtvert = new bool[nov];
		int* path;
		path = new int[nov];

		for (int i = 0; i < nov; i++)
		{

			path[i] = -1;
			distance[i] = 99999999;
			shrtvert[i] = false;
		}

		distance[sr] = 0;
		for (int j = 0; j < nov; j++)
		{
			int u = minimum_dist(distance, shrtvert, nov);
			shrtvert[u] = true;
			for (int k = 0; k < nov; k++)
				if (!shrtvert[k] && array[u][k] && distance[u] + array[u][k] < distance[k])
				{
					path[k] = u;
					distance[k] = distance[u] + array[u][k];
				}
		}

		printOutput(distance, nov, path, sr, folderName);
	}

	void appendFirstSection(int** array, int nov, string folderName)
	{
		ofile.open(folderName + "_Result.txt");
		if (!ofile)
		{
			cout << "Error creating output file.";
		}
		else
		{
			ofile << "Nodes   |   Out-neighbors" << endl;
			ofile << "----------------------------------------------------------------------" << endl;
			for (int i = 0; i < nov; i++)
			{
				if (i <= 9)
					ofile << i << "           ";
				else
					ofile << i << "          ";

				for (int j = 0; j < nov; j++)
				{
					if (array[i][j] > 0)
					{
						ofile << j << ":" << array[i][j] << "  ";
					}

				}
				ofile << endl << endl;
			}
		}
	}

	int** arrayFromGraph(ifstream& in_file, int nov)

	{
		string line;
		int** array;
		int k;
		array = new int* [nov];

		k = 1;
		int number;
		for (int i = 0; i < nov; i++) {
			array[i] = new int[nov];
			getline(in_file, line);
			while (line.empty() || line.find_first_not_of(' ') == string::npos)
				getline(in_file, line);

			stringstream iss(line);
			int j = 0;

			if (line.length() != 0) {
				while (iss >> number) {
					array[i][j] = number;
					j++;
				}
			}

		}

		return array;
	}

	string getInitialVal(string line)
	{
		regex e("^[a-zA-Z].*\\:.*?([0-9]+)");
		smatch m;
		string nVertices;
		if (regex_search(line, m, e)) {
			nVertices = m[1];
		}
		return nVertices;
	}

	string getFolderName(string file_name) {
		string folderName;
		const size_t period_idx = file_name.rfind('.');
		if (string::npos != period_idx)
		{
			folderName = file_name.erase(period_idx);
		}

		return folderName;
	}

	void getData(string file_name)
	{
		inputFile.open(file_name);
		if (!inputFile)
			cout << "\nError opening file, please check whether the file name you have entered is valid\n";			
		else
		{
			string folderName, line;
			int startVertex, noVertices;
			folderName = getFolderName(file_name);
			
			getline(inputFile, line); //retrieve noOfVertices
			while (line.empty() || line.find_first_not_of(' ') == string::npos)
				getline(inputFile, line);
			noVertices = stoi(getInitialVal(line));
			getline(inputFile, line); //retrieve start vertex
			while (line.empty() || line.find_first_not_of(' ') == string::npos)
				getline(inputFile, line);
			startVertex = stoi(getInitialVal(line));
			
			finalArray = arrayFromGraph(inputFile, noVertices);
			appendFirstSection(finalArray, noVertices, folderName);
			dijkstra(finalArray, noVertices, startVertex, folderName);
		}
	}
};

int main() {
	Digraph di;
	char file_name[100];
	cout << "Enter the complete path of the input file: ";
	cin.getline(file_name, sizeof(file_name));
	di.getData(file_name);
	return 0;
}