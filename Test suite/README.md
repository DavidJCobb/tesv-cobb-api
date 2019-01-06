This test suite is useful for performing quick API tests, and is also suitable for release as a public demo. It's a single MCM that can fire most of our APIs.

Key limits:

* We cannot use this to test any APIs that are limited to a single call stack.

* We cannot use this to test any APIs that accept an ActiveMagicEffect as an argument. The test suite uses arrays to store the arguments you've set, but the Papyrus compiler doesn't recognize or accept an *ActiveMagicEffect[]* type.

* There isn't any code or localized content for methods that return *Bool[]*, as it's impossible to return a bool array from an SKSE plug-in without the array contents getting mangled.

To make a Papyrus class available through this test suite, you must:

* Create a Quest that can host an "executor" script.

* Use the "TestSuiteProc.html" file to generate an "executor" script from your class's script file.

* Modify the core test suite script: it has a getter for an array of executors, and you will need to add yours to it. (I might change this to a FormList in the future.)
