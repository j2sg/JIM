#ifndef TESTSQLAGENT_H
#define TESTSQLAGENT_H

#include <cppunit/Test.h>
#include <cppunit/TestFixture.h>
#include "sqlagent.h"

namespace Persistence
{
    class SQLAgent;
}

namespace Test
{
    class TestSQLAgent : public CppUnit::TestFixture
    {
    public:
        void setUp();
        void tearDown();
        static CppUnit::Test *suite();
        void testInstance();
        void testInsert();
        void testUpdate();
        void testSelect();
        void testDelete();
        void testGetId();

    private:
        Persistence::SQLAgent *_agent;
    };
}

#endif // TESTSQLAGENT_H
