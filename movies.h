#include <string>
#include <iostream>
using namespace std;

class Movies {
    public:
        Movies(string n, double r);
        double getRating() const;
        string getName() const;

        bool operator<(const Movies& other) const;


    private:
        string name;
        double rating;
};

ostream& operator<<(ostream& out, const Movies& c);