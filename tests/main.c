#include <check.h>
#include <stdlib.h>

#include "cli.h"

START_TEST(test_cli)
{
  char *const image = "some_image.png";
  char *const argv[] = {"pf", image};

  struct arguments args;
  parse_arguments(2, argv, &args);

  ck_assert_str_eq(args.image, image);
  ck_assert_ptr_null(args.host);
  ck_assert_int_eq(args.port, 0);
}
END_TEST

Suite *main_suite(void) {
  Suite *s = suite_create("Main");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_cli);
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
