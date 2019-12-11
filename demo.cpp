#include "miniunit.h"  // TEST, SKIP, TestSuite, TestCase
#include <cstdio>      // printf

using namespace miniunit;

// Test PASS example
TEST(TEST1, "Test description") {
  return true;
}

// Test FAIL example
TEST(TEST2, "This test should fail") {
  return false;
}

// Custom test case
class MyTestCase : public TestSuite::TestCase {
public:
  MyTestCase(const char* n, const char* d)
    : TestSuite::TestCase(n, d) {
  }
  ~MyTestCase() {
  }
  bool test() {
    return true;
  };
} MyTestCase_instance("TEST3", "Custom test case");

// Exception example
TEST(TEST4, "Throw exception") {
  throw "Oh my god!";
  return true;
}

// Custom test run function
class MyTestSuite : public TestSuite {
public:
  static bool run() {
    bool allResults = true;
    printf("| %-8s | %-30s | %-6s | %-20s |\n",
           "Test", "Description", "Result", "Comment");
    printf("| %-8s | %-30s | %-6s | %-20s |\n",
           std::string(8, '=').c_str(),
           std::string(30, '=').c_str(),
           std::string(6, '=').c_str(),
           std::string(20, '=').c_str());
    for (auto t : getTests()) {
      printf("| %-8s | %-30s |", t->name.c_str(), t->description.c_str());
      const char* resultStr = "OK";
      const char* reasonStr = "";
      if (!t->skip) {
        bool result = false;
        try {
          result = t->test();
        } catch (...) {
          reasonStr = "Exception!";
        }
        if (!result) resultStr = "Fail!";
        allResults = allResults && result;
      } else {
        resultStr = "Skip";
        reasonStr = t->reason.c_str();
      }
      printf(" %-6s | %-20s |\n", resultStr, reasonStr);
    }
    return allResults;
  };
};

// Test run example
int main(void) {
  SKIP(TEST1, "testing skip");

  printf("Default test run function:\n");
  TestSuite::run();
  printf("\n");

  printf("Custom test run function:\n");
  bool result = MyTestSuite::run();
  printf("\n");

  printf("Result: %s\n", (result ? "PASS" : "FAIL"));
  return (result ? 0 : 1);
}
