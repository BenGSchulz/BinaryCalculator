// Ben Schulz
// Assignment2
// 3.1.18
// COMP 2691

#include <iostream>
#include <string>
#include <vector>
#include "math.h"

using namespace std;

long bits2decimal(const vector<bool>& bits);
string bits2string(const vector<bool>& bits);
vector<bool> string2bits(const string& arg);
void add(const vector<bool>& bits1, const vector<bool>& bits2, vector<bool>& result);
void sub(const vector<bool>& bits1, const vector<bool>& bits2, vector<bool>& result);
void mult(const vector<bool>& bits1, const vector<bool>& bits2, vector<bool>& result);
void divide(const vector<bool>& bits1, const vector<bool>& bits2, vector<bool>& result, string& divString);

void twosComplement(vector<bool>& bits);
void shiftLeft(vector<bool>& bits);
void shiftRight(vector<bool>& bits);

int main(int argc, char** argv)
{
	
	freopen("commands.txt", "r", stdin);  // Redirected input
	freopen("myresults.txt", "w", stdout);  // Redirected output
	
	cout << "Welcome to the BinaryCalculator" << endl;

	string arg1, op, arg2;

	// Get operation strings from input
	while(cin.good()) {
		if((cin >> arg1).eof()) {
			break;// end of input
		}
		
		if(arg1 == "quit" || arg1 == "QUIT") {
			break;
		}
		
		if((cin >> op).eof()){
			break;// end of input
		}
		
		if((cin >> arg2).eof()){
			break;// end of input
		}

		// Both arguments must be same length
		if(arg1.length() != arg2.length()){
			cerr << "ERROR: '" << arg1 << "' and '" << arg2 << "' are not the same length." << endl;
			continue;
		}
		
		// Convert parsed strings to bit vectors
		vector<bool> bits1 = string2bits(arg1);
		vector<bool> bits2 = string2bits(arg2);
		
		vector<bool> result(bits1.size());
		
		bool divideBool = false;
		string divString = "";

		// Perform appropriate op depending on parsed string
		if(op == "+"){
			add(bits1, bits2, result);
		}
		
		if (op == "-") {
			sub(bits1, bits2, result);
		}
		
		if (op == "*") {
			mult(bits1, bits2, result);
		}
		
		if (op == "/") {
			divide(bits1, bits2, result, divString);
			divideBool = true;
		}

		
		// Print out the result in both binary and decimal.
		if (!divideBool) {
//			cerr << "DEBUG: " << bits2string(bits1) << " " << op << " " << bits2string(bits2) << " = " << bits2string(result) << endl;
			
			/*
			 * Decimal (actual output):
			 */
			cout << bits2decimal(bits1) << " " << op << " " << bits2decimal(bits2) << " = " << bits2decimal(result) << endl;
		} else {
			cout << bits2decimal(bits1) << " " << op << " " << bits2decimal(bits2) << " = " << divString << endl;
			divideBool = false;
		}

	}
	
	fclose(stdin);
	fclose(stdout);
	
	// TESTING STRING FUNCTIONS
//	string in;
//	
//	while (in != "quit") {
//		cout << "Please input a binary number: ";
//		cin >> in;
//		cout << "\n";
//		
//		vector<bool> vect = string2bits(in);
//		
//		cout << "Printed back from vector: ";
//		
//		for (int i = 0; i < in.size(); ++i) {
//			cout << vect[i];
//			if (i != in.size()-1) cout << ", ";
//		}
//		
//		cout << "\nBack to string: " << bits2string(vect);
//		
//		cout << "\nDecimal Value: " << bits2decimal(vect);
//		
//		twosComplement(vect);
//		
//		cout << "\nTwos Complement: " << bits2decimal(vect);
//		
//		cout << "\n" << endl;
//	}
	
	return 0;
}

// Converts a string like "010101" into an vector of bools (bits)
// Take care of the order of bits.  The character at position 0 in the string, is the MSB
vector<bool> string2bits(const string& arg) {
	vector<bool> result(arg.size());
	
	for (int i = 0; i < arg.size(); ++i) {
		result[i] = (arg[i] == '1') ? 1 : 0;
	}
	
	return result;
}

// Converts an bit vector back into a String of '0' and '1' chars.
// Treat the order of bits the same as in string2bits.
string bits2string(const vector<bool>& bits) { 
	string result = "";
	
	for (int i = 0; i < bits.size(); ++i) {
		result.push_back(bits[i] ? '1' : '0') ;
	}
	
	return result;
}

// Converts a bit vector into an integer value.
// Treat the order of bits the same as in string2bits.
long bits2decimal(const vector<bool>& bits) {
	long result = 0;
	
	if (bits[0]) {
		result = -((long)1 << (bits.size()-1));
	}
	
	for (long i = 1; i < bits.size(); ++i) {
		if (bits[i]) {
			result += ((long)1 << (bits.size()-1-i));
		}
	}

	return result;
}

// Add two bit vectors
// results in array of bits same size as arguments (i.e overflow ingnored)
void add(const vector<bool>& bits1, const vector<bool>& bits2, vector<bool>& result) {
	bool carry = false;
	
	for (long i = bits1.size()-1; i >= 0; --i) {
		if (bits1[i] && bits2[i] && carry) {
			result[i] = 1;
			carry = 1;
		} else if ((bits1[i] && bits2[i]) || (bits1[i] && carry) || (bits2[i] && carry)) {
			result[i] = 0;
			carry = 1;
		} else if ((bits1[i] ^ bits2[i] ^ carry) && !(bits1[i] && bits2[i] && carry)) {
			result[i] = 1;
			carry = 0;
		} else if (!bits1[i] && !bits2[i] && !carry) {
			result[i] = 0;
			carry = 0;
		}
	}
}

// Subtract two bit vectors
// Use Twos Complement method
void sub(const vector<bool>& bits1, const vector<bool>& bits2, vector<bool>& result) {
	vector<bool> neg(bits2);

	twosComplement(neg);
	
	add(bits1, neg, result);
}

// Multiply two bit vectors, (overflow ignored)
void mult(const vector<bool>& bits1, const vector<bool>& bits2, vector<bool>& result) {
	vector<bool> multiplicand(bits1);
	vector<bool> multiplier(bits2);
	
	for (long i = bits2.size()-1; i >= 0; --i) {
		if (bits2[i]) {
			add(result, multiplicand, result);
			shiftLeft(multiplicand);
			shiftRight(multiplier);
		} else {
			shiftLeft(multiplicand);
			shiftRight(multiplier);
		}
	}
}

// Divide two bit vectors
// Returns string with the quotient joined with remainder by R
void divide(const vector<bool>& bits1, const vector<bool>& bits2, vector<bool>& result, string& divString) {
	bool divideByZero = true;

	for (long i = 0; i < bits2.size(); ++i) {
		if (bits2[i]) {
			divideByZero = false;
		}
	}

	if (divideByZero) {
		divString = "ERROR";
		return;
	} else {
	
		vector<bool> remainder(bits1);
		vector<bool> divisor(bits2);
		vector<bool> quotient(bits1.size(), 0);
		
		if (remainder[0]) {
			twosComplement(remainder);
		}
		
		if (divisor[0]) {
			twosComplement(divisor);
		}
		
		for (long i = 0; i < bits1.size(); ++i) {
			remainder.insert(remainder.begin(), 0);
			divisor.insert(divisor.end(), 0);
		}
		
		for (long i = 0 ; i <= bits1.size(); ++i) {
			sub(remainder, divisor, remainder);
			
			if (remainder[0]) {
				add(remainder, divisor, remainder);
				shiftLeft(quotient);
			} else {
				shiftLeft(quotient);
				quotient[quotient.size()-1] = 1;
			}
			
			shiftRight(divisor);
		}
		
		if (bits1[0] ^ bits2[0]) {
			twosComplement(quotient);
		}
		
		if (bits1[0] ^ remainder[0]) {
			twosComplement(remainder);
		}
		
		for (long i = 0; i < bits1.size(); ++i) {
			remainder.erase(remainder.begin());
		}
		
		divString = to_string(bits2decimal(quotient));
		divString += "R";
		divString += to_string(bits2decimal(remainder));
	}
}

void twosComplement(vector<bool>& bits) {
	vector<bool> one(bits.size(), 0);
	one[one.size()-1] = 1;
	for (long i = 0; i < bits.size(); ++i) {
		bits[i] = !bits[i];
	}
	add(bits, one, bits);
}

void shiftLeft(vector<bool>& bits) {
	for (long i = 0; i < bits.size()-1; ++i) {
		bits[i] = bits[i+1];
	}
	bits[bits.size()-1] = 0;
}

void shiftRight(vector<bool>& bits) {
	vector<bool> temp(bits);
	for (long i = 1; i < bits.size(); ++i) {
		bits[i] = temp[i-1];
	}
	bits[0] = 0;
}

