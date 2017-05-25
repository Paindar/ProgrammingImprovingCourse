#ifndef NOCHARACTOREXCEPTION_H
#define NOCHARACTOREXCEPTION_H
#include <exception>

class NoCharactorException : public std::exception
{
public:
    NoCharactorException();
};

#endif // NOCHARACTOREXCEPTION_H
