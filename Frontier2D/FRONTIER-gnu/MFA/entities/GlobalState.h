#ifndef MFA_GLOBAL_STATE
#define MFA_GLOBAL_STATE

// todo: This class is a bit of a hack: I am using it to replace the vars
// int nextEdgeName=1, nextVerName=1, singleVertex=1; (which appear to get defined globally in utuC.cpp)
class GlobalState {
private:
    int nextEdgeName = 1;
    int nextVerName = 1;
    int singleVertex = 1;

public:

    [[nodiscard]] int getNextEdgeName() const {
        return nextEdgeName;
    }

    [[nodiscard]] int getNextVerName() const {
        return nextVerName;
    }

    [[nodiscard]] int getSingleVertex() const {
        return singleVertex;
    }

    void setNextEdgeName(const int& value) {
        nextEdgeName = value;
    }

    void incrementNextEdgeName() {
        nextEdgeName++;
    }

    void setNextVerName(const int& value) {
        nextVerName = value;
    }

    void incrementNextVerName() {
        nextVerName++;
    }

    void setSingleVertex(const int& value) {
        singleVertex = value;
    }
};

#endif