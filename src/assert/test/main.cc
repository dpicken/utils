#include "testAssertWithNDEBUG.h"
#include "testAssertWithoutNDEBUG.h"
#include "testStaticAssert.h"
#include "testVerify.h"

int main(int, char**) {
  assert::test::testStaticAssert();
  assert::test::testVerify();
  assert::test::testAssertWithNDEBUG();
  assert::test::testAssertWithoutNDEBUG();
  return 0;
}
