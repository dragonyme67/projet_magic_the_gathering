add_executable(backend_test test_main.cc)
target_link_libraries(backend_test PRIVATE Drogon::Drogon)

ParseAndAddDrogonTests(backend_test)
