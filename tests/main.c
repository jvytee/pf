#include <check.h>
#include <stdlib.h>

#include "cli.h"

START_TEST(test_cli) {
  char *const image = "some_image.png";
  char *const argv[] = {"pf", image};

  struct arguments args;
  int res = parse_arguments(2, argv, &args);

  ck_assert_int_eq(res, CLI_SUCCESS);
  ck_assert_str_eq(args.image, image);
  ck_assert_ptr_null(args.host);
  ck_assert_int_eq(args.port, 0);
}
END_TEST

START_TEST(test_cli_network) {
  char *const image = "some_image.png";
  char *const host = "example.org";
  char *const port = "1234";
  uint16_t port_num = 1234;
  char *const argv[] = {"pf", image, "-H", host, "-p", port};

  struct arguments args;
  int res = parse_arguments(6, argv, &args);

  ck_assert_int_eq(res, CLI_SUCCESS);
  ck_assert_str_eq(args.image, image);
  ck_assert_str_eq(args.host, host);
  ck_assert_int_eq(args.port, port_num);
}
END_TEST

START_TEST(test_cli_help) {
  char *const argv[] = {"pf", "-h"};

  struct arguments args;
  int res = parse_arguments(2, argv, &args);

  ck_assert_int_eq(res, CLI_HELP);
}
END_TEST

Suite *main_suite(void) {
  Suite *s = suite_create("Main");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_cli);
  tcase_add_test(tc, test_cli_network);
  tcase_add_test(tc, test_cli_help);

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
