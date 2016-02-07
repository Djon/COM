#ifndef LABEL_H_
#define LABEL_H_

#include "Symbol.h"

class Label : public Symbol {
public:
	Label(wchar_t* const name);

	size_t const GetAddr() const;
	void SetAddr(size_t const addr);

private:
	size_t mAddr;
};

#endif