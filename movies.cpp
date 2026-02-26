#include "movies.h"

using namespace std;

Movies::Movies(string n, double r) : name(n), rating(r) {}

bool Movies::operator<(const Movies& other) const {
    int b = name.compare(other.name);
    if(b < 0) {
        return true;
    }
    return false;
}

double Movies::getRating() const {
    return rating;
};
string Movies::getName() const {
    return name;
};

ostream& operator<<(ostream& out, const Movies& c) {
    return out << c.getName() << ", " << c.getRating();
}