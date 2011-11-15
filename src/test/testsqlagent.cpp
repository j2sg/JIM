#include "testsqlagent.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include "sqlagent.h"

void Test::TestSQLAgent::setUp()
{
    _agent = Persistence::SQLAgent::instance();
}

void Test::TestSQLAgent::tearDown()
{
}

CppUnit::Test *Test::TestSQLAgent::suite()
{
    CppUnit::TestSuite *suite = new CppUnit::TestSuite("TestSQLAgent");
    suite -> addTest(new CppUnit::TestCaller<Test::TestSQLAgent>("TestInstance", &Test::TestSQLAgent::testInstance));
    return suite;
}

void Test::TestSQLAgent::testInstance()
{
    CPPUNIT_ASSERT(Persistence::SQLAgent::instance() == _agent);
}

void Test::TestSQLAgent::testInsert()
{
    QString sql=QString("INSERT INTO prueba WHERE ID=5");
    CPPUNIT_ASSERT(_agent -> insert(sql));
}

void Test::TestSQLAgent::testUpdate()
{

}

void Test::TestSQLAgent::testSelect()
{

}

void Test::TestSQLAgent::testDelete()
{

}

void Test::TestSQLAgent::testGetId()
{

}
