/******************************************************************************
 * Unit tests for OTP common library.
 * 
 * AUTHOR
 *      Zach Colbert (colbertz@oregonstate.edu)
 * 
 * DATE
 *      7 March 2020
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "../src/otp.h"

int main() {
    int i;
    char x;
    int test_OTP_CHARSPACE_SIZE, test_otp_ctoi, test_otp_itoc;
    int test_otp_strip1, test_otp_strip2, test_otp;
    char buffer[100];

    printf("==== OTP LIBRARY UNIT TESTS ====\n");

    /** OTP_CHARSPACE_SIZE **/
    printf("unit test: OTP_CHARSPACE_SIZE: ");
    test_OTP_CHARSPACE_SIZE = OTP_CHARSPACE_SIZE == 27;

    if (test_OTP_CHARSPACE_SIZE) { printf("PASS"); }
    else { printf("FAIL (value: %d)", OTP_CHARSPACE_SIZE); }
    printf("\n"); fflush(stdout);

    /** otp_ctoi() **/
    printf("unit test: otp_ctoi(): ");
    
    i = 0;
    x = ' ';
    test_otp_ctoi = otp_ctoi(x) == i;   // Edge case: space chararacter
    
    i = 1;
    while (i < OTP_CHARSPACE_SIZE && test_otp_ctoi) {
        test_otp_ctoi = otp_ctoi( (char)(i+64) ) == i;
        i++;
    }

    if (test_otp_ctoi) { printf("PASS"); }
    else { i--; printf("FAIL (value: %d / %c)", i, (char)(i+64)); }
    printf("\n"); fflush(stdout);

    /** otp_itoc() **/
    printf("unit test: otp_itoc(): ");

    i = 0;
    x = ' ';
    test_otp_itoc = otp_itoc(i) == x;

    i = 1;
    while(i < OTP_CHARSPACE_SIZE && test_otp_itoc) {
        test_otp_itoc = otp_itoc(i) == (char)(i+64);
        i++;
    }

    if (test_otp_itoc) { printf("PASS"); }
    else { i--; printf("FAIL (value: %d / %c)", i, (char)(i+64)); }
    printf("\n"); fflush(stdout);

    /** otp_strip_newline() **/
    printf("unit test: otp_strip_newline(): ");

    strcpy(buffer, "This string is terminated with a newline\n");
    test_otp_strip1 = strstr(buffer, "\n") != NULL;
    otp_strip_newline(buffer);
    test_otp_strip2 = strstr(buffer, "\n") == NULL;

    if (test_otp_strip1 && test_otp_strip2) { printf("PASS"); }
    else { printf("FAIL (value: %s)", buffer); }
    printf("\n"); fflush(stdout);

    /** Finally, print status 1 if any tests fail **/
    test_otp = test_OTP_CHARSPACE_SIZE && test_otp_ctoi && test_otp_itoc;
    test_otp = test_otp && test_otp_strip1 && test_otp_strip2;
    if (!test_otp) {
        return 1;
    }

    return 0;
}