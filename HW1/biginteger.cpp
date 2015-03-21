
#include "biginteger.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stack>
using namespace std;

BigInteger::BigInteger(){
nSets = 0;
sign = POSITIVE; //To Be Modified
}

BigInteger::BigInteger(int initial){
	if( initial > 0 ){
		sign = POSITIVE ;
	}else if( initial < 0 ){
		sign = NEGATIVE;
		initial = -initial;
	}else{
		sign = 0;
	}
	nSets = 1;
	numberSets[ 1 ] = initial >> expo2;
	numberSets[ 0 ] = initial & setMask;
}

BigInteger::BigInteger(const std::string&numStr){
	int strLen = numStr.length();
	int offset = strLen % expo10;
	//1234567890123456   strlen = 16, offset = 7 
	//^.....^^.......^
	//|  7  |    9   |
	//0     6        15
	//|head |        |
	//|chop |  chop  |

	sign = POSITIVE;
	nSets = 0;


	string strChop; // Store part of string
	Int numberChop; // Store part of number ( converted form string )
	

	if( offset != 0 ){// Convert the part of the head.
		strChop = numStr.substr( 0 , offset  );
		numberChop = stoi( strChop , 0 , 10 );		
		this -> AbsAdd( numberChop );
	}

	for( int i = 0 ; i < ( strLen / expo10 ) ; ++i ){
		(*this) *= set10Max; 
		
		strChop = numStr.substr( i * expo10 + offset , expo10 );

		numberChop = stoi( strChop , 0 , 10 );
		this -> AbsAdd( numberChop );
	}
}


BigInteger::BigInteger(const BigInteger&initial){
	sign = initial.sign;
	nSets = initial.nSets;
	for( int i = 0 ; i < nSets ; ++i ){
		numberSets[ i ] = initial.numberSets[ i ];
	}
}       


BigInteger::~BigInteger(){
	
}


void BigInteger::AbsAdd(const Int number){
	numberSets[ 0 ] += number;
	for( int i = 0 ; i <= nSets ; ++i ){
		numberSets[ i + 1 ] += numberSets[ i ] >> expo2;
		numberSets[ i ] &= setMask;
	}
	if( numberSets[ nSets ] > 0 ){
		nSets = nSets + 1;
	}	
}



int BigInteger::AbsCmp(const BigInteger&bigInt ) const{

	int nMax = max( nSets, bigInt.nSets );
	for( int i = nMax - 1 ; i >= 0 ; --i ){
		if( numberSets[ i ] < bigInt.numberSets[ i ] ){
			return LESS;
		}
		if( numberSets[ i ] > bigInt.numberSets[ i ] ){
			return GREATER;
		}
	}	
	return EQUAL;
}
	

BigInteger BigInteger::AbsAdd( const BigInteger&bigInt1, const BigInteger&bigInt2 ) const{
	BigInteger result;
	
	int nMaxSets = max( bigInt1.nSets , bigInt2.nSets );       
	for( int i = 0 ; i < nMaxSets ; ++i ){
		result.numberSets[i] = result.numberSets[i] + bigInt1.numberSets[i] + bigInt2.numberSets[i];
		result.numberSets[i+1] = result.numberSets[i] >> expo2;
		result.numberSets[i] = result.numberSets[i] & setMask ;
	}
	if( result.numberSets[ nMaxSets ] != 0 ){
		result.nSets = nMaxSets + 1;
	}else{
		result.nSets = nMaxSets;
	}

	return result;
}

BigInteger BigInteger::AbsSub( const BigInteger&bigInt1, const BigInteger&bigInt2 ) const{
	BigInteger result;	
	int nMaxSets = bigInt1.nSets;       
	int carry = 0;
	for( int i = 0 ; i < nMaxSets ; ++i ){
		result.numberSets[i] = (1 << expo2 ) + bigInt1.numberSets[i] - bigInt2.numberSets[i] + carry;
		carry = ( result.numberSets[i] >> expo2 ) - 1  ;
		result.numberSets[i] = result.numberSets[i] & setMask ;
	}	
	result.UpdateNSets();
	return result;	
}
const BigInteger BigInteger::operator-(const BigInteger&bigInt) const{
	BigInteger result;
	/*	if( sign != bigInt.sign ){
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
	*/
	
	return AbsSub( *this , bigInt );
}

	
const BigInteger BigInteger::operator*(const BigInteger&bigInt) const{
	BigInteger result;
	for( int i = 0 ; i < nSets ; ++i ){
		for( int j = 0 ; j < bigInt.nSets ; ++j ){
			result.numberSets[i+j] += numberSets[i] * bigInt.numberSets[j];
			result.numberSets[i+j+1] += (result.numberSets[i+j] >> expo2);
			result.numberSets[i+j] &= setMask;
		}
	}
	if( sign == ZERO || bigInt.sign == ZERO ){
		result.sign = ZERO;
		result.nSets = 0;
	}else{
		result.sign = sign == bigInt.sign ? POSITIVE : NEGATIVE;
		if( result.numberSets[ nSets + bigInt.nSets - 1 ] != 0 ){
			result.nSets = nSets + bigInt.nSets;
		}else{
			result.nSets = nSets + bigInt.nSets - 1;
		}
	}
	return result;
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
	for( int i = nSets - 1 ; i >= 0 ; --i ){
		numberSets[ i + offset + 1 ] += ( numberSets[ i ] & mask ) >> remain ;
		numberSets[ i + offset ] = ( numberSets[ i ] << carryout ) & setMask;
		//|10111101| << carryout = |11110100|
		if( offset > 0 ){
			numberSets[ i ] = 0;
		}
	}
	if( numberSets[ nSets + offset ] > 0 ){
		nSets = nSets + offset + 1;
	}else{
		nSets = nSets + offset ;
	}	
}


void BigInteger::shiftRight(const int n){
	//As the number being shifted, the bits would not always fit in a number set.
	//So some of the bits have to be carried in.
	//Example:(expo2 = n( bit in a set ) = 8
	//                                 ..head          
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

	//	numberSets[ 0 ] = numberSets[ offset ] >> head ;
	/*if( offset > 0 ){
		numberSets[ 0 ] = numberSets[ offset ] >> tail;
		}*/

	for( int i = 0 ; i < nSets ; ++i ){
		numberSets[ i ] = ( numberSets[ i + offset ] >> head ) & setMask ;
		numberSets[ i ] += ( numberSets[ i + offset + 1 ] & mask ) << tail;
		if( offset > 0 ){
			numberSets[ i + offset ] = 0;
		}
	}

	if( numberSets[ nSets - offset - 1] != 0 ){
		nSets = nSets - offset  ;
	}else{
		nSets = nSets - offset - 1;
	}
}

const BigInteger BigInteger::operator%(const BigInteger&bigInt) const{
	if( nSets < bigInt.nSets ){
		return (*this);
	}
	BigInteger divisor(bigInt);
	BigInteger divided( (*this) );       
	divisor.shiftLeft( (nSets - bigInt.nSets + 1) * expo2  );
	int cmp = divisor.AbsCmp( bigInt );
	while( cmp == EQUAL || cmp == GREATER ){
		while( divisor < divided || divisor == divided){
			divided = AbsSub( divided, divisor );
		}
		divisor.shiftRight(1);
		cmp = divisor.AbsCmp( bigInt );
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
	numberSets[ nSets ] = carry;
	if( numberSets[ nSets ] > 0 ){
		nSets = nSets + 1;
	}
	return (*this);
}

	
BigInteger& BigInteger::operator/=(int divisor){
	for( int i = nSets - 1 ; i >= 0 ; --i ){
		numberSets[ i - 1 ] += ( numberSets[ i ] % divisor ) << expo2;
		numberSets[ i ] /= divisor;
	}
	/*	if( numberSets[ nSets - 1  ] == 0 ){
		nSets = nSets - 1;
		}*/
	(*this).UpdateNSets();
	return *this;
}

BigInteger& BigInteger::operator=(const BigInteger&bigInt){
	for( int i = 0; i < nSetsMax ; ++i ){
		numberSets[ i ] = bigInt.numberSets[ i ];
	}
	sign = bigInt.sign;
	nSets = bigInt.nSets;
	return (*this);
	
}


bool BigInteger::iseven() const{
	if( numberSets[0] % 2 == 0 ){
		return true;
	}
	return false;
}
bool BigInteger::iszero() const{
	if( nSets == 0  ){ 
		return true;
	}
	return false;
}

std::ostream& operator<<(std::ostream&os, const BigInteger&bigInt){
	BigInteger bigIntTmp( bigInt );
	BigInteger divisor( set10Max );
	BigInteger modded;
	stack<unsigned long long int>numStack;
	if( bigInt.nSets == 0 ){
		cout << 0 << endl;
		return os;
	}
	while( bigIntTmp.nSets != 0 ){
		modded = bigIntTmp % divisor;
		Int numPart = modded.numberSets[ 0 ] ;
		//cout << numPart << endl;
		numStack.push( numPart );
		bigIntTmp /= set10Max ;
	}
	Int numTop;

	cout << setfill('0');
	numTop = numStack.top();
	numStack.pop();	
	cout << numTop ;
	while( ! numStack.empty() ){
		numTop = numStack.top();
		numStack.pop();		
		cout << setw(expo10)  << numTop ;
	}

	return os;
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

bool BigInteger::operator==(const BigInteger&bigInt) const{
	if( this->AbsCmp( bigInt ) == EQUAL ){
		return true;
	}
	return false;
}

bool BigInteger::operator<(const BigInteger&bigInt) const{
	if( sign < bigInt.sign ){
		return true;
	}
	if( sign > bigInt.sign ){
		return false;
	}

	int cmp = this->AbsCmp( bigInt );
	if( (cmp == LESS && sign == POSITIVE) ||
	    (cmp == GREATER && sign == NEGATIVE) ){
		return true;
	}else{
		return false;
	}
}

void BigInteger::printSets(){
	cout << "Sign:"
	     << sign <<endl
	     << "Number of sets:" 
	     << nSets <<endl;
	cout << "Sets:" << endl;
	for( int i = 0 ; i < nSets ; ++i ){
		cout << "\t" << i << ":\t" << numberSets[ i ] << endl;
	}
	for( int i = 0 ; i < nSets ; ++i ){
		cout << numberSets[ i ] << " * 2^" << i * expo2 << "+" ;
	}
	cout << endl;
	cout << "----------" << endl;
}


void BigInteger::UpdateNSets(){
	for( int i = nSetsMax - 1 ; i >= 0 ; --i ){
		if( numberSets[ i ] > 0 ){
			nSets = i + 1;
			return;
		}       
	}
	nSets = 0;
	return;
}
