#include "Label.h"
#include <assert.h>
#include "Scanner.h"


Label::Label(wchar_t* const name) : Symbol(SymbolType::eLabel, name, 0)
{
	assert(name != 0 && !coco_string_equal(name, L""));
}

size_t const Label::GetAddr() const
{
	return mAddr;
}

void Label::SetAddr(size_t const addr)
{
	mAddr = addr;
}
