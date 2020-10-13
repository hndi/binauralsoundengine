#include "guihelpers.h"



wxString fToStr(double n) {
    wxString ret = wxString::Format(wxT("%f"), n);
    int lastZero = ret.length();
    bool found = false, foundNumer = false;

    for (int i = ret.length() - 1; i >= 0; i--) {
        if (found == false && ret.substr(i, 1) == "0" && foundNumer == false) {
            lastZero = i;
        } else {
            foundNumer = true;
        }
        if (ret.substr(i, 1) == ".") {
            found = true;
            if (lastZero == i + 1) {
                lastZero = i;
            }
            break;
        }
    }
    if (found == true) {
        return ret.substr(0, lastZero);
    }

    return ret;
}

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
