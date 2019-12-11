// miniunit
// ========
//
// Minimalistic unitets framework.
// Just one header file.
//
// Requirements
// ------------
//
// C++11
//
// Usage example
// -------------
//
// TEST(TEST1, "Test description for TEST1") {
//   return true; // PASS
// }
//
// TEST(TEST2, "Test description for TEST2") {
//   return false; // FAIL
// }
//
// // Custom test case
// class MyTestCase : public TestSuite::TestCase {
// public:
//   MyTestCase(const char* name, const char* description)
//     : TestSuite::TestCase(name, description) {
//     // Setup action in constructor
//   }
//   ~MyTestCase() {
//     // Tear down actions in destructor
//   }
//   bool test() {
//     // Test body
//     return true;
//   };
// } MyTestCase_instance("TEST3", "Description for custom test case");
//
// int main(void) {
//   bool result = TestSuite::run();
//   std::cout << "Result: " << (result ? "PASS" : "FAIL") << std::endl;
//   return (result ? 0 : 1);
// }

#ifndef MINIUNIT_H
#define MINIUNIT_H

#include <string>
#include <vector>
#include <iostream>  // cout
#include <iomanip>   // setw

namespace miniunit {

// Test suite controls test execution
class TestSuite {
public:
  // Abstract test case
  class TestCase {
  public:
    TestCase() {};
    TestCase(const char* n, const char* d)
      : name(n), description(d), skip(false) {
        TestSuite::add(this);
    };

    virtual bool test() = 0;

    virtual void setSkip(const char* r) {
      skip = true;
      reason = r;
    };

    std::string name;
    std::string description;
    bool skip;
    std::string reason;
  };

  static TestSuite& get() {
    static TestSuite testSuite;
    return testSuite;
  };

  static void add(TestCase* test) {
    get().tests.push_back(test);
  };

  static std::vector<TestCase*>& getTests() {
    return get().tests;
  };

  static bool run(bool color = true) {
    std::string pass = "PASS";
    std::string fail = "FAIL";
    std::string skip = "SKIP";
    if (color) {
      const std::string colorRed    = "\x1b[31m";
      const std::string colorGreen  = "\x1b[32m";
      const std::string colorYellow = "\x1b[33m";
      const std::string colorReset  = "\x1b[0m";
      pass = colorGreen  + pass + colorReset;
      fail = colorRed    + fail + colorReset;
      skip = colorYellow + skip + colorReset;
    }

    size_t nameLen = 0;
    for (auto t : getTests())
      nameLen = (t->name.size() > nameLen ? t->name.size() : nameLen);

    size_t descLen = 0;
    for (auto t : getTests())
      descLen = (t->description.size() > descLen ? t->description.size() : descLen);

    bool allResults = true;
    for (auto t : getTests()) {
      std::cout << std::setw(nameLen + 4) << "[" + t->name + "] ";
      std::cout << std::setw(descLen + 3) << t->description + " : ";
      if (!t->skip) {
        bool result = false;
        std::string reason;
        try {
          result = t->test();
        } catch (...) {
          reason = "Exception!";
        }
        std::cout << (result ? pass : fail) << " " << reason << std::endl;
        allResults = allResults && result;
      } else {
        std::cout << skip << " " << t->reason << std::endl;
      }
    }
    return allResults;
  };

private:
  std::vector<TestCase*> tests;
};

// TEST macro to use when defining tests
#define TEST(name, description)                         \
class TEST_##name##_Test :                              \
      public miniunit::TestSuite::TestCase {            \
public:                                                 \
  TEST_##name##_Test(const char* n, const char* d)      \
    : miniunit::TestSuite::TestCase(n, d) {}            \
  bool test();                                          \
} TEST_##name##_Test_instance(#name, description);      \
bool TEST_##name##_Test::test()

// SKIP macro to define test as temporarily disabled
#define SKIP(name, reason) TEST_##name##_Test_instance.setSkip(reason)

};  // namespace miniunit

#endif  // MINIUNIT_H
