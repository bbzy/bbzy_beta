#include "detail/test_unify.hpp"
#include "detail/test_type.hpp"
#include "detail/test_disambiguate.hpp"
#include "detail/test_automatic.hpp"
#include "detail/test_repeat.hpp"
#include "detail/test_lock.hpp"

#include <iostream>

int main()
{
	test_type();
	test_unity();
	test_disambiguate();
	test_automatic();
	test_repeat();
    test_lock();
	std::cout << "Test All OK" << std::endl;
}
