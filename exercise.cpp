#include <iostream>
#include "lasso.h"

// Step 1: Fill out the lasso<> test definition in main()
// Step 2: What method (function) is used to run the test?
// Step 3: What method (function) is used to report test results to the terminal?
// Step 4: Define four fields for a test case
	// hint: what does an Expect() call need?
// Step 5: 
	// reminder: each line will look like:
	// .field = value,
// Step 6: fill in the expect call

int exponential( int, int );
void exponential_test();

using std::cout;

int main(){

//	lasso<> test( /* 1.1 */, /* 1.2 */ )

	test./* ? */

	cout << /* 3 */
}

int exponential( int base, int exponent ){
	if( exponent == 0 ) return 1;
	if( exponent == 1 ) return base;

	--exponent;
	return base * exponential( base, exponent );
}

void exponential_test() {
	struct testObject {
		/* 4.1 */,
		/* 4.2 */,
		/* 4.3 */,
		/* 4.4 */,
	};

	testObject table[] = {
		{	/* 5.1 */,
			/* 5.2 */,
			/* 5.3 */,
			/* 5.4 */,
		},
		/* 7 */
	};

	for( auto t : table ){
		Expect(
			/* 6.1 */,
			/* 6.2 */,
			exponentinal( /* 6.3 */, /* 6.4 */ ));
	}
}