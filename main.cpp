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
- Adding Movies to Set: O(nlog(n))
    - Each insert() operation is max log(n)
        - insert() operation is log(C) where C is size of set
        - set has a max size of n because only adding of the n movies
- Adding prefixes to vector: O(m)
    - Each push_back operation is O(1)
- For loop iterates through each prefix in prefix vector: O(m * (log(n) + klog(k)))
    - For loop runs m times
    - Find first item in set with prefix: O(log(n))
    - Keep iterating forward in the set through each movie with the prefix (max k movies per prefix)
        - Add movie to priority queue for corresponding prefix: O(log(k))
    - Pop and print the top element of max heap until empty: O(klogk)
- For loop iterates through vector with highest rating movie for each of m prefixes: O(k)

=> Time Complexity is O(nlogn + m(logn + klogk))


~ ~ ~ Space Complexity ~ ~ ~
- Set to contain movies: O(n)
- (String : Max-Heap) Map: O(m * k)
    - m prefixes
    - max k movies per prefix
- Vector of highest rated movie per prefix: O(m)

=> Space Complexity is O(n + m*k)

~ ~ ~ Tradeoffs between time/space complexity ~ ~ ~
I designed for a lower time complexity, and I was targeting a linearithmic time complexity and a space complexity as close to linear as I could.
I was able to somewhat achieve a low space complexity; however, having an unordered_map with a heap as a value sacrificed some memory to be able to instantly access the highest rated movie and print them in sorted order.

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