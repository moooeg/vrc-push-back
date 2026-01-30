
#include <string>

class ButtonPosition {
public:
    int x1, x2, y1, y2;

    bool pressing(int x, int y) {
        return (x1 <= x <= x2) && (y1 <= y <= y2);
    }

    ButtonPosition(int x1, int x2, int y1, int y2) {
        this->x1 = x1;
        this->x2 = x2;
        this->y1 = y1;
        this->y2 = y2;
    }
};

class TeamPosition {
public:

    std::string team;
    std::string position;

    std::string asString() {
        return team + "_" + position;
    }
};
