#include "qtstreambuf.h"

QtStreamBuf::QtStreamBuf(std::function<void(const QString&)> callback): callback(callback){
}

int QtStreamBuf::overflow(int c) {
    if (c == '\n') {
        // covnertit les std::string à des strings UTF-8 pour afficher des caractères spéciaux
        callback(QString::fromUtf8(buffer.c_str()));
        buffer.clear();
    } else {
        buffer += static_cast<char>(c);
    }
    return c;
}
