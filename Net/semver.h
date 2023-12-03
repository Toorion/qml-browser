#ifndef SEMVER_H
#define SEMVER_H

#include <string>
#include <algorithm>

struct Semver
{
    Semver() {}

    Semver(std::string versionStr)
    {
        name = versionStr;
        auto numericPos = std::find_if(versionStr.begin(), versionStr.end(), [](char c) {
            return std::isdigit(c);
        });

        std::string str = versionStr.substr(numericPos - versionStr.begin());

        sscanf(str.c_str(), "%d.%d.%d-%[^.].%d", &major, &minor, &patch, &pre, &build);
        if(pre == '\0') {
            sscanf(str.c_str(), "%d.%d.%d.%d", &major, &minor, &patch, &build);
        }
        isEmpty = false;
    }

    bool operator<(const Semver &otherVersion)
    {
        if(major < otherVersion.major)
            return true;
        if(minor < otherVersion.minor)
            return true;
        if(patch < otherVersion.patch)
            return true;
        if(pre != '\0' && otherVersion.pre == '\0')
            return true;
        if(pre < otherVersion.pre)
            return true;
        if(build < otherVersion.build)
            return true;
        return false;
    }

    std::string name;
    int major, minor, patch, build;
    char pre = '\0';
    bool isEmpty = true;
};



#endif // SEMVER_H
