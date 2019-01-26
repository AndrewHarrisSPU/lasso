#include <iostream>
#include "lasso.h"

uint mult( uint, uint );
uint badMult( uint, uint );
uint fib( uint );
uint fact( uint );

void mult_test();
void fib_test();
void fact_test();

int main()
{
	// PART I
	// Use Expect( "description", expected result, expression to evaluate )

	// 1) Simple example of an Expect() call
	// This is so, so not much happens.
	Expect( "a) four is 2 plus 2", 4, 2 + 2 );

	// It isn't so, so a failure will appear in std::cout
	Expect( "b) five is 2 plus 2", 5, 2 + 2 );

	// 2) What's useful: Function calls in Expect()
	// The expression to evaluate is a function call
	Expect( "c) four is 2 times 2", 4, mult( 2, 2 ));

	// Will generate console output.
	//There is a problem with this 'badMult' function!
	Expect( "d) four is 2 times 2", 4, badMult( 2, 2 ));

	// Will *not* generate console output.
	// Broken functions can, of course, pass some (even many) test cases
	Expect( "e) 0 is 0 times 2", 0, badMult( 0, 2 ));

	// Will generate console output.
	// It should never be forgotten that occasionally the test case is wrong!
	Expect( "f) five is 2 times 2", 5, mult( 2, 2 ));

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
		{ "multiply (recursive)", &mult_test },
		{ "fibonacci (recursive)", &fib_test },
		{ "factorial (recursive)", &fact_test },
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

	// 1) The ugly way: work out the pointer signature in the lasso<> constructor
	lasso< uint ( * )( uint ), uint > fibber( "fibber", &fib );

	fibber.Run( 35 );
	std::cout << fibber.TerminalString();

	// 2) The saner way: work out the pointer signature and assign to a symbol with 'using'
	using twoNumberFunc = uint ( * )( uint, uint );
	lasso< twoNumberFunc, uint, uint > multer( "multer", &mult );

	multer.Run( 3, 9 );
	std::cout << multer.TerminalString();

	// 3) Sane way made less sane and more fun with a lambda
	auto fibplex = []( uint a, uint b ){
		return fib( a ) - fib( b );
	};

	lasso< twoNumberFunc, uint, uint > fibplexer( "fibplex", fibplex );

	fibplexer.Run( 35, 34 );
	std::cout << fibplexer.TerminalString();

	// EXIT
	return 0;
}

// Mult():
// An inefficient but working multiply function
uint mult( uint m, uint n ){
	if( n == 0 ) return 0;
	if( n == 1 ) return m;

	--n;
	return m + mult( m, n );
}

// Mulitply test cases
void mult_test() {
	struct testObj {
		std::string name;
		uint m;
		uint n;
		uint result;
	};

	testObj table[] = {
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
		{	.name = "fib-complex",
			.m = fib( 35 ),
			.n = 1,
			.result = 9227465,
		},
/*		{	.name = "fib-complex crash",
			.m = 0,
			.n = fib( 35 ),
			.result = 0,
		},
*/	};

	for( auto t : table ){
		Expect( t.name, t.result, mult( t.m, t.n ));
	}
}

// badMult():
// For demonstration purposes!
uint badMult( uint x, uint y ){
	return 0 * y;
}

// Fibonacci numbers
// (See also: Lucas numbers)
uint fib( uint n ){
	if( n == 0 || n == 1 ){
		return n;
	}

	return fib( n - 1 ) + fib( n - 2 );
}

// Fibonacci test cases
void fib_test() {
	struct testObject {
		std::string name;
		int n;
		int result;
	};

	testObject table[] = {
		{	.name = "zero",
			.n = 0,
			.result = 0,
		},
		{	.name = "one",
			.n = 1,
			.result = 1,
		},
		{	.name = "ten",
			.n = 10,
			.result = 55,
		},
		{	.name = "long execution",
			.n = 40,
			.result = 102334155,
		},
/*		{	.name = "long execution fail",
			.n = 42,
			.result = 1,
		},
*/	};

	for( auto t : table ){
		Expect( t.name, t.result, fib( t.n ));
	}

	return;
}

// Factorial
uint fact( uint n ){
	if( n == 0 ) return 1;

	return n * fact( n - 1 );
}

// Factorial test cases
void fact_test() {
	struct testObject {
		std::string name;
		int n;
		int result;
	};

	testObject table[] = {
		{	.name = "zero",
			.n = 0,
			.result = 1,
		},
		{	.name = "five",
			.n = 5,
			.result = 120,
		},
		{	.name = "big number",
			.n = 30,
			.result = 1409286144,
		},
/*		{	.name = "example of test being wrong",
			.n = 6,
			.result = 270,
		},
		{	.name = "again",
			.n = 7,
			.result = 720,
		},
*/	};

	for( auto t : table ){
		Expect( t.name, t.result, fact( t.n ));
	}

	return;
}