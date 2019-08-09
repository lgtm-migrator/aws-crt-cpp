/*
 * Copyright 2010-2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
#include <aws/crt/DateTime.h>

#include <aws/crt/ByteBuf.h>
#include <aws/testing/aws_test_harness.h>

static int s_TestDateTimeBinding(struct aws_allocator *allocator, void *ctx)
{
    (void)allocator;
    (void)ctx;

    const char *validUtcDates[] = {
        "Wed, 02 Oct 2002 08:05:09 GMT",
        "Wed, 02 Oct 2002 08:05:09 UT",
        "Wed, 02 Oct 2002 08:05:09 Z",
        "Wed, 02 Oct 2002 08:05:09 UTC",
    };

    for (auto &validUtcDate : validUtcDates)
    {
        Aws::Crt::DateTime dateTime(validUtcDate, Aws::Crt::DateFormat::RFC822);

        ASSERT_TRUE(dateTime);
        ASSERT_INT_EQUALS((int)Aws::Crt::DayOfWeek::Wednesday, (int)dateTime.GetDayOfWeek());
        ASSERT_UINT_EQUALS(2, dateTime.GetDay());
        ASSERT_UINT_EQUALS((int)Aws::Crt::Month::October, (int)dateTime.GetMonth());
        ASSERT_UINT_EQUALS(2002, dateTime.GetYear());
        ASSERT_UINT_EQUALS(8, dateTime.GetHour());
        ASSERT_UINT_EQUALS(5, dateTime.GetMinute());
        ASSERT_UINT_EQUALS(9, dateTime.GetSecond());

        uint8_t dateOutput[AWS_DATE_TIME_STR_MAX_LEN];
        AWS_ZERO_ARRAY(dateOutput);
        Aws::Crt::ByteBuf strOutput(dateOutput, sizeof(dateOutput), 0);
        ASSERT_TRUE(dateTime.ToGmtString(Aws::Crt::DateFormat::RFC822, strOutput));

        const char *expectedLongStr = "Wed, 02 Oct 2002 08:05:09 GMT";
        Aws::Crt::ByteBuf expectedLongBuf(expectedLongStr);

        ASSERT_BIN_ARRAYS_EQUALS(
            expectedLongBuf.Get()->buffer, expectedLongBuf.Get()->len, strOutput.Get()->buffer, strOutput.Get()->len);
    }

    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(DateTimeBinding, s_TestDateTimeBinding)