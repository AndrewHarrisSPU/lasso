// 10 line demo ... but more readable as 20 lines.

#include "lasso.h"

bool supposedlyTrue();
void supposedlyTrue_test(); 

int main(){
	lasso<> test( "supposedly true test", &supposedlyTrue_test );
	test.Run();
	std::cout << test.TerminalString();
}

bool supposedlyTrue(){
	return false;
}

void supposedlyTrue_test(){
	Expect( "true is supposedly true", true, supposedlyTrue() );
}