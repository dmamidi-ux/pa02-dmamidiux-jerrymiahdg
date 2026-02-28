// Winter'24
// Instructor: Diba Mirza
// Student name: Devesh Mamidi and Jeremiah De Guzman
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
#include <unordered_map>
using namespace std;

#include "utilities.h"
#include "movies.h"

struct minH {
    bool operator()(const Movies& a, const Movies& b) const {
        return b.getRating() < a.getRating();
    }
};

struct maxH {
    bool operator()(const Movies& a, const Movies& b) const {
        return a.getRating() < b.getRating();
    }
};

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }

    clock_t start = clock();
  
    // Create an object of a STL data-structure to store all the movies
    set<Movies> s;
    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
            Movies m(movieName, movieRating);
            s.insert(m);
    }

    movieFile.close();

    if (argc == 2){
            for (const auto& movie: s) {
                cout << movie << endl;
            }
            return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }

    //  For each prefix,
    //  Find all movies that have that prefix and store them in an appropriate data structure
    //  If no movie with that prefix exists print the following message
    unordered_map<string, priority_queue<Movies, vector<Movies>, maxH>> map;
    vector<pair<string, Movies>> pre;
    for (string p: prefixes) {
        bool found = false;
        bool mx = true;
        int len = p.length();
        auto it = s.lower_bound(Movies(p, 0.0));
        while (it != s.end()) {
            string name = it->getName();

            if (name.length() >= len && name.substr(0, len) == p) {
                map[p].push(*it);
                found = true;
                ++it;
            }
            else if (found) {
                break;
            }
            else {
                ++it;
            }
        }
        // Fill in
        if (!found) {
            cout << "No movies found with prefix "<< p << endl;
        }
        else {
            while (!map[p].empty()) {
                Movies m = map[p].top();
                cout << m << endl;
                if (mx) {
                    pre.push_back({p, m});
                    mx = false;
                }
                map[p].pop();
            }
        }
        if (found) cout << endl;
    }
    
    for(pair<string, Movies> p : pre) {
        //  For each prefix,
        //  Print the highest rated movie with that prefix if it exists.
        cout << "Best movie with prefix " << p.first << " is: " << p.second.getName() << " with rating " << std::fixed << std::setprecision(1) << p.second.getRating() << endl;
    }
    clock_t end = clock();
    double ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
    cerr << "The running time is: " << ms << "ms";
    return 0;
}

/* Add your run time analysis for part 3 of the assignment here as commented block*/

/*

~ ~ ~ Time complexity: ~ ~ ~

    
~ ~ ~ Run Times: ~~~
input_20_random.csv: 50.773 ms
input_100_random.csv: 91.968 ms
input_1000_random.csv: 332.165 ms
input_76920_random.csv: 32432.5 ms

*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}