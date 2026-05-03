#pragma once
#include <streambuf>
#include <functional>
#include <QString>

class QtStreamBuf : public std::streambuf {
public:
    explicit QtStreamBuf(std::function<void(const QString&)> callback);
protected:
    int overflow(int c) override;
private:
    std::string buffer;
    std::function<void(const QString&)> callback;
};
