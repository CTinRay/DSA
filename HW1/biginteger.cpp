#include "biginteger.h"
#include <algorithm>
#include <iostream>
using namespace std;

BigInteger::BigInteger(){
	nSets = 0;
	sign = ZERO;
}

BigInteger::BigInteger(int initial){
	if( initial > 0 ){
		sign = ZERO;
	}else if( initial < 0 ){
		sign = NEGATIVE;
		initial = -initial;
	}else{
		sign = 0;
	}
	nSets = 2;
	numberSets[ 1 ] = initial >> expo2;
	numberSets[ 0 ] = initial & setMask;
}

void BigInteger::UpdateNSets(){
	for( int i = nSetsMax - 1 ; i >= 0 ; --i ){
		if( numberSets[ i ] > 0 ){
			nSets = i + 1;
			return;
		}
	}
}

BigInteger::BigInteger(const std::string&numStr){
	int strLen = numStr.length();
	int offset = strLen % expo10;
	//	int n10Sets = strLen / expo10;
	Int sets10[ nDigits / expo10 + 1 ];
	string strChop = numStr.substr( 0 , offset );
	Int numberChop = stoi( strChop , 0 , 10 );		
	this -> AbsAdd( numberChop );
	(*this) *= set10Max;
	for( int i = 1 ; i < ( nDigits / expo10 + 1 ) ; ++i ){
		strChop = numStr.substr( i * expo10 + offset , (i+1) * expo10 + offset );
		numberChop = stoi( strChop , 0 , 10 );
		this -> AbsAdd( numberChop );
		(*this) *= set10Max;
	}       
}
void BigInteger::AbsAdd(const Int bigInt){
	for( int i = 0 ; i < nSets ; ++i ){
		numberSets[ i ] += bigInt;
		numberSets[ i + 1 ] += bigInt >> expo2;
		numberSets[ i ] &= setMask;
	}
	if( numberSets[ nSets ] > 0 ){
		nSets = nSets + 1;
	}
}

BigInteger::BigInteger(const BigInteger&initial){
	nSets = initial.nSets;
	for( int i = 0 ; i < nSets ; ++i ){
		numberSets[ i ] = initial.numberSets[ i ];
	}
}       

BigInteger::~BigInteger(){
}

int BigInteger::AbsCmp(const BigInteger&bigInt ){
	int greater = EQUAL;
	int nMax = max( nSets, bigInt.nSets );
	for( int i = nMax - 1 ; i >= 0 ; ++i ){
		if( numberSets[ i ] < bigInt.numberSets[ i ] ){
			return LESS;
		}
		if( numberSets[ i ] < bigInt.numberSets[ i ] ){
			greater = GREATER;
		}
	}	
	return greater;
}
	

BigInteger BigInteger::AbsAdd( const BigInteger&bigInt1, const BigInteger&bigInt2 ){
	BigInteger result;
	
	int nMaxSets = max( bigInt1.nSets , bigInt2.nSets );       
	for( int i = 0 ; i < nMaxSets ; ++i ){
		result.numberSets[i] = result.numberSets[i] + bigInt1.numberSets[i] + bigInt2.numberSets[i];
		result.numberSets[i+1] = result.numberSets[i] >> expo2;
		result.numberSets[i] = result.numberSets[i] && setMask ;
	}
	if( result.numberSets[ nMaxSets ] != 0 ){
		result.nSets = nMaxSets + 1;
	}else{
		result.nSets = nMaxSets;
	}

	return result;
}

BigInteger BigInteger::AbsSub( const BigInteger&bigInt1, const BigInteger&bigInt2 ){
	BigInteger result;	
	int nMaxSets = max( bigInt1.nSets , bigInt2.nSets );       
	for( int i = 0 ; i < nMaxSets ; ++i ){
		result.numberSets[i] = result.numberSets[i] + (1 << expo2 ) + bigInt1.numberSets[i] - bigInt2.numberSets[i];
		result.numberSets[i+1] = ( result.numberSets[i] >> expo2 ) - 1 ;
		result.numberSets[i] = result.numberSets[i] && setMask ;
	}
	if( result.numberSets[ nMaxSets ] != 0 ){
		result.nSets = nMaxSets + 1;
	}else{
		result.nSets = nMaxSets;
	}

	return result;	
}
const BigInteger BigInteger::operator-(const BigInteger&bigInt) const{
	BigInteger result;
	if( sign != bigInt.sign ){
		result = AbsAdd( (*this) , bigInt );
		result.sign = sign;

	}else if( sign == bigInt.sign ){
		int cmp = this->AbsCmp( bigInt );
		if( cmp == LESS ){//If |this| < |bigInt|
			//=> |result| = |bigInt| - |this|
			//reault.sign = -sign
			result = AbsSub( bigInt, *this );
			result.sign = -sign;
			
		}else{//If |this| > |bigInt|
			//=> |result| = |this| - |bigInt|
			//   result = sign
			result = AbsSub( *this, bigInt );
			if( cmp == ZERO ){
				result.sign = ZERO;
			}else{
				result.sign = sign;
			}
		}
	}
	return result;
}

	
const BigInteger BigInteger::operator*(const BigInteger&bigInt) const{
	BigInteger result;
	for( int i = 0 ; i < nSets ; ++i ){
		for( int j = 0 ; j < bigInt.nSets ; ++j ){
			result.numberSets[i+j] += this->numberSets[i] * bigInt.numberSets[j];
			result.numberSets[i+j+1] = result.numberSets[i] >> expo2;
			result.numberSets[i+j] &= setMask;
		}
	}
	if( sign == ZERO || bigInt.sign == ZERO ){
		result.sign = ZERO;
		result.nSets = 1;
	}else{
		result.sign = sign == bigInt.sign ? POSITIVE : NEGATIVE;
		if( result.numberSets[ nSets + bigInt.nSets - 1 ] != 0 ){
			result.nSets = nSets + bigInt.nSets;
		}else{
			result.nSets = nSets + bigInt.nSets - 1;
		}
	}
}

void BigInteger::shiftLeft(const int n){
	//As the number being shifted, the bits would not always fit in a number set.
	//So some of the bits have to be carried out.
	//Example:(expo2 = n( bit in a set ) = 8               
	//        |00000000|00000000|10111101|10001111|10001100| << 10
	//       =|00000010|11110110|00111110|00110000|00000000|
	//               ^^carryout=2  ^....^remain=6
	// offset = 10 / 8 = 1
	int carryout = n % expo2; 
	int mask = ( 1 << carryout ) - 1;
	//mask = ( 1 << 2 ) -1 = |00000011|
	int remain = expo2 - carryout;
	mask = mask << remain;
	//mask << remain = |11000000|
	int offset = n / expo2;
	for( int i = nSets - 1 + offset ; i >= offset ; ++i ){
		numberSets[ i + offset + 1 ] += ( numberSets[ i ] & mask ) >> remain ;
		numberSets[ i + offset ] = numberSets[ i ] << carryout;
		//|10111101| << carryout = |11110100|
		numberSets[ i ] = numberSets[ i ] << n;
	}
	if( numberSets[ nSets + offset ] != 0 ){
		nSets = nSets + offset + 1;
	}else{
		nSets = nSets + offset;
	}	
}


void BigInteger::shiftRight(const int n){
	//As the number being shifted, the bits would not always fit in a number set.
	//So some of the bits have to be carried in.
	//Example:(expo2 = n( bit in a set ) = 8               
	//        |00000010|11110110|00111110|00110000|00000000| >> 10
	//       =|00000000|00000000|10111101|10001111|10001100| 
	//                             ^....^tail=6    ^^head=2        
	// offset = 10 / 8 = 1
	int head = n % expo2; 
	int mask = ( 1 << head ) - 1;
	//mask = ( 1 << head ) -1 = |00000011|
	int tail = expo2 - head;
	//mask = mask << tail;
	//mask << remain = |11000000|
	int offset = n / expo2;

	numberSets[ 0 ] = numberSets[ offset ] >> head ;
	if( offset > 0 ){
		numberSets[ offset -1 ] = (numberSets[ offset ] & mask) << tail;
	}

	for( int i = offset + 1 ; i < nSets ; ++i ){
		numberSets[ i - offset ] = numberSets[ i ] >> head ;
		numberSets[ i - offset -1 ] = (numberSets[ i ] & mask) << tail;
		numberSets[ i ] = ((unsigned Int ) numberSets[ i ]) >> n;
	}

	if( numberSets[ nSets - offset ] != 0 ){
		nSets = nSets - offset + 1;
	}else{
		nSets = nSets - offset;
	}
}

const BigInteger BigInteger::operator%(const BigInteger&bigInt) const{
	BigInteger divisor(bigInt);
	BigInteger divided( (*this) );
	bigInt.shiftLeft( nSets - bigInt.nSets );
	for( int i = 0 ; i < nSets - bigInt.nSets ; ++ i ){
		if( bigInt < divided ){
			divided = divided - divisor;
		}
		divisor.shiftRight(1);
	}
	divided.UpdateNSets();
	return divided;
}		
	
BigInteger& BigInteger::operator*=(int multiplier){
	int carry = 0;
	for( int i = 0 ; i < nSets ; ++i ){
		numberSets[ i ] = numberSets[ i ] * multiplier + carry;
		carry = numberSets[ i ] >> expo2;
		numberSets[ i ] &= setMask;
	}
	if( numberSets[ nSets ] > 0 ){
		nSets = nSets + 1;
	}
	       
}

	
BigInteger& BigInteger::operator/=(int divisor){
	for( int i = nSets - 1 ; i > 0 ; --i ){
		numberSets[ i - 1 ] += ( numberSets[ i ] % divisor ) << expo2;
		numberSets[ i ] /= divisor;
	}
	if( numberSets[ nSets - 1  ] = 0 ){
		nSets = nSets - 1;
	}	
}

BigInteger& BigInteger::operator=(const BigInteger&bigInt){
	for( int i = 0; i < nSets ; ++i ){
		numberSets[ i ] = bigInt.numberSets[ i ];
	}
	sign = bigInt.sign;
	nSets = bigInt.nSets;
	return (*this);
}


bool iseven(){
	return 1;
}
bool iszero(){
	return 1;
}

std::ostream& operator<<(std::ostream&os, const BigInteger&bigInt){
	BigInteger bigIntTmp( bigInt );
	BigInteger divisor( set10Max );
	BigInteger modded;
	for( int i = 0 ; i < (nDigits / expo10) + 1 ; ++i ){
		modded = bigIntTmp % divisor;
		cout << modded.numberSets[ 0 ];
		bigIntTmp /= set10Max;
	}
}
		

bool BigInteger::operator>(const BigInteger&bigInt) const{
	if( sign < bigInt.sign ){
		return false;
	}
	int cmp = this->AbsCmp( bigInt );
	if( (cmp == LESS && sign == NEGATIVE) ||
	    (cmp == GREATER && sign == POSITIVE) ){
		return true;
	}else{
		return false;
	}
}


bool BigInteger::operator<(const BigInteger&bigInt) const{
	if( sign < bigInt.sign ){
		return true;
	}
	int cmp = this->AbsCmp( bigInt );
	if( (cmp = LESS && sign == POSITIVE) ||
	    (cmp = GREATER && sign == NEGATIVE) ){
		return true;
	}else{
		return false;
	}
}
