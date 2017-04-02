#include "unify/test_unify.hpp"
#include "type/test_type.hpp"
#include "disambiguate/test_disambiguate.hpp"
#include "automatic/test_automatic.hpp"

#include <iostream>

int main()
{
	test_type();
	test_unity();
	test_disambiguate();
	test_automatic();
	std::cout << "Test All OK" << std::endl;
}
