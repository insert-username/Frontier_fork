#ifndef MFA_GLOBAL_STATE
#define MFA_GLOBAL_STATE

// todo: this class should eventually not exist, as global state is somewhat evil, but I am using it to wrap the vars
// int nextEdgeName=1, nextVerName=1, singleVertex=1; (which appear to get defined in utuC.cpp)
class MFAGlobalState {
public:
    static int nextEdgeName;
    static int nextVerName;
    static int singleVertex;
};

#endif