# CMake generated Testfile for 
# Source directory: D:/evolutionary-watches/tests
# Build directory: D:/evolutionary-watches/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(GenomeValidation "D:/evolutionary-watches/build/tests/test_genome.exe")
set_tests_properties(GenomeValidation PROPERTIES  _BACKTRACE_TRIPLES "D:/evolutionary-watches/tests/CMakeLists.txt;8;add_test;D:/evolutionary-watches/tests/CMakeLists.txt;0;")
add_test(AlgorithmValidation "D:/evolutionary-watches/build/tests/test_algorithm.exe")
set_tests_properties(AlgorithmValidation PROPERTIES  _BACKTRACE_TRIPLES "D:/evolutionary-watches/tests/CMakeLists.txt;15;add_test;D:/evolutionary-watches/tests/CMakeLists.txt;0;")
add_test(FileIOValidation "D:/evolutionary-watches/build/tests/test_fileio.exe")
set_tests_properties(FileIOValidation PROPERTIES  _BACKTRACE_TRIPLES "D:/evolutionary-watches/tests/CMakeLists.txt;22;add_test;D:/evolutionary-watches/tests/CMakeLists.txt;0;")
