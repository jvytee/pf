#include <check.h>
#include <stdlib.h>

START_TEST(example_test)
{
  ck_assert_int_eq(1, 1);
}
END_TEST

Suite *main_suite(void) {
  Suite *s = suite_create("Main");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, example_test);
  suite_add_tcase(s, tc);

  return s;
}

int main(void) {
  Suite *s = main_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  int total = srunner_ntests_run(sr);
  int failed = srunner_ntests_failed(sr);
  
  return total > 0 && failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
