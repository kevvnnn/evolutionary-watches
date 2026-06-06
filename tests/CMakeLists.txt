enable_testing()

# ----------------------------------------------------
# Test Binary for Person A (Genome Architecture)
# ----------------------------------------------------
add_executable(test_genome test_genome.cpp)
target_link_libraries(test_genome PRIVATE watch_genome)
add_test(NAME GenomeValidation COMMAND test_genome)

# ----------------------------------------------------
# Test Binary for Person B (GA Logic & Optimization)
# ----------------------------------------------------
add_executable(test_algorithm test_algorithm.cpp)
target_link_libraries(test_algorithm PRIVATE watch_algorithm watch_fileio)
add_test(NAME AlgorithmValidation COMMAND test_algorithm)

# ----------------------------------------------------
# Test Binary for Person C (Serialization / File IO)
# ----------------------------------------------------
add_executable(test_fileio test_fileio.cpp) 
target_link_libraries(test_fileio PRIVATE watch_fileio watch_algorithm) 
add_test(NAME FileIOValidation COMMAND test_fileio)