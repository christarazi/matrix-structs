#include <gtest/gtest.h>

#include <json.hpp>

#include <mtx/requests.hpp>

using json = nlohmann::json;

namespace ns = mtx::requests;

TEST(Requests, Login)
{
        ns::Login t1, t2, t3;

        t1.user                        = "@alice:matrix.org";
        t1.password                    = "secret";
        t1.initial_device_display_name = "Mobile";

        json j = t1;
        ASSERT_EQ(j.dump(),
                  "{\"initial_device_display_name\":\"Mobile\",\"password\":\"secret\",\"type\":"
                  "\"m.login.password\",\"user\":\"@alice:matrix.org\"}");

        t2.user     = "@bob:matrix.org";
        t2.password = "secret2";

        j = t2;
        ASSERT_EQ(
          j.dump(),
          "{\"password\":\"secret2\",\"type\":\"m.login.password\",\"user\":\"@bob:matrix.org\"}");

        t3.user      = "@carl:matrix.org";
        t3.password  = "secret3";
        t3.device_id = "ZSDF2RG";

        j = t3;
        ASSERT_EQ(j.dump(),
                  "{\"device_id\":\"ZSDF2RG\",\"password\":\"secret3\",\"type\":\"m.login."
                  "password\",\"user\":\"@carl:matrix.org\"}");
}

TEST(Requests, Typing)
{
        ns::TypingNotification t1, t2;

        t1.timeout = 4000;

        json j = t1;
        ASSERT_EQ(j.dump(), "{\"timeout\":4000,\"typing\":false}");

        t2.typing  = true;
        t2.timeout = 4242;

        j = t2;
        ASSERT_EQ(j.dump(), "{\"timeout\":4242,\"typing\":true}");
}
