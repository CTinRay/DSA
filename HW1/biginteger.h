#include <iostream>
#include <string>


typedef long long int Int;
#define expo2 30 //= the number of bit a number set can keep.
#define expo10 10
#define set10Max 1000000000

#define nDigits 256
#define nSetsMax ( nDigits / expo10 + 1 )
#define setMask ( (1ll << expo2) -1 )
class BigInteger{
public:
	BigInteger();
	BigInteger(int);
	BigInteger(const std::string&);
	BigInteger(const BigInteger&);
	~BigInteger();
	bool operator<(const BigInteger&) const;
	bool operator>(const BigInteger&) const;
	const BigInteger operator-(const BigInteger&) const;
	const BigInteger operator*(const BigInteger&) const;
	const BigInteger operator%(const BigInteger&) const;
	BigInteger& operator*=(int);
	BigInteger& operator/=(int);
	BigInteger& operator=(const BigInteger&);
	bool iseven();
	bool iszero();

	friend std::ostream& operator<<(std::ostream&, const BigInteger&);


	const int POSITIVE =1;
	const int ZERO = 0;
	const int NEGATIVE = -1;
	const int GREATER = 1;
	const int EQUAL = 0;
	const int LESS = -1;

private:
	bool AbsCmp(const BigInteger&)const; //Compare regardless of sign
	BigInteger AbsSub( const BigInteger&, const BigInteger& ) const; //Sub regardless of sign
	BigInteger AbsAdd( const BigInteger&, const BigInteger& ) const ; //Add regardless of sign
	int AbsCmp(const BigInteger& );
	void AbsAdd(const Int bigInt);
	void UpdateNSets();
	Int numberSets[ nSetsMax ] = {0};
	Int nSets;
	Int sign;
	void shiftLeft(const int n);
	void shiftRight(const int n);
};


