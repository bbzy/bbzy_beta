#pragma once
#include "bbzy_beta/automatic.hpp"

#include <iostream>
#include <functional>
#include <assert.h>

template <class = void>
void test_releaser()
{
	using namespace bbzy::automatic;
	struct S
	{
		static int& count()
		{
			static int a = 0;
			return a;
		}

		static void addCount()
		{
			++count();
		}

		static void addParamCount(int* inoutCount)
		{
			if (inoutCount)
			{
				++*inoutCount;
			}
		}

		static void addSRefCount(S& s)
		{
			++s.b;
		}


		static void addSPtrCount(S* s)
		{
			++s->b;
		}
		
		void addCountMemberFunction()
		{
			++b;
		}

		static void recvSCRef(const S& s)
		{
		}

		static void recvSCPtr(const S* s)
		{
		}

		void recvCMemberFunction() const
		{
		}

		int b = 0;
	};

	{
		S::count() = 0;
		auto funReleaser = createReleaser(S::addCount);
		assert(S::count() == 0);
	}
	assert(S::count() == 1);

	{
		S::count() = 0;
		auto funPtrReleaser = createReleaser(&S::addCount);
		assert(S::count() == 0);
	}
	assert(S::count() == 1);

	{
		S::count() = 0;
		auto functorReleaser = createReleaser(std::bind(S::addParamCount, &S::count()));
		assert(S::count() == 0);
	}
	assert(S::count() == 1);

	S s;
	{
		s.b = 0;
		auto memberFunctionReleaser = createReleaser(&s, &S::addCountMemberFunction);
		assert(s.b == 0);
	}
	assert(s.b == 1);

	{
		s.b = 0;
		auto funReleaser = createReleaser(&s, &S::addSPtrCount);
		assert(s.b == 0);
	}
	assert(s.b == 1);

	{
		s.b = 0;
		auto funReleaser = createReleaser(&s, &S::addSRefCount);
		assert(s.b == 0);
	}
	assert(s.b == 1);

	const S cs = {};
	createReleaser(&cs, &S::recvSCPtr);
	createReleaser(&cs, &S::recvSCRef);
	createReleaser(&cs, &S::recvCMemberFunction);
}

template <class = void>
void test_cancellableReleaser()
{
	using namespace bbzy::automatic;
	struct S
	{
		static void add()
		{
			++count();
		}

		static void addParam(int* inoutCount)
		{
			if (inoutCount)
			{
				++*inoutCount;
			}
		}

		static int& count()
		{
			static int count = 0;
			return count;
		}
	};
	
	{
		S::count() = 0;
		auto funReleaser = createCancellableReleaser(S::add);
		assert(S::count() == 0);
	}
	assert(S::count() == 1);

	{
		S::count() = 0;
		auto funPtrReleaser = createCancellableReleaser(&S::add);
		assert(S::count() == 0);
	}
	assert(S::count() == 1);

	{
		S::count() = 0;
		auto functorReleaser = createCancellableReleaser(std::bind(S::addParam, &S::count()));
		assert(S::count() == 0);
	}
	assert(S::count() == 1);

	{
		S::count() = 0;
		auto funReleaser = createCancellableReleaser(S::add);
		funReleaser.cancel();
		assert(S::count() == 0);
	}
	assert(S::count() == 0);

	{
		S::count() = 0;
		auto funReleaser = createCancellableReleaser(S::add);
		funReleaser.cancel();
		funReleaser.reenable();
		assert(S::count() == 0);
	}
	assert(S::count() == 1);
}

template <class = void>
void test_automatic()
{
	test_releaser();
	test_cancellableReleaser();
	std::cout << "Test Automatic OK" << std::endl;
}
