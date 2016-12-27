// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <stdexcept>

TBitField::TBitField(int len) : BitLen(len) { // конструктор
	if (len > -1) {
		MemLen = (len + sizeof(TELEM) * 8 - 1) >> sizeof(TELEM);
		pMem = new TELEM[MemLen];
		if (pMem != NULL)
			for (int i = 0; i < MemLen; i++) pMem[i] = 0;
	}
	else
    throw std::logic_error("Input error: enter a negative length\n");
}

TBitField::TBitField(const TBitField &bf) { // конструктор
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	if (pMem != NULL)
		for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
}

TBitField :: ~TBitField() {
	delete pMem;
	pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const {
	return n >> sizeof(TELEM);
}

TELEM TBitField::GetMemMask(const int n) const {
	return 1 << (n & (sizeof(TELEM) * 8 - 1));
}
int TBitField::GetLength(void) const {
	return BitLen;
}

void TBitField::SetBit(const int n) {
	if ((n > -1) && (n < BitLen))
		pMem[GetMemIndex(n)] |= GetMemMask(n);
	else
    throw std::logic_error("Input error: enter a inadmissible number of bits\n");
}

void TBitField::ClrBit(const int n) {
	if ((n > -1) && (n < BitLen))
		pMem[GetMemIndex(n)] &= ~GetMemMask(n);
	else
    throw std::logic_error("Input error: enter a inadmissible number of bits\n");
}

int TBitField::GetBit(const int n) const {
	if ((n > -1) && (n < BitLen))
		return pMem[GetMemIndex(n)] & GetMemMask(n);
	else
    throw std::logic_error("Input error: enter a inadmissible number of bits\n");
}

TBitField & TBitField :: operator=(const TBitField &bf) {
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen) {
		MemLen = bf.MemLen;
		if (pMem != NULL) delete pMem;
		pMem = new TELEM[MemLen];
	}
	if (pMem != NULL)
		for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField :: operator==(const TBitField &bf) const {
	int res = 1;
	if (BitLen != bf.BitLen) res = 0;
	else
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] != bf.pMem[i]) { res = 0; break; }
	return res;
}
int TBitField::operator!=(const TBitField &bf) const {
	if ((*this) == bf)
		return 0;
	else
		return 1;
}

TBitField TBitField :: operator| (const TBitField &bf) {
	int i, len = BitLen;
	if (bf.BitLen > len) len = bf.BitLen;
	TBitField temp(len);
	for (i = 0; i < MemLen; i++) temp.pMem[i] = pMem[i];
	for (i = 0; i < bf.MemLen; i++) temp.pMem[i] |= bf.pMem[i];
	return temp;
}

TBitField TBitField :: operator& (const TBitField &bf) {
	int i, len = BitLen;
	if (bf.BitLen > len) len = bf.BitLen;
	TBitField temp(len);
	for (i = 0; i < MemLen; i++) temp.pMem[i] = pMem[i];
	for (i = 0; i < bf.MemLen; i++) temp.pMem[i] &= bf.pMem[i];
	return temp;
}

TBitField TBitField :: operator~ (void) {
	int len = BitLen;
	TBitField temp(len);
	temp = *this;
	for (int i = 0; i < BitLen; i++)
		if (GetBit(i))
			temp.ClrBit(i);
		else
			temp.SetBit(i);
	return temp;
}
istream &operator >> (istream &istr, TBitField &bf) { //ввод
	int i = 0; char ch;
	do { istr >> ch; } while (ch != ' ');
	while (1) {
		istr >> ch;
		if (ch == '0') bf.ClrBit(i++);
		else if (ch == '1') bf.SetBit(i++); else break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) { //вывод
	int len = bf.GetLength();
	for (int i = 0; i < len; i++)
		if (bf.GetBit(i))	ostr << '1'; else	ostr << '0';
	return ostr;
}

