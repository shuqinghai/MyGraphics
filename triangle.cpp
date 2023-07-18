#include "triangle.h"
bool cmpx(const Triangle& t1, const Triangle& t2) {
    return t1.center[0] < t2.center[0];
}
bool cmpy(const Triangle& t1, const Triangle& t2) {
    return t1.center[1] < t2.center[1];
}
bool cmpz(const Triangle& t1, const Triangle& t2) {
    return t1.center[2] < t2.center[2];
}
