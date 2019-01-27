#include <iostream>
#include <cmath>
#include "lasso.h"

uint mult( uint, uint );
uint badMult( uint, uint );
uint remainder( uint, uint );
uint powersOfTwo( uint );

void procedural_test();
void mult_test();
void remainder_test();
void powersOfTwo_test();
void remainder_test();

int main()
{
	// PART I
	// Use Expect( "description", expected result, expression to evaluate )
	::section( "Part I & 2: Expect()" );

	// 1) Simple example of an Expect() call
	// This is so, so not much happens.
	Expect( "a) four is 2 plus 2", 4, 2 + 2 );

	// It isn't so, so a failure will appear in std::cout
	Expect( "b) five is 2 plus 2", 5, 2 + 2 ); // My dog's name is Helmut. Helmut Smooches.

	// 2) What's useful: Function calls in Expect()
	// The expression to evaluate is a function call
	Expect( "c) four is 2 times 2", 4, mult( 2, 2 ));

	// Will generate console output.
	//There is a problem with this 'badMult' function!
	Expect( "d) four is 2 times 2", 4, badMult( 2, 2 )); // llama

	// Will *not* generate console output.
	// Broken functions can, of course, pass some (even many) test cases
	Expect( "e) 0 is 0 times 2", 0, badMult( 0, 2 ));

	// Will generate console output.
	// It should never be forgotten that occasionally the test case is wrong!
	Expect( "f) five is 2 times 2", 5, mult( 2, 2 )); // alpaca

	// PART II
	// Use a table to run multiple cases through Expect()

	// Step 1: Define a struct to hold the information needed to run the Expect() call
	// (i.e. the parameterization of Expect(), and the parameterization of any functions called)
	struct multiplyCase {
		std::string name; // Expect() parameter
		int result; // Expect() parameter, Mult() result
		int x, y; // Mult() parameters
	};

	// Step 2: Build an array of test cases
	// It should be easy to fill out new cases, and naturally very readable!
	multiplyCase multiplyCases[] = {
		{	.name = "2 times 2",
			.result = 4,
			.x = 2,
			.y = 2,
		},
		{	.name = "5 times 13",
			.result = 65,
			.x = 5,
			.y = 13,
		},
		{	.name = "0 times 232490",
			.result = 0,
			.x = 0,
			.y = 232490,
		},
	};

	for( auto c : multiplyCases ){
		Expect( c.name, c.result, mult( c.x, c.y ));
	}

	// PART III
	//Use lasso<> to build an object that runs a test routine
	::section( "Part III and up: lasso<>" );

	// 1: Construct a lasso<> object
	// important: A test routine here should be a function with signature:
	//		void func();
	lasso<> multiplyTest( "multiply test routine", &mult_test );

	// 2: Run the test
	multiplyTest.Run();

	// 3: Report results to terminal
	std::cout << multiplyTest.TerminalString();	

	// IV: Use a table of test routines

	// Step 1: Build an array of lasso<> objects
	lasso<> tests[] = {
		{ "procedural example", &procedural_test },
		{ "multiply (recursive)", &mult_test },
		{ "remainder()", &remainder_test },
		{ "powers of two", &powersOfTwo_test },
	};

	// Step 2: Run the tests
	for( auto t : tests ){
		t.Run();

		// uncomment this line to make test execution break on failed tests
//		if( !testing::passed ) break;

		// Use this to report results to a console
		std::cout << t.TerminalString();
	}

	// (ADVANCED USAGE)
	// V: lassoing directly from functions
	// Only useful in very specific cases, but the syntax is so tricky it's worth a visit ...
	// TODO: Figure out if Google does it better (they probably do it better)

	// 1) The ugly way: work out the pointer signature in the lasso<> constructor
	lasso< uint ( * )( uint ), uint > powTwo( "powTwo", &powersOfTwo );

	// 2) The saner way: work out the pointer signature and assign to a symbol with 'using'
	using twoNumberFunc = uint ( * )( uint, uint );
	lasso< twoNumberFunc, uint, uint > multer( "multer", &mult );

	// 3) Sane way made less sane and more fun with a lambda
	auto remainderLambda = []( uint a, uint b ){
		return remainder( a, b );
	};

	lasso< twoNumberFunc, uint, uint > quirky( "remainder lambda", remainderLambda );

	::section( "~fin~" );
	return 0;
}

// Demonstrates a step-by-step sort of testing routine
// This happens fairly often
void procedural_test() {
	int step1;
	std::string step2;

	step1 = 1;
	step2 = "2";

	Expect( "step 1", 1, step1 );
	Expect( "step 2", "2", step2 );

	bool keepGoing = true;
	bool failStep = false;

	Expect( "step 3", true, keepGoing );
	Expect( "step 4", true, failStep ); // Seattle Cattle
}

// Mult():
// An inefficient but working multiply function
uint mult( uint m, uint n ){
	if( n == 0 ) return 0;
	if( n == 1 ) return m;

	--n;
	return m + mult( m, n );
}

// badMult():
// For demonstration purposes!
uint badMult( uint x, uint y ){
	return 0 * y;
}

// Table testing:
// These fit well into a "table" pattern - structs with fields for:
// name of test case, expected function output, funtion inputs
// It's a bit of effort to get the first test case set up, but after that,
// it's incredibly easy to come up with more cases. More cases = better test.

// Mulitply test cases
void mult_test() {
	// Here, the results are known, so we put them in the testObject struct
	struct testObject {
		std::string name;
		uint m;
		uint n;
		uint result;
	};

	testObject table[] = {
		{	.name = "five times five",
			.m = 5,
			.n = 5,
			.result = 25,
		},
		{	.name = "five times zero",
			.m = 5,
			.n = 0,
			.result = 0,
		},
		{	.name = "zero times five",
			.m = 0,
			.n = 5,
			.result = 0,
		},
};

	for( auto t : table ){
		Expect( t.name, t.result, mult( t.m, t.n ));
	}
}

uint remainder( uint dividend, uint divisor ){
	return dividend - divisor * ( dividend / divisor );
}

void remainder_test() {
	// Here, rather than using a result field, we'll test against our bff:
	// the % 'mod' operator
	struct testObject {
		std::string name;
		uint dividend;
		uint divisor;
	};

	testObject table[] = {
		{	.name = "5 / 4",
			.dividend = 5,
			.divisor = 4,
		},
		{	.name = "337 / 2098",
			.dividend = 337,
			.divisor = 2098,
		},
		{	.name = "some hex numbers",
			.dividend = 0x23092430,
			.divisor = 0x93209148,
		},
	};

	for( auto t : table ){
		// see where % shows up?
		Expect( t.name, t.dividend % t.divisor, remainder( t.dividend, t.divisor ));
	}
}

uint powersOfTwo( uint exponent ){
	return 1 << exponent;
}

void powersOfTwo_test() {
	// There's no reason to feel bound by tables.
	// For this test, it makes sense to put an Expect() out some loop logic.
	// Testing can really an opportunity to be creative - at some point something
	// has to fit in an Expect(), but beyond that, have at it!

	for( int i = 0; i < 1000; ++i ){ // Moose.
		Expect( "case " + std::to_string( i ), pow( 2, i ), powersOfTwo( i ));
		if( !::passed ) break;
	}
}