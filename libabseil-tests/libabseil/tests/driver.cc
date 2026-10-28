#include <gtest/gtest.h>

// Run the upstream GoogleTest test cases linked into this driver.
//
int
main (int argc, char* argv[])
{
  testing::InitGoogleTest (&argc, argv);
  return RUN_ALL_TESTS ();
}
